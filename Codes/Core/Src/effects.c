#include "effects.h"

void Effects_Init(uint32_t sample_rate)
{
    Chorus_Init(sample_rate);
    Delay_Init(sample_rate);
    Reverb_Init(sample_rate);
    eq_init(sample_rate);
    Effects_SetDistortionDrive(1.6f);
    Effects_SetDistortionTone(0.9f);
    Effects_SetDelayTime(220.0f);
    Effects_SetDelayFeedback(0.35f);
    Effects_SetReverbMix(0.22f);
    Effects_SetChorusDepth(0.25f);
    Effects_SetChorusRate(0.8f);
}

float Preamp_Process(float input)
{
    return input;
}
