#include "ssd1309_api.h"

/* Command Functions */
void ssd1309_set_ram_pointer(Ssd1309Defines Screen);

/* I2C functions */
void ssd_write(Ssd1309Defines Screen);
size_t load_i2c_buffer(Ssd1309Defines Screen);

/* API Functions */
void   ssd_write(Ssd1309Defines Screen);
void   ssd1309_startup(Ssd1309Defines Screen);
void   ssd1309_write_bitmap(Ssd1309Defines Screen);
void   ssd1309_ramWrite(Ssd1309Defines Screen);
size_t ssd1309_write_number(Ssd1309Defines Screen); 
void   ssd1309_cls(Ssd1309Defines Screen);
size_t ssd1309_print(Ssd1309Defines Screen);
void   blinking_cursor(Ssd1309Defines Screen);



