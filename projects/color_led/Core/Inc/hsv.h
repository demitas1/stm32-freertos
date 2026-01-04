/*
 * hsv.h - HSV to RGB Color Conversion
 */

#ifndef HSV_H
#define HSV_H

#include <stdint.h>

/**
 * @brief Convert HSV color to RGB
 * @param hue Hue angle (0-359)
 * @param sat Saturation (0-255)
 * @param val Value/Brightness (0-255)
 * @param r   Output red (0-255)
 * @param g   Output green (0-255)
 * @param b   Output blue (0-255)
 */
void hsv_to_rgb(uint16_t hue, uint8_t sat, uint8_t val,
                uint8_t *r, uint8_t *g, uint8_t *b);

#endif /* HSV_H */
