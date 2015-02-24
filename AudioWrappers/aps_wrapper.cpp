/*
 * Copyright (C) 2013 Thomas Wendt <thoemy@gmx.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "AudioPolicyServiceWrapper"
// Set to 1 to strip verbose logging messages
//#define LOG_NDEBUG 0

#include <cutils/log.h>

#include "aps_wrapper.h"
#include "common.h"

struct aps_wrapper_service {
    void * wrapped_service;
    struct audio_policy_service_ops * wrapped_aps_ops;
    struct audio_policy_service_ops aps_ops;
};

/**
 * Calls a function of the wrapped service.
 */
#define WRAPPED_CALL(service, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    aps_wrapper_service_t * __wrapped_aps = (aps_wrapper_service_t*) service; \
    return __wrapped_aps->wrapped_aps_ops->func(__wrapped_aps->wrapped_service, ##__VA_ARGS__); \
})

// deprecated: replaced by aps_open_output_on_module()
static audio_io_handle_t aps_open_output(void *service,
                                         audio_devices_t *pDevices,
                                         uint32_t *pSamplingRate,
                                         audio_format_t *pFormat,
                                         audio_channel_mask_t *pChannelMask,
                                         uint32_t *pLatencyMs,
                                         audio_output_flags_t flags)
{
#if 0
    ALOGI("%s: 0x%x, %d, %d, %x, %d, %x", __FUNCTION__, *pDevices, *pSamplingRate,
          *pFormat, *pChannelMask, *pLatencyMs, flags);
    audio_devices_t devices = convert_audio_devices(*pDevices, ICS_TO_JB);

    // The old policy managers don't set the PRIMARY flag for the
    // first output it opens but AudioFlinger needs at least one device with
    // that flag.
    // TODO: Should probably only be done for the first device opened.
    uint32_t newflags = flags | AUDIO_OUTPUT_FLAG_PRIMARY;
    flags = (audio_output_flags_t)newflags;

    WRAPPED_CALL(service, open_output, &devices, pSamplingRate, pFormat,
                 pChannelMask, pLatencyMs, flags);
#endif
    WRAPPED_CALL(service, open_output, pDevices, pSamplingRate, pFormat,
                 pChannelMask, pLatencyMs, flags);
}

static audio_io_handle_t aps_open_duplicate_output(void *service,
                                             audio_io_handle_t output1,
                                             audio_io_handle_t output2)
{
    WRAPPED_CALL(service, open_duplicate_output, output1, output2);
}

static int aps_close_output(void *service, audio_io_handle_t output)
{
    WRAPPED_CALL(service, close_output, output);
}

static int aps_suspend_output(void *service, audio_io_handle_t output)
{
    WRAPPED_CALL(service, suspend_output, output);
}

static int aps_restore_output(void *service, audio_io_handle_t output)
{
    WRAPPED_CALL(service, restore_output, output);
}

// deprecated: replaced by aps_open_input_on_module(), and acoustics parameter is ignored
static audio_io_handle_t aps_open_input(void *service,
                                        audio_devices_t *pDevices,
                                        uint32_t *pSamplingRate,
                                        audio_format_t *pFormat,
                                        audio_channel_mask_t *pChannelMask,
                                        audio_in_acoustics_t acoustics)
{
#if 0
    ALOGI("%s: 0x%x, %d, %d, %x, %d", __FUNCTION__, *pDevices, *pSamplingRate,
          *pFormat, *pChannelMask, acoustics);
    audio_devices_t devices = convert_audio_devices(*pDevices, ICS_TO_JB);
    WRAPPED_CALL(service, open_input, &devices, pSamplingRate, pFormat,
                 pChannelMask, acoustics);
#endif
    WRAPPED_CALL(service, open_input, pDevices, pSamplingRate, pFormat,
                 pChannelMask, acoustics);
}

static int aps_close_input(void *service, audio_io_handle_t input)
{
    WRAPPED_CALL(service, close_input, input);
}

static int aps_set_stream_volume(void *service, audio_stream_type_t stream,
                                 float volume, audio_io_handle_t output,
                                 int delay_ms)
{
    //ALOGI("%s: stream: %d, volume: %f, output: %d, delay_ms: %d",
    //      __FUNCTION__, stream, volume, output, delay_ms);
    WRAPPED_CALL(service, set_stream_volume, stream, volume, output, delay_ms);
}

static int aps_set_stream_output(void *service, audio_stream_type_t stream)
{
    WRAPPED_CALL(service, invalidate_stream, stream);
}

static void aps_set_parameters(void *service, audio_io_handle_t io_handle,
                               const char *kv_pairs, int delay_ms)
{
#if 0
    ALOGI("%s: io_handle: %d, kv_pairs: %s", __FUNCTION__, io_handle, kv_pairs);
    aps_wrapper_service_t * waps = (aps_wrapper_service_t*) service;

    char * fixed_kv_pairs = fixup_audio_parameters(kv_pairs, ICS_TO_JB);
    waps->wrapped_aps_ops->set_parameters(waps->wrapped_service, io_handle,
                                          fixed_kv_pairs, delay_ms);
    free(fixed_kv_pairs);
#endif
    WRAPPED_CALL(service, set_parameters, io_handle, kv_pairs, delay_ms);
}

static char * aps_get_parameters(void *service, audio_io_handle_t io_handle,
                                 const char *keys)
{
#if 0
    ALOGI("%s: io_handle: %d, keys: %s", __FUNCTION__, io_handle, keys);
    aps_wrapper_service_t * waps = (aps_wrapper_service_t*) service;
    char * kv_pairs;
    char * fixed_kv_pairs;

    kv_pairs = waps->wrapped_aps_ops->get_parameters(waps->wrapped_service, io_handle,
                                                     keys);
    fixed_kv_pairs = fixup_audio_parameters(kv_pairs, JB_TO_ICS);
    free(kv_pairs);
    return fixed_kv_pairs;
#endif
    WRAPPED_CALL(service, get_parameters, io_handle, keys);
}

static int aps_start_tone(void *service, audio_policy_tone_t tone,
                          audio_stream_type_t stream)
{
    WRAPPED_CALL(service, start_tone, tone, stream);
}

static int aps_stop_tone(void *service)
{
    WRAPPED_CALL(service, stop_tone);
}

static int aps_set_voice_volume(void *service, float volume, int delay_ms)
{
    //ALOGI("%s: volume: %f, delay_ms: %d", __FUNCTION__, volume, delay_ms);
    WRAPPED_CALL(service, set_voice_volume, volume, delay_ms);
}

static int aps_move_effects(void *service, int session,
                            audio_io_handle_t src_output,
                            audio_io_handle_t dst_output)
{
    WRAPPED_CALL(service, move_effects, session, src_output, dst_output);
}

static audio_module_handle_t aps_load_hw_module(void *service,
                                             const char *name)
{
    WRAPPED_CALL(service, load_hw_module, name);
}

static audio_io_handle_t aps_open_output_on_module(void *service,
                                                   audio_module_handle_t module,
                                                   audio_devices_t *pDevices,
                                                   uint32_t *pSamplingRate,
                                                   audio_format_t *pFormat,
                                                   audio_channel_mask_t *pChannelMask,
                                                   uint32_t *pLatencyMs,
                                                   audio_output_flags_t flags,
                                                   const audio_offload_info_t *offloadInfo)
{
#if 0
    ALOGI("%s: 0x%x, %d, %d, %x, %d, %x", __FUNCTION__, *pDevices, *pSamplingRate,
          *pFormat, *pChannelMask, *pLatencyMs, flags);
    audio_devices_t devices = convert_audio_devices(*pDevices, ICS_TO_JB);
    WRAPPED_CALL(service, open_output_on_module, module, &devices, pSamplingRate,
                 pFormat, pChannelMask, pLatencyMs, flags);
#endif
    (void)(offloadInfo);

    WRAPPED_CALL(service, open_output_on_module, module, pDevices, pSamplingRate,
                 pFormat, pChannelMask, pLatencyMs, flags, NULL);
}

static audio_io_handle_t aps_open_input_on_module(void *service,
                                                  audio_module_handle_t module,
                                                  audio_devices_t *pDevices,
                                                  uint32_t *pSamplingRate,
                                                  audio_format_t *pFormat,
                                                  audio_channel_mask_t *pChannelMask)
{
#if 0
    ALOGI("%s: 0x%x, %d, %d, %x", __FUNCTION__, *pDevices, *pSamplingRate, *pFormat, *pChannelMask);
    audio_devices_t devices = convert_audio_devices(*pDevices, ICS_TO_JB);
    WRAPPED_CALL(service, open_input_on_module, module, &devices, pSamplingRate, pFormat,
                 pChannelMask);
#endif
    WRAPPED_CALL(service, open_input_on_module, module, pDevices, pSamplingRate, pFormat,
                 pChannelMask);
}

/**
 * Wraps a service and the corresponding service ops with our mock service.
 */
int aps_wrapper_create(void *wrapped_service,
                       struct audio_policy_service_ops * wrapped_aps_ops,
                       void ** service,
                       struct audio_policy_service_ops ** aps_ops)
{
    aps_wrapper_service_t * waps;

    waps = (aps_wrapper_service_t *) malloc(sizeof(*waps));
    if (!waps)
        return -ENOMEM;

    waps->wrapped_service = wrapped_service;
    waps->wrapped_aps_ops = wrapped_aps_ops;

    waps->aps_ops.open_output = aps_open_output;
    waps->aps_ops.open_duplicate_output = aps_open_duplicate_output;
    waps->aps_ops.close_output = aps_close_output;
    waps->aps_ops.suspend_output = aps_suspend_output;
    waps->aps_ops.restore_output = aps_restore_output;
    waps->aps_ops.open_input = aps_open_input;
    waps->aps_ops.close_input = aps_close_input;
    waps->aps_ops.set_stream_volume = aps_set_stream_volume;
    waps->aps_ops.invalidate_stream = aps_set_stream_output;
    waps->aps_ops.set_parameters = aps_set_parameters;
    waps->aps_ops.get_parameters = aps_get_parameters;
    waps->aps_ops.start_tone = aps_start_tone;
    waps->aps_ops.stop_tone = aps_stop_tone;
    waps->aps_ops.set_voice_volume = aps_set_voice_volume;
    waps->aps_ops.move_effects = aps_move_effects;
    waps->aps_ops.load_hw_module = aps_load_hw_module;
    waps->aps_ops.open_output_on_module = aps_open_output_on_module;
    waps->aps_ops.open_input_on_module = aps_open_input_on_module;

    *service = waps;
    *aps_ops = &waps->aps_ops;

    return 0;
}

void aps_wrapper_destroy(void * wrapped_service)
{
    free(wrapped_service);
}
