/*
 * hsv.c - HSV to RGB Color Conversion
 */

#include "hsv.h"

void hsv_to_rgb(uint16_t hue, uint8_t sat, uint8_t val,
                uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint16_t region, remainder;
    uint8_t p, q, t;

    if (sat == 0) {
        *r = *g = *b = val;
        return;
    }

    region = hue / 60;
    remainder = (hue % 60) * 255 / 60;

    p = (val * (255 - sat)) / 255;
    q = (val * (255 - (sat * remainder) / 255)) / 255;
    t = (val * (255 - (sat * (255 - remainder)) / 255)) / 255;

    switch (region) {
        case 0:
            *r = val; *g = t; *b = p;
            break;
        case 1:
            *r = q; *g = val; *b = p;
            break;
        case 2:
            *r = p; *g = val; *b = t;
            break;
        case 3:
            *r = p; *g = q; *b = val;
            break;
        case 4:
            *r = t; *g = p; *b = val;
            break;
        default:
            *r = val; *g = p; *b = q;
            break;
    }
}
