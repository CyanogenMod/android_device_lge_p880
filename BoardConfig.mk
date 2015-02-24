# inherit from the proprietary version
-include vendor/lge/p880/BoardConfigVendor.mk

# Target arch settings
TARGET_ARCH := arm
TARGET_NO_BOOTLOADER := true
TARGET_BOARD_PLATFORM := tegra
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_VARIANT := cortex-a9
TARGET_ARCH_VARIANT_CPU := $(TARGET_CPU_VARIANT)

# Build Optimizations
ARCH_ARM_HAVE_TLS_REGISTER := true
ARCH_ARM_HAVE_32_BYTE_CACHE_LINES := true
ARCH_ARM_USE_NON_NEON_MEMCPY := true
COMMON_GLOBAL_CFLAGS += -D__ARM_USE_PLD -D__ARM_CACHE_LINE_SIZE=32
TARGET_GLOBAL_CFLAGS += -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp

# Avoid the generation of ldrcc instructions
NEED_WORKAROUND_CORTEX_A9_745320 := true

# Skip droiddoc build to save build time
BOARD_SKIP_ANDROID_DOC_BUILD := true

# Enable WEBGL in WebKit
ENABLE_WEBGL := true

# Board naming
TARGET_NO_RADIOIMAGE := true
TARGET_BOOTLOADER_BOARD_NAME := p880

# Lollipop Changes
TARGET_USES_LOGD := false
MALLOC_IMPL := dlmalloc
USE_LEGACY_AUDIO_POLICY := 1
COMMON_GLOBAL_CFLAGS += \
    -DADD_LEGACY_SET_POSITION_SYMBOL \
    -DADD_LEGACY_SURFACE_COMPOSER_CLIENT_SYMBOLS \
    -DADD_LEGACY_MEMORY_DEALER_CONSTRUCTOR_SYMBOL \
    -DADD_LEGACY_ACQUIRE_BUFFER_SYMBOL \
    -DMR0_AUDIO_BLOB \
    -DMR0_CAMERA_BLOB \
    -DNEEDS_VECTORIMPL_SYMBOLS \
    -DREFBASE_JB_MR1_COMPAT_SYMBOLS \
    -DSK_SUPPORT_LEGACY_DECODEFILE

BOARD_HAVE_PRE_KITKAT_AUDIO_BLOB := true
BOARD_HAVE_PRE_KITKAT_AUDIO_POLICY_BLOB := true

#BOARD_USES_LEGACY_MMAP := true
#BOARD_HAVE_PIXEL_FORMAT_INFO := true

BOARD_KERNEL_CMDLINE := androidboot.selinux=permissive
BOARD_KERNEL_BASE := 0x10000000
BOARD_KERNEL_PAGESIZE := 2048

# fix this up by examining /proc/mtd on a running device
BOARD_BOOTIMAGE_PARTITION_SIZE := 10485760 # 10M
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 10485760 # 10M
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1610612736 # 1.5GB
BOARD_USERDATAIMAGE_PARTITION_SIZE := 13375635456 # 12.3GB
BOARD_CACHEIMAGE_PARTITION_SIZE := 422576128 # 403M
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_FLASH_BLOCK_SIZE := 4096
BOARD_HAS_LARGE_FILESYSTEM := true

# Include an expanded selection of fonts
#EXTENDED_FONT_FOOTPRINT := true

TARGET_RECOVERY_PRE_COMMAND := "/system/bin/setup-recovery"

# Try to build the kernel
TARGET_KERNEL_CONFIG := cyanogenmod_x3_defconfig

BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_RECOVERY_FSTAB = device/lge/p880/rootdir/fstab.x3
TARGET_USERIMAGES_USE_EXT4 := true

TARGET_SPECIFIC_HEADER_PATH := device/lge/p880/include

# Hardware rendering
USE_OPENGL_RENDERER := true
BOARD_EGL_CFG := device/lge/p880/configs/egl.cfg
BOARD_USE_MHEAP_SCREENSHOT := true
BOARD_EGL_WORKAROUND_BUG_10194508 := true
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true
BOARD_EGL_SKIP_FIRST_DEQUEUE := true

# Wifi related defines
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_WLAN_DEVICE := bcmdhd
WIFI_DRIVER_FW_PATH_PARAM := "/sys/module/bcmdhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_STA := "/system/etc/firmware/fw_bcmdhd_p2p.bin"
WIFI_DRIVER_FW_PATH_P2P := "/system/etc/firmware/fw_bcmdhd_p2p.bin"
WIFI_DRIVER_FW_PATH_AP := "/system/etc/firmware/fw_bcmdhd_apsta.bin"
BOARD_LEGACY_NL80211_STA_EVENTS := true

TARGET_BOOTANIMATION_PRELOAD := true
TARGET_BOOTANIMATION_TEXTURE_CACHE := true

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/lge/p880/bluetooth
BOARD_BLUEDROID_VENDOR_CONF := device/lge/p880/bluetooth/vnd_bt.txt

BOARD_HAVE_PRE_KITKAT_AUDIO_BLOB := true
BOARD_HAVE_PRE_KITKAT_AUDIO_POLICY_BLOB := true

## Radio fixes
BOARD_RIL_CLASS := ../../../device/lge/p880/ril/

# Override healthd HAL
BOARD_HAL_STATIC_LIBRARIES := libhealthd.x3

BOARD_CUSTOM_GRAPHICS := ../../../device/lge/p880/recovery/recovery-gfx.c
BOARD_CUSTOM_RECOVERY_KEYMAPPING := ../../device/lge/p880/recovery/recovery-keys.c
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_BATTERY_DEVICE_NAME := battery

TARGET_BOOTANIMATION_PRELOAD := true
TARGET_BOOTANIMATION_TEXTURE_CACHE := true

BOARD_SEPOLICY_DIRS += \
    device/lge/p880/sepolicy

BOARD_SEPOLICY_UNION += \
    file_contexts \
    genfs_contexts \
    property_contexts \
    service_contexts \
    bluetooth.te \
    device.te \
    domain.te \
    drmserver.te \
    init_shell.te \
    file.te \
    gpsd.te \
    keystore.te \
    lmkd.te \
    mediaserver.te \
    platform_app.te \
    recovery.te \
    rild.te \
    sensors_config.te \
    surfaceflinger.te \
    system_app.te \
    system_server.te \
    ueventd.te \
    vold.te

BOARD_HARDWARE_CLASS := device/lge/p880/cmhw/
