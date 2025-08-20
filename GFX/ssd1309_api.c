#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssd1309_api.h"
#include "logger.h"


/**
 * @brief a function for writing a bitmap to the screen
 *
 * @param xlength is expected to be an integer value telling how many rows
 * @param ylength is expected to be length in bytes (how many bytes long in the y length)
 * @param bitmap expected to be a pointer to some value that is at leat a byte long so that
 */
void ssd1309_write_bitmap(ScreenDefines Screen, Ssd1309WriteBitmap args) {
    /** Preliminary checks that may or may not be useful... if a number is bigger than it is supposed to be, the extra bits will probably just be ignored. */
    // if( (args.xstart > args.xend) || (args.xstart > 128)) return;
    // if( (args.ystart > args.yend) || (args.ystart > 128)) return;

    /** Set RAM pointer constraints based on x and y values given */
    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, HORIZONTAL_ADDRESSING);
    ssd1309_send_command(Screen, SET_COLUMN_ADDRESS, args.xstart, args.xend);

    /** For now there is no deciding how to pad/ write odd size bitmaps. I hope your bitmap has a height multiple of 8...*/
    ssd1309_send_command(Screen, SET_PAGE_ADDRESS, args.ystart / 8, args.yend / 8);
    size_t size = load_i2c_buffer(Screen, (&SSD1309_RAM_WRITE_BYTE), 1, args.pbitmap, args.length);

    ssd_write(Screen, size);

    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, PAGE_ADDRESSING);
    ssd1309_set_ram_pointer(Screen, Screen.zeroed_ram_ptr);
}

/***
 * @brief A function to set the ram pointer to a position on the  (Dependent on the addressing mode)
 *
 * @param x_start defines the LEFT-most bit (pixel) or 8-bit page (if in page addressing mode)
 * @param x_end Not Implemented. For advanced box defining that will come in handy when writing text to the screen
 * @param y_start defines the TOP-most bit (pixel) or 8-bit page (if in page addressing mode)
 * @param y_end Not Implemented. For advanced box defining that will come in handy when writing text to the screen
 */
void ssd1309_ramWrite(ScreenDefines Screen, Ssd1309RamWrite args) {
    size_t size = load_i2c_buffer(Screen, (&SSD1309_RAM_WRITE_BYTE), 1, args.bitmap,  args.bitmap_length);

    ssd_write(Screen, size);
}

/**
Each page has a capacity of 21 full characters. There are 8 pages,
so that gives us 168 characters at the max that the display can
print at one time.
*/

/**
 * @brief This function prints characters right justified. That means that you can only print as many characters as
 * @brief are specified in the length paramater of the function.
 *
 * @param     Screen: the screenDefines structure that general screen definitions
 * @param     args: the Ssd1309WriteNumber struct that holds the specific paramaters for writing a number to the screen 
 *
 * This function first converts the integer data coming in, into string data using the snprintf function
 *
 * The snprintf function returns the number of characters written to the character buffer
 *
 * Then, based on the number of characters written into the buffer, we calculate how many psetup_bytes to pad
 * the i2c buffer with in order to right align the data.
 * (we need the number of bytes because the i2c buffer is loaded with zeros initialy
 *  This means that we just need to Screen.offset the pointer to where the font data
 *  is loaded into the i2c buffer)
 *
 * The i2c buffer is then loaded with zeros
 * (because each letter in the font is 5 bytes long, filling the buffer with zeros
 *  makes it possible to increment the pointer )
 *
 * Then the SSD1309_RAM_WRITE_BYTE is written to the load_i2c_buffer
 *
 * Next we write the characters to the i2c buffer via a for loop and the memcpy function
 *      the memcpy function copys memory from the source, and copies it to the destination
 *      its syntax is this:
 *                           memcpy(*Dest, *Source, how many bytes to copy)
 *
 *      for each loop of the iteration, the "n" variable keeps track of what character to write into the load_i2c_buffer
 *      The pointer to the destination buffer is calculated by:
 *
 *          Adding 1 to the pointer to the i2c buffer (don't overwrite the SSD1309_RAM_WRITE_BYTE)
 *          Adding (n * 6) to the pointer to the i2c buffer. The multiplier 6 is used because the characters are 5 bytes long and a pad byte is added in between each character
 *          Adding the right_aligh_character_offset to align the characters on the right side
 *
 *      The source pointer is calculated by:
 *
 *          Subtracting 32 from the character pointer stored in data_to_write[n] (the local font does not include the first 32 ascii characters)
 *          Multiplying that number by 5 (because each character takes up 5 bytes)
 *          Adding that number to a pointer to the font (to point to the correct character in the font array)
 *
 *      The length of data to copy is 5 because each character is 5 bytes long.
 *      the pointer buffer is incremented by 6 in order to put a column of space in between each character
 *      This is only possible because the i2c buffer is loaded with zeros before any data gets written to it.
 *
 */
size_t ssd1309_write_number(ScreenDefines Screen, Ssd1309WriteNumber args) {

    ADD_TO_STACK_DEPTH(); // ssd1309_write_number

    level_log(TRACE, "Writing Number: %d", args.data);

    if((args.constrained_length * Screen.character.width_pad) > Screen.buffer_size) {level_log(ERROR, "Cannot write more than 728 bytes to the I2C buffer");}

    ssd1309_set_ram_pointer(Screen, args.ram_ptr);

    uint8_t n;
    uint8_t number_of_chars_written; // the snprintf function actually returns an integer value, but I hope that the amount of characters will never exceed 256...
    uint8_t* data_to_write = malloc(168); // 168 bytes can hold the max number of characters that can be displayed on the screen
    
    if (data_to_write == NULL)
    {
        level_log(ERROR, "SSD1309: Memory allocation failed for data_to_write");
        return 0;
    }

    number_of_chars_written = (snprintf((char*)(data_to_write), 128, "%u", args.data)); // putting zeros at the end of the string so that it is less noise to the viewer

    uint8_t right_align_character_offset = (args.constrained_length - number_of_chars_written) * 6;

    /* Loading all Zeros into the I2C buffer */
    memset(Screen.pbuffer, 0, 128);

    memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1);

    level_log(TRACE, "Loading the I2C buffer with the numeric characters", args.data);
    for (n = 0; n < number_of_chars_written; n++)
    { // Iterate through all of the characters in the string

        /** Write 5 bytes from a (calculated pointer to a character in the font) to the i2c buffer */

        /**      The pointer to the destination buffer is calculated by:
         *
         *          Adding 1 to the pointer to the i2c buffer (don't overwrite the SSD1309_RAM_WRITE_BYTE)
         *          Adding (n * 6) to the pointer to the i2c buffer. The multiplier 6 is used because the characters are 5 bytes long and a pad byte is added in between each character
         *          Adding the right_aligh_character_offset to align the characters on the right side
         *
         *      The source pointer is calculated by:
         *
         *          Subtracting 32 from the character pointer stored in data_to_write[n] (the local font does not include the first 32 ascii characters)
         *          Multiplying that number by 5 (because each character takes up 5 bytes)
         *          Adding that number to a pointer to the font (to point to the correct character in the font array)
         *
         *      The length of data to copy is 5 because each character is 5 bytes long.
         *      the pointer buffer is incremented by 6 in order to put a column of space in between each character
         *      This is only possible because the i2c buffer is loaded with zeros before any data gets written to it.
         *
         */
        memcpy((Screen.pbuffer + (n * Screen.character.width_pad) + 1 + right_align_character_offset), (((data_to_write[n] - Screen.offset.ascii) * Screen.character.width) + Screen.offset.pfont), Screen.character.width);
    }

    ssd_write(Screen, (args.constrained_length * Screen.character.width_pad)); // The number of bytes to write to the i2c buffer is the number of characters multiplied by the width of each character (5) plus the padding (1 byte) between each character

    free(data_to_write); // Free the allocated memory for data_to_write
    data_to_write = NULL; // Set the pointer to NULL to avoid dangling pointers
    level_log(TRACE, "SSD1309: Done Writing Number");
    REMOVE_FROM_STACK_DEPTH(); // ssd1309_write_number

    return args.constrained_length * 6;
}

/**
 * @param     Screen: the screenDefines structure that general screen definitions
 * @param     args: the Ssd1309WriteNumber struct that holds the specific paramaters for writing a number to the screen 
 *
 * @brief A function to print a string on the screen
 */
size_t ssd1309_print(ScreenDefines Screen, Ssd1309Print args) {

    /* Error Checks */
    // Length is zero
    if(!args.length) {
        level_log(ERROR, "type <Ssd1309Print> args.length not defined");
    }
    // If the bytes length will be larger than the buffer size
    if((args.length * Screen.character.width_pad * (args.scale * args.scale) + Screen.offset.control) >  Screen.buffer_size) {
        level_log(ERROR, "Cannot write more than 728 bytes to the I2C buffer");
    }

    ADD_TO_STACK_DEPTH(); // ssd1309_print

    level_log(TRACE, "Printing: \"%s\"", args.text);

    uint8_t* letter_buffer;
    uint16_t tmp16; // A variable to hold the 16 bit value of a scaled byte
    uint8_t tmp[2]; // tmp16 will get split into these two bytes to get the right endianness when sending the data over the i2c bus.
    size_t write_size;

    ssd1309_set_ram_pointer(Screen, args.ram_ptr); // Put the cursor where specified in the args structure

    char message_chars[128];
    memcpy(&message_chars, args.text, args.length);

    memset(Screen.pbuffer, 0, Screen.buffer_size);
    // memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1); // Not a useful line since it gets overwritten anyway... put it where it won't get overwritten
    level_log(TRACE, "TESTING...the Screen.pbuffer is equal to: %x",  *(Screen.pbuffer));

    /** Loading the I2C buffer */
    switch (args.scale)
    {
        #warning Scaling only enables a single row of scaled text to be printed to the screen from a single function call
        case 2:
        /** Scaling the text by 2. (Right now the code does not support anything else...)*/
            level_log(TRACE, "Print: Scaling the text by 2");

            // Set addressing to vertical addressing. (simulate page addressing but across two pages instead of one)
            ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, VERTICAL_ADDRESSING);

            // Set the column range from whatever the start position is equal to and go to the end of the screen 
            ssd1309_send_command(Screen, SET_COLUMN_ADDRESS, args.ram_ptr.position, 0xFF);

            // Set the page range to 2 since each page is 8 pixels tall, and we are scaling by 2, that gives us 16 pixels of height
            ssd1309_send_command(Screen, SET_PAGE_ADDRESS, args.ram_ptr.page, args.ram_ptr.page + 1);

            ADD_TO_STACK_DEPTH(); // loading i2c buffer with scaled data

            /** 
             * Iteration for scaling the font. 
             *
             *    Iterate over the number of characters in the char* args.text (denoted by args.length)
             *          
             *          Iterate over each column of the letter to scale each column individually
             *           - use memset to set the corresponding block of memory in Screen.Screen.pbuffer
             *           - NOTE: we use memset because the vertical scaling is done with a lookup table, but the horizontal scaling is done by writing the same byte to adjacent memory addresses
             *           - NOTE: args.scale is used as the amount of bytes to write to Screen.Screen.pbuffer because the character will be args.scale times wider.
             */
            int letter_increment;
            int current_column;
            int column_offset = args.scale;

            memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1);

            level_log(TRACE, "TESTING...the Screen.pbuffer is equal to: %x",  *(Screen.pbuffer));

            for (int char_index = 0; char_index < args.length; char_index++)
            {
                /** Letter increment is the number by which to offset the buffer when writing the next letter 
                 * Since each letter will take up (width * height) bits, it will take up (width * height / 8) bytes 
                 * So a scaled letter will take up ( (scale * width) * (scale * height) / 8 ) bytes. 
                 * since "scale" will always be (scale * height / 8) since a character is 8 bits (one byte) tall 
                 * This means that we can make the translation a little more efficient by saying 
                 * each letter will take up ( (scale * width) * scale ) */

                letter_increment = char_index * Screen.character.width_pad * (args.scale * args.scale);
                for (int char_column = 0; char_column < 5; char_column++)
                {
                    /** Letter column is to calculate the Screen.offset for one column of scaled letter. */
                    /** Scaling the letter by x will give the scaled letter x^2 times as many columns. */
                    current_column = char_column * args.scale * args.scale;
                    // tmp = *( real.pfont + ((message_chars[n] - Screen.offset.ascii) * Screen.character.width) + i); // To get the individual bytes of the character
                    tmp16 = scale_lut[*(Screen.offset.pfont + ((message_chars[char_index] - Screen.offset.ascii) * Screen.character.width) + char_column)];
                    tmp[1] = (tmp16 >> 8);
                    tmp[0] = (tmp16 & 0xff);

                    memcpy(( Screen.pbuffer + Screen.offset.control + letter_increment + current_column ),                &(tmp[0]), args.scale);
                    memcpy(( Screen.pbuffer + Screen.offset.control + letter_increment + current_column + column_offset), &(tmp[0]), args.scale);

                    // memset(( Screen.pbuffer + Screen.offset.control + letter_increment + current_column ), tmp[0], 1); // Write the scaled byte to the i2c buffer
                    // memset(( Screen.pbuffer + Screen.offset.control + letter_increment + current_column + 1), tmp[1], 1);
                    level_log(TRACE, "Blah Blah Blah: %X", scale_lut[tmp[0]]);
                }
            }

            level_log(TRACE, "TESTING...the Screen.pbuffer is equal to: %x",  *(Screen.pbuffer));

            level_log(TRACE, "Truncating the write length for the scaled font. That way it doesn't overwrite itself");
            if((args.length * Screen.character.width_pad * (args.scale * args.scale) + Screen.offset.control) > Screen.ScreenWidth * args.scale) {
                write_size = Screen.ScreenWidth * args.scale;
            } else {
                write_size = (args.length * Screen.character.width_pad * (args.scale * args.scale) + Screen.offset.control);
            }

            REMOVE_FROM_STACK_DEPTH(); // loading i2c buffer with scaled data

            break;

        default:
        /** No scaling applied, and the message is copied into the i2c buffer */
            level_log(TRACE, "Print: No scale applied");

            memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1);

            for (int letter = 0; letter < args.length; letter++)
            { // Iterate through all of the characters in the string

                /** Write 5 bytes from a (calculated pointer to a character in the font) to the i2c buffer */

                /**      The pointer to the destination buffer is calculated by:
                 *
                 *          Adding 1 to the pointer to the i2c buffer (don't overwrite the SSD1309_RAM_WRITE_BYTE)
                 *          Adding (n * 6) to the pointer to the i2c buffer. The multiplier 6 is used because the characters are 5 bytes long and a pad byte is added in between each character
                 *
                 *
                 *      The source pointer is calculated by:
                 *
                 *          Subtracting 32 from the character pointer stored in message_chars[n] (the local font does not include the first 32 ascii characters)
                 *          Multiplying that number by 5 (because each character takes up 5 bytes)
                 *          Adding that number to a pointer to the font (to point to the correct character in the font array)
                 *
                 *      The length of data to copy is 5 because each character is 5 bytes long.
                 *      the pointer buffer is incremented by 6 in order to put a column of space in between each character
                 *      This is only possible because the i2c buffer is loaded with zeros before any data gets written to it.
                 */
                memcpy((Screen.pbuffer + Screen.offset.control + ((Screen.character.width_pad) * letter)), (((message_chars[letter] - Screen.offset.ascii) * Screen.character.width) + Screen.offset.pfont), Screen.character.width);
            }

            write_size = (args.length * Screen.character.width_pad * (args.scale * args.scale) + Screen.offset.control);

            break;

    } // Switch(args.scale)

    /** Writing the contents of the I2C buffer over the bus */
    switch (args.delay)
    {

    case 0:

        level_log(TRACE, "Print: No delay");
        ssd_write(Screen, write_size);

        break;

    default:

        /** Allocate memory to write a single letter each time */
        letter_buffer = calloc((Screen.character.width_pad * (args.scale * args.scale) + Screen.character.pad), sizeof(uint8_t));
        memcpy(letter_buffer, (&SSD1309_RAM_WRITE_BYTE), Screen.offset.control);

        level_log(TRACE, "Print: Delay of %d", args.delay);

        /** My own version of the ssd_write function that has a spefic delay for each character built in!! */
        for (int letter_index = 0; letter_index < args.length; letter_index++)
        {
            ADD_TO_STACK_DEPTH(); // Adding to the stack depth because this is a different I2C write than anything else in the program

            /** The width of a letter is calculated by the following logic: 
             * Since each letter will take up (width * height) bits, it will take up (width * height / 8) bytes 
             * So a scaled letter will take up ( (scale * width) * (scale * height) / 8 ) bytes. 
             * since "scale" will always be (scale * height / 8) since a character is 8 bits (one byte) tall 
             * This means that we can make the translation a little more efficient by saying 
             * each letter will take up ( (scale * width) * scale ) 
             * Since letter_index holds the index of the letter that we are currently writing to the screen,  */
            memcpy((letter_buffer + Screen.offset.control), (Screen.pbuffer + Screen.offset.control + (letter_index * Screen.character.width_pad * args.scale * args.scale)), (Screen.character.width_pad * (args.scale * args.scale)));

            /** The amount of bytes that will be written over the buffer is equal to the width of a letter (defined in the above comment) */

            if (!i2c.Write(Screen.i2c_address, letter_buffer, (Screen.character.width_pad * (args.scale * args.scale) + Screen.character.pad)))
            {
                level_log(WARNING, "Print: I2C Bus Busy");
            }

            if (!i2c.IsBusy())
            {
                if (i2c.ErrorGet() == I2C_ERROR_NONE)
                {
                    // I2C write successful
                    level_log(TRACE, "Print: I2C Write Successful");
                }
                else
                {
                    // Error handling
                    level_log(ERROR, "Print: I2C Write Failed");
                }
            }

            // Wait for 10ms before writing the next letter
            __delay_ms(10);

            REMOVE_FROM_STACK_DEPTH(); // Removing from the stack depth because this is a different I2C write than anything else in the program
        }

        free(letter_buffer);
        letter_buffer = NULL;

        break;

    } // Switch(args.delay)

    // Memory addressing mode back to page addressing
    ssd1309_send_command(Screen, SET_MEMORY_ADDRESSING_MODE, PAGE_ADDRESSING);

    // Set the column range back to its reset value
    ssd1309_send_command(Screen, SET_COLUMN_ADDRESS, 0x0, 0xFF);

    // Set the page range back to its reset value
    ssd1309_send_command(Screen, SET_PAGE_ADDRESS, 0x0, 0x7);
    level_log(TRACE, "Print: Done Printing. Setting Memory Addressing Mode to Page Addressing");

    REMOVE_FROM_STACK_DEPTH(); // ssd1309_print

    return (args.length * 6);
}


void ssd1309_cls(ScreenDefines Screen) {

    ADD_TO_STACK_DEPTH();

    level_log(TRACE, "Clearing the screen");
    ssd1309_set_ram_pointer(Screen, Screen.zeroed_ram_ptr);

    memset(Screen.pbuffer, 0, 129);
    // load_i2c_buffer((&SSD1309_RAM_WRITE_BYTE), 1, 0, 0);
    memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1);

    for (uint8_t i = 0; i < 8; i++) {
        ssd_write(Screen, 129);
    }
    // ssd_write(9); // Compensate for the control byte at the start of the buffer. (The for loop really only writes 127 bytes to the screen)

    level_log(TRACE, "SSD1309: Screen Cleared");
    REMOVE_FROM_STACK_DEPTH();
}


void ssd1309_clear_line(ScreenDefines Screen, Ssd1309ClearLine args)
{

    ADD_TO_STACK_DEPTH();

    level_log(TRACE, "Clearing the screen");
    Screen.zeroed_ram_ptr.page = args.start_page;
    ssd1309_set_ram_pointer(Screen, Screen.zeroed_ram_ptr);

    memset(Screen.pbuffer, 0, 129);
    // load_i2c_buffer((&SSD1309_RAM_WRITE_BYTE), 1, 0, 0);
    memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1);

    for (uint8_t i = args.start_page; i < args.end_page; i++)
    {
        ssd_write(Screen, 129);
    }
    // ssd_write(9); // Compensate for the control byte at the start of the buffer. (The for loop really only writes 127 bytes to the screen)

    level_log(TRACE, "SSD1309: Screen Cleared");
    REMOVE_FROM_STACK_DEPTH();
}

// void ssd1309_clear_block(ScreenDefines Screen, Ssd1309ClearBlock args)
// {

//     ADD_TO_STACK_DEPTH();

//     level_log(TRACE, "Clearing a BLOCK on the screen");

//     ssd1309_set_ram_pointer(Screen, args.ram_ptr);

//     memset(Screen.pbuffer, 0, 129);
//     // load_i2c_buffer((&SSD1309_RAM_WRITE_BYTE), 1, 0, 0);
//     memcpy(Screen.pbuffer, (&SSD1309_RAM_WRITE_BYTE), 1);

//     for (uint8_t i = args.start_page; i < args.end_page; i++)
//     {
//         ssd_write(Screen, 129);
//     }
//     // ssd_write(9); // Compensate for the control byte at the start of the buffer. (The for loop really only writes 127 bytes to the screen)

//     level_log(TRACE, "SSD1309: Screen Cleared");
//     REMOVE_FROM_STACK_DEPTH();
// }


void ssd1309_blinking_cursor(ScreenDefines Screen, Ssd1309Cursor args)
{
    /** Start by keeping track of how many function calls deep we are */
    ADD_TO_STACK_DEPTH();
    level_log(TRACE, "Blinking Cursor: page %u, column %u, repeats %u", args.ram_ptr.page, args.ram_ptr.position, args.repeats);

    size_t size;

    /** malloc so that the cursor doesn't take up extra space */
    uint8_t* cursor = malloc(5*sizeof(uint8_t));

    ssd1309_set_ram_pointer(Screen, args.ram_ptr);

    /** Modifying repeats to translate from number of repeats to iterations of the for loop that have to run */
    args.repeats *= 2;
    /** Using the pre-increment operator rather than adding one to repeats */
    args.repeats += 1; // Add one to make sure we end on a blank cursor 

    for (int i = 1; i < args.repeats; ++i)
    {

        /** Decide whether to set the cursor all zeros or all ones */
        memset(cursor, ((i % 2) * 0xFF), 5);

        /** Loading and writing the i2c buffer */
        size = load_i2c_buffer(Screen, (&SSD1309_RAM_WRITE_BYTE), 1, cursor, 5);
        ssd_write(Screen, size);

        ssd1309_set_ram_pointer(Screen, args.ram_ptr);
        __delay_ms(200);
    }

    free(cursor);
    cursor = NULL; // Set the pointer to NULL to avoid dangling pointers

    level_log(TRACE, "Blinking Cursor: Done Blinking Cursor");
    REMOVE_FROM_STACK_DEPTH();
}
