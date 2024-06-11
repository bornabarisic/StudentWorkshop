################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Flash/flash.c 

OBJS += \
./Core/Src/Flash/flash.o 

C_DEPS += \
./Core/Src/Flash/flash.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Flash/%.o Core/Src/Flash/%.su Core/Src/Flash/%.cyclo: ../Core/Src/Flash/%.c Core/Src/Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Inc/Flash -I../Core/Inc/Logger -I../Core/Inc/Measurement/AHT20 -I../Core/Inc/Measurement/Potentiometer -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Flash

clean-Core-2f-Src-2f-Flash:
	-$(RM) ./Core/Src/Flash/flash.cyclo ./Core/Src/Flash/flash.d ./Core/Src/Flash/flash.o ./Core/Src/Flash/flash.su

.PHONY: clean-Core-2f-Src-2f-Flash

