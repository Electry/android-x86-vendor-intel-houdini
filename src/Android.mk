#
# Copyright (C) 2013 The Android-x86 Open Source Project
#
# Licensed under the GNU General Public License Version 2 or later.
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.gnu.org/licenses/gpl.html
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := houdini_hook.cpp

LOCAL_MODULE := libhoudini_hook
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)
