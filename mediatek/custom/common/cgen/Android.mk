LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
#LOCAL_ARM_MODE:=arm

# DMNR 3.0
ifeq ($(strip $(MTK_HANDSFREE_DMNR_SUPPORT)),yes)
  LOCAL_CFLAGS += -DMTK_HANDSFREE_DMNR_SUPPORT
endif
# DMNR 3.0

LOCAL_SHARED_LIBRARIES:= libc libcutils
LOCAL_SRC_FILES:= \
	CFG_file_info.c
LOCAL_C_INCLUDES:= \
      $(MTK_PATH_CUSTOM)/cgen/inc \
      $(MTK_PATH_CUSTOM)/cgen/cfgfileinc \
      $(MTK_PATH_CUSTOM)/cgen/cfgdefault \
      $(MTK_PATH_SOURCE)/external/nvram/libnvram
LOCAL_MODULE:=libcustom_nvram
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE:= false
include $(BUILD_SHARED_LIBRARY)


