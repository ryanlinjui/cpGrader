#include <stdio.h>
#include <stdint.h>

int main()
{
    int32_t card1, card2, card3, card4, card5;
    int32_t rank1, rank2, rank3, rank4, rank5;
    int32_t a, b, c, d, e;
    int32_t flush=1, straight=1, pairCount=0, three=0, four=0, temp;
    char s;

    printf("Please enter 5 card : ");
    scanf("%d %d %d %d %d", &card1, &card2, &card3, &card4, &card5);
    scanf("%c", &s);
    if(card1<0 || card1>52 || card2<0 || card2>52 || card3<0 || card3>52 || card4<0 || card4>52 || card5<0 || card5>52)
    {
        printf("Error. Please try again\n");
        return 1;
    }

    rank1 = (card1-1) % 13+1;
    rank2 = (card2-1) % 13+1;
    rank3 = (card3-1) % 13+1;
    rank4 = (card4-1) % 13+1;
    rank5 = (card5-1) % 13+1;

    a = (card1-1) / 13;
    b = (card2-1) / 13;
    c = (card3-1) / 13;
    d = (card4-1) / 13;
    e = (card5-1) / 13;

    //flush
    if(a!= b || a!=c || a!=d || a!=e)
    {
        flush = 0;
    }

    // .......
    if(rank1 > rank2)
    {
        temp = rank1;
        rank1 = rank2;
        rank2 = temp;
    }
    if(rank2 > rank3)
    {
        temp = rank2;
        rank2 = rank3;
        rank3 = temp;
    }
    if(rank3 > rank4)
    {
        temp = rank3;
        rank3 = rank4;
        rank4 = temp;
    }
    if(rank4 > rank5)
    {
        temp = rank4;
        rank4 = rank5;
        rank5 = temp;
    }
    if(rank1 > rank2)
    {
        temp = rank1;
        rank1 = rank2;
        rank2 = temp;
    }
    if(rank2 > rank3)
    {
        temp = rank2;
        rank2 = rank3;
        rank3 = temp;
    }
    if(rank3 > rank4)
    {
        temp = rank3;
        rank3 = rank4;
        rank4 = temp;
    }
    if(rank1 > rank2)
    {
        temp = rank1;
        rank1 = rank2;
        rank2 = temp;
    }
    if(rank2 > rank3)
    {
        temp = rank2;
        rank2 = rank3;
        rank3 = temp;
    }

    // straight
    if(rank2 != rank1+1 || rank3 != rank2+1 || rank4 != rank3+1 || rank5 != rank4+1)
    {
        straight = 0;
    }

    // four of a kind, three of kind, pair
    if(rank1==rank2 && rank2==rank3 && rank3==rank4)
    {
        four = 1;
    }
    else if(rank2==rank3 && rank3==rank4 && rank4==rank5)
    {
        four = 1;
    }
    else if(rank1==rank2 && rank2==rank3)
    {
        three = 1;
    }
    else if(rank2==rank3 && rank3==rank4)
    {
        three = 1;
    }
    else if(rank3==rank4 && rank4==rank5)
    {
        three = 1;
    }
    else if(rank1 == rank2)
    {
        pairCount++;
    }
    else if(rank2 == rank3)
    {
        pairCount++;
    }
    else if(rank3 == rank4)
    {
        pairCount++;
    }
    else if(rank4 == rank5)
    {
        pairCount++;
    }

    // second pair
    if(rank1 == rank2 && (rank3==rank4 || rank4==rank5))
    {
        pairCount++;
    }
    else if(rank1==rank2 && rank4==rank5)
    {
        pairCount++;
    }

    // Output
    if(flush && straight)
    {
        printf("Straight Flush\n");
    }
    else if(four)
    {
        printf("Four of a Kind\n");
    }
    else if(three && pairCount==1)
    {
        printf("Full House\n");
    }
    else if(flush)
    {
        printf("Flush\n");
    }
    else if(straight)
    {
        printf("Straight\n");
    }
    else if(three)
    {
        printf("Three of a kind\n");
    }
    else if(pairCount == 2)
    {
        printf("Two Pair\n");
    }
    else if(pairCount == 1)
    {
        printf("One Pair\n");
    }
    else
    {
        printf("High Card\n");
    }

    return 0;
}