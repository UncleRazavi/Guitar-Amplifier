#ifndef INC_AUDIO_ENGINE_H_
#define INC_AUDIO_ENGINE_H_

#include <stdint.h>
#include "dsp.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t g_audio_half_ready;
extern volatile uint8_t g_audio_full_ready;
extern uint16_t adc_dma_buffer[AUDIO_BLOCK_SIZE * 2U];
extern int16_t i2s_tx_buffer[AUDIO_BLOCK_SIZE * 2U * 2U];

void AudioEngine_Init(void);
void AudioEngine_Start(void);
void AudioEngine_ProcessHalf(void);
void AudioEngine_ProcessFull(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_AUDIO_ENGINE_H_ */
