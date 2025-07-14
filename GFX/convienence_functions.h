#include "ssd1306_api.h"

/* Command Functions */
void ssd1306_set_ram_pointer(Ssd1306Defines Screen);

/* I2C functions */
void ssd_write(Ssd1306Defines Screen);
size_t load_i2c_buffer(Ssd1306Defines Screen);

/* API Functions */
void   ssd_write(Ssd1306Defines Screen);
void   ssd1306_startup(Ssd1306Defines Screen);
void   ssd1306_write_bitmap(Ssd1306Defines Screen);
void   ssd1306_ramWrite(Ssd1306Defines Screen);
size_t ssd1306_write_number(Ssd1306Defines Screen); 
void   ssd1306_cls(Ssd1306Defines Screen);
size_t ssd1306_print(Ssd1306Defines Screen);
void   blinking_cursor(Ssd1306Defines Screen);



