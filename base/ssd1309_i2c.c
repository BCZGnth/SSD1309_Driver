#include <stdio.h>
#include <string.h>

#include "logger.h"
#include "ssd1309_i2c.h"

// #define I2C_WRITE_PROTECTION null
/* No Write protection at this time... */

#ifndef I2C_WRITE_PROTECTION
void ssd_write(ScreenDefines Screen, size_t data_length){

    ADD_TO_STACK_DEPTH(); // ssd_write

    if(!i2c.IsBusy()){

        level_log(TRACE, "SSD1309: I2C Write Pending...First byte is supposed to: %X", Screen.pbuffer[0]);
        
        if(i2c.Write(Screen.i2c_address, Screen.pbuffer, data_length)){
            level_log(WARNING, "SSD1309: I2C Bus Busy");
        }

        // level_log(TRACE, "SSD1309: I2C Write Sent to Peripheral");

        if(!i2c.IsBusy())
        {
            if (i2c.ErrorGet() == I2C_ERROR_NONE)
            {
                // I2C Write sucessful
                level_log(INFO, "SSD1309: I2C Write Sucessful");
            }
            else
            {
                // Error handling 
                level_log(ERROR, "SSD1309: I2C Write Error");
            }
        }
    } 
    else 
    {
        level_log(ERROR, "SSD1309: I2C Cannot Write to Peripheral. Bus is Busy");
    }

    level_log(TRACE, "SSD1309: I2C Write Finished");
    REMOVE_FROM_STACK_DEPTH(); // ssd_write 
}

#endif // I2C_WRITE_PROTECTION

#ifdef I2C_WRITE_PROTECTION
void ssd_write(size_t data_length){

    /* Error handling for when the screen buffer is NULL */
    if(Screen.pbuffer == NULL) {
        level_log(ERROR, "Screen Buffer is NULL");
        return;
    }

    ADD_TO_STACK_DEPTH(); // ssd_write

    while(i2c.IsBusy()) {
        level_log(TRACE, "I2C bus busy, waiting...");
        __delay_ms(1);
    }

    if(!i2c.IsBusy()){

        level_log(TRACE, "SSD1309: I2C Write Pending...First byte is supposed to: %X", ssd1309_i2c_buffer[0]);
        
        if(i2c.Write(SSD1309_ADDRESS, &ssd1309_i2c_buffer, data_length)){
            level_log(WARNING, "SSD1309: I2C Bus Busy");
        }

        // level_log(TRACE, "SSD1309: I2C Write Sent to Peripheral");

        if(!i2c.IsBusy())
        {
            if (i2c.ErrorGet() == I2C_ERROR_NONE)
            {
                level_log(INFO, "SSD1309: I2C Write Sucessful");
            }
            else
            {
                level_log(ERROR, "SSD1309: I2C Write Error");
            }
        }
    } else {
        level_log(ERROR, "SSD1309: I2C Cannot Write to Peripheral. Bus is Busy");
    }

    level_log(TRACE, "SSD1309: I2C Write Finished");
    REMOVE_FROM_STACK_DEPTH(); // ssd_write 
}
#endif // I2C_WRITE_PROTECTION


/**
 * @brief Load the setup bytes and the data into the i2c buffer
 *
 * @param Screen: of type ScreenDefines. The structure that holds the relevant screen attributes
 * @param psetup_bytes: The pointer to the setup bytes that will be sent to the i2c slave
 * @param setup_length: How many setup bytes to load into the buffer.
 * @param pdata: The pointer to the data
 * @param data_length: How many bytes of data to load into the buffer.
 * 
 */
size_t load_i2c_buffer(ScreenDefines Screen, uint8_t * psetup_bytes, size_t setup_length, uint8_t * pdata, size_t data_length) {
    ADD_TO_STACK_DEPTH(); // load_i2c_buffer

    level_log(TRACE, "SSD1309: Loading the I2C Buffer");

    Screen.pbuffer = malloc(setup_length + data_length);
    if(!Screen.pbuffer){
        level_log(ERROR, "Memory Allocation for the I2C buffer failed");
        REMOVE_FROM_STACK_DEPTH();
        return 0;
    }

    /* If we have allocated the memory store the size of the buffer */
    Screen.buffer_size = (int)(setup_length + data_length);

    /* Loading the setup bytes into the buffer */
    memcpy(Screen.pbuffer, psetup_bytes, setup_length);

    /* Loading the data into the buffer after the setup bytes */
    memcpy((Screen.pbuffer + setup_length), pdata, data_length);

    level_log(TRACE, "SSD1309: I2C Buffer Loaded");
    REMOVE_FROM_STACK_DEPTH(); // load_i2c_buffer   

    free(Screen.pbuffer);
    Screen.buffer_size = 0;

    return setup_length + data_length;
}

