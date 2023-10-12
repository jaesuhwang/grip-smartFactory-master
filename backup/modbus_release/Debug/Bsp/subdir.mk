################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsp/buzzer.c \
../Bsp/cc1120.c \
../Bsp/detect.c \
../Bsp/flash.c \
../Bsp/global.c \
../Bsp/led.c \
../Bsp/max17048.c \
../Bsp/packet.c \
../Bsp/ringbuf.c \
../Bsp/send_to_pc.c \
../Bsp/softtimer.c \
../Bsp/tdma_c.c \
../Bsp/tdma_mgt.c 

OBJS += \
./Bsp/buzzer.o \
./Bsp/cc1120.o \
./Bsp/detect.o \
./Bsp/flash.o \
./Bsp/global.o \
./Bsp/led.o \
./Bsp/max17048.o \
./Bsp/packet.o \
./Bsp/ringbuf.o \
./Bsp/send_to_pc.o \
./Bsp/softtimer.o \
./Bsp/tdma_c.o \
./Bsp/tdma_mgt.o 

C_DEPS += \
./Bsp/buzzer.d \
./Bsp/cc1120.d \
./Bsp/detect.d \
./Bsp/flash.d \
./Bsp/global.d \
./Bsp/led.d \
./Bsp/max17048.d \
./Bsp/packet.d \
./Bsp/ringbuf.d \
./Bsp/send_to_pc.d \
./Bsp/softtimer.d \
./Bsp/tdma_c.d \
./Bsp/tdma_mgt.d 


# Each subdirectory must supply rules for building sources it contributes
Bsp/%.o Bsp/%.su Bsp/%.cyclo: ../Bsp/%.c Bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I"C:/Dev_Work/UHF_GRIP_OLD/master_230425_modbus_remove/Bsp/freemodbus-v1.6" -I"C:/Dev_Work/UHF_GRIP_OLD/master_230425_modbus_remove/Bsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Bsp

clean-Bsp:
	-$(RM) ./Bsp/buzzer.cyclo ./Bsp/buzzer.d ./Bsp/buzzer.o ./Bsp/buzzer.su ./Bsp/cc1120.cyclo ./Bsp/cc1120.d ./Bsp/cc1120.o ./Bsp/cc1120.su ./Bsp/detect.cyclo ./Bsp/detect.d ./Bsp/detect.o ./Bsp/detect.su ./Bsp/flash.cyclo ./Bsp/flash.d ./Bsp/flash.o ./Bsp/flash.su ./Bsp/global.cyclo ./Bsp/global.d ./Bsp/global.o ./Bsp/global.su ./Bsp/led.cyclo ./Bsp/led.d ./Bsp/led.o ./Bsp/led.su ./Bsp/max17048.cyclo ./Bsp/max17048.d ./Bsp/max17048.o ./Bsp/max17048.su ./Bsp/packet.cyclo ./Bsp/packet.d ./Bsp/packet.o ./Bsp/packet.su ./Bsp/ringbuf.cyclo ./Bsp/ringbuf.d ./Bsp/ringbuf.o ./Bsp/ringbuf.su ./Bsp/send_to_pc.cyclo ./Bsp/send_to_pc.d ./Bsp/send_to_pc.o ./Bsp/send_to_pc.su ./Bsp/softtimer.cyclo ./Bsp/softtimer.d ./Bsp/softtimer.o ./Bsp/softtimer.su ./Bsp/tdma_c.cyclo ./Bsp/tdma_c.d ./Bsp/tdma_c.o ./Bsp/tdma_c.su ./Bsp/tdma_mgt.cyclo ./Bsp/tdma_mgt.d ./Bsp/tdma_mgt.o ./Bsp/tdma_mgt.su

.PHONY: clean-Bsp

