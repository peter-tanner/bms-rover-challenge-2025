
#ifndef __CAN_TYPES_H
#define __CAN_TYPES_H

#include "stm32g0xx_hal_uart.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

#define CREATE_PTR_L(type, ...) \
    &CREATE(type, __VA_ARGS__), sizeof(struct type)

#define CREATE(type, ...)             \
    (struct type)                     \
    {                                 \
        .zid = type##_ID, __VA_ARGS__ \
    }

#define CREATE_TYPE(type, type_id, ...)                                           \
    struct type __VA_ARGS__ __attribute__((packed));                              \
    static const uint8_t type##_ID = type_id;                                     \
    _Static_assert(sizeof((struct type){}.zid) == 1, "Needs uint8_t zid member"); \
    _Static_assert(sizeof(struct type) <= 8, "SIZE ERROR")

CREATE_TYPE(zero_dummy, 0, { uint8_t zid; });

CREATE_TYPE(cv_group, 1, {
    uint8_t zid;
    uint8_t offset : 4;
    enum event_type
    {
        FULLSCAN_E = 0,
        CUV_SNAPSHOT_E = 1,
        COV_SNAPSHOT_E = 2
    } event_type : 2;
    union {
        int16_t cv[3];
        struct last
        {
            int16_t cv_16;
            int16_t stack_cc2_current_userA;
        } last;
    };
});

#define FILL_CELL_VOLTAGES(cv_groups, type, cv1, cv2, cv3, cv4, cv5, cv6, cv7, cv8, cv9, cv10, cv11, cv12, cv13, cv14, cv15, cv16, cc2_current) \
    static_assert(sizeof(cv_groups) / sizeof(cv_groups[0]) == 6, "FAILED");                                                                     \
    cv_groups[0] = CREATE(cv_group,                                                                                                             \
                          .event_type = type, .offset = 0,                                                                                      \
                          .cv = {cv1, cv2, cv3});                                                                                               \
    cv_groups[1] = CREATE(cv_group,                                                                                                             \
                          .event_type = type, .offset = 3,                                                                                      \
                          .cv = {cv4, cv5, cv6});                                                                                               \
    cv_groups[2] = CREATE(cv_group,                                                                                                             \
                          .event_type = type, .offset = 6,                                                                                      \
                          .cv = {cv7, cv8, cv9});                                                                                               \
    cv_groups[3] = CREATE(cv_group,                                                                                                             \
                          .event_type = type, .offset = 9,                                                                                      \
                          .cv = {cv10, cv11, cv12});                                                                                            \
    cv_groups[4] = CREATE(cv_group,                                                                                                             \
                          .event_type = type, .offset = 12,                                                                                     \
                          .cv = {cv13, cv14, cv15});                                                                                            \
    cv_groups[5] = CREATE(cv_group,                                                                                                             \
                          .event_type = type, .offset = 15,                                                                                     \
                          .last = {.cv_16 = cv16, .stack_cc2_current_userA = cc2_current});

CREATE_TYPE(pack_voltage, 2, {
    uint8_t zid;
    int16_t stack_voltage_userV;
    int16_t pack_pin_voltage_userV;
    int16_t ld_pin_voltage_userV;
});

CREATE_TYPE(temperature, 3, {
    uint8_t zid;
    int16_t ts1_0K1;
    int16_t ts3_0K1;
    int16_t tint_0K1;
});

CREATE_TYPE(safety_status_A, 4, {
    uint8_t zid;
    uint8_t safety_status_A;
});

CREATE_TYPE(safety_status_BC, 5, {
    uint8_t zid;
    uint8_t safety_status_B;
    uint8_t safety_status_C;
});

CREATE_TYPE(pf_status, 6, {
    uint8_t zid;
    uint8_t pf_status_A;
    uint8_t pf_status_B;
    uint8_t pf_status_C;
    uint8_t pf_status_D;
});

CREATE_TYPE(pf_alert, 7, {
    uint8_t zid;
    uint8_t pf_alert_A;
    uint8_t pf_alert_B;
    uint8_t pf_alert_C;
    uint8_t pf_alert_D;
});

CREATE_TYPE(safety_alert, 8, {
    uint8_t zid;
    uint8_t safety_alert_A;
    uint8_t safety_alert_B;
    uint8_t safety_alert_C;
});

CREATE_TYPE(pack_status, 9, {
    uint8_t zid;
    uint8_t fet_status;
    uint16_t control_status;
    uint16_t battery_status;
});

CREATE_TYPE(alarm_event, 10, {
    uint8_t zid;
    uint16_t alarm_excl_ADSCAN_FULLSCAN;
});

CREATE_TYPE(cb_status, 11, {
    uint8_t zid;
    uint16_t cb_active_cells;
    uint16_t cb_present_time_s;
});

CREATE_TYPE(cb_time, 12, {
    uint8_t zid;
    uint8_t offset;
    uint32_t cb_time_s;
});

CREATE_TYPE(bms_status, 13, {
    uint8_t zid;
    enum
    {
        BMS_STATUS_INIT = 0x676767,
        BMS_STATUS_WAKE = 0xABC123,
        BMS_STATUS_SHUTDOWN = 0xD1ED1E,
    } bms_status;
});

#endif // __CAN_TYPES_H
