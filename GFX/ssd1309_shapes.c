#include "ssd1309_shapes.h"

void ssd1309_draw_vline(ScreenDefines Screen){
    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, VERTICAL_ADDRESSING);


}

void ssd1309_draw_hline(){
    return;
}

void ssd1309_draw_rect(){
    return;
}

