#include <stdio.h>
#include <stdint.h>

#include "parity.h"

// Permutes all possible cases of min_value to max_value
void case_test(int32_t min_value, int32_t max_value)
{
    for (int32_t a = min_value; a <= max_value; a++) 
    {
        for (int32_t b = min_value; b <= max_value; b++) 
        {
            for (int32_t c = min_value; c <= max_value; c++) 
            {
                for (int32_t d = min_value; d <= max_value; d++) 
                {
                    for (int32_t e = min_value; e <= max_value; e++) 
                    {
                        printf("%lu\n", parity_2d(a, b, c, d, e));
                    }
                }
            }
        }
    }
}

int main()
{    
    int32_t min_value = 0, max_value = 0;
    scanf("%d %d", &min_value, &max_value);
    case_test(min_value, max_value);
    return 0;
}