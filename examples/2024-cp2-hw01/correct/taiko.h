#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum _Course {
    EASY = 0,
    NORMAL,
    HARD,
    ONI,
    EDIT
} Course;

typedef enum _TaikoNote { 
    BLANK = 0,
    DON,
    KA,
    BIGDON,
    BIGKA,
    DRUMROLL,
    BIGDRUMROLL,
    BALLOON,
    END,
    POTATO
} TaikoNote;

typedef struct _Measure
{
    char *chart_content;
    float bpm;
    uint64_t beat;
    uint64_t note;
} Measure;

typedef struct _Chart
{
    Course course;
    Measure *measure;
    uint64_t measure_size;
} Chart;

typedef struct _Taiko
{
    Chart *chart[5];
    float offset;
} Taiko;

bool ReadtjaFile(Taiko **);
void WritejsonFile(const Taiko *);