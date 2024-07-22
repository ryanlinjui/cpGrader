#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "taiko.h"

int main() 
{
    Taiko *src = NULL;
    
    if ( !ReadtjaFile(&src) ) return 0;


    // printf("%f\n", src->offset);
    // for (int i = 0; i >= 0; i--)
    // {
    //     if ( src->chart[i] == NULL)
    //     {
    //         printf("%d: NULL\n\n",i);
    //     }
    //     else
    //     {
    //         printf("course: %d\n", src->chart[i]->course);
    //         printf("\n\n\n");

    //         printf("m_s:%d\n", src->chart[i]->measure_size);
    //         for (int j = 0; j < src->chart[i]->measure_size; j++)
    //         {
    //             printf("content: %s\n",src->chart[i]->measure[j].chart_content);
    //             printf("len: %d\n",strlen(src->chart[i]->measure[j].chart_content));
    //             printf("bpm: %d, beat: %d, note: %d\n", src->chart[i]->measure[j].bpm, src->chart[i]->measure[j].beat, src->chart[i]->measure[j].note);
    //             printf("=================================\n");
    //         }
    //     }
        
    // }
    
    WritejsonFile(src);

    return 0;
}