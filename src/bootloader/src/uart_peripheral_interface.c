/*
 * Copyright (c) 2013, Freescale Semiconductor, Inc.
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

#include "bootloader/bl_context.h"
#include "bootloader/bl_irq_common.h"
#include "bootloader_common.h"
#include "autobaud/autobaud.h"
#include "packet/serial_packet.h"
#include "fsl_device_registers.h"
#include "uart/fsl_uart.h"
#include "utilities/fsl_assert.h"

#if (BL_CONFIG_UART)

//! @addtogroup uart_peripheral
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define UART_TYPE_REGS_BASE(x) ((UART_Type *)REGS_UART_BASE(x))

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static UART_Type *get_uart_baseAddr(uint32_t instance);
static bool uart_poll_for_activity(const peripheral_descriptor_t *self);
static status_t uart_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function);
static void uart_full_shutdown(const peripheral_descriptor_t *self);

static status_t uart_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

const peripheral_control_interface_t g_uartControlInterface = {
    .pollForActivity = uart_poll_for_activity, .init = uart_full_init, .shutdown = uart_full_shutdown, .pump = 0
};

const peripheral_byte_inteface_t g_uartByteInterface = {.init = NULL, .write = uart_write };

static serial_byte_receive_func_t s_uart_byte_receive_callback;
static bool g_uartInitDone = false;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
static UART_Type *get_uart_baseAddr(uint32_t instance)
{
    switch (instance)
    {
        default:
        case 0:
            return (UART_Type *)UART0;
        case 1:
            return UART1;
#if (FSL_FEATURE_SOC_UART_COUNT > 2U)
        case 2:
            return UART2;
#if (FSL_FEATURE_SOC_UART_COUNT > 3U)
        case 3:
            return UART3;
#if (FSL_FEATURE_SOC_UART_COUNT > 4U)
        case 4:
            return UART4;
#if (FSL_FEATURE_SOC_UART_COUNT > 5U)
        case 5:
            return UART5;
#endif // (FSL_FEATURE_SOC_UART_COUNT > 5U)
#endif // (FSL_FEATURE_SOC_UART_COUNT > 4U)
#endif // (FSL_FEATURE_SOC_UART_COUNT > 3U)
#endif // (FSL_FEATURE_SOC_UART_COUNT > 2U)
    }
}

bool uart_poll_for_activity(const peripheral_descriptor_t *self)
{
    uint32_t baud;
    status_t autoBaudCompleted = autobaud_get_rate(self->instance, &baud);

    if (autoBaudCompleted == kStatus_Success)
    {
        uart_config_t userConfig;
        UART_Type *base = get_uart_baseAddr(self->instance);
        UART_GetDefaultConfig(&userConfig);

        userConfig.baudRate_Bps = baud;
        userConfig.enableTx = true;
        userConfig.enableRx = true;

        if (UART_Init(base, &userConfig, get_uart_clock(self->instance)) == kStatus_Success)
        {
            UART_SetSystemIRQ(self->instance, kPeripheralEnableIRQ);
            UART_EnableInterrupts(base, kUART_RxDataRegFullInterruptEnable);
            // Configure selected pin as uart peripheral interface
            self->pinmuxConfig(self->instance, kPinmuxType_Peripheral);

            // This was the byte pattern identified in autobaud detection, inform the command layer
            s_uart_byte_receive_callback(kFramingPacketStartByte);
            s_uart_byte_receive_callback(kFramingPacketType_Ping);

            g_uartInitDone = true;

            return true;
        }
        else
        {
            autobaud_init(self->instance);
        }
    }

    return false;
}

status_t uart_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function)
{
    s_uart_byte_receive_callback = function;

    // Since we are using autobaud once the detection is completed
    // it will call the UART initialization and remux the pins when it completes
    self->pinmuxConfig(self->instance, kPinmuxType_GPIO);

    // Init autobaud detector.
    autobaud_init(self->instance);

    return kStatus_Success;
}

void uart_full_shutdown(const peripheral_descriptor_t *self)
{
    if (g_uartInitDone)
    {
        UART_SetSystemIRQ(self->instance, kPeripheralDisableIRQ);
        UART_Deinit(get_uart_baseAddr(self->instance));
    }

//! Note: if not deinit autobaud(IRQ method), user app may encounters hardfault
//! if it doesn't provide related pin interrupt service routine.
#if BL_UART_AUTOBAUD_IRQ
    // De-init autobaud detector.
    autobaud_deinit(self->instance);
#endif
    // Restore selected pin to default state to reduce IDD.
    self->pinmuxConfig(self->instance, kPinmuxType_Default);
}

status_t uart_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    UART_WriteBlocking(get_uart_baseAddr(self->instance), buffer, byteCount);
    return kStatus_Success;
}

/********************************************************************/
/*
 * UART0 IRQ Handler
 *
 */
void UART0_IRQHandler(void)
{
    if (UART0->S1 & UART_S1_RDRF_MASK)
    {
        s_uart_byte_receive_callback(UART0->D);
    }
}

/********************************************************************/
/*
 * UART1 IRQ Handler
 *
 */
void UART1_IRQHandler(void)
{
    if (UART1->S1 & UART_S1_RDRF_MASK)
    {
        s_uart_byte_receive_callback(UART1->D);
    }
}

//! @}

#endif // (BL_CONFIG_UART)

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
