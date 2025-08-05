#ifndef _SSD1309_API
#define _SSD1309_API

#include <stdio.h>
#include <stdint.h>
#include "ssd1309_font.h"

#ifndef __delay_ms
#error MCC does not define __delay_ms Macro. Is _XTAL_FREQ defined?
#endif

void   ssd1309_write_bitmap(ScreenDefines Screen, Ssd1309WriteBitmap);
void   ssd1309_ramWrite(ScreenDefines Screen, Ssd1309RamWrite args);
size_t ssd1309_write_number(ScreenDefines Screen, Ssd1309WriteNumber args);
void   ssd1309_cls(ScreenDefines Screen);
void   ssd1309_clear_line(ScreenDefines Screen, Ssd1309ClearLine args);
size_t ssd1309_print(ScreenDefines Screen, Ssd1309Print args);
void   ssd1309_blinking_cursor(ScreenDefines Screen, Ssd1309Cursor args);


#endif // _SSD1309_API