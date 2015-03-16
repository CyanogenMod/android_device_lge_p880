# Boot animation
TARGET_SCREEN_HEIGHT := 1280
TARGET_SCREEN_WIDTH := 720

# Release name
PRODUCT_RELEASE_NAME := p880

# Inherit some common CM stuff
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# This device has NFC
$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)

# Inherit device configuration
$(call inherit-product, device/lge/p880/p880.mk)

# Device identifier
PRODUCT_DEVICE := p880
PRODUCT_NAME := cm_p880
PRODUCT_BRAND := lge
PRODUCT_MODEL := Optimus-4XHD
PRODUCT_MANUFACTURER := LGE
PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=x3_open_eu BUILD_FINGERPRINT=lge/x3_open_eu/x3:4.1.2/JZO54K/P88020c.20d4ae5fac:user/release-keys PRIVATE_BUILD_DESC="x3_open_eu-user 4.1.2 JZO54K P88020c.20d4ae5fac release-keys"
