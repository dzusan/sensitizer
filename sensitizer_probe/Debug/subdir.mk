################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../calculations.c \
../main.c \
../periph_setup.c \
../utils.c 

OBJS += \
./calculations.o \
./main.o \
./periph_setup.o \
./utils.o 

C_DEPS += \
./calculations.d \
./main.d \
./periph_setup.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/dzusan/workspace/sensitizer_probe/CMSIS" -I"/home/dzusan/workspace/sensitizer_probe" -I"/home/dzusan/workspace/sensitizer_probe/STM32F10x_StdPeriph_Driver/inc" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


