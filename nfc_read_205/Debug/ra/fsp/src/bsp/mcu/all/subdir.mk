################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/mcu/all/bsp_clocks.c \
../ra/fsp/src/bsp/mcu/all/bsp_common.c \
../ra/fsp/src/bsp/mcu/all/bsp_delay.c \
../ra/fsp/src/bsp/mcu/all/bsp_group_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_guard.c \
../ra/fsp/src/bsp/mcu/all/bsp_io.c \
../ra/fsp/src/bsp/mcu/all/bsp_ipc.c \
../ra/fsp/src/bsp/mcu/all/bsp_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_macl.c \
../ra/fsp/src/bsp/mcu/all/bsp_ospi_b.c \
../ra/fsp/src/bsp/mcu/all/bsp_register_protection.c \
../ra/fsp/src/bsp/mcu/all/bsp_sbrk.c \
../ra/fsp/src/bsp/mcu/all/bsp_sdram.c \
../ra/fsp/src/bsp/mcu/all/bsp_security.c 

C_DEPS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.d \
./ra/fsp/src/bsp/mcu/all/bsp_common.d \
./ra/fsp/src/bsp/mcu/all/bsp_delay.d \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_guard.d \
./ra/fsp/src/bsp/mcu/all/bsp_io.d \
./ra/fsp/src/bsp/mcu/all/bsp_ipc.d \
./ra/fsp/src/bsp/mcu/all/bsp_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_macl.d \
./ra/fsp/src/bsp/mcu/all/bsp_ospi_b.d \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.d \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.d \
./ra/fsp/src/bsp/mcu/all/bsp_sdram.d \
./ra/fsp/src/bsp/mcu/all/bsp_security.d 

CREF += \
nfc_read_205.cref 

OBJS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.o \
./ra/fsp/src/bsp/mcu/all/bsp_common.o \
./ra/fsp/src/bsp/mcu/all/bsp_delay.o \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_guard.o \
./ra/fsp/src/bsp/mcu/all/bsp_io.o \
./ra/fsp/src/bsp/mcu/all/bsp_ipc.o \
./ra/fsp/src/bsp/mcu/all/bsp_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_macl.o \
./ra/fsp/src/bsp/mcu/all/bsp_ospi_b.o \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.o \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.o \
./ra/fsp/src/bsp/mcu/all/bsp_sdram.o \
./ra/fsp/src/bsp/mcu/all/bsp_security.o 

MAP += \
nfc_read_205.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/mcu/all/%.o: ../ra/fsp/src/bsp/mcu/all/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -mlittle-endian -Oz -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -ffunction-sections -flto -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\fsp_cfg\\bsp" -I"." -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_gen" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\fsp_cfg" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\aws" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\src" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc\\api" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc\\instances" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\src\\rm_freertos_port" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\aws\\FreeRTOS\\FreeRTOS\\Source\\include" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\src\\SEGGER_RTT" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T2T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T3T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T5T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\DEFINES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\GENERIC" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T2T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T3T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T5T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\RUL_HCE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV\\MCU" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV\\MCU\\RA4M2" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HIP" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HIP\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG\\NONE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG\\SERIAL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC\\UCODE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\UTILS" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\Utils" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\ptx_Example_NFCForumNDEF" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\ptx_Example_HCET4T\\HCE_T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -DPTX_ENABLE_SPI -DPTX_PLATFORM_MCU -DPTX_APP_DIVERSITY_RA4M2 -DPTX_LOG_DISABLED -DPTX_DEBUG_PRINTS_DISABLE -DPTX_DISABLE_NDEF_T3T -DPTX_DISABLE_NDEF_T5T -DPTX_DISABLE_WLC -DconfigUSE_TIMERS=0 -DPTX_DISABLE_COMP_CHECK -DPTX_DISABLE_LISTEN_B -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

