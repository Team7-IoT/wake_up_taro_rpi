#include "hcsr04drv.h"
#include <bcm2835.h>
#include <chrono>

// コンストラクタ
HCSR04::HCSR04(PinNumber pw, PinNumber trig, PinNumber echo)
	: mInitialized(false)
	, mPowerOn(false)
	, mPinPower(pw)
	, mPinTrig(trig)
	, mPinEcho(echo)
{
	// デバイス初期化処理	
	initialize();
}

// デストラクタ
HCSR04::~HCSR04()
{
	// デバイス解放処理
	finalize();
}

// デバイス初期化処理
void HCSR04::initialize()
{
	// MEMO : 事前にbcm2835_initが呼ばれているものとする
	
	// 各GPIOピンのI/O方向をセットする
	// - PowSW, TrigピンはOutput
	bcm2835_gpio_fsel(mPinPower, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(mPinTrig,  BCM2835_GPIO_FSEL_OUTP);
	// - EchoピンはInput
	bcm2835_gpio_fsel(mPinEcho,  BCM2835_GPIO_FSEL_INPT);
	
	// 初期化終了
	mInitialized = true;
}

// デバイス解放処理
void HCSR04::finalize()
{
	if(!mInitialized) return;

	// センサ給電中の場合、先に停止する
	powerOff();
	
    // 使用したすべてのGPIOピンのI/O方向をInputに戻す
    bcm2835_gpio_fsel(mPinPower, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(mPinTrig,  BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(mPinEcho,  BCM2835_GPIO_FSEL_INPT);

    // 解放終了
    mInitialized = false;
}

// センサ電源ON
void HCSR04::powerOn()
{
	if(mPowerOn) return;

	// 電源SW L->H : トランジスタ経由でHC-SR04へ5V給電
	bcm2835_gpio_write(mPinPower, HIGH);
	bcm2835_delay(100); // 状態安定まで待機

	// 電源投入完了
	mPowerOn = true;
}


// センサ電源Off
void HCSR04::powerOff()
{
    if(!mPowerOn) return;

    // 電源SW H->L : HC-SR04 給電停止
    bcm2835_gpio_write(mPinPower, LOW);
    bcm2835_delay(100); // 状態安定まで待機

    // 電源投入完了
    mPowerOn = false;
}

double HCSR04::sonar()
{
	using clock = std::chrono::high_resolution_clock;
	int cnt = 0; // ノイズ対策のため、n回連続で同一信号を検出するまで待機

	if(!mPowerOn) return std::numeric_limits<double>::infinity();

	// Trigを10μsecの間Highとする
	bcm2835_gpio_write(mPinTrig, HIGH);
	bcm2835_delayMicroseconds(10);
	bcm2835_gpio_write(mPinTrig, LOW);
	
	// EchoがHighになるまで待機
	cnt = 0;
	auto low_high_st = clock::now();
	while(true) {
		if(std::chrono::duration_cast<std::chrono::milliseconds>(clock::now()-low_high_st).count() > 20/*[msec]*/) {
			// タイムアウト : Low -> High 
			return std::numeric_limits<double>::infinity();
		}
		auto level = bcm2835_gpio_lev(mPinEcho);
		if(level == HIGH) {
			++cnt;
			if(cnt == 3) break;
		} else {
			cnt = 0;
		}
		bcm2835_delayMicroseconds(1); // busy-loop対策で1μsecは待機
	}
	
	// EchoがHigh -> Lowになる時間を計測
	auto high_low_st = clock::now();
	while(true) {
        if(std::chrono::duration_cast<std::chrono::milliseconds>(clock::now()-high_low_st).count() > 20/*[msec]*/) {
            // タイムアウト : High -> Low
            return std::numeric_limits<double>::infinity();
        }
        auto level = bcm2835_gpio_lev(mPinEcho);
        if(level == LOW) {
            ++cnt;
            if(cnt == 3) break;
        } else {
            cnt = 0;
        }
        bcm2835_delayMicroseconds(1); // busy-loop対策で1μsecは待機
    }
	auto high_low_ed = clock::now();

	bcm2835_delayMicroseconds(100); // 連続した読み取り時の誤動作対策に1090μsec待機	

	double t = std::chrono::duration_cast<std::chrono::microseconds>(high_low_ed - high_low_st).count(); // [μsec]
	double d = t * 340 / 1000 / 2; // 音速=340[msec/sec]と仮定 & t = 往復に要する時間[μsec]
	return d; // 単位 : [mm]
}
