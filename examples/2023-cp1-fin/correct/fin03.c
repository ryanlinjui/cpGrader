#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "firewall.h"

typedef int32_t (*RuleFunctionPointer)(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size);

// Rule 1: If source ID is 1, set destination ID to 5.
int32_t rule1(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size)
{
    printf("hihihi!!!!!\n");
    if (p_input_packet == NULL || input_size <= 0)
    {
        return -1;
    }

    *pp_output_packet = (uint8_t *)malloc(input_size * sizeof(uint8_t));
    memcpy(*pp_output_packet, p_input_packet, input_size * sizeof(uint8_t));
    
    uint32_t source_id = ((uint32_t *)p_input_packet)[0];
    printf("rule1: id: %d\n", source_id);
    if (source_id == 1)
    {
        ((uint32_t *)(*pp_output_packet))[1] = 62; // change destination ID to 5.   
    }

    *p_output_size = input_size;
    return 0;
}

// Rule 2: If source ID is 2, set source ID to 7.
int32_t rule2(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size) 
{
    printf("nonono!!!!!\n");
    if (p_input_packet == NULL || input_size <= 0)
    {
        return -1;
    }

    *pp_output_packet = (uint8_t *)malloc(input_size * sizeof(uint8_t));
    memcpy(*pp_output_packet, p_input_packet, input_size * sizeof(uint8_t));
    
    uint32_t source_id = ((uint32_t *)p_input_packet)[0];
    printf("rule2: id: %d\n", source_id);
    if (source_id == 2)
    {
        ((uint32_t *)(*pp_output_packet))[0] = 256; // change destination ID to 7.   
    }

    *p_output_size = input_size;
    return 0;
}

// // Rule 3: If size is 2, duplicate data.
int32_t rule3(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size) 
{
    if (p_input_packet == NULL || input_size <= 0)
    {
        return -1;
    }

    uint16_t packet_size = ((uint16_t *)p_input_packet)[4];
    printf("rule3: size: %d\n", packet_size);
    if (packet_size == 2) 
    {
        *pp_output_packet = (uint8_t*)malloc((input_size + packet_size) * sizeof(uint8_t));
        memcpy(*pp_output_packet, p_input_packet, input_size * sizeof(uint8_t));
        memcpy(*pp_output_packet + input_size, p_input_packet + input_size - packet_size, packet_size * sizeof(uint8_t)); // duplicate data
        *p_output_size = input_size + packet_size;
    }
    else
    {
        *pp_output_packet = (uint8_t *)malloc(input_size * sizeof(uint8_t));
        memcpy(*pp_output_packet, p_input_packet, input_size * sizeof(uint8_t));
        *p_output_size = input_size;
    }

    return 0;
}

// Rule 4: If source ID is 7 and destination ID is 3, Drop the packet.
int32_t rule4(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size) 
{
    if (p_input_packet == NULL || input_size <= 0)
    {
        return -1;
    }

    uint32_t source_id = ((uint32_t *)p_input_packet)[0];
    uint32_t destination_id = ((uint32_t *)p_input_packet)[1];

    printf("rule4: s_id: %d, d_id: %d\n", source_id, destination_id);
    if (source_id == 7 && destination_id == 3)
    {
        return 1; // drop signal
    }

    *pp_output_packet = (uint8_t *)malloc(input_size * sizeof(uint8_t));
    memcpy(*pp_output_packet, p_input_packet, input_size * sizeof(uint8_t));
    *p_output_size = input_size;
    return 0;
}

int main()
{
    uint8_t array[] = { 0x01, 0x00, 0x00, 0x00, // Packet 1 -> Source ID: 1
                        0x02, 0x00, 0x00, 0x00, // Destination ID: 2
                        0x03, 0x00,             // Size
                        0x01, 0x02, 0x03,       // Data
                        0x01, 0x00, 0x00, 0x00, // Packet 2 -> Source ID: 1
                        0x03, 0x00, 0x00, 0x00, // Destination ID: 3
                        0x02, 0x00,             // Size
                        0xEE, 0xFF,             // Data
                        0x02, 0x00, 0x00, 0x00, // Packet 3 -> Source ID: 2
                        0x03, 0x00, 0x00, 0x00, // Destination ID: 3
                        0x04, 0x00,             // Size
                        0x00, 0x00, 0x01, 0x02  // Data
    };

    size_t array_len = sizeof(array)/sizeof(array[0]);
    
    uint8_t *inputPacket = malloc(array_len * sizeof(uint8_t));
    memcpy(inputPacket, array, array_len);
    int32_t inputSize = array_len; 
    uint8_t *outputPacket;
    int32_t outputSize;

    printf("%d, ", set_rule(0, rule1));
    printf("%d, ", set_rule(-1, rule1));
    printf("%d, ", set_rule(100, rule1));
    printf("%d, ", unset_rule(-1));
    printf("%d, ", unset_rule(100));
    printf("%d, ", unset_rule(0));
    printf("%d\n", set_rule(5, rule1));
    printf("%d\n", set_rule(92, rule2));
    printf("%d\n", unset_rule(92));

    printf("rule1: %d\n", set_rule(0, rule1));
    printf("rule2: %d\n", set_rule(10, rule2));
    printf("rule3: %d\n", set_rule(20, rule3));
    printf("rule4: %d\n", set_rule(99, rule4));
    // printf("filter: %d\n", filter(inputPacket, inputSize, &outputPacket, &outputSize));
    
    // Case 3
    uint8_t ans_array[] = { 0x01, 0x00, 0x00, 0x00, // Packet 1 -> Source ID: 1
                            0x3E, 0x00, 0x00, 0x00, // Destination ID: 62
                            0x03, 0x00,             // Size
                            0x01, 0x02, 0x03,       // Data
                            0x01, 0x00, 0x00, 0x00, // Packet 2 -> Source ID: 1
                            0x3E, 0x00, 0x00, 0x00, // Destination ID: 62
                            0x02, 0x00,             // Size
                            0xEE, 0xFF,             // Data
                            0x00, 0x01, 0x00, 0x00, // Packet 3 -> Source ID: 256
                            0x03, 0x00, 0x00, 0x00, // Destination ID: 3
                            0x04, 0x00,             // Size
                            0x00, 0x00, 0x01, 0x02  // Data
    };

    printf("input_size: %d, output_size: %d\n", inputSize, outputSize);
    for (int i = 0; i < inputSize; i++)
    {
        printf("%d, ", inputPacket[i]);
    }
    printf("\n");
    for (int i = 0; i < outputSize; i++)
    {
        printf("%d, ", outputPacket[i]);
    }
    // printf("\n");
    // for (int i = 0; i < sizeof(ans_array)/sizeof(ans_array[0]); i++)
    // {
    //     printf("%d, ", ans_array[i]);
    // }

    printf("\n=============end===========\n");
    return 0;
}