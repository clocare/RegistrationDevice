################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CRC_program.c \
../src/DIO_program.c \
../src/Display.c \
../src/FPEC_program.c \
../src/IWDG_program.c \
../src/LCD.c \
../src/RCC_program.c \
../src/Reader.c \
../src/SPI1.c \
../src/TIMER_config.c \
../src/TIMER_program.c \
../src/UART_program.c \
../src/main.c \
../src/mfrc522.c 

OBJS += \
./src/CRC_program.o \
./src/DIO_program.o \
./src/Display.o \
./src/FPEC_program.o \
./src/IWDG_program.o \
./src/LCD.o \
./src/RCC_program.o \
./src/Reader.o \
./src/SPI1.o \
./src/TIMER_config.o \
./src/TIMER_program.o \
./src/UART_program.o \
./src/main.o \
./src/mfrc522.o 

C_DEPS += \
./src/CRC_program.d \
./src/DIO_program.d \
./src/Display.d \
./src/FPEC_program.d \
./src/IWDG_program.d \
./src/LCD.d \
./src/RCC_program.d \
./src/Reader.d \
./src/SPI1.d \
./src/TIMER_config.d \
./src/TIMER_program.d \
./src/UART_program.d \
./src/main.d \
./src/mfrc522.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


