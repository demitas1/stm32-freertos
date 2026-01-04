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

## ホスト側の設定

### udevルール (ST-Link用)

ST-Linkをroot権限なしで使用するため、udevルールを設定する。

```bash
sudo tee /etc/udev/rules.d/99-stlink.rules << 'EOF'
# ST-Link V2
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0666", GROUP="plugdev"
# ST-Link V2-1
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="0666", GROUP="plugdev"
# ST-Link V3
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374e", MODE="0666", GROUP="plugdev"
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374f", MODE="0666", GROUP="plugdev"
EOF

sudo udevadm control --reload-rules
sudo udevadm trigger
```

### plugdevグループへの追加

```bash
# 現在のユーザーがplugdevに所属しているか確認
groups | grep plugdev

# 所属していない場合は追加（要ログアウト/ログイン）
sudo usermod -aG plugdev $USER
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
