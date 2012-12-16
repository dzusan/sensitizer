################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include STM32F10x_StdPeriph_Driver/src/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
sensitizer_probe.hex \

SECONDARY_LIST += \
sensitizer_probe.lst \

SECONDARY_SIZE += \
sensitizer_probe.siz \


# All Target
all: sensitizer_probe.elf secondary-outputs

# Tool invocations
sensitizer_probe.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Linux GCC C Linker'
	arm-none-eabi-gcc -T"/home/dzusan/workspace/sensitizer_probe/stm32_flash.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,sensitizer_probe.map -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "sensitizer_probe.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

sensitizer_probe.hex: sensitizer_probe.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex sensitizer_probe.elf  "sensitizer_probe.hex"
	@echo 'Finished building: $@'
	@echo ' '

sensitizer_probe.lst: sensitizer_probe.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Create Listing'
	arm-none-eabi-objdump -h -S sensitizer_probe.elf > "sensitizer_probe.lst"
	@echo 'Finished building: $@'
	@echo ' '

sensitizer_probe.siz: sensitizer_probe.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Print Size'
	arm-none-eabi-size  --format=berkeley sensitizer_probe.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) sensitizer_probe.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
