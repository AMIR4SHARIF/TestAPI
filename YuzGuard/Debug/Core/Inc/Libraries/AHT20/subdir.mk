################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Libraries/AHT20/AHT20.c 

OBJS += \
./Core/Inc/Libraries/AHT20/AHT20.o 

C_DEPS += \
./Core/Inc/Libraries/AHT20/AHT20.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Libraries/AHT20/%.o Core/Inc/Libraries/AHT20/%.su Core/Inc/Libraries/AHT20/%.cyclo: ../Core/Inc/Libraries/AHT20/%.c Core/Inc/Libraries/AHT20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/AHT20" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/cJSON" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/DNS" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/SNTP" -I"P:/YuzGuard/1_MicroController/YuzGuard/Core/Inc/Libraries/W5500" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Libraries-2f-AHT20

clean-Core-2f-Inc-2f-Libraries-2f-AHT20:
	-$(RM) ./Core/Inc/Libraries/AHT20/AHT20.cyclo ./Core/Inc/Libraries/AHT20/AHT20.d ./Core/Inc/Libraries/AHT20/AHT20.o ./Core/Inc/Libraries/AHT20/AHT20.su

.PHONY: clean-Core-2f-Inc-2f-Libraries-2f-AHT20

