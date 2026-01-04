# STM32F4 FreeRTOS Development Environment

STM32F411CE (Black Pill) + FreeRTOS + Docker開発環境

## ディレクトリ構成

```
stm32/
├── stm32-deps/           # 共通依存ライブラリ (別途セットアップ)
├── stm32-freertos/       # このリポジトリ
│   ├── Dockerfile
│   ├── docker-compose.yml
│   └── projects/
│       └── led-blink/    # サンプルプロジェクト
└── stm32-embassy/        # Rust Embassy環境 (将来)
```

## セットアップ

### 1. 依存ライブラリのセットアップ

```bash
cd ../stm32-deps
./setup.sh
```

### 2. Dockerイメージのビルド

```bash
docker compose build
```

## 使い方

### ビルド

```bash
# コンテナに入る
docker compose run --rm stm32-dev

# プロジェクトディレクトリへ移動
cd led-blink

# ビルド
make

# クリーンビルド
make clean && make
```

### フラッシュ

ST-Linkを接続した状態で:

```bash
# st-flash を使用
make flash

# または OpenOCD を使用
make flash-openocd
```

### デバッグ

```bash
make debug
```

別ターミナルでGDBに接続:
```bash
docker compose exec stm32-dev gdb-multiarch build/led-blink.elf
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) continue
```

## ターゲットハードウェア

- **MCU**: STM32F411CEU6 (Black Pill)
- **クロック**: 100MHz (HSE 25MHz → PLL)
- **Flash**: 512KB
- **RAM**: 128KB
- **LED**: PC13 (Active Low)

## FreeRTOS設定

- ヒープサイズ: 30KB (heap_4使用)
- タスク優先度: 7レベル
- Tick: 1ms (1000Hz)
