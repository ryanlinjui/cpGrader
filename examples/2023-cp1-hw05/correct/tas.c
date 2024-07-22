#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void button_set_frame(uint8_t **src, size_t *size, const uint8_t button, const uint64_t start_frame, const uint64_t end_frame)
{
    // end frame overflow issue, realloc src
    if(end_frame >= *size)
    {
        *src = (uint8_t*)realloc(*src, (end_frame + 1) * sizeof(uint8_t));
        for (int i = *size; i <= end_frame; i++)
        {
            (*src)[i] = 0;
        }
        *size = end_frame + 1;
    }

    // set button to src
    for (int i = 0; i < *size; i++)
    {
        if(i >= start_frame && i <= end_frame)
        {
            (*src)[i] |= button;
        }
    }
}

void button_unset_frame(uint8_t *src, const size_t size, const uint8_t button, const uint64_t start_frame, const uint64_t end_frame)
{
    // unset button to src
    for (int i = 0; i < size; i++)
    {
        if(i >= start_frame && i <= end_frame)
        {
            src[i] = src[i] & (~button);
        }
    }
}