################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c2000_6.1.0

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./Control/Control.obj" \
"./Pwm/Pwm.obj" \
"./SciGui/SerialCom.obj" \
"./SciGui/SciGui.obj" \
"./main.obj" \
"./F2802x_Sci.obj" \
-l"D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/SdC-F28027-V01\..\device_support\f2802x\v210/f2802x_common/cmd/F28027.cmd" \
-l"rts2800_ml.lib" \
-l"IQmath.lib" \
"D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/device_support/f2802x/v210/f2802x_headers/cmd/F2802x_Headers_nonBIOS.cmd" \
"D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/device_support/f2802x/v210/f2802x_common/lib/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include SciGui/subdir_vars.mk
-include Pwm/subdir_vars.mk
-include Control/subdir_vars.mk
-include subdir_rules.mk
-include SciGui/subdir_rules.mk
-include Pwm/subdir_rules.mk
-include Control/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Example_F2802xLaunchPad_demo.out

# Tool invocations
Example_F2802xLaunchPad_demo.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -mt --float_support=softlib -O0 -g --define="_FLASH" --define=NDEBUG --define="_DEBUG" --define="LARGE_MODEL" --quiet --verbose_diagnostics --diag_warning=225 --gen_func_subsections=on --output_all_syms --cdebug_asm_data -z --stack_size=0x300 -m"Example_F2802xLaunchPad_demo.map" --heap_size=0x200 --warn_sections -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/lib" -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" -i"D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/SdC-F28027-V01" -i"D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/device_support/f2802x/v210/F2802x_common/lib" --reread_libs --verbose_diagnostics --issue_remarks --entry_point=code_start --rom_model -o "Example_F2802xLaunchPad_demo.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QUOTED) "Example_F2802xLaunchPad_demo.out"
	-$(RM) "F2802x_Sci.pp" "main.pp" "SciGui\SciGui.pp" "SciGui\SerialCom.pp" "Pwm\Pwm.pp" "Control\Control.pp" 
	-$(RM) "F2802x_Sci.obj" "main.obj" "SciGui\SciGui.obj" "SciGui\SerialCom.obj" "Pwm\Pwm.obj" "Control\Control.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
