#include "stm32g0xx_hal_fdcan.h"

/*
bit 10 9 8 7 6 5 4 3 2 1 0
     m m r r D D D D D D D

m = message type -> 00: critical errors | 01: command | 10: fast telemetry | 11: slow telemetry
r = reserved(11) -> for finer prio control if we need it
D = node ID -> lower id = higher priority (0x7F - 0x00)
*/

#define CAN_MSGTYPE_ERROR (0b00 << 9)
#define CAN_MSGTYPE_CMD (0b01 << 9)
#define CAN_MSGTYPE_FAST_TELEM (0b10 << 9)
#define CAN_MSGTYPE_SLOW_TELEM (0b11 << 9)

#define CAN_ID_START (0x67 << 0)

#define DEFAULT_CAN_HEADER                    \
    .IdType = FDCAN_STANDARD_ID,              \
    .TxFrameType = FDCAN_DATA_FRAME,          \
    .DataLength = FDCAN_DLC_BYTES_8,          \
    .ErrorStateIndicator = FDCAN_ESI_ACTIVE,  \
    .BitRateSwitch = FDCAN_BRS_OFF,           \
    .FDFormat = FDCAN_CLASSIC_CAN,            \
    .TxEventFifoControl = FDCAN_NO_TX_EVENTS, \
    .MessageMarker = 0,

FDCAN_TxHeaderTypeDef can_classic_tx_header = {
    .Identifier = 0x67 | CAN_MSGTYPE_SLOW_TELEM,
    DEFAULT_CAN_HEADER};