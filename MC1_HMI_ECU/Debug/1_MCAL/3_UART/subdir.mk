################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1_MCAL/3_UART/uart.c 

OBJS += \
./1_MCAL/3_UART/uart.o 

C_DEPS += \
./1_MCAL/3_UART/uart.d 


# Each subdirectory must supply rules for building sources it contributes
1_MCAL/3_UART/%.o: ../1_MCAL/3_UART/%.c 1_MCAL/3_UART/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


