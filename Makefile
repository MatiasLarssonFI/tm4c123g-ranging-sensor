# TARGET: name of the output file
TARGET = ranging_sensor_app
# MCU: part number to build for
MCU = TM4C123GH6PM
# INCLUDES: list of includes, by default, use Includes directory
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR :=  $(dir $(MAKEFILE_PATH))
INCLUDES = -I$(HOME)/embedded/cmsis/Include -I$(HOME)/embedded/include/tm4c -I$(MAKEFILE_DIR)src
# OUTDIR: directory to use for output
OUTDIR = build

# LD_SCRIPT: linker script
LD_SCRIPT = $(MCU).lds

FLOAT_ABI = softfp

# flags
CFLAGS = -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI)
CFLAGS +=-Os -ffunction-sections -fdata-sections -MD -Wall
CFLAGS += -DPART_$(MCU) -c $(INCLUDES)
CFLAGS += -DTARGET_IS_TM4C123_RB2 -D__FPU_PRESENT
CXXFLAGS := $(CFLAGS) -std=c++2a -fno-rtti -fno-exceptions -fno-threadsafe-statics
CFLAGS += -std=c99
LDFLAGS = -T $(LD_SCRIPT) --entry Reset_Handler -specs=nosys.specs
LDFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=$(FLOAT_ABI) -Wl,--gc-sections -Wl,-Map=ranging-sensor.map


# binaries

CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
RM      = rm -rf
MKDIR	= mkdir -p


# list of object files, placed in the build directory regardless of source path
OBJECTS := $(addprefix $(OUTDIR)/,$(notdir $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(wildcard src/*.cpp)))))
OBJECTS := $(OBJECTS) $(addprefix $(OUTDIR)/bsp/,$(notdir $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(wildcard src/bsp/*.cpp)))))

# default: build bin
all: $(OUTDIR)/$(TARGET).bin

$(OUTDIR)/%.o: src/%.cpp | $(OUTDIR)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	
$(OUTDIR)/bsp/%.o: src/bsp/%.cpp | $(OUTDIR)/bsp
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OUTDIR)/ranging_sensor_app.elf: $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OUTDIR)/ranging_sensor_app.bin: $(OUTDIR)/ranging_sensor_app.elf
	$(OBJCOPY) -O binary $< $@

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)

$(OUTDIR)/bsp:
	$(MKDIR) $(OUTDIR)/bsp

clean:
	-$(RM) $(OUTDIR)/*

.PHONY: all clean
