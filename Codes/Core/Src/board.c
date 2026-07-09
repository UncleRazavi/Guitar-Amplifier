#include "board.h"

extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;

static void Board_DisplayCommand(uint8_t command)
{
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &command, 1U, 100);
}

static void Board_DisplayData(const uint8_t *data, uint16_t size)
{
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)data, size, 200);
}

static void Board_DisplayReset(void)
{
    HAL_GPIO_WritePin(DISPLAY_RST_GPIO_Port, DISPLAY_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DISPLAY_RST_GPIO_Port, DISPLAY_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(120);
}

void Board_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }

    Board_AmpMute(1);
}

void Board_DisplayInit(void)
{
    Board_DisplayReset();

    Board_DisplayCommand(0x11); // Sleep out
    HAL_Delay(120);

    Board_DisplayCommand(0x36);
    {
        const uint8_t data = 0x48;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0x3A);
    {
        const uint8_t data = 0x55;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xB2);
    {
        const uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        Board_DisplayData(data, sizeof(data));
    }

    Board_DisplayCommand(0xB7);
    {
        const uint8_t data = 0x35;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xBB);
    {
        const uint8_t data = 0x19;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xC0);
    {
        const uint8_t data = 0x2C;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xC2);
    {
        const uint8_t data = 0x01;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xC3);
    {
        const uint8_t data = 0x12;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xC4);
    {
        const uint8_t data = 0x20;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xC6);
    {
        const uint8_t data = 0x0F;
        Board_DisplayData(&data, 1);
    }

    Board_DisplayCommand(0xD0);
    {
        const uint8_t data[] = {0xA4, 0xA1};
        Board_DisplayData(data, sizeof(data));
    }

    Board_DisplayCommand(0x29); // Display on
}

void Board_AmpEnable(void)
{
    HAL_GPIO_WritePin(AMP_SDZ_GPIO_Port, AMP_SDZ_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin, GPIO_PIN_RESET);
}

void Board_AmpMute(uint8_t mute)
{
    HAL_GPIO_WritePin(AMP_MUTE_GPIO_Port, AMP_MUTE_Pin, mute ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint8_t Board_AmpFaultActive(void)
{
    return HAL_GPIO_ReadPin(AMP_FAULT_GPIO_Port, AMP_FAULT_Pin) == GPIO_PIN_RESET;
}

void Board_Log(const char *msg)
{
    if (msg == 0)
    {
        return;
    }

    const uint8_t *ptr = (const uint8_t *)msg;
    while (*ptr != 0)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)ptr, 1, 100);
        ptr++;
    }
}
