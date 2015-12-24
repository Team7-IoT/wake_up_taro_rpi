#include "hcsr04drv.h"
#include <bcm2835.h>

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
