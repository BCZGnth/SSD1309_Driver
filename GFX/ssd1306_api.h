#ifndef _SSD1306_API
#define _SSD1306_API

#include <stdio.h>
#include <stdint.h>
#include "ssd1306_font.h"

#ifndef __delay_ms
#error MCC does not define __delay_ms Macro. Is _XTAL_FREQ defined?
#endif

void   ssd1306_write_bitmap(ScreenDefines Screen, Ssd1306WriteBitmap);
void   ssd1306_ramWrite(ScreenDefines Screen, Ssd1306RamWrite args);
size_t ssd1306_write_number(ScreenDefines Screen, Ssd1306WriteNumber args);
void   ssd1306_cls(ScreenDefines Screen);
void   ssd1306_clear_line(ScreenDefines Screen, Ssd1306ClearLine args);
size_t ssd1306_print(ScreenDefines Screen, Ssd1306Print args);
void   ssd1306_blinking_cursor(ScreenDefines Screen, Ssd1306Cursor args);


#endif // _SSD1306_API