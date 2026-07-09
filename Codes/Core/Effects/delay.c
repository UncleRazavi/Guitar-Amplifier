#include "effects.h"

static float delay_sample_rate = 48000.0f;
static float delay_time_ms = 220.0f;
static float delay_feedback = 0.35f;
static float delay_mix = 0.28f;
static float delay_buffer[24576];
static uint32_t delay_write_index = 0;

void Delay_Init(uint32_t sample_rate)
{
    delay_sample_rate = (float)sample_rate;
    delay_time_ms = 220.0f;
    delay_feedback = 0.35f;
    delay_mix = 0.28f;
    delay_write_index = 0;
    for (uint32_t i = 0; i < sizeof(delay_buffer) / sizeof(delay_buffer[0]); ++i)
    {
        delay_buffer[i] = 0.0f;
    }
}

float Delay_Process(float input)
{
    uint32_t max_samples = sizeof(delay_buffer) / sizeof(delay_buffer[0]);
    uint32_t delay_samples = (uint32_t)(delay_time_ms * 0.001f * delay_sample_rate);
    if (delay_samples >= max_samples)
    {
        delay_samples = max_samples - 1;
    }

    uint32_t read_index = (delay_write_index + max_samples - delay_samples) % max_samples;
    float delayed = delay_buffer[read_index];
    float output = input * (1.0f - delay_mix) + delayed * delay_mix;

    delay_buffer[delay_write_index] = input + delayed * delay_feedback;
    delay_write_index = (delay_write_index + 1U) % max_samples;

    return output;
}

void Effects_SetDelayTime(float delay_ms)
{
    delay_time_ms = delay_ms;
}

void Effects_SetDelayFeedback(float feedback)
{
    delay_feedback = feedback;
}
