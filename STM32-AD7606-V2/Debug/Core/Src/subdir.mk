################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ad7606.c \
../Core/Src/main.c \
../Core/Src/no_os_alloc.c \
../Core/Src/no_os_crc16.c \
../Core/Src/no_os_crc24.c \
../Core/Src/no_os_crc8.c \
../Core/Src/no_os_gpio.c \
../Core/Src/no_os_mutex.c \
../Core/Src/no_os_spi.c \
../Core/Src/no_os_timer.c \
../Core/Src/no_os_util.c \
../Core/Src/stm32_delay.c \
../Core/Src/stm32_gpio.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/ad7606.o \
./Core/Src/main.o \
./Core/Src/no_os_alloc.o \
./Core/Src/no_os_crc16.o \
./Core/Src/no_os_crc24.o \
./Core/Src/no_os_crc8.o \
./Core/Src/no_os_gpio.o \
./Core/Src/no_os_mutex.o \
./Core/Src/no_os_spi.o \
./Core/Src/no_os_timer.o \
./Core/Src/no_os_util.o \
./Core/Src/stm32_delay.o \
./Core/Src/stm32_gpio.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/ad7606.d \
./Core/Src/main.d \
./Core/Src/no_os_alloc.d \
./Core/Src/no_os_crc16.d \
./Core/Src/no_os_crc24.d \
./Core/Src/no_os_crc8.d \
./Core/Src/no_os_gpio.d \
./Core/Src/no_os_mutex.d \
./Core/Src/no_os_spi.d \
./Core/Src/no_os_timer.d \
./Core/Src/no_os_util.d \
./Core/Src/stm32_delay.d \
./Core/Src/stm32_gpio.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ad7606.cyclo ./Core/Src/ad7606.d ./Core/Src/ad7606.o ./Core/Src/ad7606.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/no_os_alloc.cyclo ./Core/Src/no_os_alloc.d ./Core/Src/no_os_alloc.o ./Core/Src/no_os_alloc.su ./Core/Src/no_os_crc16.cyclo ./Core/Src/no_os_crc16.d ./Core/Src/no_os_crc16.o ./Core/Src/no_os_crc16.su ./Core/Src/no_os_crc24.cyclo ./Core/Src/no_os_crc24.d ./Core/Src/no_os_crc24.o ./Core/Src/no_os_crc24.su ./Core/Src/no_os_crc8.cyclo ./Core/Src/no_os_crc8.d ./Core/Src/no_os_crc8.o ./Core/Src/no_os_crc8.su ./Core/Src/no_os_gpio.cyclo ./Core/Src/no_os_gpio.d ./Core/Src/no_os_gpio.o ./Core/Src/no_os_gpio.su ./Core/Src/no_os_mutex.cyclo ./Core/Src/no_os_mutex.d ./Core/Src/no_os_mutex.o ./Core/Src/no_os_mutex.su ./Core/Src/no_os_spi.cyclo ./Core/Src/no_os_spi.d ./Core/Src/no_os_spi.o ./Core/Src/no_os_spi.su ./Core/Src/no_os_timer.cyclo ./Core/Src/no_os_timer.d ./Core/Src/no_os_timer.o ./Core/Src/no_os_timer.su ./Core/Src/no_os_util.cyclo ./Core/Src/no_os_util.d ./Core/Src/no_os_util.o ./Core/Src/no_os_util.su ./Core/Src/stm32_delay.cyclo ./Core/Src/stm32_delay.d ./Core/Src/stm32_delay.o ./Core/Src/stm32_delay.su ./Core/Src/stm32_gpio.cyclo ./Core/Src/stm32_gpio.d ./Core/Src/stm32_gpio.o ./Core/Src/stm32_gpio.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

