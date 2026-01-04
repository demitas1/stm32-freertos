# Color LED - RGB Rainbow Effect

STM32F411CE (Black Pill) + FreeRTOS によるRGB LEDレインボーエフェクト

## 機能

- HSV色空間を使用したレインボーエフェクト
- ソフトウェアPWMによる輝度制御（任意のGPIOピンで動作）
- PC13オンボードLEDによる動作確認用点滅

## ハードウェア接続

### デフォルト設定

| 色 | GPIO | ピン |
|----|------|------|
| Red | PB2 | GPIO_PIN_2 |
| Green | PB1 | GPIO_PIN_1 |
| Blue | PB0 | GPIO_PIN_0 |

### 回路例（カソードコモンRGB LED）

```
3.3V ─┬─ [R 220Ω] ─ Red   ─┐
      ├─ [R 220Ω] ─ Green ─┼─ RGB LED ─ GND
      └─ [R 220Ω] ─ Blue  ─┘

PB2 ──────────────────── Red
PB1 ──────────────────── Green
PB0 ──────────────────── Blue
```

## GPIO設定の変更

`Core/Inc/rgb_led.h` を編集:

```c
/* GPIO Configuration - Modify these to change LED pins */
#define LED_RED_PORT    GPIOB
#define LED_RED_PIN     GPIO_PIN_2
#define LED_GREEN_PORT  GPIOB
#define LED_GREEN_PIN   GPIO_PIN_1
#define LED_BLUE_PORT   GPIOB
#define LED_BLUE_PIN    GPIO_PIN_0
```

## ビルドと実行

```bash
# コンテナ内で実行
cd color_led
make clean
make
make flash
```

## 実装詳細

### ソフトウェアPWM

- TIM2タイマー割り込みを使用
- 周波数: 約100Hz（256ステップ × 100Hz = 25.6kHz割り込み）
- 分解能: 8ビット（0-255）
- 任意のGPIOピンでPWM出力可能

### レインボーエフェクト

- HSV色空間でHue（色相）を0-359度で変化
- 50ms間隔で色を更新（1周期: 約18秒）
- 彩度・明度は最大値（255）固定

## ファイル構成

```
color_led/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── rgb_led.h      # GPIO設定
│   │   ├── hsv.h
│   │   ├── FreeRTOSConfig.h
│   │   └── stm32f4xx_hal_conf.h
│   └── Src/
│       ├── main.c         # メインタスク
│       ├── rgb_led.c      # PWMドライバ
│       ├── hsv.c          # 色変換
│       └── ...
├── Makefile
└── README.md
```

## トラブルシューティング

### LEDが点灯しない

1. GPIO接続を確認
2. LED極性を確認（アノードコモン/カソードコモン）
3. PC13が点滅しているか確認（プログラム動作確認）

### アノードコモンLEDの場合

`rgb_led.c` の `rgb_led_pwm_update()` でロジックを反転:

```c
// カソードコモン: HIGH = ON
if (pwm_counter < duty_red) {
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
} else {
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
}

// アノードコモン: LOW = ON に変更
if (pwm_counter < duty_red) {
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
} else {
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_SET);
}
```
