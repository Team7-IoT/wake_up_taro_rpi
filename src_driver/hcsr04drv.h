#pragma once

// ピン番号型
using PinNumber = int;

// HC-SR04ドライバクラス
class HCSR04
{
public:
	// コンストラクタ,デストラクタ
	HCSR04(PinNumber pw, PinNumber trig, PinNumber echo);
	~HCSR04();

	// 測距開始(測定不可 : DBL_MAX)
	double sonar(bool oneShotMode = false);
	
private:
	// デバイス初期化,解放
	void initialize();
	void finalize();

    // センサ電源 On/Off
    void powerOn();
    void powerOff();

	// パルス生成(L->H->L)
	void pulseGen(PinNumber pin, double width);
	
	// パルス幅計測(L->H->L)
	double pulseMeasure(PinNumber pin);

private:
	// コピー禁止
    HCSR04(const HCSR04&) = delete;
    HCSR04& operator=(const HCSR04&) = delete;

private:
	bool mInitialized;		// 初期化済みか否か
	bool mPowerOn;			// 電源投入済みか否か
	const PinNumber mPinPower;	// ピン番号：HC-SR04 電源SW
	const PinNumber mPinTrig;	// ピン番号：HC-SR04 Trigピン
	const PinNumber mPinEcho;	// ピン番号：HC-SR04 Echoピン
};

