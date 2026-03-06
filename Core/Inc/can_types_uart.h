#define FULLSCAN_FMT                                    \
    ">c_16_mV:%dmV\r\n"                                 \
    "\r\n"                                              \
    ">CC2_mA:%dmA\r\n"                                  \
    ">pack_mV:%dmV\r\n"                                 \
    "\r\n"                                              \
    ">ts1_degC:%ddegC\r\n"                              \
    ">ts3_degC:%ddegC\r\n"                              \
    ">tint_degC:%ddegC\r\n"                             \
    "--- END FULLSCAN EVENT ---\r\n",                   \
        cv_groups[5].last.cv_16,                        \
        cv_groups[5].last.stack_cc2_current_userA * 10, \
        pack_voltage.stack_voltage_userV * 10,          \
        temperature.ts1_0K1 / 10 - 273,                 \
        temperature.ts3_0K1 / 10 - 273,                 \
        temperature.tint_0K1 / 10 - 273

#define SSBC_UART_FMT                                  \
    "--- SSB SSC EVENT ---\r\n"                        \
    ">B_OTF:%d\r\n"                                    \
    ">B_OTINT:%d\r\n"                                  \
    ">B_OTD:%d\r\n"                                    \
    ">B_OTC:%d\r\n"                                    \
    ">B_UTINT:%d\r\n"                                  \
    ">B_UTD:%d\r\n"                                    \
    ">B_UTC:%d\r\n"                                    \
    "\r\n"                                             \
    ">C_OCD3:%d\r\n"                                   \
    ">C_SCDL:%d\r\n"                                   \
    ">C_OCDL:%d\r\n"                                   \
    ">C_COVL:%d\r\n"                                   \
    ">C_PTO:%d\r\n"                                    \
    ">C_HWDF:%d\r\n"                                   \
    "--- END SSB SSC EVENT ---\r\n",                   \
        BIT_SafetyStatusB_OTF(ssbc.safety_status_B),   \
        BIT_SafetyStatusB_OTINT(ssbc.safety_status_B), \
        BIT_SafetyStatusB_OTD(ssbc.safety_status_B),   \
        BIT_SafetyStatusB_OTC(ssbc.safety_status_B),   \
        BIT_SafetyStatusB_UTINT(ssbc.safety_status_B), \
        BIT_SafetyStatusB_UTD(ssbc.safety_status_B),   \
        BIT_SafetyStatusB_UTC(ssbc.safety_status_B),   \
                                                       \
        BIT_SafetyStatusC_OCD3(ssbc.safety_status_C),  \
        BIT_SafetyStatusC_SCDL(ssbc.safety_status_C),  \
        BIT_SafetyStatusC_OCDL(ssbc.safety_status_C),  \
        BIT_SafetyStatusC_COVL(ssbc.safety_status_C),  \
        BIT_SafetyStatusC_PTO(ssbc.safety_status_C),   \
        BIT_SafetyStatusC_HWDF(ssbc.safety_status_C)

#define SSA_UART_FMT                                 \
    "--- SSA EVENT ---\r\n"                          \
    ">SCD:%d\r\n"                                    \
    ">OCD2:%d\r\n"                                   \
    ">OCD1:%d\r\n"                                   \
    ">OCC:%d\r\n"                                    \
    ">COV:%d\r\n"                                    \
    ">CUV:%d\r\n"                                    \
    "--- END SSA EVENT ---\r\n",                     \
        BIT_SafetyStatusA_SCD(ssa.safety_status_A),  \
        BIT_SafetyStatusA_OCD2(ssa.safety_status_A), \
        BIT_SafetyStatusA_OCD1(ssa.safety_status_A), \
        BIT_SafetyStatusA_OCC(ssa.safety_status_A),  \
        BIT_SafetyStatusA_COV(ssa.safety_status_A),  \
        BIT_SafetyStatusA_CUV(ssa.safety_status_A)

#define ALARM_UART_FMT                           \
    "--- ALARM EVENT ---\r\n"                    \
    ">SSBC:%d\r\n"                               \
    ">SSA:%d\r\n"                                \
    ">PF:%d\r\n"                                 \
    ">MSK_SFALERT:%d\r\n"                        \
    ">MSK_PFALERT:%d\r\n"                        \
    ">INITSTART:%d\r\n"                          \
    ">INITCOMP:%d\r\n"                           \
    ">XCHG:%d\r\n"                               \
    ">XDSG:%d\r\n"                               \
    ">SHUTV:%d\r\n"                              \
    ">FUSE:%d\r\n"                               \
    ">CB:%d\r\n"                                 \
    ">WAKE:%d\r\n"                               \
    "--- END ALARM EVENT ---\r\n",               \
        BIT_AlarmStatus_SSBC(event_bits),        \
        BIT_AlarmStatus_SSA(event_bits),         \
        BIT_AlarmStatus_PF(event_bits),          \
        BIT_AlarmStatus_MSK_SFALERT(event_bits), \
        BIT_AlarmStatus_MSK_PFALERT(event_bits), \
        BIT_AlarmStatus_INITSTART(event_bits),   \
        BIT_AlarmStatus_INITCOMP(event_bits),    \
        BIT_AlarmStatus_XCHG(event_bits),        \
        BIT_AlarmStatus_XDSG(event_bits),        \
        BIT_AlarmStatus_SHUTV(event_bits),       \
        BIT_AlarmStatus_FUSE(event_bits),        \
        BIT_AlarmStatus_CB(event_bits),          \
        BIT_AlarmStatus_WAKE(event_bits)
