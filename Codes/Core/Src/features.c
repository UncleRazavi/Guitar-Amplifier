#include "features.h"
#include "effects.h"

#include <stdint.h>

Features_t g_features;

static float clampf_range(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}

void Features_Init(void)
{
    g_features.bypass = 0;
    g_features.distortion_enabled = 1;
    g_features.drive = 1.1f;
    g_features.bass = 1.0f;
    g_features.mid = 1.0f;
    g_features.treble = 1.0f;
    g_features.delay_ms = 220.0f;
    g_features.delay_feedback = 0.35f;
    g_features.reverb_mix = 0.22f;
    g_features.chorus_depth = 0.32f;
    g_features.chorus_rate_hz = 0.9f;
    Features_SetPreset(FEATURE_PRESET_CLEAN);
}

void Features_SetPreset(FeaturePreset_t preset)
{
    g_features.preset = preset;
    switch (preset)
    {
        case FEATURE_PRESET_CRUNCH:
            g_features.distortion_enabled = 1;
            g_features.drive = 1.5f;
            g_features.bass = 1.2f;
            g_features.mid = 1.1f;
            g_features.treble = 0.95f;
            g_features.delay_ms = 170.0f;
            g_features.delay_feedback = 0.28f;
            g_features.reverb_mix = 0.18f;
            g_features.chorus_depth = 0.18f;
            g_features.chorus_rate_hz = 0.75f;
            break;
        case FEATURE_PRESET_LEAD:
            g_features.distortion_enabled = 1;
            g_features.drive = 1.9f;
            g_features.bass = 1.05f;
            g_features.mid = 1.25f;
            g_features.treble = 1.1f;
            g_features.delay_ms = 180.0f;
            g_features.delay_feedback = 0.32f;
            g_features.reverb_mix = 0.26f;
            g_features.chorus_depth = 0.22f;
            g_features.chorus_rate_hz = 0.9f;
            break;
        case FEATURE_PRESET_METAL:
            g_features.distortion_enabled = 1;
            g_features.drive = 2.2f;
            g_features.bass = 1.3f;
            g_features.mid = 0.9f;
            g_features.treble = 1.1f;
            g_features.delay_ms = 90.0f;
            g_features.delay_feedback = 0.25f;
            g_features.reverb_mix = 0.14f;
            g_features.chorus_depth = 0.12f;
            g_features.chorus_rate_hz = 0.7f;
            break;
        case FEATURE_PRESET_AMBIENT:
            g_features.distortion_enabled = 0;
            g_features.drive = 0.85f;
            g_features.bass = 0.95f;
            g_features.mid = 1.0f;
            g_features.treble = 1.2f;
            g_features.delay_ms = 420.0f;
            g_features.delay_feedback = 0.5f;
            g_features.reverb_mix = 0.45f;
            g_features.chorus_depth = 0.45f;
            g_features.chorus_rate_hz = 0.4f;
            break;
        case FEATURE_PRESET_CLEAN:
        default:
            g_features.distortion_enabled = 0;
            g_features.drive = 0.9f;
            g_features.bass = 1.0f;
            g_features.mid = 1.0f;
            g_features.treble = 1.0f;
            g_features.delay_ms = 200.0f;
            g_features.delay_feedback = 0.25f;
            g_features.reverb_mix = 0.18f;
            g_features.chorus_depth = 0.24f;
            g_features.chorus_rate_hz = 0.8f;
            break;
    }

    Features_SetDrive(g_features.drive);
    Features_SetTone(g_features.bass, g_features.mid, g_features.treble);
    Features_SetDelay(g_features.delay_ms, g_features.delay_feedback);
    Features_SetReverbMix(g_features.reverb_mix);
    Features_SetChorus(g_features.chorus_depth, g_features.chorus_rate_hz);
}

void Features_SetBypass(uint8_t bypass)
{
    g_features.bypass = bypass ? 1 : 0;
}

void Features_SetDrive(float drive)
{
    g_features.drive = clampf_range(drive, 0.5f, 3.0f);
    Effects_SetDistortionDrive(g_features.drive);
}

void Features_SetTone(float bass, float mid, float treble)
{
    g_features.bass = clampf_range(bass, 0.5f, 2.0f);
    g_features.mid = clampf_range(mid, 0.5f, 2.0f);
    g_features.treble = clampf_range(treble, 0.5f, 2.0f);
    Effects_SetEQ(g_features.bass, g_features.mid, g_features.treble);
}

void Features_SetDelay(float delay_ms, float feedback)
{
    g_features.delay_ms = clampf_range(delay_ms, 20.0f, 600.0f);
    g_features.delay_feedback = clampf_range(feedback, 0.0f, 0.8f);
    Effects_SetDelayTime(g_features.delay_ms);
    Effects_SetDelayFeedback(g_features.delay_feedback);
}

void Features_SetReverbMix(float mix)
{
    g_features.reverb_mix = clampf_range(mix, 0.0f, 0.7f);
    Effects_SetReverbMix(g_features.reverb_mix);
}

void Features_SetChorus(float depth, float rate_hz)
{
    g_features.chorus_depth = clampf_range(depth, 0.0f, 0.6f);
    g_features.chorus_rate_hz = clampf_range(rate_hz, 0.1f, 2.0f);
    Effects_SetChorusDepth(g_features.chorus_depth);
    Effects_SetChorusRate(g_features.chorus_rate_hz);
}
