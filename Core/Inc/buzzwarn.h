#ifndef __BUZZWARN_H
#define __BUZZWARN_H

#include <stdint.h>

void sfx_tone_cb();
void sfx_fourtone(const uint8_t priority);
void sfx_twotone(const uint32_t tone_pulses, const uint32_t pulse_ms, const uint8_t priority);
void sfx_onetone(const uint32_t tone_freq, const uint32_t pulse_ms, const uint8_t priority);
void start_tone_sequence(const uint32_t tone_pulses, const uint32_t pulse_ms);

enum fault_lamp_mode
{
    FL_DISABLED = 0,
    FL_REPEAT = 1,
    FL_ONESHOT = 2,
};

void mask_fault_lamp(enum fault_lamp_mode mode);

#endif // __BUZZWARN_H