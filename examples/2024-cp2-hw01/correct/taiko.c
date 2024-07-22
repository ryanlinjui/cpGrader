#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "taiko.h"

#define BUFFER_SIZE 1000

#define INIT_BPM_TAG "BPM:"
#define OFFSET_TAG "OFFSET:"

#define COURSE_TAG "COURSE:"
#define START_TAG "#START"
#define MEASURE_TAG "#MEASURE "
#define BPM_CHANGE "#BPMCHANGE "
#define END_TAG "#END"
#define MEASURE_SLICE_TAG ','
#define CHART_HEADER_CFG_TAG ":"
#define CHART_CFG_TAG "#"

#define COURSE_SYMBOL_EDIT "Edit"
#define COURSE_SYMBOL_EDIT_NUM "4"
#define COURSE_SYMBOL_ONI "Oni"
#define COURSE_SYMBOL_ONI_NUM "3"
#define COURSE_SYMBOL_HARD "Hard"
#define COURSE_SYMBOL_HARD_NUM "2"
#define COURSE_SYMBOL_NORMAL "Normal"
#define COURSE_SYMBOL_NORMAL_NUM "1"
#define COURSE_SYMBOL_EASY "Easy"
#define COURSE_SYMBOL_EASY_NUM "0"

// utility function define
size_t io_read(char***);
bool set_chart(Chart *, const float, char**, const uint64_t, const uint64_t);
uint8_t is_chart_content(const char*);
Measure add_measure(const char*, const float, const uint64_t, const uint64_t);
bool is_exist_taiko_note(const char*);

bool ReadtjaFile(Taiko **src)
{
    // temp variable
    float offset = 0.0;
    float init_bpm = 0.0;
    uint64_t course = 0;

    // Read tja text file from stdin io
    char **text = NULL;
    size_t line = io_read(&text);
    
    *src = (Taiko*)calloc(1, sizeof(Taiko));

    (*src)->offset = 0;
    for (int i = 0; i < 5; i++)
    {
        ((*src)->chart)[i] = NULL;
    }
    
    // Process tja text file
    for (int i = 0; i < line; i++)
    {
        if (strstr(text[i], OFFSET_TAG) != NULL) // get offset
        {
            if (sscanf(text[i], OFFSET_TAG"%f", &offset) != 1)
            {
                fprintf(stderr, "offset error when getting\n");
                return false;
            }
            else
            {
                (*src)->offset = offset;
            }
        }
        else if (strstr(text[i], INIT_BPM_TAG) != NULL) // get bpm
        {
            if (sscanf(text[i], INIT_BPM_TAG"%f", &init_bpm) != 1)
            {   
                fprintf(stderr, "bpm error when getting\n");
                return false;
            }
        }
        else if (strstr(text[i], COURSE_TAG) != NULL) // get course and start to set Taiko->chart
        {
            if (strstr(text[i], COURSE_SYMBOL_EDIT) != NULL || strstr(text[i], COURSE_SYMBOL_EDIT_NUM) != NULL)
            {
                course = EDIT;
            }
            else if (strstr(text[i], COURSE_SYMBOL_ONI) != NULL || strstr(text[i], COURSE_SYMBOL_ONI_NUM) != NULL)
            {
                course = ONI;
            }
            else if (strstr(text[i], COURSE_SYMBOL_HARD) != NULL || strstr(text[i], COURSE_SYMBOL_HARD_NUM) != NULL)
            {
                course = HARD;
            }
            else if (strstr(text[i], COURSE_SYMBOL_NORMAL) != NULL || strstr(text[i], COURSE_SYMBOL_NORMAL_NUM) != NULL)
            {
                course = NORMAL;
            }
            else if (strstr(text[i], COURSE_SYMBOL_EASY) != NULL || strstr(text[i], COURSE_SYMBOL_EASY_NUM) != NULL)
            {
                course = EASY;
            }
            else
            {
                fprintf(stderr, "course value invalid\n");
                return false;
            }

            for (int j = i; i < line; j++)
            {
                if (strstr(text[j], END_TAG) != NULL)
                {
                    ((*src)->chart)[course] = (Chart*)calloc(1, sizeof(Chart));
                    if ( !set_chart((*src)->chart[course], init_bpm, text, i, j) )
                    {
                        return false;
                    }
                    ((*src)->chart)[course]->course = course;
                    break;
                }
            }
        }
    }

    return true;
}

void WritejsonFile(const Taiko *src)
{
    puts("{");
    puts("\t\"data\": [");
    for (int i = 4; i >= 0; i--)
    {
        if(src->chart[i] == NULL) continue;
        puts("\t\t{");
        printf("\t\t\t\"course\": %d,\n", src->chart[i]->course);
        puts("\t\t\t\"chart\": [");
        
        float music_time = -(src->offset);
        bool flag = false;

        for (int j = 0; j < src->chart[i]->measure_size; j++)
        {
            const char *measure = src->chart[i]->measure[j].chart_content;
            const uint64_t bpm = src->chart[i]->measure[j].bpm;
            const uint64_t beat = src->chart[i]->measure[j].beat;
            const uint64_t note = src->chart[i]->measure[j].note;
            
            const float duration = (60.0 / (float)bpm) *  ((float)beat / (float)strlen(measure)) * (4.0 / (float)note);
            
            for (int m = 0; m < strlen(measure); m++)
            {
                TaikoNote n = measure[m] - '0';            
                if (n >= DON && n <= BIGKA)
                {
                    if(flag) printf(",\n");
                    printf("\t\t\t\t[%d, %f]", n, music_time);
                    flag = true;
                }

                music_time += duration;
            }
        }
        flag = false;
        puts("\n\t\t\t]");
        printf("\t\t}");
        if(i <= 0)
        {
            printf("\n");
        }
        else
        {
            printf(",\n");
        }
        
    }
    puts("\t]");
    puts("}");
}

size_t io_read(char*** src)
{
    size_t size = 0;
    char *buffer = (char*)calloc(BUFFER_SIZE, sizeof(char*));
    while (scanf("%[^\n]%*c", buffer) != EOF)
    {
        *src = (char**)realloc(*src, (size + 1) * sizeof(char**));
        for (size_t i = 0; i < strlen(buffer); i++) 
        {
            if (buffer[i] == '\n' || buffer[i] == '\r') 
            {
                buffer[i] = 0;
            }
        }
        (*src)[size] = (char*)calloc(strlen(buffer)+1, sizeof(char*));        
        strncpy((*src)[size], buffer, strlen(buffer));
        memset(buffer, 0, BUFFER_SIZE);
        size++;
    }
    return size;
}

bool set_chart(Chart *chart, const float init_bpm, char **text, const uint64_t start, const uint64_t end)
{
    // set measure
    chart->measure = (Measure*)calloc(1, sizeof(Measure));
    chart->measure_size = 0;

    uint64_t beat = 4;
    uint64_t note = 4;
    float bpm = init_bpm;

    char *tmp_chart_content = (char*)calloc(1, sizeof(char));
    
    for (int i = start; i < end; i++)
    {
        uint8_t flag = is_chart_content(text[i]);

        // get now measure
        if (strstr(text[i], MEASURE_TAG) != NULL)
        {
            if (sscanf(text[i], MEASURE_TAG"%d/%d", &beat, &note) != 2)
            {
                fprintf(stderr, "measure error when getting\n");
                return false;
            }
        }
        else if (strstr(text[i], BPM_CHANGE) != NULL)
        {
            if (sscanf(text[i], BPM_CHANGE"%f", &bpm) != 1)
            {
                fprintf(stderr, "bpm_change error when getting\n");
                return false;
            }
        }
        else if (flag > 0)
        {
            if (flag == 1)
            {
                text[i][strlen(text[i]) - 1] = 0;
            }
            
            tmp_chart_content = (char *)realloc(tmp_chart_content, sizeof(char) * ((strlen(text[i]) + strlen(tmp_chart_content) + 1)));
            tmp_chart_content[strlen(text[i]) + strlen(tmp_chart_content)] = 0;
            strncat(tmp_chart_content, text[i], strlen(text[i]));

            if (flag == 1)
            {
                chart->measure = (Measure*)realloc(chart->measure, (chart->measure_size + 1) * sizeof(Measure));
                (chart->measure)[chart->measure_size] = add_measure(tmp_chart_content, bpm, beat, note);
                chart->measure_size++;
                memset(tmp_chart_content, 0, strlen(tmp_chart_content) * sizeof(char));
            }
        }
    }
    return true;
}


uint64_t lcm(uint64_t a, uint64_t b) 
{
    uint64_t tempA = a, tempB = b;

    while (a != b) 
    {
        if (a < b) 
        {
            a += tempA;
        } else 
        {
            b += tempB;
        }
    }

    return a;
}

Measure add_measure(const char *chart_content, const float bpm, const uint64_t beat, const uint64_t note)
{
    Measure tmp_m;
    tmp_m.beat = beat;
    tmp_m.bpm = bpm;
    tmp_m.note = note;
    size_t len_chart_content = strlen(chart_content);

    if(len_chart_content < beat)
    {
        uint64_t temp_beat = 0;
        if(len_chart_content == 0)
        {
            tmp_m.chart_content = (char*)calloc(beat + 1, sizeof(char));
            temp_beat = beat;
        } 
        else
        {
            temp_beat = lcm(len_chart_content, beat);
            tmp_m.chart_content = (char*)calloc(temp_beat + 1, sizeof(char));
        } 

        for (int i = 0; i < temp_beat; i++)
        {
            if(len_chart_content == 0)
            {
                tmp_m.chart_content[i] = BLANK + '0';
            }
            else if(i % (temp_beat/len_chart_content) == 0)
            {
                tmp_m.chart_content[i] = chart_content[i/(temp_beat/len_chart_content)];
            }
            else
            {
                tmp_m.chart_content[i] = BLANK + '0';
            }
        }
    }
    else
    {
        tmp_m.chart_content = (char*)calloc(len_chart_content + 1, sizeof(char));
        strncpy(tmp_m.chart_content, chart_content, len_chart_content);
    }
    
    return tmp_m;
}

uint8_t is_chart_content(const char *str)
{
    size_t str_len = strlen(str);

    if (strstr(str, CHART_CFG_TAG) != NULL || strstr(str,CHART_HEADER_CFG_TAG) != NULL)
    {
        return 0;
    }
    if (str[str_len - 1] == MEASURE_SLICE_TAG)
    {
        return 1;
    }
    else if(str[str_len - 1] >= '0' && str[str_len - 2] <= '9')
    {
        return 2;
    }
    return 0;
}