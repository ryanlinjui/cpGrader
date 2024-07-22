#pragma once

#include <stdint.h>

#include "linuxlist.h"

typedef struct _sLabel
{
    char *pStr;
    struct list_head list;
} sLabel;

typedef struct _sRecord
{
    struct list_head data; // Data for each item in this record
    struct list_head list; // List for the next record
} sRecord;

typedef struct _sItem
{
    char *pData;
    struct list_head next; // Data for each item in this record
} sItem;

// Setup table labels.
// Note that this function must be called before all other functions.
// Input:
//     pLabelList: a list of sLabel.
// Return:
//     0: Success; -1: Error
int32_t setup_table(const struct list_head *pLabelList);

// Add a record.
// Input:
//    pRecordList: record list head.
//    pRecord: the new record;
// Return:
//    0: Success; -1: Error
int32_t add(struct list_head *pRecordList, sRecord *pRecord);

// Get the record list size.
// Input:
//    pRecordList: record list head.
// Return:
//    -1: Error; others , record size.
int32_t get_size(struct list_head *pRecordList);

// Query the database and get the result.
// Input:
//    pRecordList: record list head.
//    pCmd: the query command.
// Output:
//    pResultList: the queried result. Note that you should put the queried item an gnore all unqueried items.
// Return:
//    The number of query result.
int32_t query(struct list_head *pResultList, struct list_head *pRecordList, char *pCmd);