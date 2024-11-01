#include <stdio.h>
#include <stdint.h>

int main()
{
    int32_t a, b, c;
    int32_t d, e, f;
    char x1, y1, z1,s;
    int32_t sum;

    printf("Please enter the first operand  : ");
    scanf("%d%c%d", &a, &x1, &b);
    if(a>9 || a<0 || b>9 || b<0)
    {
        printf("error. Please try again\n");
        return 1;
    }

    scanf("%c",&x1);

    printf("Please enter the second operand : ");
    scanf("%c%d%c", &y1, &c, &z1);
    if(c<0 || c>9)
    {
        printf("error. Please try again\n");
        return 1;
    }

    scanf("%c",&x1);

    printf("Please enter the sum : ");
    scanf("%d", &sum);
    if(sum > 1998)
    {
        printf("error.Please enter");
        return 1;
    }

    d = sum/100;
    e = (sum/10)%10;
    f = sum%10;


    

    int32_t x, y, z;
    int32_t plus = 0;


    if(b < f)
    {
        z = f-b;
    }
    else if(b > f)
    {
        z = (f+10)-b;
        plus = 1;
    }
    else if(b = 0)
    {
        z = 0;
    }
//......
    if(c < e && plus == 1)
    {
        x = e-c-plus;
        plus=0;
    }
    else if(c < e)
    {
        x = e-c;
        plus = 0; 
    }
    else if(c > e && plus == 1)
    {
        x = (e+10)-(c+plus);
        plus = 1;
    }
    else if(c > e)
    {
        x = (e+10)-c;
        plus = 1;
    }
    else if(c == e && plus == 1)
    {
        x = (e+10)-(c+plus);
        plus = 1;
    }
    else if(c == e)
    {
        x = 0;
        plus = 0;
    }
    
//.......
    if(a < d && plus == 1)
    {
        y = d-(a+plus);
    }
    else if(a < d)
    {
        y = d-a;
    }
    else if(a > d)
    {
        y = (d+10)-a;
    }
    else if(a > d &&  plus == 1)
    {
        y = (d+10)-(a+plus);
    }
    else if(a == d && plus == 1)
    {
        y = (a+plus)-d;
    }
    else if(a == d)
    {
        y = 0;
    }

    printf("Ans : x=%d , y=%d , z=%d\n",x, y , z);

    return 0;
    
}