#ifndef __HELPER_H
#define __HELPER_H

#define IF_DEBOUNCE_CAT_(a, b) a##b
#define IF_DEBOUNCE_CAT(a, b) IF_DEBOUNCE_CAT_(a, b)
#define if_debounce(cond_expr, interval_ms)                                                                                   \
    static uint32_t IF_DEBOUNCE_CAT(_if_debounce_tick_, __LINE__) = 0;                                                        \
    for (uint8_t _if_debounce_once = 1;                                                                                       \
         (_if_debounce_once) && (cond_expr) && (HAL_GetTick() - IF_DEBOUNCE_CAT(_if_debounce_tick_, __LINE__) > interval_ms); \
         IF_DEBOUNCE_CAT(_if_debounce_tick_, __LINE__) = HAL_GetTick(), _if_debounce_once = 0)

#define debounce(interval_ms)                                                                                  \
    static uint32_t IF_DEBOUNCE_CAT(_if_debounce_tick_, __LINE__) = 0;                                         \
    for (uint8_t _if_debounce_once = 1;                                                                        \
         (_if_debounce_once) && (HAL_GetTick() - IF_DEBOUNCE_CAT(_if_debounce_tick_, __LINE__) > interval_ms); \
         IF_DEBOUNCE_CAT(_if_debounce_tick_, __LINE__) = HAL_GetTick(), _if_debounce_once = 0)

#endif // __HELPER_H
