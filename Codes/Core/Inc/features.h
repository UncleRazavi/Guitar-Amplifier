#ifndef INC_FEATURES_H_
#define INC_FEATURES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FEATURE_PRESET_CLEAN = 0,
    FEATURE_PRESET_CRUNCH,
    FEATURE_PRESET_LEAD,
    FEATURE_PRESET_METAL,
    FEATURE_PRESET_AMBIENT,
    FEATURE_PRESET_COUNT
} FeaturePreset_t;

typedef struct
{
    FeaturePreset_t preset;
    uint8_t bypass;
    uint8_t distortion_enabled;
    float drive;
    float bass;
    float mid;
    float treble;
    float delay_ms;
    float delay_feedback;
    float reverb_mix;
    float chorus_depth;
    float chorus_rate_hz;
} Features_t;

extern Features_t g_features;

void Features_Init(void);
void Features_SetPreset(FeaturePreset_t preset);
void Features_SetBypass(uint8_t bypass);
void Features_SetDrive(float drive);
void Features_SetTone(float bass, float mid, float treble);
void Features_SetDelay(float delay_ms, float feedback);
void Features_SetReverbMix(float mix);
void Features_SetChorus(float depth, float rate_hz);

#ifdef __cplusplus
}
#endif

#endif /* INC_FEATURES_H_ */
