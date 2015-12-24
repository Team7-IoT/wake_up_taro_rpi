#include <cstdio>
#include <csignal>
#include <memory>
#include <bcm2835.h>
#include "hcsr04drv.h"

// ピンアサイン(for Raspberry Pi 2)
#if 1
static constexpr int PIN_SW   = RPI_V2_GPIO_P1_12; // 12番ピン : GPIO 18
static constexpr int PIN_TRIG = RPI_V2_GPIO_P1_16; // 16番ピン : GPIO 23
static constexpr int PIN_ECHO = RPI_V2_GPIO_P1_18; // 18番ピン : GPIO 24
#endif

// シグナルハンドラ
static volatile bool signaled = false;
static void signal_handler(int sig)
{
    signaled = true;
}


// main関数
int main(int argc, char *argv[])
{
	// シグナルハンドラ登録
	signal(SIGHUP,  signal_handler);
	signal(SIGINT,  signal_handler);
	signal(SIGTERM, signal_handler);
	
	// BCM2835ライブラリ初期化
	bcm2835_init();

	// HC-SR04ドライバ生成, 初期化
	auto driver = std::make_unique<HCSR04>(PIN_SW, PIN_TRIG, PIN_ECHO);

	// TODO : センサ制御コード
	driver->powerOn();
	while(!signaled) {
		double dist = driver->sonar();
		printf("%ld [mm] \n", (int)dist);
		bcm2835_delay(50);
	}

	// ドライバ解放
	driver.reset();

	// BCM2835ライブラリ終了処理
	bcm2835_close();

	return 0;
}
