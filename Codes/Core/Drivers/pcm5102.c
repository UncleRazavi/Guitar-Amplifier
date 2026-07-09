/* Minimal PCM5102 helper - controls mute pin and provides I2S DMA wrapper. */
#include "pcm5102.h"

static GPIO_TypeDef *pcm_mute_port = NULL;
static uint16_t pcm_mute_pin = 0;
static I2S_HandleTypeDef *pcm_i2s_handle = NULL;
static uint16_t *pcm_dma_buffer = NULL;
static uint32_t pcm_dma_size = 0;
static uint8_t pcm_is_muted = 1;

void pcm5102_init(GPIO_TypeDef *mute_port, uint16_t mute_pin)
{
    pcm_mute_port = mute_port;
    pcm_mute_pin = mute_pin;
    pcm_i2s_handle = NULL;
    pcm_dma_buffer = NULL;
    pcm_dma_size = 0;
    pcm_is_muted = 1;

    if (pcm_mute_port != NULL)
    {
        HAL_GPIO_WritePin(pcm_mute_port, pcm_mute_pin, GPIO_PIN_SET);
    }
}

void pcm5102_configure_i2s(I2S_HandleTypeDef *hi2s, uint16_t *buffer, uint32_t size)
{
    pcm_i2s_handle = hi2s;
    pcm_dma_buffer = buffer;
    pcm_dma_size = size;
}

int pcm5102_start(void)
{
    if (pcm_i2s_handle == NULL || pcm_dma_buffer == NULL || pcm_dma_size == 0)
    {
        return -1;
    }

    if (HAL_I2S_Transmit_DMA(pcm_i2s_handle, pcm_dma_buffer, pcm_dma_size) != HAL_OK)
    {
        return -1;
    }

    return 0;
}

int pcm5102_stop(void)
{
    if (pcm_i2s_handle == NULL)
    {
        return -1;
    }

    if (HAL_I2S_DMAStop(pcm_i2s_handle) != HAL_OK)
    {
        return -1;
    }

    return 0;
}

void pcm5102_set_mute(uint8_t mute)
{
    if (pcm_mute_port == NULL)
    {
        return;
    }

    pcm_is_muted = (mute != 0U) ? 1U : 0U;
    HAL_GPIO_WritePin(pcm_mute_port, pcm_mute_pin, pcm_is_muted ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint8_t pcm5102_is_muted(void)
{
    return pcm_is_muted;
}
