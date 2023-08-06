PROJECT = STM32_DEV_BOARD
BUILD_DIR = RESOURCES

CFILES = $(wildcard SOURCES/CORE/*.c)
CFILES += $(wildcard SOURCES/COMPONENTS/*.c)

DEVICE = stm32f103c8
OOCD_FILE = board/stm32f103c8t6

VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))

OPENCM3_DIR = ../LIBRARIES/libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

$(PROJECT).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$@ $(OBJS) $(LDLIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/$< $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/*.o