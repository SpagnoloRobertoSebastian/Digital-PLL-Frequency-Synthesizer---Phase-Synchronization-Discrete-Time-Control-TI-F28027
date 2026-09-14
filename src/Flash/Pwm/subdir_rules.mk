################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Pwm/Pwm.obj: ../Pwm/Pwm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -mt --float_support=softlib -O0 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" --include_path="D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/device_support/f2802x/v210" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="D:/WASSINGER/DOCENCIA/SISTEMAS DE CONTROL/launchxl-f28027/DigitalPLL/CCS_WKS_PLL/device_support/iqmath/v160/include" --define="_FLASH" --define=NDEBUG --define="_DEBUG" --define="LARGE_MODEL" --quiet --verbose_diagnostics --diag_warning=225 --gen_func_subsections=on --output_all_syms --cdebug_asm_data --preproc_with_compile --preproc_dependency="Pwm/Pwm.pp" --obj_directory="Pwm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


