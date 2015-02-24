/*
 * Copyright (C) 2011 The Android Open Source Project
 * Copyright (c) 2012, Code Aurora Forum. All rights reserved.
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


#ifndef WRAPPED_ANDROID_AUDIO_HAL_INTERFACE_H
#define WRAPPED_ANDROID_AUDIO_HAL_INTERFACE_H

#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <cutils/bitops.h>

#include <hardware/hardware.h>
#include <system/audio.h>
#include <hardware/audio_effect.h>

namespace wrapper {

__BEGIN_DECLS

/* common audio stream configuration parameters */
#if 0
struct audio_config {
    uint32_t sample_rate;
    audio_channel_mask_t channel_mask;
    audio_format_t  format;
};

typedef struct audio_config audio_config_t;
#endif
#ifdef QCOM_HARDWARE
typedef struct buf_info;
#endif

/* common audio stream parameters and operations */
struct audio_stream {

    /**
     * Return the sampling rate in Hz - eg. 44100.
     */
    uint32_t (*get_sample_rate)(const struct audio_stream *stream);

    /* currently unused - use set_parameters with key
     *    AUDIO_PARAMETER_STREAM_SAMPLING_RATE
     */
    int (*set_sample_rate)(struct audio_stream *stream, uint32_t rate);

    /**
     * Return size of input/output buffer in bytes for this stream - eg. 4800.
     * It should be a multiple of the frame size.  See also get_input_buffer_size.
     */
    size_t (*get_buffer_size)(const struct audio_stream *stream);

    /**
     * Return the channel mask -
     *  e.g. AUDIO_CHANNEL_OUT_STEREO or AUDIO_CHANNEL_IN_STEREO
     */
    audio_channel_mask_t (*get_channels)(const struct audio_stream *stream);

    /**
     * Return the audio format - e.g. AUDIO_FORMAT_PCM_16_BIT
     */
    audio_format_t (*get_format)(const struct audio_stream *stream);

    /* currently unused - use set_parameters with key
     *     AUDIO_PARAMETER_STREAM_FORMAT
     */
    int (*set_format)(struct audio_stream *stream, audio_format_t format);

    /**
     * Put the audio hardware input/output into standby mode.
     * Driver should exit from standby mode at the next I/O operation.
     * Returns 0 on success and <0 on failure.
     */
    int (*standby)(struct audio_stream *stream);

    /** dump the state of the audio input/output device */
    int (*dump)(const struct audio_stream *stream, int fd);

    /** Return the set of device(s) which this stream is connected to */
    audio_devices_t (*get_device)(const struct audio_stream *stream);

    /**
     * Currently unused - set_device() corresponds to set_parameters() with key
     * AUDIO_PARAMETER_STREAM_ROUTING for both input and output.
     * AUDIO_PARAMETER_STREAM_INPUT_SOURCE is an additional information used by
     * input streams only.
     */
    int (*set_device)(struct audio_stream *stream, audio_devices_t device);

    /**
     * set/get audio stream parameters. The function accepts a list of
     * parameter key value pairs in the form: key1=value1;key2=value2;...
     *
     * Some keys are reserved for standard parameters (See AudioParameter class)
     *
     * If the implementation does not accept a parameter change while
     * the output is active but the parameter is acceptable otherwise, it must
     * return -ENOSYS.
     *
     * The audio flinger will put the stream in standby and then change the
     * parameter value.
     */
    int (*set_parameters)(struct audio_stream *stream, const char *kv_pairs);

    /*
     * Returns a pointer to a heap allocated string. The caller is responsible
     * for freeing the memory for it using free().
     */
    char * (*get_parameters)(const struct audio_stream *stream,
                             const char *keys);
    int (*add_audio_effect)(const struct audio_stream *stream,
                             effect_handle_t effect);
    int (*remove_audio_effect)(const struct audio_stream *stream,
                             effect_handle_t effect);
};
typedef struct audio_stream audio_stream_t;

/**
 * audio_stream_out is the abstraction interface for the audio output hardware.
 *
 * It provides information about various properties of the audio output
 * hardware driver.
 */

struct audio_stream_out {
    struct audio_stream common;

    /**
     * Return the audio hardware driver estimated latency in milliseconds.
     */
    uint32_t (*get_latency)(const struct audio_stream_out *stream);

    /**
     * Use this method in situations where audio mixing is done in the
     * hardware. This method serves as a direct interface with hardware,
     * allowing you to directly set the volume as apposed to via the framework.
     * This method might produce multiple PCM outputs or hardware accelerated
     * codecs, such as MP3 or AAC.
     */
    int (*set_volume)(struct audio_stream_out *stream, float left, float right);

    /**
     * Write audio buffer to driver. Returns number of bytes written, or a
     * negative status_t. If at least one frame was written successfully prior to the error,
     * it is suggested that the driver return that successful (short) byte count
     * and then return an error in the subsequent call.
     */
    ssize_t (*write)(struct audio_stream_out *stream, const void* buffer,
                     size_t bytes);

    /* return the number of audio frames written by the audio dsp to DAC since
     * the output has exited standby
     */
    int (*get_render_position)(const struct audio_stream_out *stream,
                               uint32_t *dsp_frames);

#ifdef QCOM_HARDWARE
    /**
     * start audio data rendering
     */
    int (*start)(struct audio_stream_out *stream);

    /**
     * pause audio rendering
     */
    int (*pause)(struct audio_stream_out *stream);

    /**
     * flush audio data with driver
     */
    int (*flush)(struct audio_stream_out *stream);

    /**
     * stop audio data rendering
     */
    int (*stop)(struct audio_stream_out *stream);
#endif

    /**
     * get the local time at which the next write to the audio driver will be presented.
     * The units are microseconds, where the epoch is decided by the local audio HAL.
     */
    int (*get_next_write_timestamp)(const struct audio_stream_out *stream,
                                    int64_t *timestamp);
#ifdef QCOM_HARDWARE
    /**
    * return the current timestamp after quering to the driver
     */
    int (*get_time_stamp)(const struct audio_stream_out *stream,
                               uint64_t *time_stamp);
    /**
    * EOS notification from HAL to Player
     */
    int (*set_observer)(const struct audio_stream_out *stream,
                               void *observer);
    /**
     * Get the physical address of the buffer allocated in the
     * driver
     */
    int (*get_buffer_info) (const struct audio_stream_out *stream,
                                struct buf_info **buf);
    /**
     * Check if next buffer is available. Waits until next buffer is
     * available
     */
    int (*is_buffer_available) (const struct audio_stream_out *stream,
                                     int *isAvail);
#endif

};
typedef struct audio_stream_out audio_stream_out_t;

#ifdef QCOM_HARDWARE
/**
 * audio_broadcast_stream is the abstraction interface for the
 * audio output hardware.
 *
 * It provides information about various properties of the audio output
 * hardware driver.
 */

struct audio_broadcast_stream {
    struct audio_stream common;

    /**
     * return the audio hardware driver latency in milli seconds.
     */
    uint32_t (*get_latency)(const struct audio_broadcast_stream *stream);

    /**
     * Use this method in situations where audio mixing is done in the
     * hardware. This method serves as a direct interface with hardware,
     * allowing you to directly set the volume as apposed to via the framework.
     * This method might produce multiple PCM outputs or hardware accelerated
     * codecs, such as MP3 or AAC.
     */
    int (*set_volume)(struct audio_broadcast_stream *stream, float left, float right);

    int (*mute)(struct audio_broadcast_stream *stream, bool mute);

    int (*start)(struct audio_broadcast_stream *stream, int64_t absTimeToStart);
    /**
     * write audio buffer to driver. Returns number of bytes written
     */
    ssize_t (*write)(struct audio_broadcast_stream *stream, const void* buffer,
                     size_t bytes, int64_t timestamp, int audioType);

};
typedef struct audio_broadcast_stream audio_broadcast_stream_t;
#endif

struct audio_stream_in {
    struct audio_stream common;

    /** set the input gain for the audio driver. This method is for
     *  for future use */
    int (*set_gain)(struct audio_stream_in *stream, float gain);

    /** Read audio buffer in from audio driver. Returns number of bytes read, or a
     *  negative status_t. If at least one frame was read prior to the error,
     *  read should return that byte count and then return an error in the subsequent call.
     */
    ssize_t (*read)(struct audio_stream_in *stream, void* buffer,
                    size_t bytes);

    /**
     * Return the amount of input frames lost in the audio driver since the
     * last call of this function.
     * Audio driver is expected to reset the value to 0 and restart counting
     * upon returning the current value by this function call.
     * Such loss typically occurs when the user space process is blocked
     * longer than the capacity of audio driver buffers.
     *
     * Unit: the number of input audio frames
     */
    uint32_t (*get_input_frames_lost)(struct audio_stream_in *stream);
};
typedef struct audio_stream_in audio_stream_in_t;

/**
 * return the frame size (number of bytes per sample).
 */
static inline size_t audio_stream_frame_size(struct audio_stream *s)
{
    size_t chan_samp_sz;
    uint32_t chan_mask = s->get_channels(s);
    int format = s->get_format(s);
    char *tmpparam;
    int isParamEqual;

#ifdef QCOM_HARDWARE
    if (audio_is_input_channel(chan_mask)) {
        chan_mask &= (AUDIO_CHANNEL_IN_STEREO | \
                      AUDIO_CHANNEL_IN_MONO );
    }

    tmpparam = s->get_parameters(s, "voip_flag");
    isParamEqual = !strcmp(tmpparam,"voip_flag=1");
    free(tmpparam);
    if(isParamEqual) {
        if(format != AUDIO_FORMAT_PCM_8_BIT)
            return popcount(chan_mask) * sizeof(int16_t);
        else
            return popcount(chan_mask) * sizeof(int8_t);
    }

    if (audio_is_input_channel(chan_mask)) {
        chan_mask &= (AUDIO_CHANNEL_IN_STEREO | \
                      AUDIO_CHANNEL_IN_MONO | \
                      AUDIO_CHANNEL_IN_5POINT1);
    }
#endif

    switch (format) {
#ifdef QCOM_HARDWARE
    case AUDIO_FORMAT_AMR_NB:
        chan_samp_sz = 32;
        break;
    case AUDIO_FORMAT_EVRC:
        chan_samp_sz = 23;
        break;
    case AUDIO_FORMAT_QCELP:
        chan_samp_sz = 35;
        break;
#endif
    case AUDIO_FORMAT_PCM_16_BIT:
        chan_samp_sz = sizeof(int16_t);
        break;
    case AUDIO_FORMAT_PCM_8_BIT:
    default:
        chan_samp_sz = sizeof(int8_t);
        break;
    }

    return popcount(chan_mask) * chan_samp_sz;
}


/**********************************************************************/

/**
 * Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
 * and the fields of this data structure must begin with hw_module_t
 * followed by module specific information.
 */
struct audio_module {
    struct hw_module_t common;
};

struct audio_hw_device {
    struct hw_device_t common;

    /**
     * used by audio flinger to enumerate what devices are supported by
     * each audio_hw_device implementation.
     *
     * Return value is a bitmask of 1 or more values of audio_devices_t
     */
    uint32_t (*get_supported_devices)(const struct audio_hw_device *dev);

    /**
     * check to see if the audio hardware interface has been initialized.
     * returns 0 on success, -ENODEV on failure.
     */
    int (*init_check)(const struct audio_hw_device *dev);

    /** set the audio volume of a voice call. Range is between 0.0 and 1.0 */
    int (*set_voice_volume)(struct audio_hw_device *dev, float volume);

    /**
     * set the audio volume for all audio activities other than voice call.
     * Range between 0.0 and 1.0. If any value other than 0 is returned,
     * the software mixer will emulate this capability.
     */
    int (*set_master_volume)(struct audio_hw_device *dev, float volume);

#ifndef ICS_AUDIO_BLOB
    /**
     * Get the current master volume value for the HAL, if the HAL supports
     * master volume control.  AudioFlinger will query this value from the
     * primary audio HAL when the service starts and use the value for setting
     * the initial master volume across all HALs.  HALs which do not support
     * this method should may leave it set to NULL.
     */
    int (*get_master_volume)(struct audio_hw_device *dev, float *volume);
#endif

#ifdef QCOM_FM_ENABLED
    /** set the fm audio volume. Range is between 0.0 and 1.0 */
    int (*set_fm_volume)(struct audio_hw_device *dev, float volume);
#endif

    /**
     * set_mode is called when the audio mode changes. AUDIO_MODE_NORMAL mode
     * is for standard audio playback, AUDIO_MODE_RINGTONE when a ringtone is
     * playing, and AUDIO_MODE_IN_CALL when a call is in progress.
     */
    int (*set_mode)(struct audio_hw_device *dev, audio_mode_t mode);

    /* mic mute */
    int (*set_mic_mute)(struct audio_hw_device *dev, bool state);
    int (*get_mic_mute)(const struct audio_hw_device *dev, bool *state);

    /* set/get global audio parameters */
    int (*set_parameters)(struct audio_hw_device *dev, const char *kv_pairs);

    /*
     * Returns a pointer to a heap allocated string. The caller is responsible
     * for freeing the memory for it using free().
     */
    char * (*get_parameters)(const struct audio_hw_device *dev,
                             const char *keys);

    /* Returns audio input buffer size according to parameters passed or
     * 0 if one of the parameters is not supported.
     * See also get_buffer_size which is for a particular stream.
     */
    size_t (*get_input_buffer_size)(const struct audio_hw_device *dev,
#ifndef ICS_AUDIO_BLOB
                                    const struct audio_config *config);
#else
                                    uint32_t sample_rate, int format,
                                    int channel_count);
#endif

    /** This method creates and opens the audio hardware output stream */
#ifndef ICS_AUDIO_BLOB
    int (*open_output_stream)(struct audio_hw_device *dev,
                              audio_io_handle_t handle,
                              audio_devices_t devices,
                              audio_output_flags_t flags,
                              struct audio_config *config,
                              struct audio_stream_out **stream_out);
#else
    int (*open_output_stream)(struct audio_hw_device *dev, uint32_t devices,
                              int *format, uint32_t *channels,
                              uint32_t *sample_rate,
                              struct audio_stream_out **out);
#endif

#ifdef QCOM_ICS_LPA_COMPAT
    /** This method creates and opens the audio hardware output session */
    int (*open_output_session)(struct audio_hw_device *dev, uint32_t devices,
                              int *format, int sessionId,
                              struct audio_stream_out **out);
#endif

    void (*close_output_stream)(struct audio_hw_device *dev,
                                struct audio_stream_out* stream_out);

#if defined (QCOM_HARDWARE) || defined (STE_SAMSUNG_HARDWARE)
    /** This method creates and opens the audio hardware output
     *  for broadcast stream */
    int (*open_broadcast_stream)(struct audio_hw_device *dev, uint32_t devices,
                                 int format, uint32_t channels,
                                 uint32_t sample_rate,
                                 uint32_t audio_source,
                                 struct audio_broadcast_stream **out);

    void (*close_broadcast_stream)(struct audio_hw_device *dev,
                                   struct audio_broadcast_stream *out);
#endif

    /** This method creates and opens the audio hardware input stream */
#ifndef ICS_AUDIO_BLOB
    int (*open_input_stream)(struct audio_hw_device *dev,
                             audio_io_handle_t handle,
                             audio_devices_t devices,
                             struct audio_config *config,
                             struct audio_stream_in **stream_in);
#else
    int (*open_input_stream)(struct audio_hw_device *dev, uint32_t devices,
                             int *format, uint32_t *channels,
                             uint32_t *sample_rate,
                             audio_in_acoustics_t acoustics,
                             struct audio_stream_in **stream_in);
#endif

    void (*close_input_stream)(struct audio_hw_device *dev,
                               struct audio_stream_in *stream_in);

    /** This method dumps the state of the audio hardware */
    int (*dump)(const struct audio_hw_device *dev, int fd);
};
typedef struct audio_hw_device audio_hw_device_t;

/** convenience API for opening and closing a supported device */

static inline int audio_hw_device_open(const struct hw_module_t* module,
                                       struct audio_hw_device** device)
{
    return module->methods->open(module, AUDIO_HARDWARE_INTERFACE,
                                 (struct hw_device_t**)device);
}

static inline int audio_hw_device_close(struct audio_hw_device* device)
{
    return device->common.close(&device->common);
}

#ifdef QCOM_HARDWARE
/** Structure to save buffer information for applying effects for
 *  LPA buffers */
struct buf_info {
    int bufsize;
    int nBufs;
    int **buffers;
};

#ifdef __cplusplus
/**
 *Observer class to post the Events from HAL to Flinger
*/
class AudioEventObserver {
public:
    virtual ~AudioEventObserver() {}
    virtual void postEOS(int64_t delayUs) = 0;
};
#endif
#endif
__END_DECLS

} // namespace wrapper

#endif  // WRAPPED_ANDROID_AUDIO_INTERFACE_H
