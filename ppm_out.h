#ifndef PPM_OUT_H__
#define PPM_OUT_H__

#include "main.h"

#define PPM_OUT_CHANNELS 6

uint16_t ppm_out_next[PPM_OUT_CHANNELS];
uint16_t ppm_out_comp[PPM_OUT_CHANNELS];

uint8_t ppm_out_index;
uint16_t ppm_out_idletime;

void ppm_out_init(void);
void ppm_out_set_aetr(uint8_t aeleron, uint8_t elevation, uint8_t throttle, uint8_t roll);

#endif
