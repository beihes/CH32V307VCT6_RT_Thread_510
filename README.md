| 引脚   | 外设    | 功能              | GPIO_Mode     | 是否启用 |
|--------|---------|-------------------|---------------|----------|
| PA0    | -       | -                 |               |          |
| PA1    | -       | -                 |               |          |
| PA2    | SPI1    | CS (W25Q32)       | GPIO_Mode_Out_PP | 是       |
| PA3    | -       | -                 |               |          |
| PA4    | -       | -                 |               |          |
| PA5    | SPI1    | SCK (W25Q32)      | GPIO_Mode_AF_PP | 是       |
| PA6    | SPI1    | MISO (W25Q32)     | GPIO_Mode_IN_FLOATING | 是       |
| PA7    | SPI1    | MOSI (W25Q32)     | GPIO_Mode_AF_PP | 是       |
| PA8    | -       | -                 |               |          |
| PA9    | USART1  | TX                | GPIO_Mode_AF_PP | 是       |
| PA10   | USART1  | RX                | GPIO_Mode_IN_FLOATING | 是       |
| PA11   | USB | 待开发 |  | TYPEC_0 |
| PA12   | USB | 待开发 |  | TYPEC_0 |
| PA13   | -       | -                 |               |          |
| PA14   | -       | -                 |               |          |
| PA15   | LED     | RED               | GPIO_Mode_Out_PP | 是       |
| PB0    | -       | -                 |               |          |
| PB1    | -       | -                 |               |          |
| PB2    | -       | -                 |               |          |
| PB3    | -       | -                 |               |          |
| PB4    | LED     | GREEN             | GPIO_Mode_Out_PP | 是       |
| PB5    | -       | -                 |               |          |
| PB6    | USBHD | 待开发 |  | TYPEC_1 |
| PB7    | USBHD | 待开发 |  | TYPEC_1 |
| PB8    | I2C1_1 | SCL (OLED[0x78]) | GPIO_Mode_AF_OD | 是 |
| PB9    | I2C1_1 | SDA (OLED[0x78]) | GPIO_Mode_AF_OD | 是 |
| PB10   | I2C2 | SCL (AT24C32[0xA0]) | GPIO_Mode_AF_OD | 是       |
| PB11   | I2C2 | SDA (AT24C32[0xA0]) | GPIO_Mode_AF_OD | 是       |
| PB12   | -       | -                 |               |          |
| PB13   | SPI2    | SCK               | GPIO_Mode_AF_PP | 是       |
| PB14   | SPI2    | MISO              | GPIO_Mode_IN_FLOATING | 是       |
| PB15   | SPI2    | MOSI              | GPIO_Mode_AF_PP | 是       |
| PC0    | UART6 | TX | GPIO_Mode_AF_PP |          |
| PC1    | UART6 | RX | GPIO_Mode_IN_FLOATING |          |
| PC2    | UART7 | TX | GPIO_Mode_AF_PP |          |
| PC3    | UART7 | RX | GPIO_Mode_IN_FLOATING |          |
| PC4    | UART8 | TX | GPIO_Mode_AF_PP |          |
| PC5    | UART8 | RX | GPIO_Mode_IN_FLOATING |          |
| PC6    | TIM8 | PWM_CH1(serwa) | GPIO_Mode_AF_PP | 是 |
| PC7    |  |  |  |  |
| PC8    | SDIO | D0 | GPIO_Mode_AF_PP | 预留 |
| PC9    | SDIO | D1 | GPIO_Mode_AF_PP | 预留 |
| PC10   | SDIO | D2 | GPIO_Mode_AF_PP | 预留 |
| PC11   | SDIO | D3 | GPIO_Mode_AF_PP | 预留 |
| PC12   | SDIO | CLK | GPIO_Mode_AF_PP | 预留 |
| PC13   | -       | -                 |               |          |
| PC14   | -       | -                 |               |          |
| PC15   | -       | -                 |               |          |
| PD0    | -       | -                 |               |          |
| PD1    | -       | -                 |               |          |
| PD2    | SDIO | CMD | GPIO_Mode_AF_PP | 预留 |
| PD3    | -       | -                 |               |          |
| PD4    | -       | -                 |               |          |
| PD5    | USART2_1 | TX | GPIO_Mode_AF_PP | 是 |
| PD6    | USART2_1 | RX | GPIO_Mode_IN_FLOATING | 是 |
| PD7    | -       | -                 |               |          |
| PD8    | USART3_3 | TX | GPIO_Mode_AF_PP | 是 |
| PD9    | USART3_3 | RX | GPIO_Mode_IN_FLOATING | 是 |
| PD10   | -       | -                 |               |          |
| PD11   |  | -                 |               |          |
| PD12   | TIM4_1 | PWM_CH1(serwa) | GPIO_Mode_AF_PP | 是 |
| PD13   | TIM4_1 | PWM_CH2(serwa) | GPIO_Mode_AF_PP | 是 |
| PD14   | TIM4_1 | PWM_CH3(serwa) | GPIO_Mode_AF_PP | 是 |
| PD15   | TIM4_1 | PWM_CH4(serwa) | GPIO_Mode_AF_PP | 是 |
| PE0    | -       | -                 |               |          |
| PE1    | -       | -                 |               |          |
| PE2    | -       | -                 |               |          |
| PE3    | -       | -                 |               |          |
| PE4    | -       | -                 |               |          |
| PE5    | -       | -                 |               |          |
| PE6    | -       | -                 |               |          |
| PE7    | -       | -                 |               |          |
| PE8    | -       | -                 |               |          |
| PE9    | -       | -                 |               |          |
| PE10   | -       | -                 |               |          |
| PE11   | -       | -                 |               |          |
| PE12   | -       | -                 |               |          |
| PE13   | -       | -                 |               |          |
| PE14   | -       | -                 |               |          |
| PE15   | -       | -                 |               |          |
```注：TIM4_1为重映射功能```