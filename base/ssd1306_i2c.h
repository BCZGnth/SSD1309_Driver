#ifndef _SSD1306_I2C
#define _SSD1306_I2C

#include "ssd1306_base.h"

void ssd_write(ScreenDefines Screen, size_t data_length);
size_t load_i2c_buffer(ScreenDefines Screen, uint8_t *psetup_bytes, size_t setup_length, uint8_t *pdata, size_t data_length);

#endif // _SSD1306_I2C