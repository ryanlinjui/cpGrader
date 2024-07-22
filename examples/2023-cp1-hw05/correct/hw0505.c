#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "tas.h"
#include "decode.h"

int main()
{
    uint8_t *tas_src = NULL;
    size_t size = 0;
    button_set_frame(&tas_src, &size, 8, 180, 180);
    button_set_frame(&tas_src, &size, 130, 360, 1800);
    button_set_frame(&tas_src, &size, 1, 470, 500);
    button_set_frame(&tas_src, &size, 1, 515, 515);
    button_set_frame(&tas_src, &size, 1, 555, 590);
    button_set_frame(&tas_src, &size, 1, 610, 640);
    button_set_frame(&tas_src, &size, 1, 670, 710);
    button_set_frame(&tas_src, &size, 32, 730, 750);
    button_unset_frame(tas_src, size, 130, 710, 850);
    button_set_frame(&tas_src, &size, 131, 880, 910);
    button_set_frame(&tas_src, &size, 1, 950, 950);
    button_set_frame(&tas_src, &size, 131, 1260, 1320);
    button_unset_frame(tas_src, size, 1, 1260, 1280);
    button_set_frame(&tas_src, &size, 131, 1350, 1390);
    for (int i = 0; i < 30; i++)
    {
        button_set_frame(&tas_src, &size, 131, 1400 + i*10, 1400 + i*10 + 5);
    }

    extract_fm2_file(tas_src, size);
    if (size) free(tas_src);
    return 0;
}