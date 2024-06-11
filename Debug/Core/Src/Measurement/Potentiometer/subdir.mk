################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Measurement/Potentiometer/adc_if.c 

OBJS += \
./Core/Src/Measurement/Potentiometer/adc_if.o 

C_DEPS += \
./Core/Src/Measurement/Potentiometer/adc_if.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Measurement/Potentiometer/%.o Core/Src/Measurement/Potentiometer/%.su Core/Src/Measurement/Potentiometer/%.cyclo: ../Core/Src/Measurement/Potentiometer/%.c Core/Src/Measurement/Potentiometer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Inc/Flash -I../Core/Inc/Logger -I../Core/Inc/Measurement/AHT20 -I../Core/Inc/Measurement/Potentiometer -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Measurement-2f-Potentiometer

clean-Core-2f-Src-2f-Measurement-2f-Potentiometer:
	-$(RM) ./Core/Src/Measurement/Potentiometer/adc_if.cyclo ./Core/Src/Measurement/Potentiometer/adc_if.d ./Core/Src/Measurement/Potentiometer/adc_if.o ./Core/Src/Measurement/Potentiometer/adc_if.su

.PHONY: clean-Core-2f-Src-2f-Measurement-2f-Potentiometer

