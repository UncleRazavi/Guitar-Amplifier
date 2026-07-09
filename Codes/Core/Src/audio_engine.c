#include "audio_engine.h"
#include "effects.h"
#include "features.h"

#include <string.h>

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern I2S_HandleTypeDef hi2s3;

volatile uint8_t g_audio_half_ready = 0U;
volatile uint8_t g_audio_full_ready = 0U;
uint16_t adc_dma_buffer[AUDIO_BLOCK_SIZE * 2U];
int16_t i2s_tx_buffer[AUDIO_BLOCK_SIZE * 2U * 2U];

void AudioEngine_Init(void)
{
    memset(adc_dma_buffer, 0, sizeof(adc_dma_buffer));
    memset(i2s_tx_buffer, 0, sizeof(i2s_tx_buffer));
    g_audio_half_ready = 0U;
    g_audio_full_ready = 0U;
    Features_Init();
    Effects_Init(AUDIO_SAMPLE_RATE);
    DSP_Init((float)AUDIO_SAMPLE_RATE);
}

void AudioEngine_Start(void)
{
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_dma_buffer, AUDIO_BLOCK_SIZE * 2U) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t *)i2s_tx_buffer, AUDIO_BLOCK_SIZE * 2U * 2U) != HAL_OK)
    {
        Error_Handler();
    }
}

void AudioEngine_ProcessHalf(void)
{
    DSP_ProcessBlock(adc_dma_buffer, i2s_tx_buffer, AUDIO_BLOCK_SIZE);
}

void AudioEngine_ProcessFull(void)
{
    DSP_ProcessBlock(&adc_dma_buffer[AUDIO_BLOCK_SIZE], &i2s_tx_buffer[AUDIO_BLOCK_SIZE * 2U], AUDIO_BLOCK_SIZE);
}
