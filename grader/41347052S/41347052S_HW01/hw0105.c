#include <stdio.h>
#include <stdint.h>


int main()
{
    uint32_t input;
    int32_t choice;

    printf("Please input a hex : ");
    scanf("%x", &input);
    

    printf("Please choose the output type (1: integer, 2: unsigned integer, 3: float) : ");
    scanf("%d", &choice);

    printf("Binary of %X is : ", input);

    int32_t num = input;

    if((num >> 15)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    
    if ((num >> 14)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if ((num >> 13)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    
    if ((num >> 12)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    
    printf(" ");

    if ((num >> 11)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }


    if ((num >> 10)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if ((num >> 9)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if ((num >> 8)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    printf(" ");

    if ((num >> 7)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }


    if ((num >> 6)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if ((num >> 5)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if ((num >> 4)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    
    printf(" ");

    if ((num >> 3)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }


    if ((num >> 2)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if ((num >> 1)& 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }

    if(num & 1)
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    
    printf("\n");
    


    switch(choice)
    {
        case 1:
        {
            int16_t integer = (int16_t) input;
            printf("Converted signed integer is : %d\n", integer);
            break;
        }
        case 2:
        {
            printf("Converted unsigned integer is : %u\n", input);
            break;
        }
        case 3:
        {
            float Float;
            Float = *(float*)&input;
           
            printf("Converted float is : %.6f * 2^8\n", Float);
            break;
        }
        default:
        {
            printf("Error.Choice only 1-3\n");
            break;
        }
    }

    return 0;
}


