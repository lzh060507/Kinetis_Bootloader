/*
 * Copyright (c) 2014-2015, Freescale Semiconductor, Inc.
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

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "property/property.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum _clock_soruce_enum
{
    kClockSource_SystemOSC = 1u,
    kClockSource_SlowIRC = 2u,
    kClockSource_FastIRC = 3u,
    kClockSource_RtcOSC = 4u,
    kClockSource_SystemCFM = 5u,
    kClockSource_SystemPLL = 6u,
    kClockSource_UsbPhyPLL = 7u,
} clock_source_type_t;

enum
{
    kFIRC48M = 48000000ul,
    kFIRC52M = 52000000ul,
    kFIRC56M = 56000000ul,
    kFIRC60M = 60000000ul,
    kIRC8M = 8000000ul,
    kIRC2M = 2000000ul,
    //! The minimum core clock with usb workable is
    kMinCoreClockWithUsbSupport = 20000000u,
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! return the frequency of HIRC
uint32_t get_firc_clock(void);
//! return the frequency of SIRC
uint32_t get_sirc_clock(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
uint32_t get_firc_clock(void)
{
    uint32_t range = SCG_RD_FIRCCFG_RANGE(SCG);
    uint32_t clockFreq = 0;

    switch (range)
    {
        case 0:
            clockFreq = kFIRC48M;
            break;
        case 1:
            clockFreq = kFIRC52M;
            break;
        case 2:
            clockFreq = kFIRC56M;
            break;
        case 4:
            clockFreq = kFIRC60M;
            break;
        default:
            assert(false);
            break;
    }

    return clockFreq;
}

uint32_t get_sirc_clock(void)
{
    uint32_t range = SCG_RD_SIRCCFG_RANGE(SCG);
    return range ? kIRC8M : kIRC2M;
}

// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
#if defined(BL_TARGET_ROM) || defined(BL_TARGET_FLASH)
    // General procedure to be implemented:
    // 1. Read clock flags and divider from bootloader config in property store
    bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;
    uint8_t options = config->clockFlags;

    // Check if the USB HID peripheral is enabled. If it is enabled, we always
    // use the 48MHz IRC.
    bool isUsbEnabled = config->enabledPeripherals & kPeripheralType_USB_HID;

    // 2. If NOT High Speed and USB is NOT enabled, do nothing (use reset clock config)
    if ((options & kClockFlag_HighSpeed) && !isUsbEnabled)
    {
        // High speed flag is set (meaning disabled), so just use default clocks.
        return;
    }

    // 3. Set DIVCORE based on divider in config. OUTDIV4 starts out at /1.
    // The divider values are masked by the maximum bits per divider.
    uint32_t divCore = ((~config->clockDivider) & (SCG_CSR_DIVCORE_MASK >> SCG_CSR_DIVCORE_SHIFT)) + 1;
    uint32_t divSlow = SCG_RD_CSR_DIVSLOW(SCG) + 1;

    uint32_t firc_clock = get_firc_clock();
    // If USB is enabled, the CPU clock must not be allowed to go below 20 MHz. So the max
    // DIVCORE divider is 2.
    if (isUsbEnabled)
    {
        while ((divCore * kMinCoreClockWithUsbSupport) > firc_clock)
        {
            divCore--;
            assert(divCore);
        }
    }

    // Update SystemCoreClock global.
    SystemCoreClock = firc_clock / divCore;

    // 4. Keep bus freq below max.
    //
    // The bus/flash clock is divided by DIVSLOW in addition to DIVCORE:
    //      SCGCLOCK -> DIVCORE -> DIVSLOW -> bus_clk
    uint32_t freq = SystemCoreClock;
    while ((kMaxBusClock * divSlow) < freq)
    {
        // Increase bus/flash clock divider.
        ++divSlow;
    }

    // 5. Now set the dividers before we switch to the 48MHz clock.
    SCG_WR_RCCR_DIVCORE(SCG, divCore - 1);
    SCG_WR_RCCR_DIVSLOW(SCG, divSlow - 1);

    // 6. Turn on 48MHz IRC
    uint32_t clockSource = SCG_RD_CSR_SCS(SCG);
    if (clockSource != kClockSource_FastIRC)
    {
        SCG_SET_FIRCCSR(SCG, 1);
        SCG_WR_RCCR_SCS(SCG, kClockSource_FastIRC);
    }

    // Wait until the switch to HIRC is completed.
    while (!SCG_RD_FIRCCSR_FIRCVLD(SCG))
    {
    }
#endif // defined(BL_TARGET_ROM)
}

// See bootloader_common.h for documentation on this function.
// Note: this function doesn't apply to FPGA build
uint32_t get_system_core_clock(void)
{
    uint32_t systemCoreClock = SystemCoreClock;

    // Update SystemCoreClock out of reset.
    clock_source_type_t clocksource = (clock_source_type_t)SCG_RD_CSR_SCS(SCG);

    // Default clock source: FastIRC
    uint32_t systemClock = get_firc_clock();

    // Clock source is Slow IRC
    if (clocksource == kClockSource_SlowIRC)
    {
        if (SCG_RD_SIRCCFG_RANGE(SCG))
        {
            systemClock = kLIRC8M;
        }
        else
        {
            systemClock = kLIRC2M;
        }
    }

    systemCoreClock = systemClock / (SCG_RD_CSR_DIVCORE(SCG) + 1);

    return systemCoreClock;
}

// See bootloader_common.h for documentation on this function.
// Note: this function doesn't apply to FPGA build
uint32_t get_bus_clock(void)
{
#if defined(BL_TARGET_FPGA)
    extern uint32_t busClock;
    return busClock;
#elif defined(BL_TARGET_ROM) || defined(BL_TARGET_FLASH)
    return SystemCoreClock / (SCG_RD_CSR_DIVSLOW(SCG) + 1);
#endif
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
