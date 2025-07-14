#ifndef _SSD1306_COMMAND
#define _SSD1306_COMMAND

#include "ssd1306_i2c.h"
#include <stdio.h>


void ssd1306_send_command(ScreenDefines Screen, uint16_t command, ...);
void ssd1306_set_ram_pointer(ScreenDefines Screen, Ssd1306RamPointer args);
void ssd1306_startup(ScreenDefines Screen);
Ssd1306Defines ssd1306_init(uint8_t* i2c_buffer, unsigned int buffer_size, uint8_t i2c_address);
extern void ssd1306_cls(ScreenDefines Screen);


/* COMMANDS */
const uint16_t SET_DISPLAY_ON_RESUME_RAM_DISPLAY = 0x00A4;  // Entire Display ON; resume RAM content display
const uint16_t SET_DISPLAY_NORMAL = 0x00A6;  // Set normal/inverse display: Normal
const uint16_t SET_SCROLL_INACTIVE = 0x002E;  // Deactivate Scroll
const uint16_t SET_DISPLAY_ON = 0x00AF;  // dISPLAY ON in normal mode
const uint16_t SET_COLUMN_LSB_0 = 0x0007;  // Set Lower Columnm start address to 0
const uint16_t SET_COLUMN_MSB_4 = 0x0015;  // Set Higher Column start address to 4
const uint16_t SET_DISPLAY_OFF = 0x00AE;  // Display OFF  
const uint16_t SET_SEGMENT_REMAP = 0x00A1;  // Set segment re-map COL 127 mapped to SEG 0 (A0 sets normal addressing)
const uint16_t SET_COM_SCAN_DIRECTION = 0x00C8;  // Set COM output scan direction: Scan from COM[n-1] to COM[0]
const uint16_t SET_DISPLAY_START_LINE = 0x0040;                    //      Set Display start line


/* Charge Pump */
const uint16_t SET_CHARGE_PUMP = 0x018D;  // Charge pump setting

const uint8_t CHARGE_PUMP_ENABLE = 0x14;                    //      Enable Charge pump


/* Memory Addressing */
const uint16_t SET_MEMORY_ADDRESSING_MODE = 0x0120;  // Set Memory addressing mode

const uint8_t PAGE_ADDRESSING = 0x10;
const uint8_t HORIZONTAL_ADDRESSING = 0x00;
const uint8_t VERTICAL_ADDRESSING = 0x01;
/* Vertical / Horizontal Addressing */
const uint16_t SET_PAGE_ADDRESS = 0x0222; 
const uint16_t SET_COLUMN_ADDRESS = 0x0221;


/* COM Hardware Config */
const uint16_t SET_COM_HARDWARE_CONFIG = 0x01DA;  // Set COM pins hardware configuration

const uint8_t  COM_HARDWARE_CONFIG_SETTING = 0x0012;         // 


/* Set Contrast */
const uint16_t SET_CONTRAST = 0x0181;  // Set Contrast control

const uint8_t CONTRAST_HIGH = 0x00CF;                       //      (FF is highest 00 is lowest)
// #define SET_CONTRAST_TO(x) x;


/* Precharge */
const uint16_t SET_PRECHARGE = 0x01D9;  // Set PreCharge Period

const uint8_t PRECHARGE_PERIOD = 0x00F1;                    //      (see datasheet)


/* Vcomh Deselect */
const uint16_t SET_VCOMH_DESELECT = 0x01DB;  // Set Vcomh Deselect Level

const uint8_t VCOMH_DESELECT_LEVEL = 0x0040;                //      (some unspecified number times Vcc)


/* Clock Divide Ratio */
const uint16_t SET_CLK_DIV_RATIO = 0x01D5;  // Set Display clock divide ratio/oscilator frequency

const uint8_t DIVIDER_FREQ = 0x80;                    //      0x8 (0b1000) RESET frequency,  0x0 (0b0000) RESET divide ratio


/* Multiplexor Ratio / Configuration */
const uint16_t SET_MUX_RATIO = 0x01A8;  // Set multiplex ratio

const uint8_t CONFIG_BYTE = 0x3F;                       //      multiplex configuration byte


/* Display Offset */
const uint16_t SET_DISPLAY_OFFSET = 0x01D3;  // Set Display offset

const uint8_t NO_OFFSET = 0x00;              //      no display offset


const uint8_t ssd1306_startup_length = 29;

const uint8_t ssd1306_startup_sequence[29] = {
    0xAE,   // Display OFF  
    0xD5,   // Set Display clock divide ratio/oscilator frequency
    0x80,     //   	0x8 (0b1000) RESET frequency,  0x0 (0b0000) RESET divide ratio
    0xA8,   // set multiplex ratio
    0x3F,       //   multiplex configuration byte
    0xD3,   // Set Display offset
    0x00,       //   no display offset
    0x40,   // Set Display start line
    0x8D,   // Charge pump setting
    0x14,       //   Enable Charge pump
    0x20,   // Set Memory addressing mode
    0x10,   //      (00 for Horizontal addressing, 01 for Vertical addressing, 10 for Page ad
    0xA0,   // Set segment re-map COL 127 mapped to SEG 0 (A0 sets normal addressing)
    0xC0,   // Set COM output scan direction: Scan from COM[n-1] to COM[0]
    0xDA,   // Set COM pins hardware configuration
    0x12,   // 
    0x81,   // Contrast control
    0xCF,       //  (FF is highest 00 is lowest)
    0xD9,   // PreCharge Period
    0xF1,      //    (see datasheet)
    0xDB,   // Set Vcomh Deselect Level
    0x40,   //      (some unspecified number times Vcc)
    0xA4,   // Entire Display ON; resume RAM content display
    0xA6,   // Set normal/inverse display: Normal
    0x2E,   // Deactivate Scroll
    0xAF,   // dISPLAY ON in normal mode
    0xB4,   // Set page address to page 5 (pages are zero indexed so page 4 is the fifth page...)
    0x07,   // Set Lower Columnm start address to 0
    0x15    // Set Higher Column start address to 4
};

#endif // _SSD1306_COMMANDS