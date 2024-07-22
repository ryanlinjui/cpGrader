#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

void button_set_frame(uint8_t **src, size_t *size, const uint8_t button, const uint64_t start_frame, const uint64_t end_frame);
/*
    Parameters: 
        src: Button value array pointer for generate fm2 file.
        size: src size for pointer.
        button: Button uint8_t value to set, not replace.
        start_frame: Represent start frame to set button.
        end_frame: Represent end frame to set button.

    Note:
        If the frame range is overflow, please increase the size of src automatically

    Examples:
        Set 80 value (L, U button) from frame 3 to frame 480.
        
        $ button_set_frame(src, size, 80, 3, 480);
        
        Button set list: R, L, D, U, T, S, B, A. 
        That is uint8_t range value convert to. For example:
               
              RLDUTSBA 
        80 -> 01010000 -> set L, U

        Orginal src:
            {0, 8, 130, 0, 130, 130, 255}
            size of array: 7
            
        After passing the function of src will be:
            {0, 8, 130, 80, 210, 210, 255, 80 ..... 80}
            size of array: 481
*/

void button_unset_frame(uint8_t *src, const size_t size, const uint8_t button, const uint64_t start_frame, const uint64_t end_frame);
/*
    Parameters: 
        src: Button value array for generate fm2 file.
        size: src size for pointer.
        button: Button uint8_t value to unset.
        start_frame: Represent start frame to unset button.
        end_frame: Represent end frame to unset button.

    Examples:
        Unset 2 value (B button) from frame 1 to frame 100.
        
        $ button_unset_frame(src, size, 2, 1, 100);
               
             RLDUTSBA 
        2 -> 00000010 -> unset B

        Orginal src:
            {0, 8, 130, 0, 130, 130, 255}
            size of array: 7
            
        After passing the function of src will be: 
            {0, 8, 128, 0, 128, 128, 253}
            size of array: 7
*/