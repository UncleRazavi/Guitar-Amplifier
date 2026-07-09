#include "effects.h"

static float distortion_drive = 1.6f;
static float distortion_tone = 0.9f;

static float soft_clip(float x)
{
    if (x > 1.0f)
    {
        return 1.0f;
    }
    if (x < -1.0f)
    {
        return -1.0f;
    }
    return x * (27.0f + x * x) / (27.0f + 9.0f * x * x);
}

float Distortion_Process(float input)
{
    float driven = input * distortion_drive;
    float clipped = soft_clip(driven);
    float shaped = clipped * distortion_tone + input * (1.0f - distortion_tone);
    return shaped;
}

void Effects_SetDistortionDrive(float drive)
{
    if (drive < 0.5f)
    {
        distortion_drive = 0.5f;
    }
    else if (drive > 3.0f)
    {
        distortion_drive = 3.0f;
    }
    else
    {
        distortion_drive = drive;
    }
}

void Effects_SetDistortionTone(float tone)
{
    if (tone < 0.0f)
    {
        distortion_tone = 0.0f;
    }
    else if (tone > 1.0f)
    {
        distortion_tone = 1.0f;
    }
    else
    {
        distortion_tone = tone;
    }
}
