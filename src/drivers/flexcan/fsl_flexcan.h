/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _FSL_FLEXCAN_H_
#define _FSL_FLEXCAN_H_

#include "fsl_common.h"

/*!
 * @addtogroup flexcan
 * @{
 */

/*! @file*/

/******************************************************************************
 * Definitions
 *****************************************************************************/
/*! @brief FlexCAN driver version. */
#define FLEXCAN_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*! @brief FlexCAN Frame ID helper macro. */
#define FLEXCAN_ID_STD(id) \
    (((uint32_t)(((uint32_t)(id)) << CAN_ID_STD_SHIFT)) & CAN_ID_STD_MASK) /*!< Standard Frame ID helper macro. */
#define FLEXCAN_ID_EXT(id)                                \
    (((uint32_t)(((uint32_t)(id)) << CAN_ID_EXT_SHIFT)) & \
     (CAN_ID_EXT_MASK | CAN_ID_STD_MASK)) /*!< Extend Frame ID helper macro. */

/*! @brief FlexCAN Rx Message Buffer Mask helper macro. */
#define FLEXCAN_RX_MB_STD_MASK(id, rtr, ide)                                   \
    (((uint32_t)((uint32_t)(rtr) << 31) | (uint32_t)((uint32_t)(ide) << 30)) | \
     FLEXCAN_ID_STD(id)) /*!< Standard Rx Message Buffer Mask helper macro. */
#define FLEXCAN_RX_MB_EXT_MASK(id, rtr, ide)                                   \
    (((uint32_t)((uint32_t)(rtr) << 31) | (uint32_t)((uint32_t)(ide) << 30)) | \
     FLEXCAN_ID_EXT(id)) /*!< Extend Rx Message Buffer Mask helper macro. */

/*! @brief FlexCAN Rx FIFO Mask helper macro. */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_A(id, rtr, ide)                          \
    (((uint32_t)((uint32_t)(rtr) << 31) | (uint32_t)((uint32_t)(ide) << 30)) | \
     (FLEXCAN_ID_STD(id) << 1)) /*!< Standard Rx FIFO Mask helper macro Type A helper macro. */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_B_HIGH(id, rtr, ide)                     \
    (((uint32_t)((uint32_t)(rtr) << 31) | (uint32_t)((uint32_t)(ide) << 30)) | \
     (FLEXCAN_ID_STD(id) << 16)) /*!< Standard Rx FIFO Mask helper macro Type B upper part helper macro. */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_B_LOW(id, rtr, ide)                      \
    (((uint32_t)((uint32_t)(rtr) << 15) | (uint32_t)((uint32_t)(ide) << 14)) | \
     FLEXCAN_ID_STD(id)) /*!< Standard Rx FIFO Mask helper macro Type B lower part helper macro. */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_HIGH(id) \
    ((FLEXCAN_ID_STD(id) & 0x7F8) << 21) /*!< Standard Rx FIFO Mask helper macro Type C upper part helper macro. */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_MID_HIGH(id)                                                                 \
    ((FLEXCAN_ID_STD(id) & 0x7F8) << 13) /*!< Standard Rx FIFO Mask helper macro Type C mid-upper part helper macro. \
                                                */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_MID_LOW(id) \
    ((FLEXCAN_ID_STD(id) & 0x7F8) << 5) /*!< Standard Rx FIFO Mask helper macro Type C mid-lower part helper macro. */
#define FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_LOW(id) \
    ((FLEXCAN_ID_STD(id) & 0x7F8) >> 3) /*!< Standard Rx FIFO Mask helper macro Type C lower part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_A(id, rtr, ide)                          \
    (((uint32_t)((uint32_t)(rtr) << 31) | (uint32_t)((uint32_t)(ide) << 30)) | \
     (FLEXCAN_ID_EXT(id) << 1)) /*!< Extend Rx FIFO Mask helper macro Type A helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_B_HIGH(id, rtr, ide)                        \
    (                                                                             \
        ((uint32_t)((uint32_t)(rtr) << 31) | (uint32_t)((uint32_t)(ide) << 30)) | \
        ((FLEXCAN_ID_EXT(id) & 0x1FFF8000)                                        \
         << 1)) /*!< Extend Rx FIFO Mask helper macro Type B upper part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_B_LOW(id, rtr, ide)                      \
    (((uint32_t)((uint32_t)(rtr) << 15) | (uint32_t)((uint32_t)(ide) << 14)) | \
     ((FLEXCAN_ID_EXT(id) & 0x1FFF8000) >>                                     \
      15)) /*!< Extend Rx FIFO Mask helper macro Type B lower part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_HIGH(id) \
    ((FLEXCAN_ID_EXT(id) & 0x1FE00000) << 3) /*!< Extend Rx FIFO Mask helper macro Type C upper part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_MID_HIGH(id) \
    ((FLEXCAN_ID_EXT(id) & 0x1FE00000) >>            \
     5) /*!< Extend Rx FIFO Mask helper macro Type C mid-upper part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_MID_LOW(id) \
    ((FLEXCAN_ID_EXT(id) & 0x1FE00000) >>           \
     13) /*!< Extend Rx FIFO Mask helper macro Type C mid-lower part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_LOW(id) \
    ((FLEXCAN_ID_EXT(id) & 0x1FE00000) >> 21) /*!< Extend Rx FIFO Mask helper macro Type C lower part helper macro. */

/*! @brief FlexCAN Rx FIFO Filter helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(id, rtr, ide) \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_A(id, rtr, ide) /*!< Standard Rx FIFO Filter helper macro Type A helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_B_HIGH(id, rtr, ide) \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_B_HIGH(                    \
        id, rtr, ide) /*!< Standard Rx FIFO Filter helper macro Type B upper part helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_B_LOW(id, rtr, ide) \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_B_LOW(                    \
        id, rtr, ide) /*!< Standard Rx FIFO Filter helper macro Type B lower part helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_C_HIGH(id) \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_HIGH(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C upper part helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_C_MID_HIGH(id) \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_MID_HIGH(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C mid-upper part helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_C_MID_LOW(id) \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_MID_LOW(          \
        id) /*!< Standard Rx FIFO Filter helper macro Type C mid-lower part helper macro. */
#define FLEXCAN_RX_FIFO_STD_FILTER_TYPE_C_LOW(id)                                                                     \
    FLEXCAN_RX_FIFO_STD_MASK_TYPE_C_LOW(id) /*!< Standard Rx FIFO Filter helper macro Type C lower part helper macro. \
                                               */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_A(id, rtr, ide) \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_A(id, rtr, ide) /*!< Extend Rx FIFO Filter helper macro Type A helper macro. */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_B_HIGH(id, rtr, ide) \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_B_HIGH(                    \
        id, rtr, ide) /*!< Extend Rx FIFO Filter helper macro Type B upper part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_B_LOW(id, rtr, ide) \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_B_LOW(                    \
        id, rtr, ide) /*!< Extend Rx FIFO Filter helper macro Type B lower part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_C_HIGH(id)                                                                   \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_HIGH(id) /*!< Extend Rx FIFO Filter helper macro Type C upper part helper macro. \
                                                */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_C_MID_HIGH(id) \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_MID_HIGH(          \
        id) /*!< Extend Rx FIFO Filter helper macro Type C mid-upper part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_C_MID_LOW(id) \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_MID_LOW(          \
        id) /*!< Extend Rx FIFO Filter helper macro Type C mid-lower part helper macro. */
#define FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_C_LOW(id) \
    FLEXCAN_RX_FIFO_EXT_MASK_TYPE_C_LOW(id) /*!< Extend Rx FIFO Filter helper macro Type C lower part helper macro. */

/*! @brief FlexCAN transfer status. */
enum _flexcan_status
{
    kStatus_FLEXCAN_TxBusy = MAKE_STATUS(kStatusGroup_FLEXCAN, 0), /*!< Tx Message Buffer is Busy. */
    kStatus_FLEXCAN_TxIdle = MAKE_STATUS(kStatusGroup_FLEXCAN, 1), /*!< Tx Message Buffer is Idle. */
    kStatus_FLEXCAN_TxSwitchToRx = MAKE_STATUS(
        kStatusGroup_FLEXCAN, 2), /*!< Remote Message is send out and Message buffer changed to Receive one. */
    kStatus_FLEXCAN_RxBusy = MAKE_STATUS(kStatusGroup_FLEXCAN, 3),         /*!< Rx Message Buffer is Busy. */
    kStatus_FLEXCAN_RxIdle = MAKE_STATUS(kStatusGroup_FLEXCAN, 4),         /*!< Rx Message Buffer is Idle. */
    kStatus_FLEXCAN_RxOverflow = MAKE_STATUS(kStatusGroup_FLEXCAN, 5),     /*!< Rx Message Buffer is Overflowed. */
    kStatus_FLEXCAN_RxFifoBusy = MAKE_STATUS(kStatusGroup_FLEXCAN, 6),     /*!< Rx Message FIFO is Busy. */
    kStatus_FLEXCAN_RxFifoIdle = MAKE_STATUS(kStatusGroup_FLEXCAN, 7),     /*!< Rx Message FIFO is Idle. */
    kStatus_FLEXCAN_RxFifoOverflow = MAKE_STATUS(kStatusGroup_FLEXCAN, 8), /*!< Rx Message FIFO is overflowed. */
    kStatus_FLEXCAN_RxFifoWarning = MAKE_STATUS(kStatusGroup_FLEXCAN, 0),  /*!< Rx Message FIFO is almost overflowed. */
    kStatus_FLEXCAN_ErrorStatus = MAKE_STATUS(kStatusGroup_FLEXCAN, 10),   /*!< FlexCAN Module Error and Status. */
    kStatus_FLEXCAN_UnHandled = MAKE_STATUS(kStatusGroup_FLEXCAN, 11),     /*!< UnHadled Interrupt asserted. */
};

/*! @brief FlexCAN frame format. */
typedef enum _flexcan_frame_format
{
    kFLEXCAN_FrameFormatStandard = 0x0U, /*!< Standard frame format attribute. */
    kFLEXCAN_FrameFormatExtend = 0x1U,   /*!< Extend frame format attribute. */
} flexcan_frame_format_t;

/*! @brief FlexCAN frame type. */
typedef enum _flexcan_frame_type
{
    kFLEXCAN_FrameTypeData = 0x0U,   /*!< Data frame type attribute. */
    kFLEXCAN_FrameTypeRemote = 0x1U, /*!< Remote frame type attribute. */
} flexcan_frame_type_t;

/*! @brief FlexCAN clock source. */
typedef enum _flexcan_clock_source
{
    kFLEXCAN_ClkSrcOsc = 0x0U,  /*!< FlexCAN Protocol Engine clock from Oscillator. */
    kFLEXCAN_ClkSrcPeri = 0x1U, /*!< FlexCAN Protocol Engine clock from Peripheral Clock. */
} flexcan_clock_source_t;

/*! @brief FlexCAN Rx Fifo Filter type. */
typedef enum _flexcan_rx_fifo_filter_type
{
    kFLEXCAN_RxFifoFilterTypeA = 0x0U, /*!< One full ID (standard and extended) per ID Filter element. */
    kFLEXCAN_RxFifoFilterTypeB =
        0x1U, /*!< Two full standard IDs or two partial 14-bit ID slices per ID Filter Table element. */
    kFLEXCAN_RxFifoFilterTypeC =
        0x2U, /*!< Four partial 8-bit Standard or extended ID slices per ID Filter Table element. */
    kFLEXCAN_RxFifoFilterTypeD = 0x3U, /*!< All frames rejected. */
} flexcan_rx_fifo_filter_type_t;

/*!
 * @brief FlexCAN Rx FIFO priority
 *
 * The matching process will start from the Rx MB(or Rx FIFO) with higher priority.
 * If no MB(or Rx FIFO filter) is satisfied, the matching process will go on with
 * the Rx FIFO(or Rx MB) with lower priority.
 */
typedef enum _flexcan_rx_fifo_priority
{
    kFLEXCAN_RxFifoPrioLow = 0x0U,  /*!< Matching process start from Rx Message Buffer first*/
    kFLEXCAN_RxFifoPrioHigh = 0x1U, /*!< Matching process start from Rx FIFO first*/
} flexcan_rx_fifo_priority_t;

/*!
 * @brief FlexCAN interrupt configuration structure, default settings all disabled.
 *
 * This structure contains the settings for all of the FlexCAN Module interrupt configurations.
 * Note: FlexCAN Message Buffers and Rx FIFO have their own interrupts.
 */
enum _flexcan_interrupt_enable
{
    kFLEXCAN_BusOffInterruptEnable = CAN_CTRL1_BOFFMSK_MASK,    /*!< Bus Off interrupt. */
    kFLEXCAN_ErrorInterruptEnable = CAN_CTRL1_ERRMSK_MASK,      /*!< Error interrupt. */
    kFLEXCAN_RxWarningInterruptEnable = CAN_CTRL1_RWRNMSK_MASK, /*!< Rx Warning interrupt. */
    kFLEXCAN_TxWarningInterruptEnable = CAN_CTRL1_TWRNMSK_MASK, /*!< Tx Warning interrupt. */
    kFLEXCAN_WakeUpInterruptEnable = CAN_MCR_WAKMSK_MASK,       /*!< Wake Up interrupt. */
};

/*!
 * @brief FlexCAN status flags.
 *
 * This provides constants for the FlexCAN status flags for use in the FlexCAN functions.
 * Note: The CPU read action clears FlEXCAN_ErrorFlag, therefore user need to
 * read FlEXCAN_ErrorFlag and distinguish which error is occur using
 * @ref _flexcan_error_flags enumerations.
 */
enum _flexcan_flags
{
    kFLEXCAN_SynchFlag = CAN_ESR1_SYNCH_MASK,              /*!< CAN Synchronization Status. */
    kFLEXCAN_TxWarningIntFlag = CAN_ESR1_TWRNINT_MASK,     /*!< Tx Warning Interrupt Flag. */
    kFLEXCAN_RxWarningIntFlag = CAN_ESR1_RWRNINT_MASK,     /*!< Rx Warning Interrupt Flag. */
    kFLEXCAN_TxErrorWarningFlag = CAN_ESR1_TXWRN_MASK,     /*!< Tx Error Warning Status. */
    kFLEXCAN_RxErrorWarningFlag = CAN_ESR1_RXWRN_MASK,     /*!< Rx Error Warning Status. */
    kFLEXCAN_IdleFlag = CAN_ESR1_IDLE_MASK,                /*!< CAN IDLE Status Flag. */
    kFLEXCAN_FaultConfinementFlag = CAN_ESR1_FLTCONF_MASK, /*!< Fault Confinement State Flag. */
    kFLEXCAN_TransmittingFlag = CAN_ESR1_TX_MASK,          /*!< FlexCAN In Transmission Status. */
    kFLEXCAN_ReceivingFlag = CAN_ESR1_RX_MASK,             /*!< FlexCAN In Reception Status. */
    kFLEXCAN_BusOffIntFlag = CAN_ESR1_BOFFINT_MASK,        /*!< Bus Off Interrupt Flag. */
    kFLEXCAN_ErrorIntFlag = CAN_ESR1_ERRINT_MASK,          /*!< Error Interrupt Flag. */
    kFLEXCAN_WakeUpIntFlag = CAN_ESR1_WAKINT_MASK,         /*!< Wake-Up Interrupt Flag. */
    kFLEXCAN_ErrorFlag = CAN_ESR1_BIT1ERR_MASK |           /*!< All FlexCAN Error Status. */
                         CAN_ESR1_BIT0ERR_MASK |
                         CAN_ESR1_ACKERR_MASK | CAN_ESR1_CRCERR_MASK | CAN_ESR1_FRMERR_MASK | CAN_ESR1_STFERR_MASK,
};

/*!
 * @brief FlexCAN error status flags.
 *
 * The FlexCAN Error Status enumerations is used to report current error of the FlexCAN bus.
 * This enumerations should be used with KFLEXCAN_ErrorFlag in @ref _flexcan_flags enumerations
 * to ditermine which error is generated.
 */
enum _flexcan_error_flags
{
    kFLEXCAN_StuffingError = CAN_ESR1_STFERR_MASK, /*!< Stuffing Error. */
    kFLEXCAN_FormError = CAN_ESR1_FRMERR_MASK,     /*!< Form Error. */
    kFLEXCAN_CrcError = CAN_ESR1_CRCERR_MASK,      /*!< Cyclic Redundancy Check Error. */
    kFLEXCAN_AckError = CAN_ESR1_ACKERR_MASK,      /*!< Received no ACK on transmission. */
    kFLEXCAN_Bit0Error = CAN_ESR1_BIT0ERR_MASK,    /*!< Unable to send dominant bit. */
    kFLEXCAN_Bit1Error = CAN_ESR1_BIT1ERR_MASK,    /*!< Unable to send recessive bit. */
};

/*!
 * @brief FlexCAN Rx FIFO status flags.
 *
 * The FlexCAN Rx FIFO Status enumerations is used to ditermine the status of
 * Rx FIFO. Because Rx FIFO occupy the MB0 ~ MB7(Rx Fifo filter will also occupy
 * more Message Buffer space), Rx FIFO status flags are maped to corresponding
 * Message Buffer status flags.
 */
enum _flexcan_rx_fifo_flags
{
    kFLEXCAN_RxFifoOverflowFlag = CAN_IFLAG1_BUF7I_MASK, /*!< Rx FIFO overflow flag. */
    kFLEXCAN_RxFifoWarningFlag = CAN_IFLAG1_BUF6I_MASK,  /*!< Rx FIFO almost full flag. */
    kFLEXCAN_RxFifoFrameAvlFlag = CAN_IFLAG1_BUF5I_MASK, /*!< Frames available in Rx FIFO flag. */
};

#if defined(__CC_ARM)
#pragma anon_unions
#endif
/*! @brief FlexCAN message frame structure. */
typedef struct _flexcan_frame
{
    struct
    {
        uint32_t timestamp : 16; /*!< FlexCAN internal Free-Running Counter Time Stamp. */
        uint32_t length : 4;     /*!< CAN frame payload length in bytes(Range: 0~8). */
        uint32_t type : 1;       /*!< CAN Frame Type(DATA or REMOTE). */
        uint32_t format : 1;     /*!< CAN Frame Identifier(STD or EXT format). */
        uint32_t reserve1 : 1;   /*!< Reserved for placeholder. */
        uint32_t idhit : 9;      /*!< CAN Rx FIFO filter hit id(This value is only used in Rx FIFO receive mode). */
    };
    struct
    {
        uint32_t id : 29; /*!< CAN Frame Identifier, should be set using FLEXCAN_ID_EXT() or FLEXCAN_ID_STD() macro. */
        uint32_t reserve2 : 3; /*!< Reserved for placeholder. */
    };
    union
    {
        struct
        {
            uint32_t dataWord0; /*!< CAN Frame payload word0. */
            uint32_t dataWord1; /*!< CAN Frame payload word1. */
        };
        struct
        {
            uint8_t dataByte3; /*!< CAN Frame payload byte3. */
            uint8_t dataByte2; /*!< CAN Frame payload byte2. */
            uint8_t dataByte1; /*!< CAN Frame payload byte1. */
            uint8_t dataByte0; /*!< CAN Frame payload byte0. */
            uint8_t dataByte7; /*!< CAN Frame payload byte7. */
            uint8_t dataByte6; /*!< CAN Frame payload byte6. */
            uint8_t dataByte5; /*!< CAN Frame payload byte5. */
            uint8_t dataByte4; /*!< CAN Frame payload byte4. */
        };
    };
} flexcan_frame_t;

/*! @brief FlexCAN module configuration structure. */
typedef struct _flexcan_config
{
    uint32_t baudRate;             /*!< FlexCAN baud rate in bps. */
    flexcan_clock_source_t clkSrc; /*!< Clock source for FlexCAN Protocol Engine. */
    uint8_t maxMbNum;              /*!< The maximum number of Message Buffers used by user. */
    bool enableLoopBack;           /*!< Enable or Disable Loop Back Self Test Mode. */
    bool enableSelfWakeup;         /*!< Enable or Disable Self Wakeup Mode. */
    bool enableIndividMask;        /*!< Enable or Disable Rx Individual Mask. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT) && FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT)
    bool enableDoze; /*!< Enable or Disable Doze Mode. */
#endif
} flexcan_config_t;

/*! @brief FlexCAN protocol timing characteristic configuration structure. */
typedef struct _flexcan_timing_config
{
    uint8_t preDivider; /*!< Clock Pre-scaler Division Factor. */
    uint8_t rJumpwidth; /*!< Re-sync Jump Width. */
    uint8_t phaseSeg1;  /*!< Phase Segment 1. */
    uint8_t phaseSeg2;  /*!< Phase Segment 2. */
    uint8_t propSeg;    /*!< Propagation Segment. */
} flexcan_timing_config_t;

/*!
 * @brief FlexCAN Receive Message Buffer configuration structure
 *
 * This structure is used as the parameter of FLEXCAN_SetRxMbConfig() function.
 * The FLEXCAN_SetRxMbConfig() function is used to configure FlexCAN Receive
 * Message Buffer. The function abort previous receiving process, clean the
 * Message Buffer and activate the Rx Message Buffer using given Message Buffer
 * setting.
 */
typedef struct _flexcan_rx_mb_config
{
    uint32_t id;                   /*!< CAN Message Buffer Frame Identifier, should be set using
                                        FLEXCAN_ID_EXT() or FLEXCAN_ID_STD() macro. */
    flexcan_frame_format_t format; /*!< CAN Frame Identifier format(Standard of Extend). */
    flexcan_frame_type_t type;     /*!< CAN Frame Type(Data or Remote). */
} flexcan_rx_mb_config_t;

/*! @brief FlexCAN Rx FIFO configure structure. */
typedef struct _flexcan_rx_fifo_config
{
    uint32_t *idFilterTable;                    /*!< Pointer to FlexCAN Rx FIFO identifier filter table. */
    uint8_t idFilterNum;                        /*!< The quantity of filter elements. */
    flexcan_rx_fifo_filter_type_t idFilterType; /*!< The FlexCAN Rx FIFO Filter type. */
    flexcan_rx_fifo_priority_t priority;        /*!< The FlexCAN Rx FIFO receive priority. */
} flexcan_rx_fifo_config_t;

/*! @brief FlexCAN Message Buffer transfer. */
typedef struct _flexcan_mb_transfer
{
    flexcan_frame_t *frame; /*!< The buffer of CAN Message to be transfer. */
    uint8_t mbIdx;          /*!< The index of Message buffer used to transfer Message. */
} flexcan_mb_transfer_t;

/*! @brief FlexCAN Rx FIFO transfer. */
typedef struct _flexcan_fifo_transfer
{
    flexcan_frame_t *frame; /*!< The buffer of CAN Message to be received from Rx FIFO. */
} flexcan_fifo_transfer_t;

/*! @brief FlexCAN handle structure definition. */
typedef struct _flexcan_handle flexcan_handle_t;

/*! @brief FlexCAN transfer callback function.
 *
 *  The FlexCAN transfer callback will return value from the underlying layer.
 *  If the status equals to kStatus_FLEXCAN_ErrorStatus, the result parameter will be the Content of
 *  FlexCAN status register which can be used to get the working status(or error status) of FlexCAN module.
 *  If the status equals to other FlexCAN Message Buffer transfer status, the result will be the index of
 *  Message Buffer that generate transfer event.
 *  If the status equals to other FlexCAN Message Buffer transfer status, the result is meaningless and should be
 *  Ignored.
 */
typedef void (*flexcan_transfer_callback_t)(
    CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData);

/*! @brief FlexCAN handle structure. */
struct _flexcan_handle
{
    flexcan_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                       /*!< FlexCAN callback function parameter.*/
    flexcan_frame_t *volatile mbFrameBuf[CAN_WORD1_COUNT];
    /*!< The buffer for received data from Message Buffers. */
    flexcan_frame_t *volatile rxFifoFrameBuf;  /*!< The buffer for received data from Rx FIFO. */
    volatile uint8_t mbState[CAN_WORD1_COUNT]; /*!< Message Buffer transfer state. */
    volatile uint8_t rxFifoState;              /*!< Rx FIFO transfer state. */
};

/******************************************************************************
 * API
 *****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Initializes a FlexCAN instance.
 *
 * This function initializes the FlexCAN module with user-defined settings.
 * This example shows how to set up the flexcan_config_t parameters and how
 * to call the FLEXCAN_Init function by passing in these parameters:
 *  @code
 *   flexcan_config_t flexcanConfig;
 *   flexcanConfig.clkSrc            = KFLEXCAN_ClkSrcOsc;
 *   flexcanConfig.baudRate          = 125000U;
 *   flexcanConfig.maxMbNum          = 16;
 *   flexcanConfig.enableLoopBack    = false;
 *   flexcanConfig.enableSelfWakeup  = false;
 *   flexcanConfig.enableIndividMask = false;
 *   flexcanConfig.enableDoze        = false;
 *   FLEXCAN_Init(CAN0, &flexcanConfig, 8000000UL);
 *   @endcode
 *
 * @param base FlexCAN peripheral base address.
 * @param config Pointer to user-defined configuration structure.
 * @param sourceClock_Hz FlexCAN Protocol Engine clock source frequency in Hz.
 */
void FLEXCAN_Init(CAN_Type *base, const flexcan_config_t *config, uint32_t sourceClock_Hz);

/*!
 * @brief De-initializes a FlexCAN instance.
 *
 * This function disable the FlexCAN module clock and set all register value
 * to reset value.
 *
 * @param base FlexCAN peripheral base address.
 */
void FLEXCAN_Deinit(CAN_Type *base);

/*!
 * @brief Get the default configuration structure.
 *
 * This function initializes the FlexCAN configure structure to default value. The default
 * value are:
 *   flexcanConfig->clkSrc            = KFLEXCAN_ClkSrcOsc;
 *   flexcanConfig->baudRate          = 125000U;
 *   flexcanConfig->maxMbNum          = 16;
 *   flexcanConfig->enableLoopBack    = false;
 *   flexcanConfig->enableSelfWakeup  = false;
 *   flexcanConfig->enableIndividMask = false;
 *   flexcanConfig->enableDoze        = false;
 *
 * @param config Pointer to FlexCAN configuration structure.
 */
void FLEXCAN_GetDefaultConfig(flexcan_config_t *config);

/* @} */

/*!
 * @name Configuration.
 * @{
 */

/*!
 * @brief Setting FlexCAN protocol timing characteristic.
 *
 * This function give user fine settings to CAN bus timing characteristic.
 * The function is for user who is really versed in CAN protocol, for these
 * users who just what to establish CAN communication among MCUs, just call
 * FLEXCAN_Init() and fill the baud rate field with desired one.
 * Doing this, default timing characteristic will provide to the module.
 *
 * Note: Calling FLEXCAN_SetTimingConfig() will override the baud rate setted
 * in FLEXCAN_Init().
 *
 * @param base FlexCAN peripheral base address.
 * @param config Pointer to the timing configuration structure.
 */
void FLEXCAN_SetTimingConfig(CAN_Type *base, const flexcan_timing_config_t *config);

/*!
 * @brief Set the FlexCAN Receive Message Buffer Global Mask.
 *
 * This function Set the global mask for FlexCAN Message Buffer in matching process.
 * The configuration is only effective when Rx Individual Mask is disabled in FLEXCAN_Init().
 *
 * @param base FlexCAN peripheral base address.
 * @param mask Rx Message Buffer Global Mask value.
 */
void FlEXCAN_SetRxMbGlobalMask(CAN_Type *base, uint32_t mask);

/*!
 * @brief Set the FlexCAN Receive FIFO Global Mask.
 *
 * This function Set the global mask for FlexCAN FIFO in matching process.
 *
 * @param base FlexCAN peripheral base address.
 * @param mask Rx Fifo Global Mask value.
 */
void FlEXCAN_SetRxFifoGlobalMask(CAN_Type *base, uint32_t mask);

/*!
 * @brief Set the FlexCAN Receive Individual Mask.
 *
 * This function Set the Individual mask for FlexCAN matching process.
 * The configuration is only effective when Rx Individual Mask is enabled in FLEXCAN_Init().
 * If Rx FIFO is disabled, the Individual Mask is applied to corresponding Message Buffer.
 * If Rx FIFO is enabled, the Individual Mask for Rx FIFO occupied Message Buffer will be applied to
 * Rx Filter with same index. What calls for special attention is that only the first 32
 * Individual Mask can be used as Rx FIFO Filter Mask.
 *
 * @param base FlexCAN peripheral base address.
 * @param maskIdx The Index of individual Mask.
 * @param mask Rx Individual Mask value.
 */
void FlEXCAN_SetRxIndividualMask(CAN_Type *base, uint8_t maskIdx, uint32_t mask);

/*!
 * @brief Configure a FlexCAN Transmit Message Buffer.
 *
 * This function abort privious transmission, clean the Message Buffer and
 * configure it as a Transmit Message Buffer.
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The Message Buffer index.
 * @param enable Enable/Disable Tx Message Buffer.
 *               - true: Enable Tx Message Buffer.
 *               - false: Disable Tx Message Buffer.
 */
void FLEXCAN_SetTxMbConfig(CAN_Type *base, uint8_t mbIdx, bool enable);

/*!
 * @brief Configure a FlexCAN Receive Message Buffer.
 *
 * This function clean a FlexCAN build-in Message Buffer and configure it
 * as a Receive Message Buffer.
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The Message Buffer index.
 * @param config Pointer to FlexCAN Message Buffer configuration structure.
 * @param enable Enable/Disable Rx Message Buffer.
 *               - true: Enable Rx Message Buffer.
 *               - false: Disable Rx Message Buffer.
 */
void FLEXCAN_SetRxMbConfig(CAN_Type *base, uint8_t mbIdx, const flexcan_rx_mb_config_t *config, bool enable);

/*!
 * @brief Configure the FlexCAN Rx FIFO.
 *
 * This function Configure the Rx FIFO with given Rx FIFO configuration.
 *
 * @param base FlexCAN peripheral base address.
 * @param config Pointer to FlexCAN Rx FIFO configuration structure.
 * @param enable Enable/Disable Rx FIFO.
 *               - true: Enable Rx FIFO.
 *               - false: Disable Rx FIFO.
 */
void FlEXCAN_SetRxFifoConfig(CAN_Type *base, const flexcan_rx_fifo_config_t *config, bool enable);

/* @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Get FlexCAN module interrupt flags.
 *
 * This function get all FlexCAN status flags, the flags are returned as the logical
 * OR value of the enumerators @ref _flexcan_flags. To check specific status,
 * compare the return value with enumerators in @ref _flexcan_flags.
 *
 * @param base FlexCAN peripheral base address.
 * @return FlexCAN status flags which are ORed by the enumerators in the _flexcan_flags.
 */
static inline uint32_t FLEXCAN_GetStatusFlags(CAN_Type *base)
{
    return base->ESR1;
}

/*!
 * @brief Clear status flags with provide mask.
 *
 * This function clears FlexCAN status flags with provided mask. Automatically cleared flag
 * can't be cleared by this function.
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The status flags to be cleared, it is logical OR value of @ref _flexcan_flags.
 */
static inline void FLEXCAN_ClearStatusFlags(CAN_Type *base, uint32_t mask)
{
    /* Write 1 to clear status flag. */
    base->ESR1 = mask;
}

/*!
 * @brief Get the FlexCAN Bus Error Counter value.
 *
 * This function get the FlexCAN Bus Error Counter value for both Tx &
 * Rx direction. These values may needed in upper layer error handling.
 *
 * @param base FlexCAN peripheral base address.
 * @param txErrBuf Buffer to store Tx Error Counter value.
 * @param rxErrBuf Buffer to store Rx Error Counter value.
 */
static inline void FlEXCAN_GetBusErrCount(CAN_Type *base, uint8_t *txErrBuf, uint8_t *rxErrBuf)
{
    if (txErrBuf)
    {
        *txErrBuf = (uint8_t)((base->ECR & CAN_ECR_TXERRCNT_MASK) >> CAN_ECR_TXERRCNT_SHIFT);
    }

    if (rxErrBuf)
    {
        *rxErrBuf = (uint8_t)((base->ECR & CAN_ECR_RXERRCNT_MASK) >> CAN_ECR_RXERRCNT_SHIFT);
    }
}

/*!
 * @brief Get FlexCAN Message Buffer interrupt flags.
 *
 * This function get interrupt flags of a given Message Buffers.
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The ORed FlexCAN Message Buffer mask.
 * @return The status of given Message Buffers.
 */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
static inline uint64_t FLEXCAN_GetMbStatusFlags(CAN_Type *base, uint64_t mask)
#else
static inline uint32_t FLEXCAN_GetMbStatusFlags(CAN_Type *base, uint32_t mask)
#endif
{
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    return ((((uint64_t)base->IFLAG1) & mask) | ((((uint64_t)base->IFLAG2) << 32) & mask));
#else
    return (base->IFLAG1 & mask);
#endif
}

/*!
 * @brief Clear FlexCAN Message Buffer interrupt flags.
 *
 * This function clear the interrupt flags of a given Message Buffers.
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The ORed FlexCAN Message Buffer mask.
 */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
static inline void FLEXCAN_ClearMbStatusFlags(CAN_Type *base, uint64_t mask)
#else
static inline void FLEXCAN_ClearMbStatusFlags(CAN_Type *base, uint32_t mask)
#endif
{
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    base->IFLAG1 = (uint32_t)(mask & 0xFFFFFFFF);
    base->IFLAG2 = (uint32_t)(mask >> 32);
#else
    base->IFLAG1 = mask;
#endif
}

/* @} */

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enable FlexCAN interrupts according to provided mask.
 *
 * This function enables the FlexCAN interrupts according to provided mask. The mask
 * is a logical OR of enumeration members, see @ref _flexcan_interrupt_enable.
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The interrupts to enable. Logical OR of @ref _flexcan_interrupt_enable.
 */
static inline void FLEXCAN_EnableInterrupts(CAN_Type *base, uint32_t mask)
{
    /* Solve Wake Up Interrupt. */
    if (mask & kFLEXCAN_WakeUpInterruptEnable)
    {
        base->MCR |= CAN_MCR_WAKMSK_MASK;
    }

    /* Solve others. */
    base->CTRL1 |= (mask & (~((uint32_t)kFLEXCAN_WakeUpInterruptEnable)));
}

/*!
 * @brief Disable FlexCAN interrupts according to provided mask.
 *
 * This function disables the FlexCAN interrupts according to provided mask. The mask
 * is a logical OR of enumeration members, see @ref _flexcan_interrupt_enable.
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The interrupts to disable. Logical OR of @ref _flexcan_interrupt_enable.
 */
static inline void FLEXCAN_DisableInterrupts(CAN_Type *base, uint32_t mask)
{
    /* Solve Wake Up Interrupt. */
    if (mask & kFLEXCAN_WakeUpInterruptEnable)
    {
        base->MCR &= ~CAN_MCR_WAKMSK_MASK;
    }

    /* Solve others. */
    base->CTRL1 &= ~(mask & (~((uint32_t)kFLEXCAN_WakeUpInterruptEnable)));
}

/*!
 * @brief Enable FlexCAN Message Buffer interrupts.
 *
 * This function enable the interrupts of given Message Buffers
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The ORed FlexCAN Message Buffer mask.
 */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
static inline void FLEXCAN_EnableMbInterrupts(CAN_Type *base, uint64_t mask)
#else
static inline void FLEXCAN_EnableMbInterrupts(CAN_Type *base, uint32_t mask)
#endif
{
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    base->IMASK1 |= (uint32_t)(mask & 0xFFFFFFFF);
    base->IMASK2 |= (uint32_t)(mask >> 32);
#else
    base->IMASK1 |= mask;
#endif
}

/*!
 * @brief Disable FlexCAN Message Buffer interrupts.
 *
 * This function disable the interrupts of given Message Buffers
 *
 * @param base FlexCAN peripheral base address.
 * @param mask The ORed FlexCAN Message Buffer mask.
 */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
static inline void FLEXCAN_DisableMbInterrupts(CAN_Type *base, uint64_t mask)
#else
static inline void FLEXCAN_DisableMbInterrupts(CAN_Type *base, uint32_t mask)
#endif
{
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    base->IMASK1 &= ~((uint32_t)(mask & 0xFFFFFFFF));
    base->IMASK2 &= ~((uint32_t)(mask >> 32));
#else
    base->IMASK1 &= ~mask;
#endif
}

/* @} */

#if (defined(FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA) && FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA)
/*!
 * @name DMA Control
 * @{
 */

/*!
 * @brief Enable or disable FlexCAN Rx FIFO DMA request.
 *
 * This function enable or disable the DMA feature of FlexCAN build-in Rx FIFO.
 *
 * @param base FlexCAN peripheral base address.
 * @param enable true to enable, false to disable.
 */
void FLEXCAN_EnableRxFifoDMA(CAN_Type *base, bool enable);

/*!
 * @brief Get the Rx FIFO Head address.
 *
 * This function return the FlexCAN Rx FIFO Head address, which is mainly used by DMA/eDMA case.
 *
 * @param base FlexCAN peripheral base address.
 * @return FlexCAN Rx FIFO Head address.
 */
static inline uint32_t FLEXCAN_GetRxFifoHeadAddr(CAN_Type *base)
{
    return (uint32_t) & (base->MB[0].CS);
}

/* @} */
#endif /* FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA */

/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Enables or disable the FlexCAN module operation.
 *
 * This function enables or disables the FlexCAN module.
 *
 * @param base FlexCAN base pointer.
 * @param enable true to enable, false to disable.
 */
static inline void FLEXCAN_Enable(CAN_Type *base, bool enable)
{
    if (enable)
    {
        base->MCR &= ~CAN_MCR_MDIS_MASK;

        /* Wait FlexCAN exit from low-power mode. */
        while (base->MCR & CAN_MCR_LPMACK_MASK)
        {
        }
    }
    else
    {
        base->MCR |= CAN_MCR_MDIS_MASK;

        /* Wait FlexCAN enter low-power mode. */
        while (!(base->MCR & CAN_MCR_LPMACK_MASK))
        {
        }
    }
}

/*!
 * @brief Write FlexCAN Message to Transmit Message Buffer.
 *
 * This function write a CAN Message to the specified Transmit Message Buffer
 * and change the Message Buffer state to start CAN Message transmit, after
 * that the function will return immediately.
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The FlexCAN Message Buffer index.
 * @param txFrame Pointer to CAN message frame to be sent.
 * @retval kStatus_Success - Write Tx Message Buffer Successfully.
 * @retval kStatus_Fail    - Tx Message Buffer is currently in use.
 */
status_t FLEXCAN_WriteTxMb(CAN_Type *base, uint8_t mbIdx, const flexcan_frame_t *txFrame);

/*!
 * @brief Read a FlexCAN Message from Receive Message Buffer.
 *
 * This function read a CAN message from a specified Receive Message Buffer.
 * The function will fill receive CAN message frame structure with
 * just received data and activate the Message Buffer again.
 * The function will return immediately.
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The FlexCAN Message Buffer index.
 * @param rxFrame Pointer to CAN message frame structure for reception.
 * @retval kStatus_Success            - Rx Message Buffer is full and has been read successfully.
 * @retval kStatus_FLEXCAN_RxOverflow - Rx Message Buffer is already overflowed and has been read successfully.
 * @retval kStatus_Fail               - Rx Message Buffer is empty.
 */
status_t FLEXCAN_ReadRxMb(CAN_Type *base, uint8_t mbIdx, flexcan_frame_t *rxFrame);

/*!
 * @brief Read a FlexCAN Message from Rx FIFO.
 *
 * This function Read a CAN message from the FlexCAN build-in Rx FIFO.
 *
 * @param base FlexCAN peripheral base address.
 * @param rxFrame Pointer to CAN message frame structure for reception.
 * @retval kStatus_Success - Read Message from Rx FIFO successfully.
 * @retval kStatus_Fail    - Rx FIFO is not enabled.
 */
status_t FlEXCAN_ReadRxFifo(CAN_Type *base, flexcan_frame_t *rxFrame);

/* @} */

/*!
 * @name Transactional
 * @{
 */

/*!
 * @brief Performs a polling send transaction on the CAN bus.
 *
 * Note: There is no need to create transfer handle before calling this API.
 *
 * @param base FlexCAN peripheral base pointer.
 * @param mbIdx The FlexCAN Message Buffer index.
 * @param txFrame Pointer to CAN message frame to be sent.
 * @retval kStatus_Success - Write Tx Message Buffer Successfully.
 * @retval kStatus_Fail    - Tx Message Buffer is currently in use.
 */
status_t FlEXCAN_TransferSendBlocking(CAN_Type *base, uint8_t mbIdx, flexcan_frame_t *txFrame);

/*!
 * @brief Performs a polling receive transaction on the CAN bus.
 *
 * Note: There is no need to create transfer handle before calling this API.
 *
 * @param base FlexCAN peripheral base pointer.
 * @param mbIdx The FlexCAN Message Buffer index.
 * @param rxFrame Pointer to CAN message frame structure for reception.
 * @retval kStatus_Success            - Rx Message Buffer is full and has been read successfully.
 * @retval kStatus_FLEXCAN_RxOverflow - Rx Message Buffer is already overflowed and has been read successfully.
 * @retval kStatus_Fail               - Rx Message Buffer is empty.
 */
status_t FlEXCAN_TransferReceiveBlocking(CAN_Type *base, uint8_t mbIdx, flexcan_frame_t *rxFrame);

/*!
 * @brief Performs a polling receive transaction from Rx FIFO on the CAN bus.
 *
 * Note: There is no need to create transfer handle before calling this API.
 *
 * @param base FlexCAN peripheral base pointer.
 * @param rxFrame Pointer to CAN message frame structure for reception.
 * @retval kStatus_Success - Read Message from Rx FIFO successfully.
 * @retval kStatus_Fail    - Rx FIFO is not enabled.
 */
status_t FlEXCAN_TransferReceiveFifoBlocking(CAN_Type *base, flexcan_frame_t *rxFrame);

/*!
 * @brief Initialize the FlexCAN handle.
 *
 * This function initializes the FlexCAN handle which can be used for other FlexCAN
 * transactional APIs. Usually, for a specified FlexCAN instance, user only need
 * to call this API once to get the initialized handle.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 * @param callback The callback function.
 * @param userData The parameter of the callback function.
 */
void FLEXCAN_TransferCreateHandle(CAN_Type *base,
                                  flexcan_handle_t *handle,
                                  flexcan_transfer_callback_t callback,
                                  void *userData);

/*!
 * @brief send message using IRQ
 *
 * This function send message using IRQ, this is non-blocking function, will return
 * right away, when message have been sent out, the send callback function will be called.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 * @param xfer FlexCAN Message Buffer transfer structure, refer to #flexcan_mb_transfer_t.
 * @retval kStatus_Success        Start Tx Message Buffer sending process successfully.
 * @retval kStatus_Fail           Write Tx Message Buffer failed.
 * @retval kStatus_FLEXCAN_TxBusy Tx Message Buffer is in use.
 */
status_t FLEXCAN_TransferSendNonBlocking(CAN_Type *base, flexcan_handle_t *handle, flexcan_mb_transfer_t *xfer);

/*!
 * @brief Receive message using IRQ
 *
 * This function receive message using IRQ, this is non-blocking function, will return
 * right away, when message have been received, the receive callback function will be called.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 * @param xfer FlexCAN Message Buffer transfer structure, refer to #flexcan_mb_transfer_t.
 * @retval kStatus_Success        - Start Rx Message Buffer receiving process successfully.
 * @retval kStatus_FLEXCAN_RxBusy - Rx Message Buffer is in use.
 */
status_t FLEXCAN_TransferReceiveNonBlocking(CAN_Type *base, flexcan_handle_t *handle, flexcan_mb_transfer_t *xfer);

/*!
 * @brief Receive message from Rx FIFO using IRQ
 *
 * This function receive message using IRQ, this is non-blocking function, will return
 * right away, when all messages have been received, the receive callback function will be called.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 * @param xfer FlexCAN Rx FIFO transfer structure, refer to @ref flexcan_fifo_transfer_t.
 * @retval kStatus_Success            - Start Rx FIFO receiving process successfully.
 * @retval kStatus_FLEXCAN_RxFifoBusy - Rx FIFO is currently in use.
 */
status_t FLEXCAN_TransferReceiveFifoNonBlocking(CAN_Type *base,
                                                flexcan_handle_t *handle,
                                                flexcan_fifo_transfer_t *xfer);

/*!
 * @brief Abort interrupt driven message send process.
 *
 * This function aborts interrupt driven message send process.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 * @param mbIdx The FlexCAN Message Buffer index.
 */
void FLEXCAN_TransferAbortSend(CAN_Type *base, flexcan_handle_t *handle, uint8_t mbIdx);

/*!
 * @brief Abort interrupt driven message receive process.
 *
 * This function abort interrupt driven message receive process.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 * @param mbIdx The FlexCAN Message Buffer index.
 */
void FLEXCAN_TransferAbortReceive(CAN_Type *base, flexcan_handle_t *handle, uint8_t mbIdx);

/*!
 * @brief Abort interrupt driven message receive from Rx FIFO process.
 *
 * This function abort interrupt driven message receive from Rx FIFO process.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 */
void FLEXCAN_TransferAbortReceiveFifo(CAN_Type *base, flexcan_handle_t *handle);

/*!
 * @brief FlexCAN IRQ handle function
 *
 * This function handles the FlexCAN Error, Message Buffer and Rx FIFO IRQ request.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle FlexCAN handle pointer.
 */
void FLEXCAN_TransferHandleIRQ(CAN_Type *base, flexcan_handle_t *handle);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_FLEXCAN_H_ */
