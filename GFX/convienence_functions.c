#include "convienence_functions.h"

/* Command Functions */
void ssd1309_set_ram_pointer(Ssd1309Defines Screen) {
    if(Screen.Screen.pbuffer) {
        ssd1309_set_ram_pointer(Screen.Screen, Screen.ram_ptr);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

/* API Functions */
void   ssd1309_write_bitmap(Ssd1309Defines Screen) {
    if(Screen.Screen.pbuffer) {
        ssd1309_write_bitmap(Screen.Screen, Screen.write_bitmap);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

void   ssd1309_ramWrite(Ssd1309Defines Screen) {
    if(Screen.Screen.pbuffer) {
        ssd1309_ramWrite(Screen.Screen, Screen.ram_write);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

size_t ssd1309_write_number(Ssd1309Defines Screen) {
    if(Screen.Screen.pbuffer) {
        return ssd1309_write_number(Screen.Screen, Screen.write_number); 
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

void   ssd1309_cls(Ssd1309Defines Screen) {

    if(Screen.Screen.pbuffer) {
        ssd1309_cls(Screen.Screen);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

size_t ssd1309_print(Ssd1309Defines Screen) {
    if(Screen.Screen.pbuffer) {
        return ssd1309_print(Screen.Screen, Screen.print);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

void   blinking_cursor(Ssd1309Defines Screen) {
    if(Screen.Screen.pbuffer) {
        blinking_cursor(Screen.Screen, Screen.cursor);
    } else{
        // Error Handling
        level_log(TRACE, "Screen.Screen.pbuffer not defined")
    }
}

