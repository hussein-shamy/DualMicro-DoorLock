################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../2_HAL/2_KEYPAD/keypad.c 

OBJS += \
./2_HAL/2_KEYPAD/keypad.o 

C_DEPS += \
./2_HAL/2_KEYPAD/keypad.d 


# Each subdirectory must supply rules for building sources it contributes
2_HAL/2_KEYPAD/%.o: ../2_HAL/2_KEYPAD/%.c 2_HAL/2_KEYPAD/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


