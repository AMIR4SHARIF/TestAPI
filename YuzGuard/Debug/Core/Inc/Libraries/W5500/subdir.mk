################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Libraries/W5500/socket.c \
../Core/Inc/Libraries/W5500/w5500.c \
../Core/Inc/Libraries/W5500/wizchip_conf.c 

OBJS += \
./Core/Inc/Libraries/W5500/socket.o \
./Core/Inc/Libraries/W5500/w5500.o \
./Core/Inc/Libraries/W5500/wizchip_conf.o 

C_DEPS += \
./Core/Inc/Libraries/W5500/socket.d \
./Core/Inc/Libraries/W5500/w5500.d \
./Core/Inc/Libraries/W5500/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Libraries/W5500/%.o Core/Inc/Libraries/W5500/%.su Core/Inc/Libraries/W5500/%.cyclo: ../Core/Inc/Libraries/W5500/%.c Core/Inc/Libraries/W5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/AHT20" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/cJSON" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/DNS" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/SNTP" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/W5500" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Libraries-2f-W5500

clean-Core-2f-Inc-2f-Libraries-2f-W5500:
	-$(RM) ./Core/Inc/Libraries/W5500/socket.cyclo ./Core/Inc/Libraries/W5500/socket.d ./Core/Inc/Libraries/W5500/socket.o ./Core/Inc/Libraries/W5500/socket.su ./Core/Inc/Libraries/W5500/w5500.cyclo ./Core/Inc/Libraries/W5500/w5500.d ./Core/Inc/Libraries/W5500/w5500.o ./Core/Inc/Libraries/W5500/w5500.su ./Core/Inc/Libraries/W5500/wizchip_conf.cyclo ./Core/Inc/Libraries/W5500/wizchip_conf.d ./Core/Inc/Libraries/W5500/wizchip_conf.o ./Core/Inc/Libraries/W5500/wizchip_conf.su

.PHONY: clean-Core-2f-Inc-2f-Libraries-2f-W5500
