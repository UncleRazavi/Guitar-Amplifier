#include "dsp.h"
#include "effects.h"
#include "features.h"

#include <math.h>

volatile float g_audio_level = 0.0f;
static float g_sample_rate = (float)AUDIO_SAMPLE_RATE;
static float g_hp_x_prev = 0.0f;
static float g_hp_y_prev = 0.0f;
static float g_tone_state = 0.0f;

void DSP_Init(float sample_rate)
{
    g_sample_rate = sample_rate;
    g_hp_x_prev = 0.0f;
    g_hp_y_prev = 0.0f;
    g_tone_state = 0.0f;
}

static float soft_clip(float value)
{
    if (value > 1.0f)
    {
        return 1.0f;
    }
    if (value < -1.0f)
    {
        return -1.0f;
    }
    return value / (1.0f + fabsf(value));
}

static float highpass_filter(float input)
{
    const float cutoff_hz = 25.0f;
    const float rc = 1.0f / (2.0f * 3.14159265358979323846f * cutoff_hz);
    const float dt = 1.0f / g_sample_rate;
    const float alpha = rc / (rc + dt);

    float hp = alpha * (g_hp_y_prev + input - g_hp_x_prev);
    g_hp_x_prev = input;
    g_hp_y_prev = hp;
    return hp;
}

static float noise_gate(float input)
{
    const float threshold = 0.015f;
    return (fabsf(input) < threshold) ? 0.0f : input;
}

static float limiter(float input)
{
    const float threshold = 0.96f;
    if (input > threshold)
    {
        return threshold + (input - threshold) * 0.2f;
    }
    if (input < -threshold)
    {
        return -threshold + (input + threshold) * 0.2f;
    }
    return input;
}

float DSP_ProcessSample(float input)
{
    float signal = highpass_filter(input);
    signal = noise_gate(signal);

    if (g_features.bypass != 0U)
    {
        return limiter(signal);
    }

    signal *= g_features.drive;
    if (g_features.distortion_enabled != 0U)
    {
        signal = Distortion_Process(signal);
    }

    signal = eq_process(signal);
    signal = Chorus_Process(signal);
    signal = Delay_Process(signal);
    signal = Reverb_Process(signal);

    float tone = signal * 0.8f + g_tone_state * 0.2f;
    g_tone_state = tone;

    return limiter(tone);
}

void DSP_ProcessBlock(const uint16_t *adc_in, int16_t *i2s_out, uint32_t count)
{
    float level_accum = 0.0f;

    for (uint32_t i = 0; i < count; ++i)
    {
        float centered = ((float)adc_in[i] - 2048.0f) / 2048.0f;
        float processed = DSP_ProcessSample(centered);
        int16_t sample = (int16_t)(processed * 32767.0f);

        if (processed < 0.0f)
        {
            level_accum -= processed;
        }
        else
        {
            level_accum += processed;
        }

        i2s_out[2U * i] = sample;
        i2s_out[2U * i + 1U] = sample;
    }

    g_audio_level = level_accum / (float)count;
}
