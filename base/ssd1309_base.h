#ifndef _SSD1309_BASE
#define _SSD1309_BASE

#include <stdint.h>

#include "system.h" 
// Also this is not a good include...
// Taking out this include because it is not relavant.

const uint8_t SSD1309_COMMAND_BYTE = 0x00;
const uint8_t SSD1309_RAM_WRITE_BYTE = 0x40;

// #warning SSD1309: The device address can be 0x78 or 0x79 depending on whether th D/C pin is low or high
// const uint8_t SSD1309_ADDRESS = 0x78;


// uint8_t SSD1309_I2C_BUFFER[129]; // This is the buffer that will be used to send data to the SSD1309 screen.
// // It is 129 bytes long because the screen is 128x64 pixels and each byte represents a column of 8 pixels.
// // Every I2C command begins with a control byte, which gives 128 bytes of data to the screen.

/* 
 * Command Structures
 *
 * Ssd1309CommandLengths: Another not especially helpful structure... 
 * 
 * Ssd1309RamPointer
 */
typedef struct {
    uint8_t set_ram_pointer;
    uint8_t send_command;
    uint8_t startup;
    uint8_t init;
} Ssd1309CommandLengths;

typedef struct {
    uint8_t page;
    uint8_t position;
} Ssd1309RamPointer;


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
 * SSD1309 Screen Defines:
 * 
 * This is the main structure used in data transmission to the screen.
 * */
typedef struct {
    uint8_t  ScreenHeight;
    uint8_t  ScreenWidth;

    uint8_t* pbuffer;
    int      buffer_size;

    uint8_t* startup_buffer;
    uint8_t  startup_size;

    uint8_t  i2c_address;

    uint8_t* rst_lat_port;
    uint8_t  rst_pin;

    Ssd1309RamPointer zeroed_ram_ptr;

    FontOffset offset;
    CharAttributes character;
} ScreenDefines;



/* 
 * API Structures
 *
 * Ssd1309WriteNumber
 * 
 * Ssd1309Print
 * 
 * Ssd1309Prnt
 * 
 * Ssd1309CommandLengths
 * 
 * Ssd1309Cursor
 * 
 * Ssd1309WriteBitmap
 * 
 * Ssd1309WriteNumber
 * 
 * Ssd1309VLine
 * 
 * Ssd1309HLine
 * 
 * Ssd1309Rect
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

    Ssd1309RamPointer ram_ptr;
} Ssd1309WriteNumber;

typedef struct {
    const char * text;
    uint8_t      length;
    uint8_t      delay;
    uint8_t      scale;

    Ssd1309RamPointer ram_ptr;
} Ssd1309Print;

/* A combination of the Print and Write number structures */
typedef struct {
    const char * text;
    int          data;
    uint8_t      msg_length;
    uint8_t      constrained_length; // make a maximum number of characters that can be printed. (neede to right align the characters.)
    uint8_t      right_align;
    uint8_t      delay;
    uint8_t      scale;

    Ssd1309RamPointer ram_ptr;
} Ssd1309Prnt;

typedef struct {
    uint8_t * pbitmap;
    uint8_t   length;
    uint8_t   xstart;
    uint8_t   ystart;
    uint8_t   xend;
    uint8_t   yend;

    Ssd1309RamPointer ram_ptr;
} Ssd1309WriteBitmap;

typedef struct {
    uint8_t repeats;

    Ssd1309RamPointer ram_ptr;
} Ssd1309Cursor;

typedef struct {
    uint8_t* bitmap;
    uint8_t bitmap_length;

    Ssd1309RamPointer ram_ptr;
} Ssd1309RamWrite;

typedef struct {
    uint8_t start_page;
    uint8_t end_page;

} Ssd1309ClearLine;

typedef struct {
    uint8_t xstart;
    uint8_t ystart;
    uint8_t length;
} Ssd1309HVLine;

typedef struct {
    uint8_t xstart;
    uint8_t ystart;
    uint8_t xend;
    uint8_t yend;
} Ssd1309Rect;

typedef struct {
    ScreenDefines Screen;
    Ssd1309WriteNumber write_number;
    Ssd1309Print print;
    Ssd1309Prnt PRINT;
    Ssd1309WriteBitmap write_bitmap;
    Ssd1309Cursor cursor;
    Ssd1309RamWrite ram_write;
    Ssd1309RamPointer ram_ptr;
    Ssd1309ClearLine clr_line;
    ScreenStringPerLine screen_strings;
} Ssd1309Defines;


#endif // _SSD1309_BASE