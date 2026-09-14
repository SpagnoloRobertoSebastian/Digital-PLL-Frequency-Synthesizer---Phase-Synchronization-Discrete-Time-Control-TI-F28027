/*
 *  Title: control.c
 *	Version:
 *  Description:
 *  Created:  xx/xx/xxxx XX
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *  		Departamento de Electrónica
 *  		Facultad de Ingeniería
 *  		Universidad Nacional de Mar del Plata
 *
 *  El alumno solamente deberá intervenir en el archivo <config.h> en la selección del modo de operación (ModeFinal) y del número
 *  de placa utilizados y en el archivo <Control.c> en la sección destinada a tal efecto dentro de la función control_SdC_function()
 *
 *  <config.h>
 *	 	//--- UNCOMMENT ONLY THE BOARD IN USE
 *			#define Placa1
 *			// #define Placa2
 *			// #define Placa3
 *		//--- UNCOMMENT ONLY THE MODE IN USE
 *			// #define ModeTest
 *			#define ModeNormal
 *
 *	<Control.c>
 *		// --**--**-- Inicio del código del alumno --**--**--
 *		*
 *		*
 *		// --**--**-- Fin del código del alumno --**--**--
 *
 */

#include <stdio.h>
#include <file.h>

#include <math.h>

#include "DSP28x_Project.h"     // DSP28x Headerfile
#include "ti_ascii.h"

#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/sci.h"
#include "f2802x_common/include/sci_io.h"
#include "f2802x_common/include/wdog.h"
#include "f2802x_common/include/timer.h"

// Select the global Q value to use:
// #define GLOBAL_Q    20
// long GlobalQ = GLOBAL_Q;      // Used for legacy GEL & Graph Debug.
// #include "IQmathLib.h"

#include "config.h"
#include "SciGui/SciGui.h"
#include "Control/Control.h"
#include "Pwm/Pwm.h"

#include "config.h"

#define CONV_WAIT 1L //Micro-seconds to wait for ADC conversion. Longer than necessary.

extern void DSP28x_usDelay(Uint32 Count);

ADC_Handle myAdc;
CLK_Handle myClk;
FLASH_Handle myFlash;
GPIO_Handle myGpio;
PIE_Handle myPie;
SCI_Handle mySci;
TIMER_Handle myTimer0;
PWM_Handle myPwm1, myPwm2, myPwm3, myPwm4;

// --------------------------------------------
interrupt void cpu_timer0_isr(void);
void control_function(void);
void flag_function(void);


int DataCommPeriod;
int ControlPeriod;
int FlagPeriod;
int DataCommCounter=0;
int ControlCounter=0;
int FlagCounter=0;



int   ADC0_int,ADC1_int,ADC2_int,ADC3_int;
float ADC0_ft, ADC1_ft, ADC2_ft, ADC3_ft;

int UserLED, UserGPIO;
float UserPC;

int Adq0Period=1;
int Adq1Period=1;
int Adq0Counter=0;
int Adq1Counter=0;

int DAC1_int,DAC2_int;
float DAC1_ft,DAC2_ft;




int SignalGeneratorClockDividerCounter=0;
// --------------------------------------------

int16_t referenceTemp;
int16_t currentTemp;
uint32_t TimerPeriod;


int16_t sampleTemperature(void)
{
    //Force start of conversion on SOC0 and SOC1
    ADC_forceConversion(myAdc, ADC_SocNumber_0);
    ADC_forceConversion(myAdc, ADC_SocNumber_1);
    ADC_forceConversion(myAdc, ADC_SocNumber_2);

    //Wait for end of conversion.
    while(ADC_getIntStatus(myAdc, ADC_IntNumber_1) == 0) {
    }

    // Clear ADCINT1
    ADC_clearIntFlag(myAdc, ADC_IntNumber_1);

    // Get temp sensor sample result from SOC1
    return (ADC_readResult(myAdc, ADC_ResultNumber_0));
}



// SCIA  8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scia_init()
{
    CLK_enableSciaClock(myClk);

    // 1 stop bit,  No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    SCI_disableParity(mySci);
    SCI_setNumStopBits(mySci, SCI_NumStopBits_One);
    SCI_setCharLength(mySci, SCI_CharLength_8_Bits);
    
    SCI_enableTx(mySci);
    SCI_enableRx(mySci);
    SCI_enableTxInt(mySci);
    SCI_enableRxInt(mySci);

    // SCI BRR = LSPCLK/(SCI BAUDx8) - 1
    // Configured for 115.2kbps -- 60MHz CPU Freq
    SCI_setBaudRate(mySci, SCI_BaudRate_115_2_kBaud);    

    SCI_enableFifoEnh(mySci);
    SCI_resetTxFifo(mySci);
    SCI_clearTxFifoInt(mySci);
    SCI_resetChannels(mySci);
    SCI_setTxFifoIntLevel(mySci, SCI_FifoLevel_Empty);

    SCI_resetRxFifo(mySci);
    SCI_clearRxFifoInt(mySci);
    SCI_setRxFifoIntLevel(mySci, SCI_FifoLevel_1_Word);
//    SciaRegs.SCIFFRX.all=0x2021;
    SCI_enableRxFifoInt(mySci);

    SCI_setPriority(mySci, SCI_Priority_FreeRun);
    
  //  DSP28x_usDelay(CONV_WAIT);

    SCI_enable(mySci);
}

// --
interrupt void cpu_timer0_isr(void){
	GpioDataRegs.GPASET.bit.GPIO19 = 1;
	ControlCounter++;
	if (ControlCounter>=ControlPeriod) {
		control_function();
		ControlCounter=0;
	}
	FlagCounter++;
	if (FlagCounter>=FlagPeriod) {
		flag_function();
		FlagCounter=0;
	}
	/*
	DataCommCounter++;
	if (DataCommCounter>=DataCommPeriod) {
		datacomm_function();
		DataCommCounter=0;
	}
	*/
	GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // clear interruption
    return;
}


void control_function(void)
{

	static float ADC1_ft_Acc=0;
	static int   ADC1_PromCounter=0;

    // DATA ADQUISITION
		// Force start of conversion
			ADC_forceConversion(myAdc, ADC_SocNumber_0);
			ADC_forceConversion(myAdc, ADC_SocNumber_1);
			ADC_forceConversion(myAdc, ADC_SocNumber_2);
			ADC_forceConversion(myAdc, ADC_SocNumber_3);
        // Wait until end of conversion
			while(ADC_getIntStatus(myAdc, ADC_IntNumber_1) == 0) { }
		// Read acquired values
			ADC1_int=ADC_readResult(myAdc, ADC_ResultNumber_1);
			ADC2_int=ADC_readResult(myAdc, ADC_ResultNumber_2);
			ADC3_int=ADC_readResult(myAdc, ADC_ResultNumber_3);
		// Format conversions
			// DSP internal temperature
			ADC0_ft=ADC_getTemperatureC(myAdc,ADC_readResult(myAdc, ADC_ResultNumber_0));
			// Water temperature
			ADC1_ft=(float)ADC1_int * ADC1_SLOPE + ADC1_OFFSET;
			// ADC channel 1
			ADC2_ft=(float)ADC2_int * ADC2_SLOPE + ADC2_OFFSET;
			// ADC channel 2
			ADC3_ft=(float)ADC3_int * ADC3_SLOPE + ADC3_OFFSET;

	// TEMPERATURE PROMEDIATION
			ADC1_PromCounter ++;
			ADC1_ft_Acc += ADC1_ft*0.1;
			if (ADC1_PromCounter==10){
				ADC1_PromCounter=0;
				DataToPc1=ADC1_ft_Acc;
				ADC1_ft_Acc=0;}

	// CONTROL ALGORITHM IMPLEMENTATION
			#ifdef ModeTest
				if (ADC2_ft>5)
				{
					DAC1_ft=ADC2_ft*0.333333;
					UserGPIO=1;
					UserLED=0;
				}
				else
				{
					DAC1_ft=(10-ADC2_ft)*0.333333;
					UserGPIO=0;
					UserLED=1;
				}
			#endif
			#ifdef ModeNormal
				DAC1_ft=PLL_SdC_function(ADC2_ft,&UserLED,&UserGPIO);
				//Output33V=PLL_SdC_function(Input10V,&UserLED,&UserGPIO);
			#endif


	// DIGITAL SIGNALS GENERATION
		// LED control
			if (UserLED==1){ GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;} // Led3 ON
			else {       GpioDataRegs.GPASET.bit.GPIO3 = 1;}   // Led3 OFF
		// GPIO control
			if (UserGPIO==1){ GpioDataRegs.GPASET.bit.GPIO5 = 1;}   // Moc ON
			else {        GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;} // Moc OFF

	// ANALOG SIGNALS GENERATION BY FILTERED PWM
	// -- DAC1_ft and DAC2_ft, 0-3.3V, 8bits
			DAC1_int=(int)(DAC1_ft*255.0/3.3);
			//DAC2_int=(int)(DAC2_ft*255.0/3.3);
			if (DAC1_int>=255) DAC1_int=255; else if (DAC1_int<=0) DAC1_int=0;
			//if (DAC2_int>=255) DAC2_int=255; else if (DAC2_int<=0) DAC2_int=0;
			//
			PWM_setCmpA(myPwm4, 255-DAC1_int);
			//PWM_setCmpB(myPwm4, 255-DAC2_int);

	// Clear ADCINT1
	ADC_clearIntFlag(myAdc, ADC_IntNumber_1);
}
// --------------------------------------------
void flag_function(void)
{
	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;


	if (datacomm_enable_flag==1){
		if (GpioDataRegs.GPADAT.bit.GPIO12==1){
			if (datacomm_enable==0){datacomm_enable=1;}
			else {datacomm_enable=0;}
			datacomm_enable_flag=0;
		}
	} else {
		if (GpioDataRegs.GPADAT.bit.GPIO12==0){
			datacomm_enable_flag=1;
		}
	}
	GpioDataRegs.GPADAT.bit.GPIO1=(datacomm_enable==0);
}

void main()
{
    volatile int status = 0;
//    volatile FILE *fid;
    
    CPU_Handle myCpu;
    PLL_Handle myPll;
    WDOG_Handle myWDog;
    
    // Initialize all the handles needed for this application    
    myAdc = ADC_init((void *)ADC_BASE_ADDR, sizeof(ADC_Obj));
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myFlash = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
    myPie = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    mySci = SCI_init((void *)SCIA_BASE_ADDR, sizeof(SCI_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
    myTimer0 = TIMER_init((void *)TIMER0_BASE_ADDR, sizeof(TIMER_Obj));
    myPwm1 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    myPwm2 = PWM_init((void *)PWM_ePWM2_BASE_ADDR, sizeof(PWM_Obj));
    myPwm3 = PWM_init((void *)PWM_ePWM3_BASE_ADDR, sizeof(PWM_Obj));
    myPwm4 = PWM_init((void *)PWM_ePWM4_BASE_ADDR, sizeof(PWM_Obj));

    // Perform basic system initialization    
    WDOG_disable(myWDog);
    CLK_enableAdcClock(myClk);
    (*Device_cal)();
    
    //Select the internal oscillator 1 as the clock source
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);
    
    // Setup the PLL for x12 /2 which will yield 60Mhz = 10Mhz * 12 / 2
    PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);
    
    // Disable the PIE and all interrupts
    PIE_disable(myPie);
    PIE_disableAllInts(myPie);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);
        
    // If running from flash copy RAM only functions to RAM   
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif      

    // Initalize GPIO
    // Enable XCLOCKOUT to allow monitoring of oscillator 1
    GPIO_setMode(myGpio, GPIO_Number_18, GPIO_18_Mode_XCLKOUT);
    CLK_setClkOutPreScaler(myClk, CLK_ClkOutPreScaler_SysClkOut_by_1);

    // --------------------------------------------
    // Setup a debug vector table and enable the PIE
    PIE_setDebugIntVectorTable(myPie);
    PIE_enable(myPie);   

    // Register interrupt handlers in the PIE vector table
    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_1, PIE_SubGroupNumber_7, (intVec_t)&cpu_timer0_isr);

    TIMER_stop(myTimer0);

    // Configure CPU-Timer 0:
    // 60MHz CPU Freq, 1 uSecond Period
    TimerPeriod=1000;   // 1KHz
    TIMER_setPeriod(myTimer0, 60 * TimerPeriod);

    TIMER_setPreScaler(myTimer0, 0);
    TIMER_reload(myTimer0);
    TIMER_setEmulationMode(myTimer0, TIMER_EmulationMode_StopAfterNextDecrement);
    TIMER_enableInt(myTimer0);

    TIMER_start(myTimer0);

    // Enable CPU int1 which is connected to CPU-Timer 0
    CPU_enableInt(myCpu, CPU_IntNumber_1);

    // Enable CPU Timer_0 INTn in the PIE: Group 1 interrupt 7
    PIE_enableTimer0Int(myPie);

    // Enable global Interrupts and higher priority real-time debug events:
    CPU_enableGlobalInts(myCpu);
    CPU_enableDebugInt(myCpu);
    // --------------------------------------------

    // Initialize SCIA
    // scia_init();
    
    // Initialize the ADC
    ADC_enableBandGap(myAdc);
    ADC_enableRefBuffers(myAdc);
    ADC_powerUp(myAdc);
    ADC_enable(myAdc);
    ADC_setVoltRefSrc(myAdc, ADC_VoltageRefSrc_Int);

    ADC_enableTempSensor(myAdc);                                            //Connect channel A5 internally to the temperature sensor
    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_0, ADC_SocTrigSrc_Sw);
    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_1, ADC_SocTrigSrc_Sw);
    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_2, ADC_SocTrigSrc_Sw);
    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_3, ADC_SocTrigSrc_Sw);

    ADC_setSocChanNumber (myAdc, ADC_SocNumber_0, ADC_SocChanNumber_A5);    //Set SOC0 channel select to ADCINA5
    ADC_setSocChanNumber (myAdc, ADC_SocNumber_1, ADC_SocChanNumber_A0);    //Set SOC1 channel select to ADCINA0
    ADC_setSocChanNumber (myAdc, ADC_SocNumber_2, ADC_SocChanNumber_A1);    //Set SOC2 channel select to ADCINA1
    ADC_setSocChanNumber (myAdc, ADC_SocNumber_3, ADC_SocChanNumber_A2);    //Set SOC2 channel select to ADCINA2
    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_0, ADC_SocSampleWindow_7_cycles);   //Set SOC0 acquisition period to 7 ADCCLK
    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_1, ADC_SocSampleWindow_7_cycles);   //Set SOC1 acquisition period to 7 ADCCLK
    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_2, ADC_SocSampleWindow_7_cycles);   //Set SOC2 acquisition period to 7 ADCCLK
    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_3, ADC_SocSampleWindow_7_cycles);   //Set SOC2 acquisition period to 7 ADCCLK
    ADC_setIntSrc(myAdc, ADC_IntNumber_1, ADC_IntSrc_EOC2);                 //Connect ADCINT1 to EOC1
    ADC_enableInt(myAdc, ADC_IntNumber_1);                                  //Enable ADCINT1

    // Set the flash OTP wait-states to minimum. This is important
    // for the performance of the temperature conversion function.
    FLASH_setup(myFlash);

    // Initalize GPIO
    GPIO_setPullUp(myGpio, GPIO_Number_28, GPIO_PullUp_Enable);
    GPIO_setPullUp(myGpio, GPIO_Number_29, GPIO_PullUp_Disable);
    GPIO_setQualification(myGpio, GPIO_Number_28, GPIO_Qual_ASync);
    GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);
    GPIO_setMode(myGpio, GPIO_Number_29, GPIO_29_Mode_SCITXDA);
    
    // Configure GPIO 0-3 as outputs
    GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_GeneralPurpose);
    GPIO_setMode(myGpio, GPIO_Number_1, GPIO_0_Mode_GeneralPurpose);
    GPIO_setMode(myGpio, GPIO_Number_2, GPIO_0_Mode_GeneralPurpose);
    GPIO_setMode(myGpio, GPIO_Number_3, GPIO_0_Mode_GeneralPurpose);
    
    GPIO_setDirection(myGpio, GPIO_Number_0, GPIO_Direction_Output);
    GPIO_setDirection(myGpio, GPIO_Number_1, GPIO_Direction_Output);
    GPIO_setDirection(myGpio, GPIO_Number_2, GPIO_Direction_Output);
    GPIO_setDirection(myGpio, GPIO_Number_3, GPIO_Direction_Output);
    
    GPIO_setMode(myGpio, GPIO_Number_19, GPIO_19_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_19, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio, GPIO_Number_19, GPIO_PullUp_Disable);

    GPIO_setMode(myGpio, GPIO_Number_4, GPIO_4_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_4, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio, GPIO_Number_4, GPIO_PullUp_Disable);

    GPIO_setMode(myGpio, GPIO_Number_5, GPIO_5_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_5, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio, GPIO_Number_5, GPIO_PullUp_Disable);

    GPIO_setMode(myGpio, GPIO_Number_12, GPIO_12_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_12, GPIO_Direction_Input);
    GPIO_setPullUp(myGpio, GPIO_Number_12, GPIO_PullUp_Disable);
    
    GPIO_setPullUp(myGpio, GPIO_Number_6, GPIO_PullUp_Disable);
    GPIO_setPullUp(myGpio, GPIO_Number_7, GPIO_PullUp_Disable);
    GPIO_setMode(myGpio, GPIO_Number_6, GPIO_6_Mode_EPWM4A);
    GPIO_setMode(myGpio, GPIO_Number_7, GPIO_7_Mode_EPWM4B);

    // Initialize SCIA
    GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_28, GPIO_Direction_Input);

    // Register interrupt handlers in the PIE vector table
    // ISR functions found within this file.
        EALLOW;    // This is needed to write to EALLOW protected registers
    //   PieVectTable.SCIRXINTA = &sciaRxFifoIsr;
        ((PIE_Obj *)myPie)->SCIRXINTA = &sciaRxFifoIsr;
        EDIS;   // This is needed to disable write to EALLOW protected registers
    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_9, PIE_SubGroupNumber_1, (intVec_t)&sciaRxFifoIsr);
    scia_init();
    // Enable interrupts required for SCI reception
    PIE_enableInt(myPie, PIE_GroupNumber_9, PIE_InterruptSource_SCIARX);
    CPU_enableInt(myCpu, CPU_IntNumber_9);
    GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);

  //  InitSciGui();

    GpioDataRegs.GPASET.bit.GPIO0 = 1;
    GpioDataRegs.GPASET.bit.GPIO1 = 1;
    GpioDataRegs.GPASET.bit.GPIO2 = 1;
    GpioDataRegs.GPASET.bit.GPIO3 = 1;

    // Periods (*TimerPeriod [us])
    DataCommPeriod=100;
    ControlPeriod=1;
    FlagPeriod=500;


    CLK_disableTbClockSync(myClk);

// Some useful Period vs Frequency values
//  SYSCLKOUT =     60 MHz       40 MHz
//  --------------------------------------
//    Period            Frequency    Frequency
//    1000            60 kHz       40 kHz
//    800                75 kHz       50 kHz
//    600                100 kHz      67 kHz
//    500                120 kHz      80 kHz
//    250                240 kHz      160 kHz
//    200                300 kHz      200 kHz
//    100                600 kHz      400 kHz
//    50                1.2 Mhz      800 kHz
//    25                2.4 Mhz      1.6 MHz
//    20                3.0 Mhz      2.0 MHz
//    12                5.0 MHz      3.3 MHz
//    10                6.0 MHz      4.0 MHz
//    9                6.7 MHz      4.4 MHz
//    8                7.5 MHz      5.0 MHz
//    7                8.6 MHz      5.7 MHz
//    6                10.0 MHz     6.6 MHz
//    5                12.0 MHz     8.0 MHz

//====================================================================
// ePWM and HRPWM register initializaition
//====================================================================

    HRPWM4_Config(255);        // ePWM4 target, Period = 200/60MHz (300KHz)

    CLK_enableTbClockSync(myClk);

//  PWM_setCmpA(myPwm3, 50);
//  PWM_setCmpB(myPwm3, 100);


    PWM_setCmpA(myPwm4, 10);
    PWM_setCmpB(myPwm4, 125);

    //Main program loop - continually sample temperature
    for(;;) {
    	PumpSciGui();
    }
}




























