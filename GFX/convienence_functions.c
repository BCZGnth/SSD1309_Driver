#include "convienence_functions.h"

/* Command Functions */
void ssd1306_set_ram_pointer(Ssd1306Defines Screen) {
    if(Screen.Screen.pbuffer) {
        ssd1306_set_ram_pointer(Screen.Screen, Screen.ram_ptr);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

/* API Functions */
void   ssd1306_write_bitmap(Ssd1306Defines Screen) {
    if(Screen.Screen.pbuffer) {
        ssd1306_write_bitmap(Screen.Screen, Screen.write_bitmap);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

void   ssd1306_ramWrite(Ssd1306Defines Screen) {
    if(Screen.Screen.pbuffer) {
        ssd1306_ramWrite(Screen.Screen, Screen.ram_write);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

size_t ssd1306_write_number(Ssd1306Defines Screen) {
    if(Screen.Screen.pbuffer) {
        return ssd1306_write_number(Screen.Screen, Screen.write_number); 
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

void   ssd1306_cls(Ssd1306Defines Screen) {

    if(Screen.Screen.pbuffer) {
        ssd1306_cls(Screen.Screen);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

size_t ssd1306_print(Ssd1306Defines Screen) {
    if(Screen.Screen.pbuffer) {
        return ssd1306_print(Screen.Screen, Screen.print);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

void   blinking_cursor(Ssd1306Defines Screen) {
    if(Screen.Screen.pbuffer) {
        blinking_cursor(Screen.Screen, Screen.cursor);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

