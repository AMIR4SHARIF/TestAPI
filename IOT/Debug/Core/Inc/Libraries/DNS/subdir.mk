################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Libraries/DNS/dns.c 

OBJS += \
./Core/Inc/Libraries/DNS/dns.o 

C_DEPS += \
./Core/Inc/Libraries/DNS/dns.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Libraries/DNS/%.o Core/Inc/Libraries/DNS/%.su Core/Inc/Libraries/DNS/%.cyclo: ../Core/Inc/Libraries/DNS/%.c Core/Inc/Libraries/DNS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/AHT20" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/cJSON" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/DNS" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/SNTP" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/W5500" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/RTC_CONF" -I"P:/YuzGuard/1_MicroController/IOT/Core/Inc/Libraries/HTTP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Libraries-2f-DNS

clean-Core-2f-Inc-2f-Libraries-2f-DNS:
	-$(RM) ./Core/Inc/Libraries/DNS/dns.cyclo ./Core/Inc/Libraries/DNS/dns.d ./Core/Inc/Libraries/DNS/dns.o ./Core/Inc/Libraries/DNS/dns.su

.PHONY: clean-Core-2f-Inc-2f-Libraries-2f-DNS

