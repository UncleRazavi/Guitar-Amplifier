#include "effects.h"
#include <math.h>

static float chorus_sample_rate = 48000.0f;
static float chorus_depth = 0.25f;
static float chorus_rate = 0.8f;
static float chorus_phase = 0.0f;
static float chorus_buffer[2048];
static uint32_t chorus_write_index = 0;

void Chorus_Init(uint32_t sample_rate)
{
    chorus_sample_rate = (float)sample_rate;
    chorus_depth = 0.25f;
    chorus_rate = 0.8f;
    chorus_phase = 0.0f;
    chorus_write_index = 0;
    for (uint32_t i = 0; i < sizeof(chorus_buffer) / sizeof(chorus_buffer[0]); ++i)
    {
        chorus_buffer[i] = 0.0f;
    }
}

float Chorus_Process(float input)
{
    float lfo = 0.5f + 0.5f * sinf(2.0f * 3.14159265358979323846f * chorus_phase);
    chorus_phase += chorus_rate / chorus_sample_rate;
    if (chorus_phase >= 1.0f)
    {
        chorus_phase -= 1.0f;
    }

    float delay_samples = 240.0f + 160.0f * chorus_depth * lfo;
    if (delay_samples >= (float)(sizeof(chorus_buffer) / sizeof(chorus_buffer[0]) - 1))
    {
        delay_samples = (float)(sizeof(chorus_buffer) / sizeof(chorus_buffer[0]) - 2);
    }

    int32_t read_index = (int32_t)chorus_write_index - (int32_t)delay_samples;
    if (read_index < 0)
    {
        read_index += (int32_t)(sizeof(chorus_buffer) / sizeof(chorus_buffer[0]));
    }

    uint32_t index0 = (uint32_t)read_index;
    uint32_t index1 = (index0 + 1U) % (sizeof(chorus_buffer) / sizeof(chorus_buffer[0]));
    float frac = delay_samples - (float)((int32_t)delay_samples);
    float delayed = chorus_buffer[index0] * (1.0f - frac) + chorus_buffer[index1] * frac;

    chorus_buffer[chorus_write_index] = input;
    chorus_write_index = (chorus_write_index + 1U) % (sizeof(chorus_buffer) / sizeof(chorus_buffer[0]));

    return input * 0.65f + delayed * 0.35f;
}

void Effects_SetChorusDepth(float depth)
{
    chorus_depth = depth;
}

void Effects_SetChorusRate(float rate_hz)
{
    chorus_rate = rate_hz;
}
