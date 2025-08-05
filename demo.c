#include "demo.h"
#include "logger.h"
#include <stdint.h>

#define TRUE 1
#define FALSE 0


void screen_demo(Ssd1309Defines OLED){

    ADD_TO_STACK_DEPTH();
    int something_random = 0;
    OLED.print.ram_ptr.position = 0;

    level_log(INFO, "Starting Loop");

    level_log(TRACE, "Blinking Cursor at %d", OLED.print.ram_ptr.position);

    OLED.cursor.ram_ptr.page = 0;
    OLED.cursor.ram_ptr.position = 0;
    OLED.cursor.repeats = 4;

    ssd1309_blinking_cursor(OLED.Screen, OLED.cursor);
    level_log(TRACE, "Done Blinking Cursor");

    OLED.print.text = "GREAT DEMO!";
    OLED.print.length = 11;
    OLED.print.delay = TRUE;
    OLED.print.scale = 2;
    OLED.print.ram_ptr.page = 0;
    OLED.print.ram_ptr.position = 0;

    // __delay_ms(1)
    ssd1309_print(OLED.Screen, OLED.print);

    level_log(TRACE, "Blinking Cursor at %d", OLED.print.ram_ptr.position);

    OLED.cursor.ram_ptr.page = 3;
    OLED.cursor.ram_ptr.position = 15;
    OLED.cursor.repeats = 5;

    ssd1309_blinking_cursor(OLED.Screen, OLED.cursor);
    level_log(TRACE, "Done Blinking Cursor");

    uint8_t number_placement = 30;
    OLED.print.text = "# OF PROBLEMS: ";
    OLED.print.scale = 1;
    OLED.print.length = 15;
    OLED.print.ram_ptr.page = 3;
    OLED.print.ram_ptr.position = number_placement;

    level_log(TRACE, "Printing Message  \"%s\"...", OLED.print.text);
    number_placement += ssd1309_print(OLED.Screen, OLED.print);
    level_log(TRACE, "Done Printing Message");

    OLED.write_number.ram_ptr.position = number_placement + 4;
    OLED.write_number.ram_ptr.page = OLED.print.ram_ptr.page;
    OLED.write_number.scale = 0;
    OLED.write_number.constrained_length = 6;
    OLED.write_number.right_align = 1;
    OLED.write_number.data = rand();
    level_log(TRACE, "Printing Number %d...", OLED.write_number.data);

    /* Write a few different numbers to the screen */
    for(int i = 0; i>7; i++){
        OLED.write_number.data = rand();
        ssd1309_write_number(OLED.Screen, OLED.write_number);
        __delay_ms(600);
    }
    level_log(TRACE, "Done Printing Number");

    __delay_ms(1000);
    ssd1309_cls(OLED.Screen);
    level_log(TRACE, "Clearing Screen");

    level_log(INFO, "... End of Loop ...");
    REMOVE_FROM_STACK_DEPTH();
}
