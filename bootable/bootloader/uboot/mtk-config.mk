ifeq (,$(MTK_ROOT))
include $(abspath $(TOPDIR)/../../../mediatek/build/Makefile)
$(call codebase-path,uboot)
endif

$(if $(MTK_PATH_PLATFORM),,$(error mtk config is not initialized))
MTK_INC := $(addprefix -I,$(strip          \
    $(MTK_PATH_PLATFORM)/inc               \
    $(MTK_PATH_CUSTOM)/inc                 \
    $(MTK_ROOT_CUSTOM)/$(TARGET_PRODUCT)/common \
    $(MTK_ROOT_CUSTOM_OUT)/kernel/dct      \
    $(MTK_ROOT_CUSTOM_OUT)/kernel/lcm/inc  \
    $(MTK_ROOT_CUSTOM_OUT)/kernel/leds/inc \
))
MTK_CDEFS    += $(call mtk.custom.generate-macros)

CPPFLAGS     += $(MTK_INC) $(MTK_CDEFS)
CFLAGS       += $(MTK_INC) $(MTK_CDEFS)
AFLAGS       += $(MTK_INC) $(MTK_CDEFS)
HOSTCPPFLAGS += $(MTK_INC) $(MTK_CDEFS)

sinclude $(MTK_PATH_CUSTOM)/config.mk  # include board specific rules
