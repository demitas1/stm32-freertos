# 新規プロジェクトの作成手順

このドキュメントでは、stm32-freertos環境で新規プロジェクトを作成する手順を説明する。

## 1. プロジェクトディレクトリの作成

`led-blink`をテンプレートとしてコピーする。

```bash
cd stm32-freertos/projects
cp -r led-blink my-project
cd my-project
```

## 2. Makefileの編集

### ターゲット名の変更

```makefile
# TARGET = led-blink
TARGET = my-project
```

### 使用するHALモジュールの追加

必要に応じてC_SOURCESにHALドライバを追加する。

```makefile
C_SOURCES = \
Core/Src/main.c \
Core/Src/stm32f4xx_it.c \
Core/Src/stm32f4xx_hal_msp.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_cortex.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_gpio.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_rcc.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_rcc_ex.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_pwr.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_pwr_ex.c \
# 以下、必要なモジュールを追加
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_uart.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_i2c.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_spi.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_tim.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_dma.c \
...
```

### アプリケーションソースの追加

```makefile
C_SOURCES = \
Core/Src/main.c \
Core/Src/stm32f4xx_it.c \
Core/Src/stm32f4xx_hal_msp.c \
Core/Src/app_task.c \       # 追加
Core/Src/uart_driver.c \    # 追加
...
```

## 3. stm32f4xx_hal_conf.h の編集

使用するHALモジュールを有効化する。

```c
// Core/Inc/stm32f4xx_hal_conf.h

/* Module Selection */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED

/* 必要なモジュールのコメントを外す */
#define HAL_UART_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
```

## 4. main.c の編集

### 基本構造

```c
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

/* Private function prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void MyTask(void *pvParameters);

/* Task handles */
TaskHandle_t myTaskHandle = NULL;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    // 他の初期化...

    xTaskCreate(
        MyTask,
        "MyTask",
        256,                // Stack size (words)
        NULL,
        2,                  // Priority
        &myTaskHandle
    );

    vTaskStartScheduler();

    while (1) {}
}

static void MyTask(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        // Task processing
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

## 5. 割り込みハンドラの追加 (必要な場合)

`stm32f4xx_it.c` に割り込みハンドラを追加する。

```c
// Core/Src/stm32f4xx_it.c

extern UART_HandleTypeDef huart2;

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}
```

## 6. FreeRTOS設定の調整 (必要な場合)

`FreeRTOSConfig.h` で以下の項目を調整できる。

```c
// ヒープサイズ (デフォルト: 30KB)
#define configTOTAL_HEAP_SIZE    ( ( size_t ) ( 30 * 1024 ) )

// 最大タスク優先度
#define configMAX_PRIORITIES     ( 7 )

// タスク名の最大長
#define configMAX_TASK_NAME_LEN  ( 16 )

// タイマー機能
#define configUSE_TIMERS         1
```

## 7. ビルドと実行

```bash
# コンテナ内で実行
cd my-project
make clean
make
make flash
```

## 利用可能なHALモジュール一覧

よく使用されるモジュール：

| モジュール | ファイル | 用途 |
|-----------|---------|------|
| GPIO | stm32f4xx_hal_gpio.c | デジタル入出力 |
| UART | stm32f4xx_hal_uart.c | シリアル通信 |
| I2C | stm32f4xx_hal_i2c.c | I2Cバス通信 |
| SPI | stm32f4xx_hal_spi.c | SPIバス通信 |
| TIM | stm32f4xx_hal_tim.c | タイマー、PWM |
| ADC | stm32f4xx_hal_adc.c | アナログ入力 |
| DMA | stm32f4xx_hal_dma.c | DMA転送 |
| EXTI | stm32f4xx_hal_exti.c | 外部割り込み |

全モジュールは `stm32-deps/stm32f4xx_hal_driver/Src/` を参照。

## トラブルシューティング

### ビルドエラー: undefined reference

HALモジュールのソースファイルがMakefileのC_SOURCESに追加されているか確認。

### ビルドエラー: unknown type name

`stm32f4xx_hal_conf.h` で該当モジュールが有効化されているか確認。

### ヒープ不足 (malloc failed)

`FreeRTOSConfig.h` の `configTOTAL_HEAP_SIZE` を増やす。
STM32F411CEは128KB RAMなので最大約100KB程度まで設定可能。

### スタックオーバーフロー

`xTaskCreate` の第3引数（スタックサイズ）を増やす。
