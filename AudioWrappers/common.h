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

#ifndef AUDIO_WRAPPER_COMMON_H
#define AUDIO_WRAPPER_COMMON_H

#if 0
#include <media/AudioParameter.h>
#include <hardware/audio.h>
#endif
#include <hardware/hardware.h>

#if 0
#define ANDROID_VERSION(maj, min) \
    (maj * 10 + min)

#define WRAPPED_AUDIO_POLICY_VERSION ANDROID_VERSION(4, 0)
#define WRAPPED_AUDIO_HAL_VERSION ANDROID_VERSION(4, 0)

enum flags_conversion_mode {
    ICS_TO_JB,
    JB_TO_ICS,
};
typedef enum flags_conversion_mode flags_conversion_mode_t;
#endif

int load_vendor_module(const hw_module_t* wrapper_module, const char* name,
                       hw_device_t** device, const char* inst);
#if 0
char* fixup_audio_parameters(const char* kv_pairs, flags_conversion_mode_t mode);

uint32_t convert_audio_devices(uint32_t devices, flags_conversion_mode_t mode);
#endif

#endif // AUDIO_WRAPPER_COMMON_H
