#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

uint64_t parity_2d(int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5)
{
    // generate parity table by 5 input numbers
    bool parity_table[6][33] = {0};
    int32_t num_list[6] = {n1, n2, n3, n4, n5};

    for (int i = 0; i < 6; i++)
    {
        for (int bit = 31; bit >= 0; bit--)
        {
            parity_table[i][31-bit] = (num_list[i] & (1 << bit)) ? 1 : 0;
        }
    }

    // count 1's by row
    for (int i = 0; i < 5; i++)
    {
        int count = 0;
        for (int j = 0; j < 32; j++)
        {
            count = parity_table[i][j] ? count + 1 : count;   
        }
        parity_table[i][32] = (count % 2) ? 0 : 1;
    }

    // count 1's by colume
    for (int i = 0; i < 33; i++)
    {
        int count = 0;
        for (int j = 0; j < 5; j++)
        {
            count = parity_table[j][i] ? count + 1 : count;
        }
        parity_table[5][i] = (count % 2) ? 0 : 1;
    }

    // print parity_table
    // for (size_t i = 0; i < 6; i++)
    // {
    //     for (size_t j = 0; j < 33; j++)
    //     {
    //         printf("%u ", parity_table[i][j]);
    //     }
    //     printf("\n");
    // }

    // caculate parity number
    uint64_t parity_num = 0;
    for (int i = 0; i < 33; i++) 
    {
        parity_num |= ((uint64_t)(parity_table[5][i]) << (32 - i));
    }

    return parity_num;
}