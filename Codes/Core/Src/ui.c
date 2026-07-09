#include "ui.h"
#include "board.h"
#include "features.h"
#include "dsp.h"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

extern SPI_HandleTypeDef hspi1;

static const uint8_t font_upper[26][5] = {
    {0x7C,0x12,0x11,0x12,0x7C}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x0C,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x3F,0x40,0x38,0x40,0x3F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}  // Z
};

static const uint8_t font_digits[10][5] = {
    {0x3E,0x45,0x49,0x51,0x3E}, // 0
    {0x00,0x21,0x7F,0x01,0x00}, // 1
    {0x21,0x43,0x45,0x49,0x31}, // 2
    {0x22,0x41,0x49,0x49,0x36}, // 3
    {0x0C,0x14,0x24,0x7F,0x04}, // 4
    {0x72,0x51,0x51,0x51,0x4E}, // 5
    {0x3E,0x49,0x49,0x49,0x26}, // 6
    {0x40,0x47,0x48,0x50,0x60}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x32,0x49,0x49,0x49,0x3E}  // 9
};

static const uint8_t font_symbols[5][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x08,0x08,0x08,0x08,0x08}, // -
    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x00,0x40,0x60,0x00,0x00}, // .
    {0x20,0x10,0x08,0x04,0x02}  // /
};

static uint16_t ui_color_background = 0x18E3;
static uint16_t ui_color_text = 0xFFFF;
static uint16_t ui_color_accent = 0x07E0;
static uint16_t ui_color_warning = 0xF800;
static uint16_t ui_color_bar = 0xFFE0;

static void Board_DisplayCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &cmd, 1U, 100);
}

static void Board_DisplayData(const uint8_t *data, uint16_t size)
{
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)data, size, 200);
}

static void Board_DisplaySetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];
    Board_DisplayCommand(0x2A);
    data[0] = (uint8_t)(x0 >> 8);
    data[1] = (uint8_t)(x0 & 0xFF);
    data[2] = (uint8_t)(x1 >> 8);
    data[3] = (uint8_t)(x1 & 0xFF);
    Board_DisplayData(data, 4);

    Board_DisplayCommand(0x2B);
    data[0] = (uint8_t)(y0 >> 8);
    data[1] = (uint8_t)(y0 & 0xFF);
    data[2] = (uint8_t)(y1 >> 8);
    data[3] = (uint8_t)(y1 & 0xFF);
    Board_DisplayData(data, 4);

    Board_DisplayCommand(0x2C);
}

static void UI_FillScreen(uint16_t color)
{
    uint8_t buffer[64 * 2];
    for (uint16_t i = 0; i < sizeof(buffer); i += 2)
    {
        buffer[i] = (uint8_t)(color >> 8);
        buffer[i + 1] = (uint8_t)color;
    }

    Board_DisplaySetWindow(0, 0, 239, 319);
    uint32_t pixels = 240U * 320U;
    while (pixels > 0U)
    {
        uint16_t batch = (pixels > 64U) ? 64U : (uint16_t)pixels;
        Board_DisplayData(buffer, batch * 2U);
        pixels -= batch;
    }
}

static void UI_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint8_t buffer[64 * 2];
    for (uint16_t i = 0; i < sizeof(buffer); i += 2)
    {
        buffer[i] = (uint8_t)(color >> 8);
        buffer[i + 1] = (uint8_t)color;
    }

    Board_DisplaySetWindow(x, y, x + w - 1, y + h - 1);
    uint32_t pixels = (uint32_t)w * h;
    while (pixels > 0U)
    {
        uint16_t batch = (pixels > 64U) ? 64U : (uint16_t)pixels;
        Board_DisplayData(buffer, batch * 2U);
        pixels -= batch;
    }
}

static const uint8_t *UI_GetGlyph(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return font_upper[c - 'A'];
    }
    if (c >= '0' && c <= '9')
    {
        return font_digits[c - '0'];
    }
    if (c == ' ')
    {
        return font_symbols[0];
    }
    if (c == '-')
    {
        return font_symbols[1];
    }
    if (c == ':')
    {
        return font_symbols[2];
    }
    if (c == '.')
    {
        return font_symbols[3];
    }
    if (c == '/')
    {
        return font_symbols[4];
    }
    return font_symbols[0];
}

static void UI_DrawChar(uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg)
{
    const uint8_t *glyph = UI_GetGlyph(c);
    for (uint8_t col = 0; col < 5; ++col)
    {
        uint8_t bits = glyph[col];
        for (uint8_t row = 0; row < 7; ++row)
        {
            uint16_t color = (bits & (1U << row)) ? fg : bg;
            UI_FillRect(x + col, y + row, 1, 1, color);
        }
    }
    UI_FillRect(x + 5, y, 1, 7, bg);
}

static void UI_DrawString(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg)
{
    while (*text != '\0')
    {
        UI_DrawChar(x, y, *text, fg, bg);
        x += 6;
        ++text;
    }
}

static void UI_DrawMeter(uint16_t x, uint16_t y, uint16_t width, uint16_t height, float level)
{
    UI_FillRect(x, y, width, height, 0x2104);
    uint16_t fill = (uint16_t)((level <= 0.0f) ? 0U : (level >= 1.0f ? width : (uint32_t)(width * level)));
    if (fill > 0U)
    {
        UI_FillRect(x, y, fill, height, ui_color_bar);
    }
}

static void UI_DrawLayout(void)
{
    UI_FillScreen(ui_color_background);
    UI_FillRect(0, 0, 240, 36, 0x08C3);
    UI_DrawString(10, 8, "GUITAR AMP", ui_color_text, 0x08C3);
    UI_DrawString(10, 22, "DIGITAL DSP", ui_color_text, 0x08C3);
    UI_FillRect(10, 46, 220, 70, 0x0000);
    UI_FillRect(10, 126, 220, 70, 0x0000);
    UI_FillRect(10, 206, 220, 70, 0x0000);
    UI_FillRect(10, 286, 220, 24, 0x0000);
    UI_DrawString(12, 50, "PRESET:", ui_color_text, 0x0000);
    UI_DrawString(12, 60, "DRIVE:", ui_color_text, 0x0000);
    UI_DrawString(12, 70, "BASS:", ui_color_text, 0x0000);
    UI_DrawString(12, 130, "MID:", ui_color_text, 0x0000);
    UI_DrawString(12, 140, "TREBLE:", ui_color_text, 0x0000);
    UI_DrawString(12, 150, "DELAY:", ui_color_text, 0x0000);
    UI_DrawString(12, 210, "REVERB:", ui_color_text, 0x0000);
    UI_DrawString(12, 220, "CHORUS:", ui_color_text, 0x0000);
    UI_DrawString(12, 230, "LEVEL:", ui_color_text, 0x0000);
    UI_DrawString(12, 294, "AMP STATUS:", ui_color_text, 0x0000);
}

static void UI_DrawStatus(void)
{
    const char *preset_text;
    switch (g_features.preset)
    {
        case FEATURE_PRESET_CRUNCH: preset_text = "CRUNCH"; break;
        case FEATURE_PRESET_LEAD: preset_text = "LEAD"; break;
        case FEATURE_PRESET_METAL: preset_text = "METAL"; break;
        case FEATURE_PRESET_AMBIENT: preset_text = "AMBIENT"; break;
        case FEATURE_PRESET_CLEAN:
        default: preset_text = "CLEAN"; break;
    }

    char text[24];
    uint16_t base_y = 50;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    UI_DrawString(80, base_y, preset_text, ui_color_text, 0x0000);

    base_y = 60;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.1fx", g_features.drive);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    base_y = 70;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.1f", g_features.bass);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    base_y = 130;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.1f", g_features.mid);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    base_y = 140;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.1f", g_features.treble);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    base_y = 150;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.0fms", g_features.delay_ms);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    base_y = 210;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.2f", g_features.reverb_mix);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    base_y = 220;
    UI_FillRect(80, base_y, 150, 14, 0x0000);
    sprintf(text, "%.2f", g_features.chorus_depth);
    UI_DrawString(80, base_y, text, ui_color_text, 0x0000);

    float level = g_audio_level;
    if (level > 1.0f) level = 1.0f;
    if (level < 0.0f) level = 0.0f;
    UI_DrawMeter(80, 230, 150, 12, level);

    UI_FillRect(120, 294, 110, 14, 0x0000);
    if (Board_AmpFaultActive() != 0U)
    {
        UI_DrawString(120, 294, "FAULT", ui_color_warning, 0x0000);
    }
    else if (g_features.bypass != 0U)
    {
        UI_DrawString(120, 294, "MUTED", ui_color_warning, 0x0000);
    }
    else
    {
        UI_DrawString(120, 294, "READY", ui_color_accent, 0x0000);
    }
}

void UI_Init(void)
{
    Board_DisplayInit();
    UI_DrawLayout();
    UI_DrawStatus();
}

void UI_Update(void)
{
    UI_DrawStatus();
}
