################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Display/lcd_driver.c \
../Core/Src/Display/lcd_i2c.c \
../Core/Src/Display/lcd_interface.c 

OBJS += \
./Core/Src/Display/lcd_driver.o \
./Core/Src/Display/lcd_i2c.o \
./Core/Src/Display/lcd_interface.o 

C_DEPS += \
./Core/Src/Display/lcd_driver.d \
./Core/Src/Display/lcd_i2c.d \
./Core/Src/Display/lcd_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Display/%.o Core/Src/Display/%.su Core/Src/Display/%.cyclo: ../Core/Src/Display/%.c Core/Src/Display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Inc/Flash -I../Core/Inc/Logger -I../Core/Inc/Measurement/AHT20 -I../Core/Inc/Measurement/Potentiometer -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Display

clean-Core-2f-Src-2f-Display:
	-$(RM) ./Core/Src/Display/lcd_driver.cyclo ./Core/Src/Display/lcd_driver.d ./Core/Src/Display/lcd_driver.o ./Core/Src/Display/lcd_driver.su ./Core/Src/Display/lcd_i2c.cyclo ./Core/Src/Display/lcd_i2c.d ./Core/Src/Display/lcd_i2c.o ./Core/Src/Display/lcd_i2c.su ./Core/Src/Display/lcd_interface.cyclo ./Core/Src/Display/lcd_interface.d ./Core/Src/Display/lcd_interface.o ./Core/Src/Display/lcd_interface.su

.PHONY: clean-Core-2f-Src-2f-Display

