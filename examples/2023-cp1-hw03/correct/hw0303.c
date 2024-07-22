#include <stdio.h>

void printBinary(int n, int bit) {
    if (bit < 0) {
        printf("\n");
        return;
    }

    int mask = 1 << bit;
    char binaryDigit = (n & mask) ? '1' : '0';
    printf("%c", binaryDigit);

    if (bit % 8 == 0)
        printf(" ");

    printBinary(n, bit - 1);
}

int main() {
    int num;
    printf("Please enter the number: ");
    scanf("%d", &num);

    printBinary(num, 31);

    return 0;
}
