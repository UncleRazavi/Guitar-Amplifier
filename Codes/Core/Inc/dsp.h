#ifndef INC_DSP_H_
#define INC_DSP_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AUDIO_SAMPLE_RATE 48000U
#define AUDIO_BLOCK_SIZE 64U

void DSP_Init(float sample_rate);
float DSP_ProcessSample(float input);
void DSP_ProcessBlock(const uint16_t *adc_in, int16_t *i2s_out, uint32_t count);

extern volatile float g_audio_level;

#ifdef __cplusplus
}
#endif

#endif /* INC_DSP_H_ */
