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

	// センサ電源 On/Off	
	void powerOn();
	void powerOff();

	// 測距開始
	double sonar();
	
private:
	// デバイス初期化,解放
	void initialize();
	void finalize();

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

