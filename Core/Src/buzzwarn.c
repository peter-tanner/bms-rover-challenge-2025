#include "buzzwarn.h"
#include "main.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_hal_tim.h"
#include <stdint.h>

// #define MUTE

extern TIM_HandleTypeDef htim2, htim7;

/*             +100%   NORM
 *  siren high  1130    570
 *  siren low   850     429
 *  tone        959     484
 *
 *  1           1195
 *  2           1129
 *  3           1053
 *  4           983
 */

#define PWM_PRESCALER_FCLK 1000000
#define TONE_PRESCALER_FCLK 1000
#define TONE_TIMER &htim7
#define PWM_TIMER &htim2
#define PWM_TIMER_CH TIM_CHANNEL_1

void set_buzz_freq(uint32_t f)
{
    __HAL_TIM_SET_AUTORELOAD(PWM_TIMER, PWM_PRESCALER_FCLK / f - 1);
    __HAL_TIM_SET_COMPARE(PWM_TIMER, PWM_TIMER_CH, PWM_PRESCALER_FCLK / f / 2);
}

#define TONE_LENGTH 4

uint32_t tones[TONE_LENGTH];
uint32_t pulses = 0;
enum fault_lamp_mode fault_lamp_mode = FL_DISABLED;
uint8_t current_priority = 0;

void sfx_onetone(const uint32_t tone_freq, const uint32_t pulse_ms, const uint8_t priority)
{
    if (priority < current_priority)
        return;
    current_priority = priority;

    tones[0] = tone_freq;
    tones[1] = tone_freq;
    tones[2] = tone_freq;
    tones[3] = tone_freq;
    start_tone_sequence(pulse_ms / 10, 10);
}

void sfx_twotone(const uint32_t tone_pulses, const uint32_t pulse_ms, const uint8_t priority)
{
    if (priority < current_priority)
        return;
    current_priority = priority;

    tones[0] = 1130;
    tones[1] = 850;
    tones[2] = 1130;
    tones[3] = 850;
    start_tone_sequence(tone_pulses, pulse_ms);
}

void sfx_fourtone(const uint8_t priority)
{
    if (priority < current_priority)
        return;
    current_priority = priority;

    tones[0] = 983;
    tones[1] = 1053;
    tones[2] = 1129;
    tones[3] = 1195;
    start_tone_sequence(4, 100);
}

void start_tone_sequence(const uint32_t tone_pulses, const uint32_t pulse_ms)
{
    pulses = tone_pulses;

    if (fault_lamp_mode)
        HAL_GPIO_WritePin(FAULT_LAMP_GPIO_Port, FAULT_LAMP_Pin, GPIO_PIN_SET);

    __HAL_TIM_SET_COUNTER(PWM_TIMER, 0);
    sfx_tone_cb();
#ifndef MUTE
    HAL_TIM_PWM_Start_IT(PWM_TIMER, PWM_TIMER_CH);
#endif // MUTE

    HAL_TIM_Base_Stop_IT(TONE_TIMER);
    __HAL_TIM_SET_AUTORELOAD(TONE_TIMER, TONE_PRESCALER_FCLK * pulse_ms / 1000 - 1);
    HAL_TIM_GenerateEvent(TONE_TIMER, TIM_EVENTSOURCE_UPDATE);
    HAL_TIM_Base_Start_IT(TONE_TIMER);
}

void mask_fault_lamp(enum fault_lamp_mode mode)
{
    fault_lamp_mode = mode;
}

void sfx_tone_cb()
{
    set_buzz_freq(tones[(pulses-- - 1) % TONE_LENGTH]);
    if (fault_lamp_mode != FL_DISABLED)
        HAL_GPIO_TogglePin(FAULT_LAMP_GPIO_Port, FAULT_LAMP_Pin);
    if (pulses == -1)
    {
        current_priority = 0;
        HAL_TIM_PWM_Stop_IT(PWM_TIMER, PWM_TIMER_CH);

        if (fault_lamp_mode == FL_ONESHOT)
        {
            fault_lamp_mode = FL_DISABLED;
            HAL_TIM_Base_Stop_IT(TONE_TIMER);
            HAL_GPIO_WritePin(FAULT_LAMP_GPIO_Port, FAULT_LAMP_Pin, GPIO_PIN_RESET);
        }
    }
}
