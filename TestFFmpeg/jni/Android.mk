# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# LOCAL_PREBUILT_LIBS := libffmpeg.so
# LOCAL_LDLIBS+= -L$(SYSROOT)/usr/lib -llog
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
#����˱�־������ΪFFmpeg��gcc ����� NDK ����Ҫ���ӵĿ���ʹ�� g++ �����CPP�ļ���
#�п��ܳ��֡�UINT64_C�� was not declared in this scope�Ĵ���Ϊ����˴������˱�־
#LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS=1

# �����ͷ�ļ�Ŀ¼����Ҫ���� ffmpeg �ڲ�ͷ�ļ�������λ��
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ffmpeg

# ��ӵ��õĶ�̬���ӿ�  libffmpeg.so log
LOCAL_LDLIBS 	+= -L$(LOCAL_PATH) -lffmpeg -llog

#ȡ��δ�ҵ�����Ĵ���
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

# ģ�����ƶ��壬�����ǰ���ģ���������ɿ�
LOCAL_MODULE    := FFmpegJni

# Ҫ�����Դ�ļ�
LOCAL_SRC_FILES := FFmpegJni.c

# �������� [��̬ �� ��̬]
include $(BUILD_SHARED_LIBRARY)