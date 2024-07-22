#include <stdint.h>

// First, you should have a rule array, where its size is 100 elements. 
// Then set rule[idx] to the input function pointer.
// Rule Function Pointer:
// int32_t (*rule)( const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size )
// 
//      Input:  p_input_packet -> input packet (one packet)
//              input_size -> input_packet size
//      Output: pp_output_packet -> output_packet (one packet)
//              p_output_size -> output_packet size
//              return -> 1 if the input packet is dropped, -1 if the input is invalid and skip this rule; otherwise, return 0
// If the inputs are invalid, return -1; otherwise, return 0.
int32_t set_rule( int32_t idx, int32_t (*rule)( const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t * p_output_size ) );

// Set rule[idx] to NULL.
// If the inputs are invalid, return -1; otherwise, return 0.
int32_t unset_rule( int32_t idx );

// For every input packet, apply all rules on the packet and store every ouput packet on the pp_output_packets.
// If the inputs are invalid, return -1; otherwise, return 0.
int32_t filter( const uint8_t *p_input_packets, const int32_t input_size, uint8_t **pp_output_packets, int32_t *p_output_size );