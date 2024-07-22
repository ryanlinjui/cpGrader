#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "firewall.h"

#define MAX_RULES 100
#define OFFSET 10

typedef int32_t (*RuleFunctionPointer)(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size);

RuleFunctionPointer rules[MAX_RULES] = {NULL};

int32_t set_rule(int32_t idx, RuleFunctionPointer rule) 
{
    if (idx < 0 || idx >= MAX_RULES || rule == NULL) 
    {
        return -1;
    }
    
    rules[idx] = rule;
    return 0;
}

int32_t unset_rule(int32_t idx) 
{
    if (idx < 0 || idx >= MAX_RULES) 
    {
        return -1;
    }
    
    rules[idx] = NULL;
    return 0;
}

int32_t filter(const uint8_t *p_input_packets,
               const int32_t input_size,
               uint8_t **pp_output_packets,
               int32_t *p_output_size) 
{
    if (p_input_packets == NULL || input_size <= 0)
    {
        return -1;
    }
    *p_output_size = 0;

    for (int count = 0; count < input_size;) 
    {
        const uint8_t *p_input_packet = &(p_input_packets[count]);
        const uint32_t p_input_packet_size = ((uint16_t *)(p_input_packet))[4] + OFFSET; // data_size + offset
        
        uint8_t *tmp_input_packet = malloc(p_input_packet_size * sizeof(uint8_t));
        memcpy(tmp_input_packet, p_input_packet, p_input_packet_size * sizeof(uint8_t));
        uint32_t tmp_input_size = p_input_packet_size;
        int32_t result = 0;

        for (int i = 0; i < MAX_RULES; i++) 
        {
            if (rules[i] == NULL) continue;
            uint8_t *tmp_output_packet = NULL;
            int32_t tmp_output_size = 0;
            
            result = rules[i](tmp_input_packet, tmp_input_size, &tmp_output_packet, &tmp_output_size);
            
            if (result == 1) break;
            else if (result == -1) continue;
            
            free(tmp_input_packet);
            tmp_input_packet = malloc(tmp_output_size * sizeof(uint8_t));
            tmp_input_size = tmp_output_size;
            memcpy(tmp_input_packet, tmp_output_packet, tmp_output_size * sizeof(uint8_t));
            free(tmp_output_packet);
        }

        if(result != 1)
        {
            *pp_output_packets = realloc(*pp_output_packets, (*p_output_size + tmp_input_size) * sizeof(uint8_t));
            memcpy(*pp_output_packets + (*p_output_size), tmp_input_packet, tmp_input_size * sizeof(uint8_t));
            (*p_output_size) += tmp_input_size;
        }
        count += p_input_packet_size;
    }
    return 0;
}