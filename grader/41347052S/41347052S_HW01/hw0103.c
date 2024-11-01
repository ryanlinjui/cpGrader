#include <stdio.h>
#include <stdint.h>

int main()
{
    uint32_t num;
    char a;

    printf("Please enter an unsigned 16_bits number : ");
    scanf("%u", &num);

    scanf("%c",&a);

    if(num > 65535)
    {
        printf("Error");
    }
    else
    {
        uint32_t  temp = num, octal = 0, place = 1, flipped_n = 0, flip=0;

        while(temp > 0)
        {
            flip = temp % 8;
            octal += (temp % 8)*place;
            flipped_n = flipped_n*10 + flip;
            temp /= 8;
            place *= 10;
        }

        printf("Before Flip :\n %u_10 = %u_8\n", num, octal);

        unsigned int flipped_o = 0;
        temp = flipped_n;
        place = 1;

        while(temp > 0)
        {
            flipped_o += (temp % 10)*place;
            temp /= 10;
            place *= 8;
        }

        printf("After Flip  :\n %u_8 = %u_10\n", flipped_n, flipped_o);
    }

    return 0;
}