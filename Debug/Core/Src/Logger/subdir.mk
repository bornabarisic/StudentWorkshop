################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Logger/log.c 

OBJS += \
./Core/Src/Logger/log.o 

C_DEPS += \
./Core/Src/Logger/log.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Logger/%.o Core/Src/Logger/%.su Core/Src/Logger/%.cyclo: ../Core/Src/Logger/%.c Core/Src/Logger/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Inc/Flash -I../Core/Inc/Logger -I../Core/Inc/Measurement/AHT20 -I../Core/Inc/Measurement/Potentiometer -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Logger

clean-Core-2f-Src-2f-Logger:
	-$(RM) ./Core/Src/Logger/log.cyclo ./Core/Src/Logger/log.d ./Core/Src/Logger/log.o ./Core/Src/Logger/log.su

.PHONY: clean-Core-2f-Src-2f-Logger

