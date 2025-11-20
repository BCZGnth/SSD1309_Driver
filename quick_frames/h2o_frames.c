#include "h2o_frames.h"
#include "ssd1309.h"

// void h2o_rectangular_frames(ScreenDefines Screen) {

//     Ssd1309Rect half_screen_outline = {
//         .xstart = 0,
//         .xend = 59,
//         .ystart = 0,
//         .yend = 63
//     };

//     Ssd1309Rect other_half_screen_outline = {
//         .xstart = 60,
//         .xend = 127,
//         .ystart = 0,
//         .yend = 31
//     };

//     Ssd1309Rect other_other_half_screen_outline = {
//         .xstart = 60,
//         .xend = 127,
//         .ystart = 32,
//         .yend = 63
//     };

//     Ssd1309Print h2o = {
//         .text = "H2O",
//         .length = 3,
//         .ram_ptr = {
//             .position = 3,
//             .page = 1
//         },
//         .scale = 1,
//         .delay = 1
//     };

//     Ssd1309Print connected = {
//         .text = "Connected",
//         .length = 9,
//         .ram_ptr = {
//             .position = 3,
//             .page = 2
//         },
//         .scale = 1,
//         .delay = 1
//     };

//     Ssd1309HVLine underscore = {
//         .xstart = 3,
//         .ystart = 25,
//         .length = 54
//     };

//     ssd1309_cls(Screen);
//     ssd1309_draw_rect(Screen, half_screen_outline);
//     ssd1309_draw_rect(Screen, other_half_screen_outline);
//     ssd1309_draw_rect(Screen, other_other_half_screen_outline);
//     ssd1309_print(Screen, h2o);
//     ssd1309_print(Screen, connected);
//     ssd1309_draw_hline(Screen, underscore);
// }

void h2o_outline(ScreenDefines Screen) {

    Ssd1309Print h2o_connected = {
        .text = "H2O Disconnected",
        .length = 16,
        .ram_ptr = {
            .position = 15,
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

void generic_payload_frame(ScreenDefines Screen, uint32_t serial_number)
{
    Ssd1309Print ser_num = {
        .text = "Ser #:",
        .length = 6,
        .ram_ptr = {
            .page = 0,
            .position = 3
        },
        .scale = 1,
        .delay = 0
    };

    /* Print Serial number */
    Ssd1309WriteNumber ser_val = {
        .constrained_length = 6,
        .data = serial_number,
        .ram_ptr = {
            .page = 0,
            .position = 42, 
        },
        .scale = 1,
    };

    ssd1309_write_number(Screen, ser_val);
    ssd1309_print(       Screen, ser_num);
}

void pretty_payload_frame(ScreenDefines Screen, uint32_t serial_number)
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
            .page = 1,
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

    /* Print Serial number */
    Ssd1309WriteNumber ser_val = {
        .constrained_length = 6,
        .data = serial_number,
        .ram_ptr = {
            .page = 1,
            .position = 42, 
        },
        .scale = 1,
    };

    ssd1309_write_number(Screen, ser_val);
    ssd1309_draw_rect(   Screen, full_outline);
    ssd1309_print(       Screen, SerNUM);
    ssd1309_draw_hline(  Screen, ser_num_underline);
}