
#include "ssd1309_font.h"


uint16_t scale_byte(uint8_t input) {
    return scale_lut[input];
}

uint8_t* get_font_pointer (uint8_t ascii_character) {
    return (((ascii_character - offset.ascii) * character.width) + offset.pfont);
}

void parse_screen_message(Ssd1309Print args, ScreenStringPerLine* strings) {
    uint8_t breakpoints[8];

    for(int i = 0; i < args.length; i++){
        
    }
}

#ifdef OLD_FONT

uint8_t reverse_bits(uint8_t byte) {
    return reverse_table[byte];
}

#endif // OLD_FONT
