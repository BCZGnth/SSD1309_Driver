#include "ssd1309_commands.h"


#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"


/*
 * @brief a function that sends commands to the ssd1309 screen
 * @param command: The command that should be sent to the screen and also the number of extra paramaters in the function. the first byte is the number of extra paramaters, and the second byte is the actual command
 * 
*/
void ssd1309_send_command(ScreenDefines Screen, uint16_t command, ...){

    ADD_TO_STACK_DEPTH(); // ssd1309_send_command
    level_log(TRACE, "Sending command: %X", command);

    uint8_t tmp, i, n_o_args, cmd, j;
    j = 0;

    n_o_args = (command >> 8) & 0xFF;
    cmd = command & 0xFF;

    memcpy(Screen.pbuffer, &SSD1309_COMMAND_BYTE, 1);
    memcpy(Screen.pbuffer + 1, &cmd, 1);


    if(n_o_args){

        va_list ap;

        va_start (ap, 1);
        for(i=0; i<n_o_args; i++){
            tmp = va_arg (ap, uint8_t);
            if((i + 2) > Screen.buffer_size) level_log(ERROR, "Cannot write more than %d bytes to the I2C buffer", Screen.buffer_size);
            memcpy(Screen.pbuffer + i + 2, &tmp, 1);
        }
        va_end (ap);
    }

    ssd_write(Screen, n_o_args + 2);

    level_log(TRACE, "SSD1309: Command %X Sent", command);
    REMOVE_FROM_STACK_DEPTH(); // ssd1309_send_command
}


void ssd1309_set_ram_pointer(ScreenDefines Screen, Ssd1309RamPointer args) {

    ADD_TO_STACK_DEPTH(); // ssd1309_set_ram_pointer

    size_t control_length, data_length;
    control_length = 1;
    data_length = 3;

    level_log(TRACE, "Setting RAM Pointer: page %u, position %u", args.page, args.position);


    /* Calculate the MSB and LSB commands */
    uint8_t msb_column = ((args.position >> 4) & 0x0F) | 0x10;
    uint8_t lsb_column = args.position & 0x0F;
    
    uint8_t msg[3] = {(args.page | 0xB0), msb_column, lsb_column};

    size_t size = load_i2c_buffer(Screen, (&SSD1309_COMMAND_BYTE), control_length, (&msg[0]), data_length);

    ssd_write(Screen, size);

    level_log(TRACE, "SSD1309: RAM Pointer Set to page %u, column %u", args.page, args.position);
    REMOVE_FROM_STACK_DEPTH(); // ssd1309_set_ram_pointer

}

ssd1309_reset_addressing(ScreenDefines Screen){
    ssd1309_send_command(Screen, SET_COLUMN_ADDRESS, 0, 127);
    ssd1309_send_command(Screen, SET_PAGE_ADDRESS, 0, 7);
    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, HORIZONTAL_ADDRESSING);
}

/* Sends the startup sequence to the SSD1309 for information on the startup sequence,
 * refer to the definition of the ssd1309_startup_sequence and the DATASHEET which can be found at:
 * https://cdn-shop.adafruit.com/datasheets/SSD1309.pdf  */
void ssd1309_startup(ScreenDefines Screen)
{
    ADD_TO_STACK_DEPTH(); // ssd1309_startup
    level_log(TRACE, "Starting up the SSD1309");

    // The Sd1309 needs to be reset before it will accept any I2C communications
    ssd1309_reset(Screen);

    size_t size = load_i2c_buffer(Screen, (&SSD1309_COMMAND_BYTE), Screen.offset.control, Screen.startup_buffer, Screen.startup_size);
    ssd_write(Screen, size);

    ssd1309_cls(Screen);

    size = load_i2c_buffer(Screen, (&SSD1309_COMMAND_BYTE), Screen.offset.control, &initializer, initializer_length);
    ssd_write(Screen, size);

    level_log(TRACE, "SSD1309: Startup Sequence Done");
    REMOVE_FROM_STACK_DEPTH(); // ssd1309_startup
}

void ssd1309_reset(ScreenDefines Screen){
    /* Reset the SSD1309 */
    // SCREEN_RST_SetHigh();
    *(Screen.rst_lat_port) |= (1 << Screen.rst_pin);
    __delay_ms(100);
    // SCREEN_RST_SetLow();
    *(Screen.rst_lat_port) &= !(1 << Screen.rst_pin);
    __delay_ms(3);
    // SCREEN_RST_SetHigh();
    *(Screen.rst_lat_port) |= (1 << Screen.rst_pin);
    /* End of reset */
}


/* ssd1309 Needs to be called before anything else is called so that the i2c buffer pointer is defined */
extern uint8_t ascii_font[1]; // Don't know if this is a good way to code or if I should re-arrange the file include order.


/**
 *  ssd1309 initilization function
 * @return Ssd1306Defines: A structure that holds all of the data used to write to the screen
 * 
 * @param i2c_buffer: The buffer to store i2c data before we write it to the screen
 * @param buffer_size: The size of the i2c buffer
 * @param screen_i2c_address: 7-bit i2c address for the device ( b6, b5, b4, b3, b2, b1, b0 ) The R/W bit will be added in the code depeending on whether we are reading or not 
 * 
 * This function calls:
 *      ssd1309_startup
 *          Sends the initializer commands over i2c 
 * 
 *      ssd1309_cls
 *          Writes all zeros to the GDDRAM
 */
Ssd1309Defines ssd1309_init(uint8_t* i2c_buffer, unsigned int buffer_size, uint8_t screen_i2c_address, uint8_t* rst_lat_port, uint8_t rst_pin){

    Ssd1309Defines Screen = {

        .Screen = {
            .ScreenHeight = 64,
            .ScreenWidth = 128,
            .pbuffer = i2c_buffer,
            .buffer_size = buffer_size,
            .startup_buffer = (&ssd1309_startup_sequence),
            .startup_size = (sizeof(ssd1309_startup_sequence) / sizeof(ssd1309_startup_sequence[0])),
            .i2c_address = screen_i2c_address,
            .rst_lat_port = rst_lat_port,
            .rst_pin = rst_pin,

            .offset = {
                .pfont = (&ascii_font[0]),
                .ascii = 32, // The first character in the local font is a space, which is ascii code 0x20 (32 decimal)
                .control = 1
            },

            .character = {
                .width = 5,
                .height = 8,
                .pad = 1,
                .width_pad = 6 // The width of the character in the i2c buffer is 5 bytes, but a pad byte is added in between each character, so the width of the character in the i2c buffer is 6 bytes    
            }
        },

        .write_number = {0},
        .print = {0},
        .PRINT = {0},
        .write_bitmap = {0},
        .cursor = {0},
        .ram_write = {0},
        .ram_ptr = {0},
        .clr_line = {0},
        .screen_strings = {
            .line_length = 21,
            .line0 = 0,
            .line1 = 0,
            .line2 = 0,
            .line3 = 0,
            .line4 = 0,
            .line5 = 0,
            .line6 = 0,
            .line7 = 0,
        }
        };


    ssd1309_startup(Screen.Screen);

    return Screen;
}