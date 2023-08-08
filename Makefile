PROJECT = STM32_DEV_BOARD
BUILD_DIR = COMPILED

CFILES = $(wildcard SOURCES/CORE/*.c) $(wildcard SOURCES/COMPONENTS/*.c)
HFILES = $(wildcard SOURCES/CORE/*.h) $(wildcard SOURCES/COMPONENTS/*.h)

DEVICE = stm32f103c8
OOCD_FILE = board/stm32f103c8t6

VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR) $(dir $(HFILES)))
OPENCM3_DIR = LIBRARIES/libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk