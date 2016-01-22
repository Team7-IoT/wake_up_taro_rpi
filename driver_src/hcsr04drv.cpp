#include "hcsr04drv.h"
#include "config.h"
#include <chrono>
#include <cfloat>

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

	// センサ電源投入中の場合、先に停止する
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

	// 電源SW L->H : トランジスタ経由でHC-SR04へ5V電源投入
	bcm2835_gpio_write(mPinPower, HIGH);
	bcm2835_delay(SENSOR_POWER_ON_DELAY_MSEC); // 状態安定まで待機

	// 電源投入完了
	mPowerOn = true;
}


// センサ電源Off
void HCSR04::powerOff()
{
    if(!mPowerOn) return;

    // 電源SW H->L : HC-SR04 電源断
    bcm2835_gpio_write(mPinPower, LOW);
    bcm2835_delay(SENSOR_POWER_OFF_DELAY_MSEC); // 状態安定まで待機

    // 電源断完了
    mPowerOn = false;
}

void HCSR04::pulseGen(PinNumber pin, double width)
{
	if(!mPowerOn) return;

	bcm2835_gpio_write(pin, HIGH);
    bcm2835_delayMicroseconds(static_cast<uint64_t>((width>0?width:0) * 1e+6));
    bcm2835_gpio_write(pin, LOW);
}

double HCSR04::pulseMeasure(PinNumber pin)
{
	using clock = std::chrono::high_resolution_clock;

	if(!mPowerOn) return DBL_MAX;
	
	int cnt = 0; // ノイズ対策のため、n回連続で同一信号を検出するまで待機
    auto st = clock::now();
    while(true) {
        if(std::chrono::duration_cast<std::chrono::milliseconds>(clock::now()-st).count() > SENSOR_ECHO_TIMEOUT_MSEC) {
            // タイムアウト : Low -> High
            return DBL_MAX;
        }
        auto level = bcm2835_gpio_lev(mPinEcho);
        if(level == HIGH) {
            ++cnt;
            if(cnt == 3) break;
        } else {
            cnt = 0;
        }
        bcm2835_delayMicroseconds(5); // busy-loop対策で5μsecは待機
    }

    // EchoがHigh -> Lowになる時間を計測
	cnt = 0;
    auto high_low_st = clock::now();
    while(true) {
        if(std::chrono::duration_cast<std::chrono::milliseconds>(clock::now()-st).count() > SENSOR_ECHO_TIMEOUT_MSEC) {
            // タイムアウト : High -> Low
            return DBL_MAX;
        }
        auto level = bcm2835_gpio_lev(mPinEcho);
        if(level == LOW) {
            ++cnt;
            if(cnt == 3) break;
        } else {
            cnt = 0;
        }
        bcm2835_delayMicroseconds(5); // busy-loop対策で5μsecは待機
    }
    auto high_low_ed = clock::now();

    return std::chrono::duration_cast<std::chrono::duration<double>>(high_low_ed - high_low_st).count(); // [μsec]
}

double HCSR04::sonar(bool oneShotMode)
{
	// 電源投入(投入済みの場合何もしない)
	powerOn();

	// Trigを10μsecの間Highとする
	pulseGen(mPinTrig, 10e-6);
	
	// EchoがHighになるまで待機
	double t = pulseMeasure(mPinEcho);

	// 連続した読み取り時の誤動作対策に待機
	bcm2835_delay(SENSOR_SONAR_INTERVAL_MSEC);

	if(oneShotMode) {
		// 単一読み取りモードの場合、電源断
		powerOff();
	} else {
		// 連続読み取りモードの場合、タイムアウト時のみ電源再投入する
		if(t == DBL_MAX) {
			powerOff();
			powerOn();
		}
	}

	double d = DBL_MAX;
	// 距離の算出
	if(t != DBL_MAX) {
		// 音速=340[m/sec]と仮定, t = 往復に要する時間[sec], 単位 : [m]
		d = t * 340 / 2;
		// 距離が1cmいないの場合、正常に取得できない扱いとする。
		if(d < 0.01) d = DBL_MAX;
	}
	return d;
}
