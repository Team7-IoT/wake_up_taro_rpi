#include <cstdio>
#include <csignal>
#include <unistd.h>
#include <memory>
#include "config.h"
#include "hcsr04drv.h"
#include "util.h"

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
	EdgeDetector detector(THRESHOLD_DISTNCE_LOW_MM/1000.0, THRESHOLD_DISTNCE_HIGH_MM/1000.0);

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
		bool p = false;
		const char * status = "";
		switch(st) {
		case EdgeDetector::Status::Unknown 	: status="U"; p=true; break;
		case EdgeDetector::Status::Raise	: status="R"; p=true; break;
		case EdgeDetector::Status::Fall	 	: status="F"; p=true; break;
		case EdgeDetector::Status::Low	 	: status="L"; p=true; break;
		case EdgeDetector::Status::High	 	: status="H"; p=true; break;
		} 
		if(p) {
			printf("%s", status);
			printf(" fil : %5.3lf [cm]", dist_filtered*100);
			printf(" raw : %5.3lf [cm]", dist_raw*100);
			printf("\n");
		}
	}

	// ドライバ解放
	driver.reset();

	// BCM2835ライブラリ終了処理
	bcm2835_close();

	return 0;
}
