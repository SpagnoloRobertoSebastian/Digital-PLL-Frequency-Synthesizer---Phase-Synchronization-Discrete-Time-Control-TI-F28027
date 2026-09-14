/*
 *  Title: Pwm.c
 *	Version:
 *  Description:
 *  Created:  03/11/2014 NW
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *  		Departamento de Electrónica
 *  		Facultad de Ingeniería
 *  		Universidad Nacional de Mar del Plata
 */

#include <stdio.h>

#include "DSP28x_Project.h"     // DSP28x Headerfile

#include "f2802x_common/include/clk.h"

#include "Pwm.h"

void HRPWM1_Config(int period)
{

    CLK_enablePwmClock(myClk, PWM_Number_1);

    // ePWM1 register configuration with HRPWM
    // ePWM1A toggle low/high with MEP control on Rising edge
    PWM_setPeriodLoad(myPwm1, PWM_PeriodLoad_Immediate);
    PWM_setPeriod(myPwm1, period-1);    // Set timer period
    PWM_setCmpA(myPwm1, period / 2);
    PWM_setCmpAHr(myPwm1, (1 << 8));
    PWM_setCmpB(myPwm1, period / 2);
    PWM_setPhase(myPwm1, 0x0000);   // Phase is 0
    PWM_setCount(myPwm1, 0x0000);   // Clear counter

    PWM_setCounterMode(myPwm1, PWM_CounterMode_Up);     // Count up
    PWM_disableCounterLoad(myPwm1);                     // Disable phase loading
    PWM_setSyncMode(myPwm1, PWM_SyncMode_Disable);
    PWM_setHighSpeedClkDiv(myPwm1, PWM_HspClkDiv_by_1); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm1, PWM_ClkDiv_by_1);

    PWM_setShadowMode_CmpA(myPwm1, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setShadowMode_CmpB(myPwm1, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm1, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(myPwm1, PWM_LoadMode_Zero);

    PWM_setActionQual_Zero_PwmA(myPwm1, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm1, PWM_ActionQual_Set);
    PWM_setActionQual_Zero_PwmB(myPwm1, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpB_PwmB(myPwm1, PWM_ActionQual_Set);

    //MEP control on Rising edge
    PWM_setHrEdgeMode(myPwm1, PWM_HrEdgeMode_Rising);
    PWM_setHrControlMode(myPwm1, PWM_HrControlMode_Duty);
    PWM_setHrShadowMode(myPwm1, PWM_HrShadowMode_CTR_EQ_0);

}

void HRPWM2_Config(int period)
{

    CLK_enablePwmClock(myClk, PWM_Number_2);

    // ePWM2 register configuration with HRPWM
    // ePWM2A toggle low/high with MEP control on Rising edge
    PWM_setPeriodLoad(myPwm2, PWM_PeriodLoad_Immediate);
    PWM_setPeriod(myPwm2, period-1);    // Set timer period
    PWM_setCmpA(myPwm2, period / 2);
    PWM_setCmpAHr(myPwm2, (1 << 8));
    PWM_setCmpB(myPwm2, period / 2);
    PWM_setPhase(myPwm2, 0x0000);   // Phase is 0
    PWM_setCount(myPwm2, 0x0000);   // Clear counter

    PWM_setCounterMode(myPwm2, PWM_CounterMode_Up);     // Count up
    PWM_disableCounterLoad(myPwm2);                     // Disable phase loading
    PWM_setSyncMode(myPwm2, PWM_SyncMode_Disable);
    PWM_setHighSpeedClkDiv(myPwm2, PWM_HspClkDiv_by_1); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm2, PWM_ClkDiv_by_1);

    PWM_setShadowMode_CmpA(myPwm2, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setShadowMode_CmpB(myPwm2, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm2, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(myPwm2, PWM_LoadMode_Zero);

    PWM_setActionQual_Zero_PwmA(myPwm2, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm2, PWM_ActionQual_Set);
    PWM_setActionQual_Zero_PwmB(myPwm2, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpB_PwmB(myPwm2, PWM_ActionQual_Set);

    //MEP control on Rising edge
    PWM_setHrEdgeMode(myPwm2, PWM_HrEdgeMode_Rising);
    PWM_setHrControlMode(myPwm2, PWM_HrControlMode_Duty);
    PWM_setHrShadowMode(myPwm2, PWM_HrShadowMode_CTR_EQ_0);

}

void HRPWM3_Config(int period)
{

    CLK_enablePwmClock(myClk, PWM_Number_3);

    // ePWM3 register configuration with HRPWM
    // ePWM3A toggle high/low with MEP control on falling edge
    PWM_setPeriodLoad(myPwm3, PWM_PeriodLoad_Immediate);
    PWM_setPeriod(myPwm3, period-1);    // Set timer period
    PWM_setCmpA(myPwm3, period / 2);
    PWM_setCmpAHr(myPwm3, (1 << 8));
    PWM_setCmpB(myPwm3, period / 2);
    PWM_setPhase(myPwm3, 0x0000);   // Phase is 0
    PWM_setCount(myPwm3, 0x0000);   // Clear counter

    PWM_setCounterMode(myPwm3, PWM_CounterMode_Up);     // Count up
    PWM_disableCounterLoad(myPwm3);                     // Disable phase loading
    PWM_setSyncMode(myPwm3, PWM_SyncMode_Disable);
    PWM_setHighSpeedClkDiv(myPwm3, PWM_HspClkDiv_by_1); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm3, PWM_ClkDiv_by_1);

    PWM_setShadowMode_CmpA(myPwm3, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setShadowMode_CmpB(myPwm3, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm3, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(myPwm3, PWM_LoadMode_Zero);

    PWM_setActionQual_Zero_PwmA(myPwm3, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm3, PWM_ActionQual_Set);
    PWM_setActionQual_Zero_PwmB(myPwm3, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpB_PwmB(myPwm3, PWM_ActionQual_Set);

    //MEP control on falling edge
    PWM_setHrEdgeMode(myPwm3, PWM_HrEdgeMode_Falling);
    PWM_setHrControlMode(myPwm3, PWM_HrControlMode_Duty);
    PWM_setHrShadowMode(myPwm3, PWM_HrShadowMode_CTR_EQ_0);

}

void HRPWM4_Config(int period)
{

    CLK_enablePwmClock(myClk, PWM_Number_4);

    // ePWM4 register configuration with HRPWM
    // ePWM4A toggle high/low with MEP control on falling edge
    PWM_setPeriodLoad(myPwm4, PWM_PeriodLoad_Immediate);
    PWM_setPeriod(myPwm4, period-1);    // Set timer period
    PWM_setCmpA(myPwm4, period / 2);
    PWM_setCmpAHr(myPwm4, (1 << 8));
    PWM_setCmpB(myPwm4, period / 2);
    PWM_setPhase(myPwm4, 0x0000);   // Phase is 0
    PWM_setCount(myPwm4, 0x0000);   // Clear counter

    PWM_setCounterMode(myPwm4, PWM_CounterMode_Up);     // Count up
    PWM_disableCounterLoad(myPwm4);                     // Disable phase loading
    PWM_setSyncMode(myPwm4, PWM_SyncMode_Disable);
    PWM_setHighSpeedClkDiv(myPwm4, PWM_HspClkDiv_by_1); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm4, PWM_ClkDiv_by_1);

    PWM_setShadowMode_CmpA(myPwm4, PWM_ShadowMode_Shadow);  // Load registers every ZERO
    PWM_setShadowMode_CmpB(myPwm4, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm4, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(myPwm4, PWM_LoadMode_Zero);

    PWM_setActionQual_Zero_PwmA(myPwm4, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm4, PWM_ActionQual_Set);
    PWM_setActionQual_Zero_PwmB(myPwm4, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpB_PwmB(myPwm4, PWM_ActionQual_Set);

    //MEP control on falling edge
    PWM_setHrEdgeMode(myPwm4, PWM_HrEdgeMode_Falling);
    PWM_setHrControlMode(myPwm4, PWM_HrControlMode_Duty);
    PWM_setHrShadowMode(myPwm4, PWM_HrShadowMode_CTR_EQ_0);

}


