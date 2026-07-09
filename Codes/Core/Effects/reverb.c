#include "effects.h"

static float reverb_sample_rate = 48000.0f;
static float reverb_mix = 0.22f;
static float reverb_buffer[16384];
static uint32_t reverb_write_index = 0;

void Reverb_Init(uint32_t sample_rate)
{
    reverb_sample_rate = (float)sample_rate;
    reverb_mix = 0.22f;
    reverb_write_index = 0;
    for (uint32_t i = 0; i < sizeof(reverb_buffer) / sizeof(reverb_buffer[0]); ++i)
    {
        reverb_buffer[i] = 0.0f;
    }
}

float Reverb_Process(float input)
{
    uint32_t max_samples = sizeof(reverb_buffer) / sizeof(reverb_buffer[0]);
    uint32_t delay_samples = (uint32_t)(0.035f * reverb_sample_rate);
    if (delay_samples >= max_samples)
    {
        delay_samples = max_samples - 1;
    }

    uint32_t read_index = (reverb_write_index + max_samples - delay_samples) % max_samples;
    float delayed = reverb_buffer[read_index];
    float out = input + delayed * 0.7f;
    reverb_buffer[reverb_write_index] = out * 0.5f;
    reverb_write_index = (reverb_write_index + 1U) % max_samples;

    return input * (1.0f - reverb_mix) + out * reverb_mix;
}

void Effects_SetReverbMix(float mix)
{
    reverb_mix = mix;
}
