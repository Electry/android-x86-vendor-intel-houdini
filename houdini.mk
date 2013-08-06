#
# Copyright (C) 2013 The Android-x86 Open Source Project
#
# Licensed under the GNU General Public License Version 2 or later.
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.gnu.org/licenses/gpl.html
#

INTEL_HOUDINI := true

LOCAL_SRC_FILES := $(filter-out %.mk %.xml,$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH)/{bin,lib} -type f)))

PRODUCT_COPY_FILES := $(foreach f,$(LOCAL_SRC_FILES),$(LOCAL_PATH)/$(f):system/$(f))

PRODUCT_PACKAGES := check.xml

PRODUCT_PROPERTY_OVERRIDES := \
    ro.product.cpu.abi2 = armeabi-v7a \
    ro.product.cpu.upgradeabi = armeabi

$(call inherit-product-if-exists,external/intel-driver/intel-driver.mk)
$(call inherit-product-if-exists,hardware/intel/wrs_omxil_core/wrs_omxil_core.mk)
$(call inherit-product-if-exists,hardware/intel/omx-components/omx-components.mk)
