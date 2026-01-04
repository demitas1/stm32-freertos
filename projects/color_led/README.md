# Color LED - RGB Rainbow Effect

STM32F411CE (Black Pill) + FreeRTOS によるRGB LEDレインボーエフェクト

## 機能

- HSV色空間を使用したレインボーエフェクト
- TIM4ハードウェアPWMによる輝度制御
- PC13オンボードLEDによる動作確認用点滅

## ハードウェア接続

### GPIO設定 (TIM4 PWM)

| 色 | GPIO | タイマーチャンネル |
|----|------|-------------------|
| Red | PB6 | TIM4_CH1 |
| Green | PB7 | TIM4_CH2 |
| Blue | PB8 | TIM4_CH3 |

### 回路例（カソードコモンRGB LED）

```
PB6 ─[R 220Ω]─ Red   ─┐
PB7 ─[R 220Ω]─ Green ─┼─ RGB LED ─ GND
PB8 ─[R 220Ω]─ Blue  ─┘
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

### ハードウェアPWM (TIM4)

- タイマークロック: 100MHz (APB1 x2)
- PWM周波数: 約1kHz
- 分解能: 8ビット（0-255）
- プリスケーラ: 390
- カウンタ周期: 255

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
│   │   ├── rgb_led.h      # PWM設定
│   │   ├── hsv.h
│   │   ├── FreeRTOSConfig.h
│   │   └── stm32f4xx_hal_conf.h
│   └── Src/
│       ├── main.c         # メインタスク
│       ├── rgb_led.c      # TIM4 PWMドライバ
│       ├── hsv.c          # 色変換
│       └── ...
├── Makefile
└── README.md
```

## トラブルシューティング

### LEDが点灯しない

1. GPIO接続を確認（PB6, PB7, PB8）
2. LED極性を確認（アノードコモン/カソードコモン）
3. PC13が点滅しているか確認（プログラム動作確認）

### アノードコモンLEDの場合

`rgb_led.c` の `rgb_led_init()` で極性を変更:

```c
/* 極性を反転 */
sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;  // HIGH → LOW に変更
```

## ピン選定の理由

TIM4 (PB6, PB7, PB8) を選択した理由:
- ハードウェアPWM対応
- 同一タイマーの連続チャンネルで制御が容易
- 連続したピン番号で配線しやすい
