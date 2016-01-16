#pragma once

#include <cstdint>
#include <bcm2835.h>

// ピンアサイン(for Raspberry Pi B+, 2)
static constexpr int PIN_SW   = RPI_GPIO_P1_12; // 12番ピン : GPIO 18
static constexpr int PIN_TRIG = RPI_GPIO_P1_16; // 16番ピン : GPIO 23
static constexpr int PIN_ECHO = RPI_GPIO_P1_18; // 18番ピン : GPIO 24

//遠近検知閾値
static constexpr double THRESHOLD_DISTNCE_LOW_MM  = 50;  // H->L 閾値[mm]
static constexpr double THRESHOLD_DISTNCE_HIGH_MM = 100; // L->H 閾値[mm]

// センサ制御パラメータ
static constexpr int      SENSOR_POWER_ON_DELAY_MSEC  = 50;  // 電源投入後待機時間[msec]
static constexpr int      SENSOR_POWER_OFF_DELAY_MSEC = 50;  // 電源断後待機時間[msec]
static constexpr int      SENSOR_ECHO_TIMEOUT_MSEC    = 100; // エコーパルス待機タイムアウト時間[msec]
static constexpr uint64_t SENSOR_SONAR_INTERVAL_MSEC  = 10;  // ソナー間インターバル時間[μsec]

// --- 設定値テスト ---
// ピンアサインはRPi B+とRPi 2で共通(万が一異なればコンパイルエラー)
static_assert(RPI_GPIO_P1_12 == RPI_V2_GPIO_P1_12, "");
static_assert(RPI_GPIO_P1_16 == RPI_V2_GPIO_P1_16, "");
static_assert(RPI_GPIO_P1_18 == RPI_V2_GPIO_P1_18, "");
