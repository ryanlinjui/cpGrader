#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUTTON_LIST "RLDUTSBA"
#define TAS_HEADER \
    "version 3\n" \
    "emuVersion 22020\n" \
    "palFlag 0\n" \
    "romFilename SuperMarioBros\n" \
    "romChecksum base64:jjYwGG411HcjG/j9UOVM3Q==\n" \
    "fourscore 0\n" \
    "microphone 0\n" \
    "port0 1\n" \
    "port1 0\n" \
    "port2 0\n" \
    "FDS 0\n" \
    "NewPPU 0\n" \
    "comment TA\n"

void extract_fm2_file(const uint8_t *src, const size_t size)
{
    FILE *file = fopen("output.fm2", "w");
    if (file == NULL) 
    {
        perror("Error opening file");
        return;
    }
    if (size <= 0)
    {
        perror("Invalid Size");
        return;
    }

    fprintf(file, TAS_HEADER);
    for (size_t i = 0; i < size; i++)
    {
        fprintf(file, "|0|");
        for (int j = 0; j < 8; j++)
        {
            fprintf(file, "%c", (src[i] >> (7 - j)) & 1 ? BUTTON_LIST[j] : '.');
        }
        fprintf(file, "|||\n");
    }

    fclose(file);
    printf("Output written to output.fm2\n");
}