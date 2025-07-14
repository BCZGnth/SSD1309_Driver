#ifndef _SSD1306_BASE
#define _SSD1306_BASE

#include <stdint.h>

#include "system.h" 
// Also this is not a good include...
// Taking out this include because it is not relavant.

const uint8_t SSD1306_COMMAND_BYTE = 0x00;
const uint8_t SSD1306_RAM_WRITE_BYTE = 0x40;
const uint8_t SSD1306_ADDRESS = 0x3c;


// uint8_t SSD1306_I2C_BUFFER[129]; // This is the buffer that will be used to send data to the SSD1306 screen.
// // It is 129 bytes long because the screen is 128x64 pixels and each byte represents a column of 8 pixels.
// // Every I2C command begins with a control byte, which gives 128 bytes of data to the screen.

/* 
 * Command Structures
 *
 * Ssd1306CommandLengths: Another not especially helpful structure... 
 * 
 * Ssd1306RamPointer
 */
typedef struct {
    uint8_t set_ram_pointer;
    uint8_t send_command;
    uint8_t startup;
    uint8_t init;
} Ssd1306CommandLengths;

typedef struct {
    uint8_t page;
    uint8_t position;
} Ssd1306RamPointer;


/* Font Structures 
 * 
 * FontOffset: 
 * CharAttributes: 
 */
typedef struct {
    uint8_t* pfont;
    uint8_t ascii;
    uint8_t control;
} FontOffset;

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t pad;
    uint8_t width_pad;
} CharAttributes;

/* 
 * SSD1306 Screen Defines:
 * 
 * This is the main structure used in data transmission to the screen.
 * */
typedef struct {
    uint8_t ScreenHeight;
    uint8_t ScreenWidth;

    uint8_t* pbuffer;
    unsigned int buffer_size;

    uint8_t* startup_buffer;
    uint8_t startup_size;

    uint8_t i2c_address;

    Ssd1306RamPointer zeroed_ram_ptr;

    FontOffset offset;
    CharAttributes character;
} ScreenDefines;



/* 
 * API Structures
 *
 * Ssd1306WriteNumber
 * 
 * Ssd1306Print
 * 
 * Ssd1306Prnt
 * 
 * Ssd1306CommandLengths
 * 
 * Ssd1306Cursor
 * 
 * Ssd1306WriteBitmap
 * 
 * Ssd1306WriteNumber
 * 
 * ScreenStringPerLine
 */

typedef struct {
    uint8_t line_length;
    char* line0;
    char* line1;
    char* line2;
    char* line3;
    char* line4;
    char* line5;
    char* line6;
    char* line7;
} ScreenStringPerLine;

typedef struct {
    int data;
    uint8_t constrained_length; // make a maximum number of characters that can be printed. (neede to right align the characters.)
    uint8_t right_align;
    uint8_t scale;

    Ssd1306RamPointer ram_ptr;
} Ssd1306WriteNumber;

typedef struct {
    const char * text;
    uint8_t      length;
    uint8_t      delay;
    uint8_t      scale;

    Ssd1306RamPointer ram_ptr;
} Ssd1306Print;

/* A combination of the Print and Write number structures */
typedef struct {
    const char * text;
    int          data;
    uint8_t      msg_length;
    uint8_t      constrained_length; // make a maximum number of characters that can be printed. (neede to right align the characters.)
    uint8_t      right_align;
    uint8_t      delay;
    uint8_t      scale;

    Ssd1306RamPointer ram_ptr;
} Ssd1306Prnt;

typedef struct {
    uint8_t * pbitmap;
    uint8_t   xstart;
    uint8_t   ystart;
    uint8_t   xlength;
    uint8_t   ylength;

    Ssd1306RamPointer ram_ptr;
} Ssd1306WriteBitmap;

typedef struct {
    uint8_t repeats;

    Ssd1306RamPointer ram_ptr;
} Ssd1306Cursor;

typedef struct {
    uint8_t* bitmap;
    uint8_t bitmap_length;

    Ssd1306RamPointer ram_ptr;
} Ssd1306RamWrite;

typedef struct {
    uint8_t start_page;
    uint8_t end_page;

} Ssd1306ClearLine;

typedef struct {
    ScreenDefines Screen;
    Ssd1306WriteNumber write_number;
    Ssd1306Print print;
    Ssd1306Prnt PRINT;
    Ssd1306WriteBitmap write_bitmap;
    Ssd1306Cursor cursor;
    Ssd1306RamWrite ram_write;
    Ssd1306RamPointer ram_ptr;
    Ssd1306ClearLine clr_line;
    ScreenStringPerLine screen_strings;
} Ssd1306Defines;


#endif // _SSD1306_BASE