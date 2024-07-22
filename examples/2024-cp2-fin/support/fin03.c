#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "linuxlist.h"

const char *COLUMNS[] = {"FirstName","LastName", "phone", "year"};
const size_t COLUMNS_SIZE = sizeof(COLUMNS) / sizeof(COLUMNS[0]);

static inline void INIT_LIST_HEAD(struct list_head *head)
{
    head->next = head;
    head->prev = head;
}

struct list_head *new_column_list(const char *columns[], int columns_size)
{
    struct list_head *column_list = (struct list_head *)malloc(sizeof(struct list_head));
    if (!column_list)
    {
        return NULL;
    }
    INIT_LIST_HEAD(column_list);

    for (size_t i = 0; i < columns_size; i++)
    {
        sLabel *label = (sLabel *)malloc(sizeof(sLabel));
        if (!label)
        {
            return NULL;
        }
        label->pStr = strdup(columns[i]);
        list_add_tail(&label->list, column_list);
    }

    return column_list;
}

int main()
{
    struct list_head *column_list = new_column_list(COLUMNS, COLUMNS_SIZE);
    if (!column_list)
    {
        return EXIT_FAILURE;
    }

    if (setup_table(column_list) == -1)
    {
        return EXIT_FAILURE;
    }

    struct list_head *record_list = (struct list_head *)malloc(sizeof(struct list_head));
    if (!record_list)
    {
        return EXIT_FAILURE;
    }
    INIT_LIST_HEAD(record_list);

    while (1)
    {
        printf("Current table size: %d\n", get_size(record_list));
        printf("Do you want to add a record? (y/n): ");
        char c;
        scanf("%c", &c);
        fgetc(stdin);
        if (c != 'y')
        {
            break;
        }

        sRecord *record = (sRecord *)malloc(sizeof(sRecord));
        if (!record)
        {
            return EXIT_FAILURE;
        }
        INIT_LIST_HEAD(&record->data);
        INIT_LIST_HEAD(&record->list);

        for (size_t i = 0; i < COLUMNS_SIZE; i++)
        {
            sItem *item = (sItem *)malloc(sizeof(sItem));
            if (!item)
            {
                return EXIT_FAILURE;
            }

            printf("%s: ", COLUMNS[i]);
            item->pData = (char *)malloc(256);
            if (!item->pData)
            {
                return EXIT_FAILURE;
            }
            scanf("%[^\n]", item->pData);
            fgetc(stdin);

            list_add_tail(&item->next, &record->data);
        }

        if (add(record_list, record) == -1)
        {
            return EXIT_FAILURE;
        }
    }

    while (1)
    {
        printf("Do you want to query the database? (y/n): ");
        char c;
        scanf("%c", &c);
        fgetc(stdin);
        if (c != 'y')
        {
            break;
        }

        char cmd[256];
        printf("Enter the query command: ");
        scanf("%[^\n]", cmd);
        fgetc(stdin);

        struct list_head *result_list = (struct list_head *)malloc(sizeof(struct list_head));
        if (!result_list)
        {
            return EXIT_FAILURE;
        }
        INIT_LIST_HEAD(result_list);

        int32_t result_count = query(result_list, record_list, cmd);
        printf("Query Result count: %d\n", result_count);
        struct list_head *pos;
        list_for_each(pos, result_list)
        {
            sRecord *record = list_entry(pos, sRecord, list);
            struct list_head *pos_item;
            size_t idx = 0;
            list_for_each(pos_item, &record->data)
            {
                sItem *item = list_entry(pos_item, sItem, next);
                printf("%s: %s, ", COLUMNS[idx], item->pData);
                idx++;
            }
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}