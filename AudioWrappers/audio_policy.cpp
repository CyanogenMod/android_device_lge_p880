/*
 * Copyright (C) 2011 The Android Open Source Project
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

#define LOG_TAG "AudioPolicyWrapper"
//#define LOG_NDEBUG 0

#include <stdint.h>

#include <cutils/log.h>

#include <hardware/hardware.h>
#include <system/audio.h>
#include <system/audio_policy.h>
#include <hardware/audio_policy.h>

//#include "include/system/audio.h"
//#include "include/hardware/audio_policy.h"
#include "aps_wrapper.h"
#include "common.h"

struct wrapper_ap_module {
    struct audio_policy_module module;
};

struct wrapper_ap_device {
    struct audio_policy_device device;
    struct wrapper::audio_policy_device *wrapped_device;
};

struct wrapper_audio_policy {
    struct audio_policy policy;
    struct wrapper::audio_policy *wrapped_policy;
    void * aps_wrapper;
};

/**
 * Get the wrapped policy device from audio_policy_device struct.
 */
#define WRAPPED_DEVICE(d) ((struct wrapper_ap_device*) d)->wrapped_device

/**
 * Get the wrapped policy from the audio_policy struct.
 */
#define WRAPPED_POLICY(p) ((struct wrapper_audio_policy*) p)->wrapped_policy

/**
 * Calls func on the wrapped wrapped audio policy and returns the result.
 */
#define RETURN_WRAPPED_CALL(policy, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    return WRAPPED_POLICY(policy)->func(WRAPPED_POLICY(policy), ##__VA_ARGS__); \
})

/**
 * Calls func on the wrapped wrapped audio policy.
 */
#define WRAPPED_CALL(policy, func, ...) ({\
    ALOGV("%s", __FUNCTION__); \
    WRAPPED_POLICY(policy)->func(WRAPPED_POLICY(policy), ##__VA_ARGS__); \
})

/* configuration functions */
static int ap_set_device_connection_state(struct audio_policy *pol,
                                          audio_devices_t device,
                                          audio_policy_dev_state_t state,
                                          const char *device_address)
{
#if 0
    ALOGI("%s: device: 0x%x, state: %d, address: %s", __FUNCTION__, device, state,
          device_address);
    device = convert_audio_devices(device, JB_TO_ICS);
#endif
    RETURN_WRAPPED_CALL(pol, set_device_connection_state, device,
                        state, device_address);
}

static audio_policy_dev_state_t ap_get_device_connection_state(
                                            const struct audio_policy *pol,
                                            audio_devices_t device,
                                            const char *device_address)
{
#if 0
    ALOGI("%s: device: 0x%x, address: %s", __FUNCTION__, device, device_address);
    device = convert_audio_devices(device, JB_TO_ICS);
#endif
    RETURN_WRAPPED_CALL(pol, get_device_connection_state, device,
                        device_address);
}

static void ap_set_phone_state(struct audio_policy *pol, audio_mode_t state)
{
    WRAPPED_POLICY(pol)->set_phone_state(WRAPPED_POLICY(pol), state);
}

// deprecated, never called
static void ap_set_ringer_mode(struct audio_policy *pol, uint32_t mode,
                               uint32_t mask)
{
    WRAPPED_CALL(pol, set_ringer_mode, mode, mask);
}

static void ap_set_force_use(struct audio_policy *pol,
                          audio_policy_force_use_t usage,
                          audio_policy_forced_cfg_t config)
{
    WRAPPED_CALL(pol, set_force_use, usage, config);
}

/* retreive current device category forced for a given usage */
static audio_policy_forced_cfg_t ap_get_force_use(
                                               const struct audio_policy *pol,
                                               audio_policy_force_use_t usage)
{
    RETURN_WRAPPED_CALL(pol, get_force_use, usage);
}

/* if can_mute is true, then audio streams that are marked ENFORCED_AUDIBLE
 * can still be muted. */
static void ap_set_can_mute_enforced_audible(struct audio_policy *pol,
                                             bool can_mute)
{
    WRAPPED_CALL(pol, set_can_mute_enforced_audible, can_mute);
}

static int ap_init_check(const struct audio_policy *pol)
{
    RETURN_WRAPPED_CALL(pol, init_check);
}

/* Audio routing query functions */
static audio_io_handle_t ap_get_output(struct audio_policy *pol,
                                       audio_stream_type_t stream,
                                       uint32_t sampling_rate,
                                       audio_format_t format,
                                       audio_channel_mask_t channelMask,
                                       audio_output_flags_t flags,
                                       const audio_offload_info_t *offloadInfo)
{
    (void)(offloadInfo);
    offloadInfo = NULL;

    RETURN_WRAPPED_CALL(pol, get_output, stream, sampling_rate, format, channelMask, flags);
}

static int ap_start_output(struct audio_policy *pol, audio_io_handle_t output,
                           audio_stream_type_t stream, int session)
{
    RETURN_WRAPPED_CALL(pol, start_output, output, stream, session);
}

static int ap_stop_output(struct audio_policy *pol, audio_io_handle_t output,
                          audio_stream_type_t stream, int session)
{
    RETURN_WRAPPED_CALL(pol, stop_output, output, stream, session);
}

static void ap_release_output(struct audio_policy *pol,
                              audio_io_handle_t output)
{
    WRAPPED_CALL(pol, release_output, output);
}

static audio_io_handle_t ap_get_input(struct audio_policy *pol, audio_source_t inputSource,
                                      uint32_t sampling_rate,
                                      audio_format_t format,
                                      audio_channel_mask_t channelMask,
                                      audio_in_acoustics_t acoustics)
{
    switch (inputSource) {
        case AUDIO_SOURCE_REMOTE_SUBMIX:
            inputSource = AUDIO_SOURCE_DEFAULT;
            break;

        case AUDIO_SOURCE_HOTWORD:
            inputSource = AUDIO_SOURCE_VOICE_RECOGNITION;
            break;

        default:
            break;
    }

    RETURN_WRAPPED_CALL(pol, get_input, inputSource, sampling_rate, format, channelMask, acoustics);
}

static int ap_start_input(struct audio_policy *pol, audio_io_handle_t input)
{
    RETURN_WRAPPED_CALL(pol, start_input, input);
}

static int ap_stop_input(struct audio_policy *pol, audio_io_handle_t input)
{
    RETURN_WRAPPED_CALL(pol, stop_input, input);
}

static void ap_release_input(struct audio_policy *pol, audio_io_handle_t input)
{
    WRAPPED_CALL(pol, release_input, input);
}

/* volume control functions */
static void ap_init_stream_volume(struct audio_policy *pol,
                                  audio_stream_type_t stream, int index_min,
                                  int index_max)
{
    //ALOGI("%s: stream %d, index_min %d, index_max: %d", __FUNCTION__, stream, index_min, index_max);
    WRAPPED_CALL(pol, init_stream_volume, stream, index_min, index_max);
}

static int ap_set_stream_volume_index(struct audio_policy *pol,
                                      audio_stream_type_t stream,
                                      int index)
{
    //ALOGI("%s: stream %d, index %d", __FUNCTION__, stream, index);
    RETURN_WRAPPED_CALL(pol, set_stream_volume_index, stream, index);
}

static int ap_get_stream_volume_index(const struct audio_policy *pol,
                                      audio_stream_type_t stream,
                                      int *index)
{
#if 0
    int ret = WRAPPED_POLICY(pol)->get_stream_volume_index(WRAPPED_POLICY(pol), stream, index);
    ALOGI("%s: stream %d, index %d", __FUNCTION__, stream, *index);
    return ret;
#endif
    RETURN_WRAPPED_CALL(pol, get_stream_volume_index, stream, index);
}

static int ap_set_stream_volume_index_for_device(struct audio_policy *pol,
                                      audio_stream_type_t stream,
                                      int index,
                                      audio_devices_t device)
{
#if 0
    ALOGI("%s: stream %d, index %d, device: 0x%x", __FUNCTION__, stream, index, device);
    device = convert_audio_devices(device, JB_TO_ICS);
    // This function does not exist for ICS audio HALs so the have to call the
    // old function that doesn't differentiate between devices.
    // TODO: Somehow track the current active devices and only allow to set
    // volumes for those devices.
    RETURN_WRAPPED_CALL(pol, set_stream_volume_index, stream, index);
#endif
    RETURN_WRAPPED_CALL(pol, set_stream_volume_index_for_device, stream, index, device);
}

static int ap_get_stream_volume_index_for_device(const struct audio_policy *pol,
                                      audio_stream_type_t stream,
                                      int *index,
                                      audio_devices_t device)
{
#if 0
    int ret;
    device = convert_audio_devices(device, JB_TO_ICS);
    ret = WRAPPED_POLICY(pol)->get_stream_volume_index(WRAPPED_POLICY(pol), stream, index);
    //ret = WRAPPED_POLICY(pol)->get_stream_volume_index_for_device(WRAPPED_POLICY(pol), stream, index, device);
    ALOGI("%s: stream %d, index %d, device: 0x%x", __FUNCTION__, stream, *index, device);
    return ret;
#endif
    RETURN_WRAPPED_CALL(pol, get_stream_volume_index_for_device, stream, index, device);
}

static uint32_t ap_get_strategy_for_stream(const struct audio_policy *pol,
                                           audio_stream_type_t stream)
{
    RETURN_WRAPPED_CALL(pol, get_strategy_for_stream, stream);
}

static audio_devices_t ap_get_devices_for_stream(const struct audio_policy *pol,
                                          audio_stream_type_t stream)
{
#if 0
    ALOGI("%s: stream_type: %d", __FUNCTION__, stream);
    audio_devices_t result;
    result = WRAPPED_POLICY(pol)->get_devices_for_stream(WRAPPED_POLICY(pol), stream);
    return convert_audio_devices(result, ICS_TO_JB);
#endif
    RETURN_WRAPPED_CALL(pol, get_devices_for_stream, stream);
}

/* Audio effect management */
static audio_io_handle_t ap_get_output_for_effect(struct audio_policy *pol,
                                            const struct effect_descriptor_s *desc)
{
    RETURN_WRAPPED_CALL(pol, get_output_for_effect, desc);
}

static int ap_register_effect(struct audio_policy *pol,
                              const struct effect_descriptor_s *desc,
                              audio_io_handle_t output,
                              uint32_t strategy,
                              int session,
                              int id)
{
    RETURN_WRAPPED_CALL(pol, register_effect, desc, output, strategy, session, id);
}

static int ap_unregister_effect(struct audio_policy *pol, int id)
{
    RETURN_WRAPPED_CALL(pol, unregister_effect, id);
}

static int ap_set_effect_enabled(struct audio_policy *pol, int id, bool enabled)
{
    RETURN_WRAPPED_CALL(pol, set_effect_enabled, id, enabled);
}

static bool ap_is_stream_active(const struct audio_policy *pol, audio_stream_type_t stream,
                                uint32_t in_past_ms)
{
    RETURN_WRAPPED_CALL(pol, is_stream_active, stream, in_past_ms);
}

static bool ap_is_stream_active_remotely(const struct audio_policy *pol, audio_stream_type_t stream,
                                             uint32_t in_past_ms)
{
#if 1
    ALOGV("%s", __FUNCTION__);
    // Don't warn about unused parameters
    (void)(pol);
    (void)(stream);
    (void)(in_past_ms);
    // No NULL check in AudioPolicyService.cpp for this function but we can
    // just return 0.
    return 0;
#else
    RETURN_WRAPPED_CALL(pol, is_stream_active_remotely, stream, in_past_ms);
#endif
}

static bool ap_is_source_active(const struct audio_policy *pol,
                                    audio_source_t source)
{
#if 1
    ALOGV("%s", __FUNCTION__);
    // Don't warn about unused parameters
    (void)(pol);
    (void)(source);
    return 0;
#else
    RETURN_WRAPPED_CALL(pol, is_source_active, source);
#endif
}

/* dump state */
static int ap_dump(const struct audio_policy *pol, int fd)
{
    RETURN_WRAPPED_CALL(pol, dump, fd);
}

/* Offload support */
static bool ap_is_offload_supported(const struct audio_policy *pol,
                                   const audio_offload_info_t *info)
{
    // Offload audio is not supported in MR0 HAL.
    (void)(pol);
    (void)(info);

    return false;
}

static int create_wrapper_ap(const struct audio_policy_device *device,
                             struct audio_policy_service_ops *aps_ops,
                             void *service,
                             struct audio_policy **ap)
{
    int ret = 0;
    struct wrapper_ap_device *dev;
    struct wrapper_audio_policy *dap;
    struct wrapper::audio_policy *iap;

    *ap = NULL;

    if (!device || !service || !aps_ops) {
        ret = -EINVAL;
        goto fail_alloc;
    }

    dev = (struct wrapper_ap_device *)device;
    dap = (struct wrapper_audio_policy *)calloc(1, sizeof(*dap));
    if (!dap) {
        ret = -ENOMEM;
        goto fail_alloc;
    }

    // Wrap audio_policy_service_ops
    void * aps_wrapper;
    struct audio_policy_service_ops *aps_wrapper_ops;
    ret = aps_wrapper_create(service, aps_ops, (void **) &aps_wrapper,
                             &aps_wrapper_ops);
    if (ret) {
        ALOGE("Failed to create audio policy service wrapper");
        goto fail;
    }

    dap->aps_wrapper = aps_wrapper;
    ret = dev->wrapped_device->create_audio_policy(dev->wrapped_device,
                            /* Ugly hack, needs to be revisited. */
                            reinterpret_cast<struct wrapper::audio_policy_service_ops *>(aps_wrapper_ops),
                            aps_wrapper, &iap);
    if (ret) {
        ALOGE("Failed to create wrapped audio policy");
        goto fail;
    }

    dap->wrapped_policy = iap;

    /* configuration functions */
    dap->policy.set_device_connection_state = ap_set_device_connection_state;
    dap->policy.get_device_connection_state = ap_get_device_connection_state;
    dap->policy.set_phone_state = ap_set_phone_state;
    dap->policy.set_ringer_mode = ap_set_ringer_mode;
    dap->policy.set_force_use = ap_set_force_use;
    dap->policy.get_force_use = ap_get_force_use;
    dap->policy.set_can_mute_enforced_audible = ap_set_can_mute_enforced_audible;
    dap->policy.init_check = ap_init_check;
    /* Audio routing query functions */
    dap->policy.get_output = ap_get_output;
    dap->policy.start_output = ap_start_output;
    dap->policy.stop_output = ap_stop_output;
    dap->policy.release_output = ap_release_output;
    dap->policy.get_input = ap_get_input;
    dap->policy.start_input = ap_start_input;
    dap->policy.stop_input = ap_stop_input;
    dap->policy.release_input = ap_release_input;
    /* volume control functions */
    dap->policy.init_stream_volume = ap_init_stream_volume;
    dap->policy.set_stream_volume_index = ap_set_stream_volume_index;
    dap->policy.get_stream_volume_index = ap_get_stream_volume_index;
    dap->policy.set_stream_volume_index_for_device = ap_set_stream_volume_index_for_device;
    dap->policy.get_stream_volume_index_for_device = ap_get_stream_volume_index_for_device;
    dap->policy.get_strategy_for_stream = ap_get_strategy_for_stream;
    dap->policy.get_devices_for_stream = ap_get_devices_for_stream;
    /* Audio effect management */
    dap->policy.get_output_for_effect = ap_get_output_for_effect;
    dap->policy.register_effect = ap_register_effect;
    dap->policy.unregister_effect = ap_unregister_effect;
    dap->policy.set_effect_enabled = ap_set_effect_enabled;
    dap->policy.is_stream_active = ap_is_stream_active;
    dap->policy.is_stream_active_remotely = ap_is_stream_active_remotely;
    dap->policy.is_source_active = ap_is_source_active;
    /* dump state */
    dap->policy.dump = ap_dump;
    /* Offload support */
    dap->policy.is_offload_supported = ap_is_offload_supported;

    *ap = &dap->policy;
    return 0;

fail:
    free(dap);
fail_alloc:
    return ret;
}

static int destroy_wrapper_ap(const struct audio_policy_device *ap_dev,
                              struct audio_policy *ap)
{
    struct wrapper_ap_device *dev = (struct wrapper_ap_device *)ap_dev;
    struct wrapper_audio_policy *policy = (struct wrapper_audio_policy *)ap;

    dev->wrapped_device->destroy_audio_policy(dev->wrapped_device,
                                            policy->wrapped_policy);
    aps_wrapper_destroy(policy->aps_wrapper);
    free(policy);
    return 0;
}

static int wrapper_ap_dev_close(hw_device_t* device)
{
    WRAPPED_DEVICE(device)->common.close((hw_device_t*)&(WRAPPED_DEVICE(device)));
    free(device);
    return 0;
}

static int wrapper_ap_dev_open(const hw_module_t* module, const char* name,
                               hw_device_t** device)
{
    struct wrapper_ap_device *dev;
    int ret = 0;

    ALOGI("Wrapping vendor audio policy");

    *device = NULL;

    if (strcmp(name, AUDIO_POLICY_INTERFACE) != 0)
        return -EINVAL;

    dev = (struct wrapper_ap_device*) calloc(1, sizeof(struct wrapper_ap_device));
    if (!dev)
        return -ENOMEM;

    ret = load_vendor_module(module, name, (hw_device_t**) &dev->wrapped_device, NULL);
    if (ret) {
        free(dev);
        return ret;
    }

    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version = 0;
    dev->device.common.module = (hw_module_t *)module;
    dev->device.common.close = wrapper_ap_dev_close;
    dev->device.create_audio_policy = create_wrapper_ap;
    dev->device.destroy_audio_policy = destroy_wrapper_ap;

    *device = &dev->device.common;

    return 0;
}

static struct hw_module_methods_t wrapper_ap_module_methods = {
    .open = wrapper_ap_dev_open,
};

struct wrapper_ap_module HAL_MODULE_INFO_SYM = {
    .module = {
        .common = {
            .tag            = HARDWARE_MODULE_TAG,
            .version_major  = 1,
            .version_minor  = 0,
            .id             = AUDIO_POLICY_HARDWARE_MODULE_ID,
            .name           = "Wrapper audio policy HAL",
            .author         = "The Android Open Source Project",
            .methods        = &wrapper_ap_module_methods,
            .dso            = NULL,
            .reserved       = {0},
        },
    },
};
