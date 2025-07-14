#include "ssd1306_shapes.h"

void ssd1306_draw_vline(ScreenDefines Screen){
    ssd1306_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, VERTICAL_ADDRESSING);


}

void ssd1306_draw_hline(){
    return;
}

void ssd1306_draw_rect(){
    return;
}

