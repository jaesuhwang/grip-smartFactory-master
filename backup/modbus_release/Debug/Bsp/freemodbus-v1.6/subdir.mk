################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsp/freemodbus-v1.6/mb.c \
../Bsp/freemodbus-v1.6/mbascii.c \
../Bsp/freemodbus-v1.6/mbcrc.c \
../Bsp/freemodbus-v1.6/mbfunccoils.c \
../Bsp/freemodbus-v1.6/mbfuncdiag.c \
../Bsp/freemodbus-v1.6/mbfuncdisc.c \
../Bsp/freemodbus-v1.6/mbfuncholding.c \
../Bsp/freemodbus-v1.6/mbfuncinput.c \
../Bsp/freemodbus-v1.6/mbfuncother.c \
../Bsp/freemodbus-v1.6/mbrtu.c \
../Bsp/freemodbus-v1.6/mbtcp.c \
../Bsp/freemodbus-v1.6/mbutils.c \
../Bsp/freemodbus-v1.6/port.c \
../Bsp/freemodbus-v1.6/portevent.c \
../Bsp/freemodbus-v1.6/portserial.c \
../Bsp/freemodbus-v1.6/porttimer.c 

OBJS += \
./Bsp/freemodbus-v1.6/mb.o \
./Bsp/freemodbus-v1.6/mbascii.o \
./Bsp/freemodbus-v1.6/mbcrc.o \
./Bsp/freemodbus-v1.6/mbfunccoils.o \
./Bsp/freemodbus-v1.6/mbfuncdiag.o \
./Bsp/freemodbus-v1.6/mbfuncdisc.o \
./Bsp/freemodbus-v1.6/mbfuncholding.o \
./Bsp/freemodbus-v1.6/mbfuncinput.o \
./Bsp/freemodbus-v1.6/mbfuncother.o \
./Bsp/freemodbus-v1.6/mbrtu.o \
./Bsp/freemodbus-v1.6/mbtcp.o \
./Bsp/freemodbus-v1.6/mbutils.o \
./Bsp/freemodbus-v1.6/port.o \
./Bsp/freemodbus-v1.6/portevent.o \
./Bsp/freemodbus-v1.6/portserial.o \
./Bsp/freemodbus-v1.6/porttimer.o 

C_DEPS += \
./Bsp/freemodbus-v1.6/mb.d \
./Bsp/freemodbus-v1.6/mbascii.d \
./Bsp/freemodbus-v1.6/mbcrc.d \
./Bsp/freemodbus-v1.6/mbfunccoils.d \
./Bsp/freemodbus-v1.6/mbfuncdiag.d \
./Bsp/freemodbus-v1.6/mbfuncdisc.d \
./Bsp/freemodbus-v1.6/mbfuncholding.d \
./Bsp/freemodbus-v1.6/mbfuncinput.d \
./Bsp/freemodbus-v1.6/mbfuncother.d \
./Bsp/freemodbus-v1.6/mbrtu.d \
./Bsp/freemodbus-v1.6/mbtcp.d \
./Bsp/freemodbus-v1.6/mbutils.d \
./Bsp/freemodbus-v1.6/port.d \
./Bsp/freemodbus-v1.6/portevent.d \
./Bsp/freemodbus-v1.6/portserial.d \
./Bsp/freemodbus-v1.6/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
Bsp/freemodbus-v1.6/%.o Bsp/freemodbus-v1.6/%.su Bsp/freemodbus-v1.6/%.cyclo: ../Bsp/freemodbus-v1.6/%.c Bsp/freemodbus-v1.6/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I"C:/Dev_Work/UHF_GRIP_OLD/master_230425_modbus_remove/Bsp/freemodbus-v1.6" -I"C:/Dev_Work/UHF_GRIP_OLD/master_230425_modbus_remove/Bsp" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Bsp-2f-freemodbus-2d-v1-2e-6

clean-Bsp-2f-freemodbus-2d-v1-2e-6:
	-$(RM) ./Bsp/freemodbus-v1.6/mb.cyclo ./Bsp/freemodbus-v1.6/mb.d ./Bsp/freemodbus-v1.6/mb.o ./Bsp/freemodbus-v1.6/mb.su ./Bsp/freemodbus-v1.6/mbascii.cyclo ./Bsp/freemodbus-v1.6/mbascii.d ./Bsp/freemodbus-v1.6/mbascii.o ./Bsp/freemodbus-v1.6/mbascii.su ./Bsp/freemodbus-v1.6/mbcrc.cyclo ./Bsp/freemodbus-v1.6/mbcrc.d ./Bsp/freemodbus-v1.6/mbcrc.o ./Bsp/freemodbus-v1.6/mbcrc.su ./Bsp/freemodbus-v1.6/mbfunccoils.cyclo ./Bsp/freemodbus-v1.6/mbfunccoils.d ./Bsp/freemodbus-v1.6/mbfunccoils.o ./Bsp/freemodbus-v1.6/mbfunccoils.su ./Bsp/freemodbus-v1.6/mbfuncdiag.cyclo ./Bsp/freemodbus-v1.6/mbfuncdiag.d ./Bsp/freemodbus-v1.6/mbfuncdiag.o ./Bsp/freemodbus-v1.6/mbfuncdiag.su ./Bsp/freemodbus-v1.6/mbfuncdisc.cyclo ./Bsp/freemodbus-v1.6/mbfuncdisc.d ./Bsp/freemodbus-v1.6/mbfuncdisc.o ./Bsp/freemodbus-v1.6/mbfuncdisc.su ./Bsp/freemodbus-v1.6/mbfuncholding.cyclo ./Bsp/freemodbus-v1.6/mbfuncholding.d ./Bsp/freemodbus-v1.6/mbfuncholding.o ./Bsp/freemodbus-v1.6/mbfuncholding.su ./Bsp/freemodbus-v1.6/mbfuncinput.cyclo ./Bsp/freemodbus-v1.6/mbfuncinput.d ./Bsp/freemodbus-v1.6/mbfuncinput.o ./Bsp/freemodbus-v1.6/mbfuncinput.su ./Bsp/freemodbus-v1.6/mbfuncother.cyclo ./Bsp/freemodbus-v1.6/mbfuncother.d ./Bsp/freemodbus-v1.6/mbfuncother.o ./Bsp/freemodbus-v1.6/mbfuncother.su ./Bsp/freemodbus-v1.6/mbrtu.cyclo ./Bsp/freemodbus-v1.6/mbrtu.d ./Bsp/freemodbus-v1.6/mbrtu.o ./Bsp/freemodbus-v1.6/mbrtu.su ./Bsp/freemodbus-v1.6/mbtcp.cyclo ./Bsp/freemodbus-v1.6/mbtcp.d ./Bsp/freemodbus-v1.6/mbtcp.o ./Bsp/freemodbus-v1.6/mbtcp.su ./Bsp/freemodbus-v1.6/mbutils.cyclo ./Bsp/freemodbus-v1.6/mbutils.d ./Bsp/freemodbus-v1.6/mbutils.o ./Bsp/freemodbus-v1.6/mbutils.su ./Bsp/freemodbus-v1.6/port.cyclo ./Bsp/freemodbus-v1.6/port.d ./Bsp/freemodbus-v1.6/port.o ./Bsp/freemodbus-v1.6/port.su ./Bsp/freemodbus-v1.6/portevent.cyclo ./Bsp/freemodbus-v1.6/portevent.d ./Bsp/freemodbus-v1.6/portevent.o ./Bsp/freemodbus-v1.6/portevent.su ./Bsp/freemodbus-v1.6/portserial.cyclo ./Bsp/freemodbus-v1.6/portserial.d ./Bsp/freemodbus-v1.6/portserial.o ./Bsp/freemodbus-v1.6/portserial.su ./Bsp/freemodbus-v1.6/porttimer.cyclo ./Bsp/freemodbus-v1.6/porttimer.d ./Bsp/freemodbus-v1.6/porttimer.o ./Bsp/freemodbus-v1.6/porttimer.su

.PHONY: clean-Bsp-2f-freemodbus-2d-v1-2e-6

