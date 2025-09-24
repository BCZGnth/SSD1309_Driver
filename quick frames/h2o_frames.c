#include "h2o_frames.h"
#include "ssd1309.h"

void h2o_rectangular_frames(ScreenDefines Screen) {

    Ssd1309Rect half_screen_outline = {
        .xstart = 0,
        .xend = 59,
        .ystart = 0,
        .yend = 63
    };

    Ssd1309Rect other_half_screen_outline = {
        .xstart = 60,
        .xend = 127,
        .ystart = 0,
        .yend = 31
    };

    Ssd1309Rect other_other_half_screen_outline = {
        .xstart = 60,
        .xend = 127,
        .ystart = 32,
        .yend = 63
    };

    Ssd1309Print h2o = {
        .text = "H2O",
        .length = 3,
        .ram_ptr = {
            .position = 3,
            .page = 1
        },
        .scale = 1,
        .delay = 1
    };

    Ssd1309Print connected = {
        .text = "Connected",
        .length = 9,
        .ram_ptr = {
            .position = 3,
            .page = 2
        },
        .scale = 1,
        .delay = 1
    };

    Ssd1309HVLine underscore = {
        .xstart = 3,
        .ystart = 25,
        .length = 54
    };

    ssd1309_cls(Screen);
    ssd1309_draw_rect(Screen, half_screen_outline);
    ssd1309_draw_rect(Screen, other_half_screen_outline);
    ssd1309_draw_rect(Screen, other_other_half_screen_outline);
    ssd1309_print(Screen, h2o);
    ssd1309_print(Screen, connected);
    ssd1309_draw_hline(Screen, underscore);
}

void h2o_outline(ScreenDefines Screen) {

    Ssd1309Print h2o_connected = {
        .text = "H2O Connected",
        .length = 13,
        .ram_ptr = {
            .position = 24,
            .page = 2
        },
        .scale = 1,
        .delay = 1
    };

    Ssd1309Rect full_outline = {
        .xstart = 0,
        .xend = 127,
        .ystart = 0,
        .yend = 63
    };

    ssd1309_draw_rect(Screen, full_outline);
    ssd1309_print(Screen, h2o_connected);
}

void generic_payload_frame(ScreenDefines Screen)
{
    Ssd1309Rect full_outline = {
        .xstart = 0,
        .xend = 127,
        .ystart = 0,
        .yend = 63
    };

    Ssd1309Print SerNUM = {
        .text = "Ser #:",
        .length = 6,
        .ram_ptr = {
            .page = 2,
            .position = 3
        },
        .scale = 1,
        .delay = 0
    };

    Ssd1309HVLine ser_num_underline = {
        .ystart = 18,
        .xstart = 41,
        .length = 50
    };

    ssd1309_draw_rect( Screen, full_outline);
    ssd1309_print(     Screen, SerNUM);
    ssd1309_draw_hline(Screen, ser_num_underline);
}