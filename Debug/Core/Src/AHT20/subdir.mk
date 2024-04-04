################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AHT20/aht20_driver.c \
../Core/Src/AHT20/aht20_i2c.c \
../Core/Src/AHT20/aht20_interface.c 

OBJS += \
./Core/Src/AHT20/aht20_driver.o \
./Core/Src/AHT20/aht20_i2c.o \
./Core/Src/AHT20/aht20_interface.o 

C_DEPS += \
./Core/Src/AHT20/aht20_driver.d \
./Core/Src/AHT20/aht20_i2c.d \
./Core/Src/AHT20/aht20_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/AHT20/%.o Core/Src/AHT20/%.su Core/Src/AHT20/%.cyclo: ../Core/Src/AHT20/%.c Core/Src/AHT20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-AHT20

clean-Core-2f-Src-2f-AHT20:
	-$(RM) ./Core/Src/AHT20/aht20_driver.cyclo ./Core/Src/AHT20/aht20_driver.d ./Core/Src/AHT20/aht20_driver.o ./Core/Src/AHT20/aht20_driver.su ./Core/Src/AHT20/aht20_i2c.cyclo ./Core/Src/AHT20/aht20_i2c.d ./Core/Src/AHT20/aht20_i2c.o ./Core/Src/AHT20/aht20_i2c.su ./Core/Src/AHT20/aht20_interface.cyclo ./Core/Src/AHT20/aht20_interface.d ./Core/Src/AHT20/aht20_interface.o ./Core/Src/AHT20/aht20_interface.su

.PHONY: clean-Core-2f-Src-2f-AHT20

