#include "ssd1309_shapes.h"
#include "system.h"
#include <string.h>
/**
 * Takes a start x,y coordinate and a length of pixels to draw a line
 */
void ssd1309_draw_vline(ScreenDefines Screen, Ssd1309VLine Line){
    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, VERTICAL_ADDRESSING);

    // There are only 128 columns on the screen hence the bit-mask
    // If you try to give a bigger number than 128, only the least significant 7 bits will be accounted for
    ssd1309_send_command(Screen, SET_COLUMN_ADDRESS, Line.xstart, 123);

    // One can only start at the top of a page, so the page will need to be set as well as the first and last bytes of the line 
    uint8_t page = Line.ystart / 8; // There are 8 pages
    uint8_t ymod = Line.ystart % 8;
    if(ymod) { page += 1; }

    uint8_t line_byte_len = Line.length / 8;
    uint8_t line_len_mod = Line.length % 8;
    if(line_len_mod) { line_byte_len += 1; }
    uint8_t page_end = 0x7;
    ssd1309_send_command(Screen, SET_PAGE_ADDRESS, page, page_end);

    // This is our counter variable to keep track of bytes written to the display
    uint8_t j = 0;

    // Fill the useful part of the i2c buffer with zeros
    memset(Screen.pbuffer, 0, 10);
    memset(Screen.pbuffer + j++, SSD1309_RAM_WRITE_BYTE, 1);

    // To calculate the first byte of the buffer
    if(ymod){
        // First byte:
        //  we will calculate how many bytes there will be in the first byte
            uint8_t num_of_bits = 8 - ymod; // ymod is a remainder of blank bits in the first byte, hence the subtraction
        // account in the length for the first byte
            Line.length -= num_of_bits;
            uint8_t first = 0;
        // Convert the number of bits into actual bits
            for(int i = num_of_bits; i > 0; i--) {
                first |= (1 << (8 - i));
            }

        // Start filling the i2c buffer
        memset(Screen.pbuffer + j++, first, 1);
    }

    while(Line.length / 8) {
        memset(Screen.pbuffer + j++, 0xff, 1);
        Line.length -= 8;
    }

    // Last Byte:
    if(Line.length){
        uint8_t last = 0;
        for(int i = Line.length; i > 0; i--) {
            // fill the byte MSB to LSB that way the LSB(s) will be zeros
            last |= (1 << (i-1));
        }
        memset(Screen.pbuffer + j++, last, 1);
    }


    uint8_t length = j;

    ssd_write(Screen, length);

    ssd1309_reset_addressing(Screen);
}

void ssd1309_draw_hline(ScreenDefines Screen, Ssd1309HLine Line){
    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, PAGE_ADDRESSING);

    // There are only 128 columns on the screen hence the bit-mask
    // If you try to give a bigger number than 128, only the least significant 7 bits will be accounted for

    // One can only start at the top of a page, so the page will need to be set as well as the first and last bytes of the line 
    uint8_t page = Line.ystart / 8; // There are 8 pages
    if((Line.ystart % 8) != 0) { page += 1; }

    Ssd1309RamPointer ram_ptr = {
        .page = page - 1,
        .position = Line.xstart
    };
    ssd1309_set_ram_pointer(Screen, ram_ptr);

    // This will be a RAM write
    memset(Screen.pbuffer, 0, 129);
    memset(Screen.pbuffer, SSD1309_RAM_WRITE_BYTE, 1);


    uint8_t ymod = Line.ystart % 8;

    // to set a single bit in the byte in order to write the correct line
    // (horizontal addressing mode does not draw lines of pixels... :(  )
    uint8_t pixel_in_byte = (1 << (7 - ymod));

    // fill the buffer with the line information
    memset(Screen.pbuffer + 1, pixel_in_byte, Line.length);

    ssd_write(Screen, Line.length + 1);

    ssd1309_reset_addressing(Screen);
}

void ssd1309_draw_rect(ScreenDefines Screen, Ssd1309Rect Rect){
    // ssd1309_send_command(Screen, SET_DISPLAY_ON_OUTPUT_IGNORES_RAM_CONTENT);

    uint8_t hlength = (Rect.xend - Rect.xstart - 1);
    uint8_t hxstart = Rect.xstart + 1;
    uint8_t vlength = (Rect.yend - Rect.ystart);

    Ssd1309HLine hline1 = {
        .xstart = hxstart,
        .ystart = Rect.ystart,
        .length = hlength
    };

    Ssd1309VLine vline1 = {
        .xstart = Rect.xstart,
        .ystart = Rect.ystart,
        .length = vlength
    };

    Ssd1309HLine hline2 = {
        .xstart = hxstart,
        .ystart = Rect.yend,
        .length = hlength
    };

    Ssd1309VLine vline2 = {
        .xstart = Rect.xend,
        .ystart = Rect.ystart,
        .length = vlength
    };

    ssd1309_draw_vline(Screen, vline1);
    ssd1309_draw_vline(Screen, vline2);
    ssd1309_draw_hline(Screen, hline1);
    ssd1309_draw_hline(Screen, hline2);

    // ssd1309_send_command(Screen, SET_DISPLAY_ON_RESUME_RAM_CONTENT_DISPLAY);
}

