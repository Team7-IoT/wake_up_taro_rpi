#include <cstdio>
#include <csignal>
#include <memory>
#include <bcm2835.h>
#include "hcsr04drv.h"
#include "util.h"

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
	if(!bcm2835_init()) {
		return -1; // failed : cannot initialize bcm2835
	}

	// メディアンフィルタ生成
	MedianFilter<5> filter;
	// エッジ検出器生成
	EdgeDetector detector(10e-2, 10e-2); // Low:10cm以下, High:30cm以上

	// HC-SR04ドライバ生成, 初期化
	auto driver = std::make_unique<HCSR04>(PIN_SW, PIN_TRIG, PIN_ECHO);
	
	// TODO : センサ制御コード
	while(!signaled) {
		// 測距
		double dist_raw = driver->sonar();
		// フィルタ
		double dist_filtered = filter.update(dist_raw);
		// エッジ検出
		auto st = detector.update(dist_filtered);
		// 表示
		switch(st) {
		case EdgeDetector::Status::Unknown 	: printf("U"); break;
		case EdgeDetector::Status::Raise	: printf("R"); break;
		case EdgeDetector::Status::Fall	 	: printf("F"); break;
		case EdgeDetector::Status::Low	 	: printf("L"); break;
		case EdgeDetector::Status::High	 	: printf("H"); break;
		} 
		printf(" fil : %5.3f [mm]", static_cast<int>(dist_filtered));
		printf(" raw : %5.3f [mm]", static_cast<int>(dist_raw));
		printf("\n");
	}

	// ドライバ解放
	driver.reset();

	// BCM2835ライブラリ終了処理
	bcm2835_close();

	return 0;
}
