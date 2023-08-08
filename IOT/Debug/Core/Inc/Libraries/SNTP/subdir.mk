################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Libraries/SNTP/sntp.c 

OBJS += \
./Core/Inc/Libraries/SNTP/sntp.o 

C_DEPS += \
./Core/Inc/Libraries/SNTP/sntp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Libraries/SNTP/%.o Core/Inc/Libraries/SNTP/%.su Core/Inc/Libraries/SNTP/%.cyclo: ../Core/Inc/Libraries/SNTP/%.c Core/Inc/Libraries/SNTP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/AHT20" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/cJSON" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/DNS" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/SNTP" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/W5500" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/RTC_CONF" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/HTTP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Libraries-2f-SNTP

clean-Core-2f-Inc-2f-Libraries-2f-SNTP:
	-$(RM) ./Core/Inc/Libraries/SNTP/sntp.cyclo ./Core/Inc/Libraries/SNTP/sntp.d ./Core/Inc/Libraries/SNTP/sntp.o ./Core/Inc/Libraries/SNTP/sntp.su

.PHONY: clean-Core-2f-Inc-2f-Libraries-2f-SNTP

