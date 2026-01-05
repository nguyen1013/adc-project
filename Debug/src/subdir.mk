################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/startup_stm32l152xe.s 

C_SRCS += \
../src/main.c \
../src/spi.c \
../src/syscalls.c \
../src/system_stm32l1xx.c \
../src/timer.c \
../src/tiny_printf.c \
../src/usart.c 

OBJS += \
./src/main.o \
./src/spi.o \
./src/startup_stm32l152xe.o \
./src/syscalls.o \
./src/system_stm32l1xx.o \
./src/timer.o \
./src/tiny_printf.o \
./src/usart.o 

S_DEPS += \
./src/startup_stm32l152xe.d 

C_DEPS += \
./src/main.d \
./src/spi.d \
./src/syscalls.d \
./src/system_stm32l1xx.d \
./src/timer.d \
./src/tiny_printf.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32L152xE -c -I../src -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/STM32L1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
src/%.o: ../src/%.s src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DSTM32L152xE -c -I../src -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/STM32L1xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-src

clean-src:
	-$(RM) ./src/main.cyclo ./src/main.d ./src/main.o ./src/main.su ./src/spi.cyclo ./src/spi.d ./src/spi.o ./src/spi.su ./src/startup_stm32l152xe.d ./src/startup_stm32l152xe.o ./src/syscalls.cyclo ./src/syscalls.d ./src/syscalls.o ./src/syscalls.su ./src/system_stm32l1xx.cyclo ./src/system_stm32l1xx.d ./src/system_stm32l1xx.o ./src/system_stm32l1xx.su ./src/timer.cyclo ./src/timer.d ./src/timer.o ./src/timer.su ./src/tiny_printf.cyclo ./src/tiny_printf.d ./src/tiny_printf.o ./src/tiny_printf.su ./src/usart.cyclo ./src/usart.d ./src/usart.o ./src/usart.su

.PHONY: clean-src

