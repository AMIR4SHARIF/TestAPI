
#include "stm32f1xx_hal.h"
#include "main.h"

/*
#define CS_PIN      GPIO_PIN_0
#define CS_PORT     GPIOB
#define RESET_PIN   GPIO_PIN_1
#define RESET_PORT  GPIOB
*/

extern SPI_HandleTypeDef hspi1;

void W5500_Select(void) {
    HAL_GPIO_WritePin(SPI_SELECT_GPIO_Port, SPI_SELECT_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
    HAL_GPIO_WritePin(SPI_SELECT_GPIO_Port, SPI_SELECT_Pin, GPIO_PIN_SET);
}

void W5500_Restart(void) {
    HAL_GPIO_WritePin(SPI_RST_GPIO_Port, SPI_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);  // delay 1ms
    HAL_GPIO_WritePin(SPI_RST_GPIO_Port, SPI_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);  // delay 100ms
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}






