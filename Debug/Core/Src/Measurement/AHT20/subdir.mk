################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Measurement/AHT20/aht20_driver.c \
../Core/Src/Measurement/AHT20/aht20_i2c.c \
../Core/Src/Measurement/AHT20/aht20_interface.c 

OBJS += \
./Core/Src/Measurement/AHT20/aht20_driver.o \
./Core/Src/Measurement/AHT20/aht20_i2c.o \
./Core/Src/Measurement/AHT20/aht20_interface.o 

C_DEPS += \
./Core/Src/Measurement/AHT20/aht20_driver.d \
./Core/Src/Measurement/AHT20/aht20_i2c.d \
./Core/Src/Measurement/AHT20/aht20_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Measurement/AHT20/%.o Core/Src/Measurement/AHT20/%.su Core/Src/Measurement/AHT20/%.cyclo: ../Core/Src/Measurement/AHT20/%.c Core/Src/Measurement/AHT20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Inc/Flash -I../Core/Inc/Logger -I../Core/Inc/Measurement/AHT20 -I../Core/Inc/Measurement/Potentiometer -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Measurement-2f-AHT20

clean-Core-2f-Src-2f-Measurement-2f-AHT20:
	-$(RM) ./Core/Src/Measurement/AHT20/aht20_driver.cyclo ./Core/Src/Measurement/AHT20/aht20_driver.d ./Core/Src/Measurement/AHT20/aht20_driver.o ./Core/Src/Measurement/AHT20/aht20_driver.su ./Core/Src/Measurement/AHT20/aht20_i2c.cyclo ./Core/Src/Measurement/AHT20/aht20_i2c.d ./Core/Src/Measurement/AHT20/aht20_i2c.o ./Core/Src/Measurement/AHT20/aht20_i2c.su ./Core/Src/Measurement/AHT20/aht20_interface.cyclo ./Core/Src/Measurement/AHT20/aht20_interface.d ./Core/Src/Measurement/AHT20/aht20_interface.o ./Core/Src/Measurement/AHT20/aht20_interface.su

.PHONY: clean-Core-2f-Src-2f-Measurement-2f-AHT20

