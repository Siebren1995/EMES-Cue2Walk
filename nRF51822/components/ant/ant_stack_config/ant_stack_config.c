/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "sdk_config.h"
#if ANT_STACK_CONFIG_ENABLED
#include "nrf_assert.h"
#include "ant_stack_config.h"
#include "ant_interface.h"
#include "ant_parameters.h"

#define ANT_BUFFER_SIZE_FOR_SD    ANT_ENABLE_GET_REQUIRED_SPACE(ANT_CONFIG_TOTAL_CHANNELS_ALLOCATED, \
                                                                ANT_CONFIG_ENCRYPTED_CHANNELS,       \
                                                                ANT_CONFIG_BURST_QUEUE_SIZE,         \
                                                                ANT_CONFIG_EVENT_QUEUE_SIZE)

static union
{
    uint8_t  u8[ANT_BUFFER_SIZE_FOR_SD];
    uint32_t u32[1]; // force allign to uint32_t
}ant_stack_buffer; /*!< Memory buffer provided in order to support channel configuration */

uint32_t ant_stack_static_config(void)
{
    ASSERT(ANT_CONFIG_TOTAL_CHANNELS_ALLOCATED <= MAX_ANT_CHANNELS);
    ASSERT(ANT_CONFIG_ENCRYPTED_CHANNELS <= ANT_CONFIG_TOTAL_CHANNELS_ALLOCATED);

    ANT_ENABLE m_ant_enable_cfg =
    {
        .ucTotalNumberOfChannels        = ANT_CONFIG_TOTAL_CHANNELS_ALLOCATED,
        .ucNumberOfEncryptedChannels    = ANT_CONFIG_ENCRYPTED_CHANNELS,
        .usNumberOfEvents               = ANT_CONFIG_EVENT_QUEUE_SIZE,
        .pucMemoryBlockStartLocation    = ant_stack_buffer.u8,
        .usMemoryBlockByteSize          = ANT_BUFFER_SIZE_FOR_SD
    };

    return sd_ant_enable(&m_ant_enable_cfg);
}

#endif // ANT_STACK_CONFIG_ENABLED
