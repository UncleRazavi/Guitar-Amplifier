#ifndef INC_EFFECTS_H_
#define INC_EFFECTS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Effects_Init(uint32_t sample_rate);

void eq_init(uint32_t sample_rate);
float eq_process(float in_sample);

float Preamp_Process(float input);
float Distortion_Process(float input);

void Chorus_Init(uint32_t sample_rate);
float Chorus_Process(float input);

void Delay_Init(uint32_t sample_rate);
float Delay_Process(float input);

void Reverb_Init(uint32_t sample_rate);
float Reverb_Process(float input);

void Effects_SetDistortionDrive(float drive);
void Effects_SetDistortionTone(float tone);
void Effects_SetEQ(float bass, float mid, float treble);
void Effects_SetDelayTime(float delay_ms);
void Effects_SetDelayFeedback(float feedback);
void Effects_SetReverbMix(float mix);
void Effects_SetChorusDepth(float depth);
void Effects_SetChorusRate(float rate_hz);

#ifdef __cplusplus
}
#endif

#endif /* INC_EFFECTS_H_ */
