########## Project Configuration ##########

TARGET = NHNS
BUILD_DIR = build
APPLICATION = NHNS-main
DEVICE = STM32F207xx
LINKER = STM32F207ZGTX_FLASH.ld

########## Compiler Configuration ##########

CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

########## Header Files and Includes ##########

# Include all header files in the Application directory
APPLICATION_DIR = Application
APPLICATION_HEADERS = $(wildcard $(APPLICATION_DIR)/$(APPLICATION)/**/*.h)
APPLICATION_INCLUDE = $(patsubst %/,%,$(dir $(APPLICATION_HEADERS)))

# Include all header files in the Board directory
BOARD_DIR = Board
BOARD_HEADERS = $(wildcard $(BOARD_DIR)/$(BOARD)/*.h)
BOARD_INCLUDE = $(patsubst %/,%,$(dir $(BOARD_HEADERS)))

# Include all header files in the Device directory
DEVICE_DIR = Device
DEVICE_HEADERS = $(wildcard $(DEVICE_DIR)/$(DEVICE)/*.h)
DEVICE_INCLUDE = $(patsubst %/,%,$(dir $(DEVICE_HEADERS)))

# Include all header files in the Driver directory
DRIVER_DIR = Driver
DRIVER_HEADERS = $(wildcard $(DRIVER_DIR)/**/*.h)
DRIVER_INCLUDE = $(patsubst %/,%,$(dir $(DRIVER_HEADERS)))

# Include all header files in the Peripheral directory
PERIPHERAL_DIR = Peripheral
PERIPHERAL_HEADERS = $(wildcard $(PERIPHERAL_DIR)/**/*.h)
PERIPHERAL_INCLUDE = $(patsubst %/,%,$(dir $(PERIPHERAL_HEADERS)))

# Include all header files in the Services directory
SERVICES_DIR = Service
SERVICES_HEADERS = $(wildcard $(SERVICES_DIR)/**/*.h)
SERVICES_INCLUDE = $(patsubst %/,%,$(dir $(SERVICES_HEADERS)))

# List of all include directories
INCLUDES = $(APPLICATION_INCLUDE) $(BOARD_INCLUDE) $(DEVICE_INCLUDE) $(DRIVER_INCLUDE) $(PERIPHERAL_INCLUDE) $(SERVICES_INCLUDE) $(BUILD_DIR)

########## Libraries ##########

CMSIS = Library/CMSIS
HAL = Library/HAL/STM32F2xx_HAL_Driver
FREERTOS = Library/FreeRTOS

########## Compiler Flags ##########

CFLAGS  = -g -O3 -Wall
CFLAGS += -mlittle-endian -mcpu=cortex-m3 -mthumb
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
CFLAGS += "-D$(DEVICE)"
CFLAGS += $(addprefix -I,$(INCLUDES)) -IInclude 
CFLAGS += -I$(CMSIS)/Include -I$(CMSIS)/Device/ST/STM32F2xx/Include 
CFLAGS += -I$(HAL)/Inc 
CFLAGS += -I$(FREERTOS)/include -I$(FREERTOS)/portable/GCC/ARM_CM3
CFLAGS += -DDEBUG -DFW_DEBUG

########## Application Source Files ##########

APPLICATION_SRCS = \
		$(APPLICATION_DIR)/$(APPLICATION)/main/main.c			\

BOARD_SRCS = \

DEVICE_SRCS = \
		$(DEVICE_DIR)/$(DEVICE)/stm32f2xx_it.c		\
		$(DEVICE_DIR)/$(DEVICE)/syscalls.c			\
		$(DEVICE_DIR)/$(DEVICE)/system_stm32f2xx.c	\

DRIVER_SRCS = \

PERIPHERAL_SRCS = \
		
SERVICES_SRCS = \

########## Library Source Files ##########

HAL_SRCS = \
	$(HAL)/Src/stm32f2xx_hal.c				\

FREERTOS_SRCS =	\
	$(FREERTOS)/tasks.c							\
	$(FREERTOS)/queue.c							\
	$(FREERTOS)/list.c							\
	$(FREERTOS)/timers.c						\
	$(FREERTOS)/event_groups.c					\
	$(FREERTOS)/portable/MemMang/heap_4.c		\
	$(FREERTOS)/portable/GCC/ARM_CM3/port.c	\

########## Start-up & Linker ##########

# Location of startup source file
STARTUP_SRCS = $(DEVICE_DIR)/$(DEVICE)/startup_stm32f207zgtx.s

# Location of the linker scripts
LDSCRIPT = $(DEVICE_DIR)/$(DEVICE)/Linker/$(LINKER)

########## Compiler Source Files ##########

SRCS += $(APPLICATION_SRCS)
SRCS += $(BOARD_SRCS)
SRCS += $(DEVICE_SRCS)
SRCS += $(DRIVER_SRCS)
SRCS += $(PERIPHERAL_SRCS)
SRCS += $(SERVICES_SRCS)
SRCS += $(HAL_SRCS)
SRCS += $(FREERTOS_SRCS)
SRCS += $(STARTUP_SRCS)

########## Makefile Commands ##########

.PHONY: proj clean FORCE

all: $(BUILD_DIR) $(BUILD_DIR)/build_stamp.h proj

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Generate build_stamp header
$(BUILD_DIR)/build_stamp.h: FORCE
	@echo "Generating build_stamp.h"
	@mkdir -p $(BUILD_DIR)
	@echo "#ifndef BUILDSTAMP_H" > $(BUILD_DIR)/build_stamp.h
	@echo "#define BUILDSTAMP_H" >> $(BUILD_DIR)/build_stamp.h
	@echo "" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define PRJ_NAME \"$(TARGET)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define PRJ_MAKE_TIME \"$(shell date +'%Y-%m-%d %H:%M:%S')\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define PRJ_GIT_USER_NAME \"$(shell git config user.name)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define PRJ_GIT_CURR_BRANCH \"$(shell git rev-parse --abbrev-ref HEAD)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define PRJ_GIT_COMMIT_TIME \"$(shell git log -1 --format=%cd --date=format:'%Y-%m-%d %H:%M:%S')\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define PRJ_GIT_HASH \"$(shell git describe --tags --always --abbrev=60 --dirty)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define APPLICATION_NAME \"$(APPLICATION)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define FW_VERSION \"$(FW_VERSION)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define BOARD_NAME \"$(BOARD)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "#define BUILD_TYPE \"$(BUILD_TYPE)\"" >> $(BUILD_DIR)/build_stamp.h
	@echo "" >> $(BUILD_DIR)/build_stamp.h
	@echo "#endif // BUILDSTAMP_H" >> $(BUILD_DIR)/build_stamp.h

# Compile the project
proj: $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/build_stamp.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(TARGET).elf: $(SRCS) $(BUILD_DIR)/build_stamp.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ -T$(LDSCRIPT) --specs=nano.specs -lc -lnosys
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(OBJDUMP) -St $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).lst
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

clean:
	rm -rf $(BUILD_DIR)/*

flash:
	STM32_Programmer_CLI.exe -c port=swd -w build/$(TARGET).bin 0x08000000 -Rst

# Dummy target to force rebuilds
FORCE: