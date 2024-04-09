################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ADC_Prog.c \
../Buzzer_Prog.c \
../DIO_Prog.c \
../EXTINT_Prog.c \
../GIE_prog.c \
../LCD_Prog.c \
../LED_Prog.c \
../PushButton_Prog.c \
../Relay.c \
../app.c 

OBJS += \
./ADC_Prog.o \
./Buzzer_Prog.o \
./DIO_Prog.o \
./EXTINT_Prog.o \
./GIE_prog.o \
./LCD_Prog.o \
./LED_Prog.o \
./PushButton_Prog.o \
./Relay.o \
./app.o 

C_DEPS += \
./ADC_Prog.d \
./Buzzer_Prog.d \
./DIO_Prog.d \
./EXTINT_Prog.d \
./GIE_prog.d \
./LCD_Prog.d \
./LED_Prog.d \
./PushButton_Prog.d \
./Relay.d \
./app.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


