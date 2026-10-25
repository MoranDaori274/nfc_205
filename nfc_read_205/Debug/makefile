################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/SRC/EXAMPLES/ptx_Example_NFCForumNDEF/subdir.mk
-include src/SRC/EXAMPLES/Utils/subdir.mk
-include src/SRC/EXAMPLES/HCE_T4T/subdir.mk
-include src/SRC/COMPS/UTILS/subdir.mk
-include src/SRC/COMPS/NSC/UCODE/subdir.mk
-include src/SRC/COMPS/NSC/RUL/subdir.mk
-include src/SRC/COMPS/NSC/subdir.mk
-include src/SRC/COMPS/LOG/NONE/subdir.mk
-include src/SRC/COMPS/LOG/subdir.mk
-include src/SRC/COMPS/HIP/RUL/subdir.mk
-include src/SRC/COMPS/HIP/subdir.mk
-include src/SRC/COMPS/HAL/DRV/MCU/RA4M2/subdir.mk
-include src/SRC/COMPS/HAL/subdir.mk
-include src/SRC/API/RUL_HCE/subdir.mk
-include src/SRC/API/RUL/subdir.mk
-include src/SRC/API/NDEF/TAG_TYPES/NDEF_T5T/subdir.mk
-include src/SRC/API/NDEF/TAG_TYPES/NDEF_T4T/subdir.mk
-include src/SRC/API/NDEF/TAG_TYPES/NDEF_T3T/subdir.mk
-include src/SRC/API/NDEF/TAG_TYPES/NDEF_T2T/subdir.mk
-include src/SRC/API/NDEF/GENERIC/subdir.mk
-include src/SRC/API/NATIVE_TAG/T5T/subdir.mk
-include src/SRC/API/NATIVE_TAG/T4T/subdir.mk
-include src/SRC/API/NATIVE_TAG/T3T/subdir.mk
-include src/SRC/API/NATIVE_TAG/T2T/subdir.mk
-include src/SEGGER_RTT/subdir.mk
-include src/subdir.mk
-include ra_gen/subdir.mk
-include ra/fsp/src/rm_freertos_port/subdir.mk
-include ra/fsp/src/r_sci_uart/subdir.mk
-include ra/fsp/src/r_sci_spi/subdir.mk
-include ra/fsp/src/r_ioport/subdir.mk
-include ra/fsp/src/r_icu/subdir.mk
-include ra/fsp/src/r_gpt/subdir.mk
-include ra/fsp/src/bsp/mcu/ra2e3/subdir.mk
-include ra/fsp/src/bsp/mcu/all/subdir.mk
-include ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/subdir.mk
-include ra/board/ra2e3_fpb/subdir.mk
-include ra/aws/FreeRTOS/FreeRTOS/Source/subdir.mk
-include ra/arm/CMSIS_6/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
$(C_DEPS):$(SMS_ASM)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
OBJCOPY += \
nfc_read_205.srec \

SECONDARY_SIZE += \
nfc_read_205.siz \


# All Target
all: nfc_read_205.elf secondary-outputs

# Tool invocations
nfc_read_205.elf: $(OBJS) $(USER_OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(LINKER_SCRIPT)
	$(file > nfc_read_205.elf.in,-mcpu=cortex-m23 -mthumb -mlittle-endian -Oz -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0 -funsigned-char -ffunction-sections -flto -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -g3 -o "nfc_read_205.elf" $(OBJS) $(USER_OBJS) -T "fsp.lld" -L"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205/script" -L"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\Debug" -Wl,-Map=nfc_read_205.map -Wl,--start-group -lcrt0 -lc++abi  -Wl,--end-group -Wl,--gc-sections -Wl,--cref -Wl,-z,norelro -Wl,-mllvm,-enable-machine-outliner -flto -Wl,--icf=none -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\fsp_cfg\\bsp" -I"." -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_gen" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\fsp_cfg" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra_cfg\\aws" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\src" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc\\api" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\inc\\instances" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\fsp\\src\\rm_freertos_port" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\aws\\FreeRTOS\\FreeRTOS\\Source\\include" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\src\\SEGGER_RTT" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T2T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T3T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NATIVE_TAG\\T5T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\DEFINES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\GENERIC" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T2T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T3T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\NDEF\\TAG_TYPES\\NDEF_T5T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\API\\RUL_HCE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV\\MCU" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HAL\\DRV\\MCU\\RA4M2" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HIP" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\HIP\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG\\NONE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\LOG\\SERIAL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC\\RUL" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\NSC\\UCODE" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\COMPS\\UTILS" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\Utils" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\ptx_Example_NFCForumNDEF" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ptx_sdk\\SRC\\EXAMPLES\\ptx_Example_HCET4T\\HCE_T4T" -I"C:\\Users\\a5133459\\e2_studio\\nfc_read_205\\nfc_read_205\\ra\\arm\\CMSIS_6\\CMSIS\\Core\\Include")
	@echo Building target: $@ && clang  --target=arm-none-eabi @"nfc_read_205.elf.in"> nfc_read_205.cref
nfc_read_205.srec: nfc_read_205.elf
	llvm-objcopy "nfc_read_205.elf" -O srec  "nfc_read_205.srec"
nfc_read_205.siz: nfc_read_205.elf
	llvm-size --format=berkeley "nfc_read_205.elf"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(JMP_DEPS)  $(FSY_DEPS)  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(SRC_UPPER_DEPS)  $(P_UPPER_DEPS)  $(S_DEPS)  $(PP_UPPER_DEPS)  $(P_DEPS)  $(FSY_UPPER_DEPS)  $(C_DEPS)  $(CREF)  $(SRC_DEPS)  $(OBJCOPY)  $(JMP_UPPER_DEPS)  $(PP_DEPS)  $(SX_DEPS)  $(ASM_UPPER_DEPS)  $(OBJS)  $(SX_UPPER_DEPS)  $(ASM_DEPS)  $(MAP)  $(S_UPPER_DEPS) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  nfc_read_205.elf
	-@echo ' '

secondary-outputs: $(OBJCOPY) $(SECONDARY_SIZE)

.PHONY: all clean dependents 

-include ../makefile.targets
