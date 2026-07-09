/* Simple 3-band EQ implemented with biquad filters.
 * Designed to be lightweight and run on Cortex-M4 with FPU.
 */
#include "effects.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

typedef struct {
    float b0, b1, b2, a1, a2;
    float x1, x2, y1, y2;
} biquad_t;

static biquad_t low, mid, high;
static uint32_t eq_sample_rate = 48000;
static float eq_bass_gain = 1.0f;
static float eq_mid_gain = 1.0f;
static float eq_treble_gain = 1.0f;

static void biquad_init(biquad_t *b)
{
    b->b0 = b->b1 = b->b2 = b->a1 = b->a2 = 0.0f;
    b->x1 = b->x2 = b->y1 = b->y2 = 0.0f;
}

static float biquad_process(biquad_t *b, float x)
{
    float y = b->b0 * x + b->b1 * b->x1 + b->b2 * b->x2 - b->a1 * b->y1 - b->a2 * b->y2;
    b->x2 = b->x1; b->x1 = x;
    b->y2 = b->y1; b->y1 = y;
    return y;
}

/* Create a shelving low/high or peaking biquad using bilinear transform
 * type: 0=low-shelf, 1=peaking, 2=high-shelf
 */
static void biquad_design(biquad_t *b, int type, float fc, float q, float gain_db, uint32_t fs)
{
    float A = powf(10.0f, gain_db / 40.0f);
    float omega = 2.0f * (float)M_PI * fc / (float)fs;
    float sn = sinf(omega);
    float cs = cosf(omega);
    float alpha = sn / (2.0f * q);
    float beta = sqrtf(A) / q;

    float b0,b1,b2,a0,a1,a2;

    if (type == 1) {
        // peaking
        b0 = 1 + alpha * A;
        b1 = -2 * cs;
        b2 = 1 - alpha * A;
        a0 = 1 + alpha / A;
        a1 = -2 * cs;
        a2 = 1 - alpha / A;
    } else if (type == 0) {
        // low shelf
        float sqrtA = sqrtf(A);
        b0 = A*((A+1) - (A-1)*cs + 2*sqrtA*alpha);
        b1 = 2*A*((A-1) - (A+1)*cs);
        b2 = A*((A+1) - (A-1)*cs - 2*sqrtA*alpha);
        a0 = (A+1) + (A-1)*cs + 2*sqrtA*alpha;
        a1 = -2*((A-1) + (A+1)*cs);
        a2 = (A+1) + (A-1)*cs - 2*sqrtA*alpha;
    } else {
        // high shelf
        float sqrtA = sqrtf(A);
        b0 = A*((A+1) + (A-1)*cs + 2*sqrtA*alpha);
        b1 = -2*A*((A-1) + (A+1)*cs);
        b2 = A*((A+1) + (A-1)*cs - 2*sqrtA*alpha);
        a0 = (A+1) - (A-1)*cs + 2*sqrtA*alpha;
        a1 = 2*((A-1) - (A+1)*cs);
        a2 = (A+1) - (A-1)*cs - 2*sqrtA*alpha;
    }

    b->b0 = b0 / a0;
    b->b1 = b1 / a0;
    b->b2 = b2 / a0;
    b->a1 = a1 / a0;
    b->a2 = a2 / a0;
}

/* Public API for EQ used by DSP module */
void eq_init(uint32_t sample_rate)
{
    eq_sample_rate = sample_rate;
    eq_bass_gain = 1.0f;
    eq_mid_gain = 1.0f;
    eq_treble_gain = 1.0f;
    biquad_init(&low); biquad_init(&mid); biquad_init(&high);

    biquad_design(&low, 0, 120.0f, 0.707f, 0.0f, eq_sample_rate);  // low shelf at 120Hz
    biquad_design(&mid, 1, 1000.0f, 0.707f, 0.0f, eq_sample_rate); // peaking at 1kHz
    biquad_design(&high, 2, 5000.0f, 0.707f, 0.0f, eq_sample_rate); // high shelf at 5kHz
}

void Effects_SetEQ(float bass, float mid, float treble)
{
    eq_bass_gain = bass;
    eq_mid_gain = mid;
    eq_treble_gain = treble;

    biquad_design(&low, 0, 120.0f, 0.707f, 6.0f * (bass - 1.0f), eq_sample_rate);
    biquad_design(&mid, 1, 1000.0f, 0.707f, 4.0f * (mid - 1.0f), eq_sample_rate);
    biquad_design(&high, 2, 5000.0f, 0.707f, 6.0f * (treble - 1.0f), eq_sample_rate);
}

float eq_process(float in_sample)
{
    float x = in_sample;
    float y = biquad_process(&low, x) * eq_bass_gain;
    y = biquad_process(&mid, y) * eq_mid_gain;
    y = biquad_process(&high, y) * eq_treble_gain;
    return y;
}
