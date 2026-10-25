################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/nfc_thread.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/nfc_thread.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

CREF += \
nfc_read_205.cref 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/nfc_thread.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

MAP += \
nfc_read_205.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -mlittle-endian -Oz -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -ffunction-sections -flto -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -std=c99 -fshort-enums -fno-unroll-loops -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\fsp_cfg\\bsp" -I"." -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_gen" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\fsp_cfg" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\aws" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\src" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc\\api" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc\\instances" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\src\\rm_freertos_port" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\aws\\FreeRTOS\\FreeRTOS\\Source\\include" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\src\\SEGGER_RTT" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T2T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T3T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T5T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\DEFINES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\GENERIC" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T2T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T3T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T5T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\RUL_HCE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV\\MCU" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV\\MCU\\RA4M2" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HIP" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HIP\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG\\NONE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG\\SERIAL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC\\UCODE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\UTILS" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\Utils" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\ptx_Example_NFCForumNDEF" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\ptx_Example_HCET4T\\HCE_T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include" -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -DPTX_ENABLE_SPI -DPTX_PLATFORM_MCU -DPTX_APP_DIVERSITY_RA4M2 -DPTX_LOG_DISABLED -DPTX_DEBUG_PRINTS_DISABLE -DPTX_DISABLE_NDEF_T3T -DPTX_DISABLE_NDEF_T5T -DPTX_DISABLE_WLC -DconfigUSE_TIMERS=0 -DPTX_DISABLE_COMP_CHECK -DPTX_DISABLE_LISTEN_B -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

