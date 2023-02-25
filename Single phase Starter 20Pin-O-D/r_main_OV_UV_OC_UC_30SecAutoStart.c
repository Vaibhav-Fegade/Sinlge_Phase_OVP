/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G12 V2.01.00.01 [26 Jul 2013] 
* Device(s)    : R5F10268
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 27-Jan-17
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_lvd.h"
/* Start user code for include. Do not edit comment generated here */
#include "pfdl_types.h"
#include "pfdl.h"
#define Data_Count  14
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/*
Hiren Patel
Pin1  AC Volt Input 500V 220K+220K+2K2+220K
Pin2  DP Active High
Pin3  D2 Active Low
Pin4  C Active High
Pin5  RESET
Pin6  INPUT
Pin7  STOP KEY
Pin8  START KEY
Pin9  GND
Pin10 VCC
Pin11 D0 Active Low
Pin12 D1 Active Low
Pin13 E Active High
Pin14 D Active High
Pin15 G Active High + DN KEY
Pin16 F Active High + UP KEY
Pin17 B Active High + SET KEY
Pin18 RELAY
Pin19 A Active High
Pin20 AC Amp Input 40A 39E+15E 1200T CT

*/
unsigned char ssd[3],digit[3],disp_no,k,kc,key_press,key_no;
unsigned int ms;

unsigned char CalPara,KeyDelay,AutoOut,StopKeyDelay,Fault;
unsigned int SetOverAmp,SetDryRunAmp,SetOverVolt,SetUnderVolt;
unsigned int FaultCheckDelay,ScrollTime,MessageTime,Delaymenu;

unsigned int OverAmpTime,OverVoltTime,UnderVoltTime,UnderAmpTime;
unsigned long OverAmpResetTime,RelayTestTime, Relayontime;

unsigned int SetOnTime,RunTime;
unsigned int AutoStartDelayTimeSec;

bit KeySense,CalFlag,StartFlag,Pon;
bit menumodeflag,changmenuflag, OverAmpResetflag,RelayTestflag, OverVoltageResetflag, DisplayOffEnableflag;

//ADC variables
signed long int ADCValue;
unsigned char msdelay;//int
signed int FinalOutput,LastADCdata;
unsigned int SamplesTime;
long int Average,Tempdata[2],FinalValue[2];
unsigned char ReadyFlag,testvar;
unsigned char dp,Count; 
unsigned int SpanVolt,SpanAmp,TempRef,FinalRef;
unsigned int DispVolt,DispAmp;

const unsigned char character_table[] =
{
    0xc0,     // '0'
    0xf9,     // '1'
    0xa4,     // '2
    0xb0,     // '3
    0x99,     // '4
    0x92,     // '5
    0x82,     // '6
    0xf8,     // '7
    0x80,     // '8
    0x90,     // '9'
    0x88,     // 'a'   
    0x83,     // 'b'  
    0xc6,     // 'c'
    0xa1,     // 'd'        
    0x86,     // 'e'
    0x8e,     // 'f'
	0x40,     // '.0'        + 0x10
    0x79,     // '.1'
    0x24,     // '.2'
    0x30,     // '.3'
    0x19,     // '.4'
    0x12,     // '.5'
    0x02,     // '.6'
    0x78,     // '.7'
    0x00,     // '.8'
    0x10,     // '.9'
    0x88,     // 'A' (+ 'a') 1a
    0x83,     // 'B' (+ 'b') 1b
    0xc6,     // 'C' (+ 'c') 1c
    0xa1,     // 'D' (+ 'd') 1d
    0x86,     // 'E' (+ 'e') 1e
    0x8e,     // 'F' (+ 'f') 1f
    0xc2,     // 'G' (+ 'g') 20
    0x89,     // 'H' (+ 'h') 21
    0xfb,     // 'I' (+ 'i') 22
    0xf1,     // 'J' (+ 'j') 23
    0x8d,     // 'K' (+ 'k') 24
    0xc7,     // 'L' (+ 'l') 25
    0xaa,     // 'M' (+ 'm') 26
    0xab,     // 'N' (+ 'n') 27
    0xa3,     // 'O' (+ 'o') 28
    0x8c,     // 'P' (+ 'p') 29
    0x00,     // 'Q' (+ 'q') 2a
    0xaf,     // 'R' (+ 'r') 2b
    0x92,     // 'S' (+ 's') 2c
    0x87,     // 'T' (+ 't') 2d
    0xc1,     // 'U' (+ 'u') 2e
    0xe3,     // 'V' (+ 'v') 2f
    0xe2,     // 'W' (+ 'w') 30
    0x00,     // 'X' (+ 'x') 31
    0x91,     // 'Y' (+ 'y') 32
    0xa4,     // 'Z' (+ 'z') 33
    0xbf,     // '_'         34
    0xff,     // ' '         35
};

#define A  0x1a
#define B  0x1b
#define C  0x1c
#define D  0x1d
#define E  0x1e
#define F  0x1f
#define G  0x20
#define H  0x21
#define I  0x22
#define J  0x23
#define K  0x24
#define L  0x25
#define M  0x26
#define N  0x27
#define O  0x28
#define P  0x29
#define Q  0x2a
#define R  0x2b
#define S  0x2c
#define T  0x2d
#define U  0x2e
#define V  0x2f
#define W  0x30
#define X  0x31
#define Y  0x32
#define Z  0x33
#define DASH  0x34
#define BLANK 0x35
#define DP    0x10

#define MAX_AMP  250 //50.0A
#define MIN_AMP  20  // 10.0A

#define MAX_VOLT 290
#define MIN_VOLT 120

#define CAL_ZERO    	1
#define CAL_240V    	2   
#define CAL_30A  		3
#define MSG_OL          4
#define MSG_UC          5
#define MSG_HV          6
#define MSG_UV          7
#define SET_OVER_AMP   	8
#define SET_DRYRUN_AMP 	9
#define SET_OVER_VOLT   10
#define SET_UNDER_VOLT  11
#define MSG_SET         12

#define FAULT_CHECK_mSEC  8000//8 sec

#define NO_FAULT         0
#define OVER_VOLT_FAULT  1
#define OVER_AMP_FAULT   2
#define DRY_RUN_FAULT    3
#define UNDER_VOLT_FAULT 4

#define CT  40 //30

#define R1_ON  {P2.3 = 1U;}  //pin 18 Motor
#define R1_OFF {P2.3 = 0U;}

#define SET_KEY  P13.7 //pin 6
#define DN_KEY   P12.2 //pin 7
#define UP_KEY   P12.1 //pin 8

//SQUARE ROOT FUNCTION
#define iter1(N) \
    try = root + (1 << (N)); \
    if (n >= try << (N))   \
    {   n -= try << (N);   \
        root |= 2 << (N); \
    }
 
void TimerInterrupt(void);
void ADCInterrupt(void);
void ADCInputSelect(void);
void LVDInterrupt(void);
void FaultChecking(void);
void Display(void);
void KeyCheck(void);
void Bin2Bcd(unsigned int);   

//************************************** Flash Memory Functions Declaration ***********************************************************************
void Write_Memory(void);
void Read_Memory(void);  

//#define  Data_Count  13 /*do not define Data_Count here because in each program no. of data bytes stored in eeprom will be different*/
pfdl_descriptor_t FDL_SysInit={32,0};	//CPU clock 8MHz is used & Full speed mode selected as programming voltage mode
pfdl_status_t FDL_Status = PFDL_IDLE;	// Declaration of FDL_Status with initial value as PFDL_IDLE
pfdl_request_t FDL_Request = {0,0,0,0};	// Declaration of FDL_Request with initial value

//unsigned char Write_DataBuffer[5] ={0x01,0x23,0x45,0x67,0x89};	//Write data buffer array for data flash write operations 
unsigned char Write_DataBuffer[Data_Count]= {0};			// ={'*', 'S', 'a', 't'};//, 'G', 'u', 'r', 'u', '*', '*' };	//Write data buffer array for data flash write operations 
unsigned char Read_DataBuffer[Data_Count] = {0}; 			//Read data buffer array for Read and store the data
unsigned int FDL_VersionChk=0; 						//buffer used to store the start address where the FDL version details located.

void FDL_status_chk(void);	//Function that used to check the status of FDL Handler
void FDL_Blank_Chk(void);	//Function that used to execute FDL blank check command
void FDL_Erase(void);		//Function that used to execute FDL erase command
void FDL_Write(void);		//Function that used to execute FDL write command
void FDL_Verify(void);		//Function that used to execute FDL verify command
void FDL_Read(void);		//Function that used to execute FDL read command
void DataFlash_Read(void);
void DataFlash_Write(void);
//*************************************************************************************************************************************************

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
		if(!CalPara)
		{
			FaultChecking();
			
			if(Fault == NO_FAULT)
			{
				if(StartFlag) 
				{ 
			           //if(RelayTestTime == 0) R1_ON;
				   R1_ON;
				   DisplayOffEnableflag = 0;
				}
				else 
				{
					R1_OFF;
					DisplayOffEnableflag = 1;
				}
			}
			else
			{
				R1_OFF;
				DisplayOffEnableflag = 1;
			}
			/*if(!DN_KEY) 
			{
			  R1_OFF;
			  RelayTestflag = 1;
			  RelayTestTime = 100000;
			}*/
		}
		else
		{
			//R1_OFF;
		}
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */	
	unsigned int delay;	
    R1_OFF;
	
	for(delay=0;delay<60000;delay++);
	
	Read_Memory();
	
	if(SetOverAmp >= 500) SetOverAmp = 250; //400; //100;
	if(SetDryRunAmp >= 500) SetDryRunAmp = 10; //10;
	if(SpanVolt >= 999) SpanVolt = 321; //329;
	if(SpanAmp >= 999) SpanAmp = 535; //480; //273; //650; //338; //530; //522; //415; //565;
	if(FinalRef >= 999) FinalRef = 512;
	if(SetOverVolt > 300) SetOverVolt = 300; //290; //280;
	if(SetUnderVolt > 300) SetUnderVolt = 120; // 150;
	
	StartFlag = 0;
	Fault = NO_FAULT;
	UnderAmpTime = UnderVoltTime = OverVoltTime = OverAmpTime = OverAmpResetTime = FAULT_CHECK_mSEC;
	
	
	AutoStartDelayTimeSec = 15000;
	Relayontime = 10000;
	
	R_ADC_Set_OperationOn();
    for (k = 0; k<= 16; k++) {   /* Wait for 1us */  }
    R_ADC_Start();
    R_TAU0_Channel0_Start();
	R_LVD_InterruptMode_Start();
	ADS = 0x00; //Pin1  Volt 
	
	Pon = 1;
	RunTime = 0;
	
	EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void LVDInterrupt(void)
{
	Write_Memory();
}
void TimerInterrupt(void)
{
	//1 ms interrupt
	Display();
	
	/* For CC
	P6.1 = P6.0 = P4.1 = 1U;
	P2.2 = P1.0 = P1.1 = P1.2 = P1.3 = P1.4 = P4.0 = P4.2 = 0U;//must set 1 for key+display
	
	if(~character_table[ssd[disp_no]] & 0x01) P2.2 = 1;  else P2.2 = 0;//PIN 19  a
	if(~character_table[ssd[disp_no]] & 0x02) P1.0 = 1;  else P1.0 = 0;//PIN 17  b
	if(~character_table[ssd[disp_no]] & 0x04) P4.0 = 1;  else P4.0 = 0;//PIN 4   c
	if(~character_table[ssd[disp_no]] & 0x08) P1.3 = 1;  else P1.3 = 0;//PIN 14  d
	if(~character_table[ssd[disp_no]] & 0x10) P1.4 = 1;  else P1.4 = 0;//PIN 13  e
	if(~character_table[ssd[disp_no]] & 0x20) P1.1 = 1;  else P1.1 = 0;//PIN 16  f
	if(~character_table[ssd[disp_no]] & 0x40) P1.2 = 1;  else P1.2 = 0;//PIN 15  g
	if(~character_table[ssd[disp_no]] & 0x80) P4.2 = 1;  else P4.2 = 0;//PIN 2   dp

	if(disp_no == 0) P6.0 = 0U;//D0
	else if(disp_no == 1) P6.1 = 0U;//D1
	else if(disp_no == 2) P4.1 = 0U;//D2
	*/
	//For CA
	P4.2 = P4.0 = P4.1 = 0U;
	P2.2 = P1.0 = P1.1 = P1.2 = P1.3 = P1.4 = P6.0 = P6.1 = 1U;//must set 1 for key+display
	
	if(character_table[ssd[disp_no]] & 0x01) P2.2 = 1;  else P2.2 = 0;//PIN 19  a
	if(character_table[ssd[disp_no]] & 0x02) P1.0 = 1;  else P1.0 = 0;//PIN 17  b
	if(character_table[ssd[disp_no]] & 0x04) P6.0 = 1;  else P6.0 = 0;//PIN 4   c
	if(character_table[ssd[disp_no]] & 0x08) P1.3 = 1;  else P1.3 = 0;//PIN 14  d
	if(character_table[ssd[disp_no]] & 0x10) P1.4 = 1;  else P1.4 = 0;//PIN 13  e
	if(character_table[ssd[disp_no]] & 0x20) P1.1 = 1;  else P1.1 = 0;//PIN 16  f
	if(character_table[ssd[disp_no]] & 0x40) P1.2 = 1;  else P1.2 = 0;//PIN 15  g
	if(character_table[ssd[disp_no]] & 0x80) P6.1 = 1;  else P6.1 = 0;//PIN 2   dp

	     if(disp_no == 0) P4.0 = 1U;//D0
	else if(disp_no == 1) P4.2 = 1U;//D1 
	else if(disp_no == 2) P4.1 = 1U;//D2
	 
	disp_no++;
	if (disp_no >= 3)
	disp_no = 0;
	
	if(++kc >= 200)
	{
		KeyCheck();		
		kc = 0;
	}
	
	if(MessageTime)MessageTime--;
	//if(FaultCheckDelay)FaultCheckDelay--;
	
	/*if(Pon)
	{
		if(AutoStartDelayTimeSec) AutoStartDelayTimeSec--;
		else
		{
			StartFlag = 1; 
			Pon = 0;
		}
	}*/
	
	if(StartFlag)
	{
		if(ScrollTime) ScrollTime--;
		else           ScrollTime = 14000;
	}
	else
	{
		//ScrollTime = 1;//only volt
		if(ScrollTime) ScrollTime--;
		else           ScrollTime = 12000;
	}
		
	if(OverAmpTime)OverAmpTime--;
	if(OverVoltTime)OverVoltTime--;
	if(UnderVoltTime)UnderVoltTime--;
	if(UnderAmpTime)UnderAmpTime--;
	if(OverAmpResetflag == 1)
	{
	  if(OverAmpResetTime)OverAmpResetTime--;
	  if(OverAmpResetTime == 0 && DispVolt <= SetOverVolt - 15 && Relayontime == 0 && Fault != DRY_RUN_FAULT && Fault != UNDER_VOLT_FAULT)
	  {
	    Fault = NO_FAULT;
	  }
	}
	if(RelayTestflag == 1)
	{
	  if(RelayTestTime)RelayTestTime--;
	  if(RelayTestTime == 0)
	  {
	    RelayTestflag = 0;
	  }
	}
	//if (DispVolt <= SetOverVolt - 15)
	if (OverVoltageResetflag == 0)
	{
	  if(Relayontime)Relayontime--;
	}
	if (!CalPara)
	{
	  if(Delaymenu)Delaymenu--;
	}
}
unsigned long sqrt (unsigned long n)
{
    unsigned long root = 0, try;
    iter1 (15);    iter1 (14);    iter1 (13);    iter1 (12);
    iter1 (11);    iter1 (10);    iter1 ( 9);    iter1 ( 8);
    iter1 ( 7);    iter1 ( 6);    iter1 ( 5);    iter1 ( 4);
    iter1 ( 3);    iter1 ( 2);    iter1 ( 1);    iter1 ( 0);
    return root >> 1;
}
void ADCInterrupt(void)
{
	TempRef = ADCValue = ADCR >> 6U;  
	FinalOutput = (int)(ADCValue - FinalRef);//FinalRef
	FinalOutput *= 2;
	 
	++msdelay;
	if(msdelay > 10)  //25.33us*10 = 253.3
	{
		++SamplesTime;
		if(SamplesTime < 1000)//1000
		{
			Average += ((long int)FinalOutput*(long int)FinalOutput);
		}
		else if((FinalOutput<=0) && (LastADCdata>0))
		{
			Average /= ((long int)SamplesTime);
			Tempdata[Count] = Average;	
			if(++testvar >= 1)
			{
				ReadyFlag = 1;
				if(CalFlag == 0)//for other than cal mode ,scanning channel
				{
					ADCInputSelect(); 
				}	
				testvar = 0;
			}
			SamplesTime = 0;
			Average = 0;
		}
		else if(SamplesTime > 3000)//3000
		{
			Tempdata[Count] = 0;	
			ReadyFlag = 1;
			SamplesTime = 0;
			Average = 0;
			if(++testvar >= 1)
			{
				ReadyFlag = 1;
				if(CalFlag == 0)//for other than cal mode ,scanning channel
				{
					ADCInputSelect();
				}
				testvar = 0;
			}
		}
		else
		{
			Average += ((long int)FinalOutput*(long int)FinalOutput);
		}
		LastADCdata = FinalOutput; 
		msdelay = 0;
	}	
	if(ReadyFlag == 1)
	{ 
		if(Count == 0)
		{
			FinalValue[0] = sqrt(Tempdata[0]); 
			ReadyFlag = 0; 
			FinalValue[0] *= 240; 
			FinalValue[0] /= SpanVolt;
			if(FinalValue[0] < 10) FinalValue[0] = 0;
			
			DispVolt = (int)FinalValue[0];
		}
		else if(Count == 1)
		{
			FinalValue[1] = sqrt(Tempdata[1]);
			ReadyFlag = 0; 
			FinalValue[1] *= 50000;
			FinalValue[1] /= SpanAmp;
			FinalValue[1] *= CT; //40
			FinalValue[1] /= 5000;		
			
			if(FinalValue[1] < 5) //10
				FinalValue[1] = 0;
			
			DispAmp = (int)FinalValue[1];
		}
	} 
}
void ADCInputSelect(void)   
{ 
	if(++Count >= 2)   
	{	
		Count = 0; 
	} 
	if(Count == 0 )
	{   
		ADS = 0x00; //Pin1  Volt
 	}
	else if(Count == 1 )
	{
		ADS = 0x01;  //Pin20  Ampere
	} 
	else 
		Count = 0;  
}
void FaultChecking(void)
{
	if(DispAmp && (DispAmp >= SetOverAmp) && (Fault == NO_FAULT))
	{
		if(OverAmpTime == 0)
		{
			Fault = OVER_AMP_FAULT;
		}
		if(DispAmp >= 400)            // amp is greater then 40 amp then instant trip
		{
		        Fault = OVER_AMP_FAULT;
		}
		OverAmpResetTime = 550000;
	}
	else 
	{
		OverAmpTime = 32000; //FAULT_CHECK_mSEC;
		OverAmpResetflag = 1;
	}
		
	if(DispVolt && (DispVolt >= SetOverVolt) && (Fault == NO_FAULT))
	{
		if(OverVoltTime == 0)
		{
			Fault = OVER_VOLT_FAULT;
			Relayontime = 25000;    // Relay on time after hv error happen
		}
		OverVoltageResetflag = 1;
		StartFlag = 0;
	}
	else 
	{
		OverVoltTime = 10; //500; //FAULT_CHECK_mSEC;
		if (DispVolt <= SetOverVolt - 15)
		{
	          OverVoltageResetflag = 0;
		  if (DispAmp <= SetOverAmp) 
		  {
		    if (Relayontime == 0 && Fault != OVER_AMP_FAULT && Fault != DRY_RUN_FAULT && Fault != UNDER_VOLT_FAULT) 
		    {
		      //StartFlag = 1;
		      Fault = NO_FAULT;
		    }
		  }
		}
	}
		
	if(DispVolt && (DispVolt <= 120) && (Fault == NO_FAULT))   // Low Voltage
	{
		if(UnderVoltTime == 0)
		{
			Fault = UNDER_VOLT_FAULT;
		}
	}
	else 
	{
		UnderVoltTime = FAULT_CHECK_mSEC;
		if (DispVolt >= 135)
		{
		  if (DispAmp <= SetOverAmp) 
		  {
		    if (Relayontime == 0 && Fault != OVER_AMP_FAULT && Fault != DRY_RUN_FAULT && Fault != OVER_VOLT_FAULT) 
		    {
		      Fault = NO_FAULT;
		    }
		  }
		}
	}
		
	if(DispAmp && (DispAmp <= SetDryRunAmp) && (Fault == NO_FAULT))
	{
		if(UnderAmpTime == 0)
		{
			Fault = DRY_RUN_FAULT;
		}
	}
	else
		UnderAmpTime = FAULT_CHECK_mSEC;
		
}
void Display(void)
{ 
	if(CalPara)
	{
		if(CalFlag)
		{
			if(CalPara == CAL_ZERO)
			{
				FinalRef = TempRef;
				
				if(MessageTime)
				{
					ssd[0] = 0;
					ssd[1] = R;
					ssd[2] = 2;
				}
				else
				{
					Bin2Bcd(FinalRef);
					ssd[0] = digit[0];
					ssd[1] = digit[1];
					ssd[2] = digit[2];
				}
			}
			else if(CalPara == CAL_240V)
			{
				SpanVolt = (unsigned int)sqrt(Tempdata[0]); 
				
				if(MessageTime)
				{
					ssd[0] = 0;
					ssd[1] = 4;
					ssd[2] = 2;
				}
				else
				{
					Bin2Bcd(SpanVolt);
					ssd[0] = digit[0];
					ssd[1] = digit[1];
					ssd[2] = digit[2];
				}
			}
			else if(CalPara == CAL_30A)
			{ 
				SpanAmp = (unsigned int)sqrt(Tempdata[1]);  
				
				if(MessageTime)
				{
					ssd[0] = A;
					ssd[1] = 0;
					ssd[2] = 3;
				}
				else
				{
					Bin2Bcd(SpanAmp);
					ssd[0] = digit[0];
					ssd[1] = digit[1];
					ssd[2] = digit[2];
				}
			}
		}
		else
		{
			if(CalPara == MSG_HV)
			{
				ssd[0] = BLANK;
				ssd[1] = U;
				ssd[2] = H;
			}
			else if(CalPara == MSG_UV)
			{
				ssd[0] = BLANK;
				ssd[1] = V;
				ssd[2] = U;
			}
			else if(CalPara == MSG_OL)
			{
				ssd[0] = BLANK;
				ssd[1] = L;
				ssd[2] = 0;
			}
			else if(CalPara == MSG_UC)
			{
				ssd[0] = Y; //BLANK;
				ssd[1] = R ;//C;
				ssd[2] = D ;//U;
			}
			else if(CalPara == MSG_SET)
			{
				ssd[0] = T;
				ssd[1] = E;
				ssd[2] = S;
			}
			else if(CalPara == SET_OVER_VOLT)
			{
				Bin2Bcd(SetOverVolt);
				ssd[0] = digit[0];
				ssd[1] = digit[1];
				ssd[2] = digit[2];
			}
			else if(CalPara == SET_UNDER_VOLT)
			{
				Bin2Bcd(SetUnderVolt);
				ssd[0] = digit[0];
				ssd[1] = digit[1];
				ssd[2] = digit[2];
			}
			else if(CalPara == SET_OVER_AMP)
			{
				Bin2Bcd(SetOverAmp);
				ssd[0] = digit[0];
				ssd[1] = digit[1]+DP;
				ssd[2] = digit[2];
			}
			else if(CalPara == SET_DRYRUN_AMP)
			{
				Bin2Bcd(SetDryRunAmp);
				ssd[0] = digit[0];
				ssd[1] = digit[1]+DP;
				ssd[2] = digit[2];
			}
		}
	}
	else
	{
		if(Fault)
		{
			if(Fault == OVER_VOLT_FAULT)
			{
				ssd[0] = U;
				ssd[1] = H;
				ssd[2] = BLANK;
			}
			else if(Fault == OVER_AMP_FAULT)
			{
				ssd[0] = BLANK;
				ssd[1] = L;
				ssd[2] = 0;
			}
			else if(Fault == DRY_RUN_FAULT)
			{
				ssd[0] = Y; //F;
				ssd[1] = R; //C;
				ssd[2] = D; //U;
			}
			else if(Fault == UNDER_VOLT_FAULT)
			{
				ssd[0] = U;
				ssd[1] = L;
				ssd[2] = BLANK;
			}
		}
		else
		{
	          if (menumodeflag)
		  {
			/*if(CalPara == MSG_SET)
	                {
		          menumodeflag = 0;
	                }*/
		    
		  }
		  else
		  {
			if(ScrollTime > 13000 && DisplayOffEnableflag == 0)// > 9sec means for 1 sec
			{
				ssd[0] = BLANK;
				ssd[1] = A;
				ssd[2] = BLANK;
			}
			else if(ScrollTime > 9000 && DisplayOffEnableflag == 0)// > 5sec means for 4 sec
			{
				Bin2Bcd(DispAmp);
				ssd[0] = digit[0];
				ssd[1] = digit[1]+DP;
				ssd[2] = digit[2];
			}
			else if(ScrollTime > 9000 && DisplayOffEnableflag == 1)
			{
			 	ssd[0] = F;
				ssd[1] = F;
				ssd[2] = O;
			}
			else if(ScrollTime > 8000 && DisplayOffEnableflag == 0)// > 4sec means for 1 sec
			{
				ssd[0] = BLANK;
				ssd[1] = U;
				ssd[2] = BLANK;
			}
			else //for last 4 sec
			{
				Bin2Bcd(DispVolt);
				ssd[0] = digit[0];
				ssd[1] = digit[1];
				ssd[2] = digit[2];
			}
		  }
		}
	}
}
void SetKey(void)  
{ 
	AutoOut = 0;
	MessageTime = 2000;
	
	if(!CalPara)
	{
		  if(StartFlag != 1)
	          { 
		     CalPara = MSG_OL;
		     menumodeflag = 1;
		  }
		  else
		  {
		     CalPara = MSG_SET;
		    // menumodeflag = 0;
		  }
	}
	else
	{
		//CalPara++;
		if(CalFlag)
		{
			CalPara++;
			if(CalPara == CAL_240V)
			{
				ADS = 0x00; //Pin1  Volt
				Count = 0;
			}
			else if(CalPara == CAL_30A)
			{
				ADS = 0x01;  //Pin20  Ampere
				Count = 1;
			}
			
			if(CalPara > CAL_30A)
			{
				CalPara = 0;
				Write_Memory();
				CalFlag = 0;
				KeySense = 0;
				ScrollTime = 14000;
			}
		}
		else
		{
			Write_Memory(); 
			
			/*if(CalPara == MSG_HV)
	                {
		          CalPara = MSG_OL;
		          KeySense = 0;
	                }*/
	                if(CalPara == MSG_OL /*&& KeyDelay == 0*/)   
	                {
		          CalPara = MSG_UC; //MSG_HV;
		          KeySense = 0;
	                }
			else if(CalPara == SET_OVER_AMP /*&& KeyDelay == 0*/)
			{
			  CalPara = MSG_UC; //MSG_HV;
		          KeySense = 0;
			  //menumodeflag = 0;
			}
			else if((CalPara == MSG_UC || CalPara == SET_DRYRUN_AMP) && KeyDelay == 0)
	                {
		          CalPara = MSG_SET;
		          KeySense = 0;
			  menumodeflag = 0;
			  Delaymenu = 0;
	                }
			else if(CalPara == MSG_SET)
			{
			  menumodeflag = 0;
			  KeySense = 0;
			}
			
		}
	}
}
void UPKey(void)
{
	AutoOut = 0;
	
	if(CalPara == MSG_HV)
	{
	  CalPara = SET_OVER_VOLT;
	}
	else if(CalPara == MSG_OL)
	{
          CalPara = SET_OVER_AMP;
	}
	else if(CalPara == MSG_UC)
	{
          CalPara = SET_DRYRUN_AMP;
	}
			
	if(CalPara == SET_OVER_VOLT)
	{
		if(++SetOverVolt >= MAX_VOLT)
		{
			SetOverVolt = MAX_VOLT;
		}
	}
	else if(CalPara == SET_UNDER_VOLT)
	{
		if(++SetUnderVolt >= (SetOverVolt-20))
		{
			SetUnderVolt = (SetOverVolt-20);
		}
	}
	else if(CalPara == SET_OVER_AMP)
	{
		if(++SetOverAmp >= MAX_AMP)
		{
			SetOverAmp = MAX_AMP;
		}
	}
	else if(CalPara == SET_DRYRUN_AMP)
 	{
		//if (dryRun < (uint16_t)(overLoad * 0.8)) dryRun++;
		if(++SetDryRunAmp >= (SetOverAmp - 20))//MAX_AMP-1.0A
		//if(++SetDryRunAmp < (uint16_t)(SetOverAmp * 0.8))//MAX_AMP-1.0A
		{
			SetDryRunAmp = (SetOverAmp - 20);
		}
	}
}
void DownKey(void)
{
	AutoOut = 0;
	
	if(CalPara == MSG_HV)
	{
	  CalPara = SET_OVER_VOLT;
	}
	else if(CalPara == MSG_OL)
	{
          CalPara = SET_OVER_AMP;
	}
	else if(CalPara == MSG_UC)
	{
          CalPara = SET_DRYRUN_AMP;
	}
	if(CalPara == SET_OVER_VOLT)
	{
		if(SetOverVolt > 260)	SetOverVolt--;
	}
	else if(CalPara == SET_UNDER_VOLT)
 	{
		if(SetUnderVolt > MIN_VOLT) SetUnderVolt--;
	}
	else if(CalPara == SET_OVER_AMP)
	{
		if(SetOverAmp > 20)SetOverAmp--;
	}
	else if(CalPara == SET_DRYRUN_AMP)
 	{
		if(SetDryRunAmp > 1)SetDryRunAmp--;
	}
}
void KeyCheck(void)
{
	if(!SET_KEY && !UP_KEY && !DN_KEY)// set+up+dn
	{
		if(++KeyDelay >= 50)//5sec  means 50
		{
			KeyDelay = 0;
			CalFlag = 1;
			CalPara = CAL_ZERO;
			MessageTime = 2000;
			ADS = 0x01;  //Pin20  Ampere for ZERO CAL Count Display Proper
			KeySense = 0;
			//CalPara = MSG_SET;
			Delaymenu = 0;
			menumodeflag = 0;
		}
	}
	else if(!UP_KEY)     // stop relay
	{
		/*if(CalPara == 0)
	        {
	          StartFlag = 1; 
	        }*/
		if(CalPara)
		{
			DownKey();
		}
		else
		{
			if(Fault)
			{
				Fault = NO_FAULT;
				//StartFlag = 0;
				ScrollTime = 14000;
			}
			else
			{
			        if (CalPara != MSG_OL && CalPara != MSG_UC && CalPara != MSG_SET)
				{
					StartFlag = 0;//OFF;
				//if(StartFlag)
					//StartFlag = 0;//OFF;
				}
			}	
		}
	}
	else if(!DN_KEY)      // Start Relay
	{
		
		if(CalPara)
		{
			if((CalPara == SET_OVER_VOLT) || (CalPara == SET_UNDER_VOLT) || (CalPara == SET_OVER_AMP) || (CalPara == SET_DRYRUN_AMP))
			{
				KeySense = 1;
			}
			
			if(KeySense)
			{
				UPKey();
				KeySense = 0;
			}
		}
		else
		{
			//if(++KeyDelay >= 3)//300ms
			//{
				KeyDelay = 0;
				if (CalPara != MSG_OL && CalPara != MSG_UC && CalPara != MSG_SET)
				{
				if(!StartFlag)
				{
					StartFlag = 1;
				}
				}
			//}	
		}
	}
	else if(!SET_KEY)
	{
		if (Delaymenu == 0)
		{
	        if(++KeyDelay >= 10) // 30
		{
		  KeySense = 1;
		  Delaymenu = 1;
		}
		
		}
		if(KeySense)
		{
			//Delaymenu = 20000;
			//if (Delaymenu){
			SetKey();
			KeySense = 0;
			//}
		}
	}
	else 
	{
		if (Delaymenu == 1) KeySense = 1;
		//KeySense = 1;
		KeyDelay = 0;
		
		if(CalPara > CAL_30A)
		{
			if(menumodeflag == 0)
			{
				if(++AutoOut >= 30)  // 50 //100 * 100 = 10000 = 10 sec
				{
					AutoOut = 0;
					CalPara = 0;
					if (StartFlag == 1) {
					SetOverAmp =  (uint16_t)(DispAmp * 1.2) ;
                                	SetDryRunAmp = (uint16_t)(DispAmp * 0.8);
                                	if (SetOverAmp > 250)  SetOverAmp = 250;
                                	if (SetOverAmp < 20)  SetOverAmp = 20;
                                	if (SetDryRunAmp < 1)  SetDryRunAmp = 1;
					}
				
					Write_Memory();
					//StartFlag = 0;
					ScrollTime = 14000;
					//menumodeflag = 0;
				}
			}
		}
	}
}
void Bin2Bcd(unsigned int x)
{
    digit[2] = digit[1] = digit[0] = 0; 
	
    while(x >= 100)
    {
	    digit[2]++;
        x-=100;  
	} 
	
    while(x >= 10)
    {
	    digit[1]++;
        x-=10;  
	} 
	
	digit[0] = (char)x;
}
//************************************** Flash Memory Functions ***********************************************
void Write_Memory(void)
{
	FDL_Status=PFDL_Open(&FDL_SysInit);
	if (FDL_Status!= PFDL_OK)
	{
		while(1); 			//FDL initialization failed, user may insert RESET function
	}

	FDL_Erase();
	if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
	{
		FDL_Erase();
	}
	
	Write_DataBuffer[0]= SpanAmp >> 8;
	Write_DataBuffer[1]= SpanAmp;

	Write_DataBuffer[2]= SpanVolt >> 8;
	Write_DataBuffer[3]= SpanVolt;
	
	Write_DataBuffer[4]= FinalRef >> 8;
	Write_DataBuffer[5]= FinalRef;
	
	Write_DataBuffer[6]= SetOverAmp >> 8;
	Write_DataBuffer[7]= SetOverAmp;
	
	Write_DataBuffer[8]= SetDryRunAmp >> 8;
	Write_DataBuffer[9]= SetDryRunAmp;
	
	Write_DataBuffer[10]= SetOverVolt >> 8;
	Write_DataBuffer[11]= SetOverVolt;
	
	Write_DataBuffer[12]= SetUnderVolt >> 8;
	Write_DataBuffer[13]= SetUnderVolt;
	
	FDL_Write();				//Execute Write command
	PFDL_Close(); 				//Disable FDL library
	//DataFlash_Write();
} 
void Read_Memory(void)
{
	//DataFlash_Read();
	FDL_Status=PFDL_Open(&FDL_SysInit);
	if (FDL_Status!= PFDL_OK)
	{
		while(1); 			//FDL initialization failed, user may insert RESET function
	}
	
	FDL_Read();	
	
	SpanAmp = Read_DataBuffer[0];
	SpanAmp = (SpanAmp << 8) | Read_DataBuffer[1];

	SpanVolt = Read_DataBuffer[2];
	SpanVolt = (SpanVolt << 8) | Read_DataBuffer[3];

	FinalRef = Read_DataBuffer[4];
	FinalRef= (FinalRef << 8) | Read_DataBuffer[5];

	SetOverAmp = Read_DataBuffer[6];
	SetOverAmp = (SetOverAmp << 8) | Read_DataBuffer[7];

	SetDryRunAmp = Read_DataBuffer[8];
	SetDryRunAmp = (SetDryRunAmp << 8) | Read_DataBuffer[9];
	
	SetOverVolt = Read_DataBuffer[10];
	SetOverVolt = (SetOverVolt << 8) | Read_DataBuffer[11];
	
	SetUnderVolt = Read_DataBuffer[12];
	SetUnderVolt = (SetUnderVolt << 8) | Read_DataBuffer[13];
	
	PFDL_Close(); 				//Disable FDL library
}
void DataFlash_Read(void)
{ 
	FDL_Status=PFDL_Open(&FDL_SysInit);
    	if (FDL_Status!= PFDL_OK)
    	{
		while(1); 			//FDL initialization failed, user may insert RESET function
    	}
  //  	FDL_Blank_Chk(); 
  //  	if (FDL_Status==PFDL_ERR_MARGIN) 	//Execute Erase comamnd if Blank Check error
    	{
   // 	FDL_Erase();				//Execute Erase command
	//	if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
		{
	//		 while(1); 		//FDL Erase fucntion failed, user may insert RESET function
		}
    	}
    //	FDL_Erase();
    	if (FDL_Status==PFDL_ERR_ERASE) 	//if Erase error
	{
		while(1); 			//FDL Erase fucntion failed, user may insert RESET function
	}
	
	/*FDL_Read();				//Execute Read command 
	FDL_VersionChk=PFDL_GetVersionString();
	FDL_status_chk();*/
    
	//FDL_Write();				//Execute Write command
    	if (FDL_Status==PFDL_ERR_MARGIN) 	//Execute Erase comamnd if Blank Check error
    	{
    		FDL_Erase();			//Execute Erase command
		if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
		{
			 while(1); 		//FDL Erase fucntion failed, user may insert RESET function
		}
    	}
    	FDL_Verify();				//Execute Verify command
    	if (FDL_Status==PFDL_ERR_MARGIN) 	//Execute Erase comamnd if verify error
    	{
    		FDL_Erase();			//Execute Erase command
		if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
		{
			 while(1); 		//FDL Erase fucntion failed, user may insert RESET function
		}
    	}
    
    	FDL_Read();				//Execute Read command 
    	FDL_VersionChk=PFDL_GetVersionString();
    	FDL_status_chk();
    
    	PFDL_Close(); 				//Disable FDL library
}

void DataFlash_Write(void) 
{
	FDL_Status=PFDL_Open(&FDL_SysInit);
	if (FDL_Status!= PFDL_OK)
    	{
	    while(1); 				//FDL initialization failed, user may insert RESET function
    	}
    	FDL_Blank_Chk();
    	if (FDL_Status==PFDL_ERR_MARGIN) 	//Execute Erase comamnd if Blank Check error
    	{
    		FDL_Erase();			//Execute Erase command
		if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
		{
			 while(1); 		//FDL Erase fucntion failed, user may insert RESET function
		}
    	}
    	FDL_Erase();
    	if (FDL_Status==PFDL_ERR_ERASE) 	//if Erase error
	{
		 while(1); 			//FDL Erase fucntion failed, user may insert RESET function
	}
	
	/*FDL_Read();				//Execute Read command 
    	FDL_VersionChk=PFDL_GetVersionString();
    	FDL_status_chk();
   	*/ 
    	FDL_Write();				//Execute Write command

    	if (FDL_Status==PFDL_ERR_MARGIN) 	//Execute Erase comamnd if Blank Check error
    	{
    		FDL_Erase();			//Execute Erase command
		if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
		{
			 while(1); 		//FDL Erase fucntion failed, user may insert RESET function
		}
    	}
    
    	FDL_Verify();				//Execute Verify command
    	if (FDL_Status==PFDL_ERR_MARGIN) 	//Execute Erase comamnd if verify error
    	{
    		FDL_Erase();			//Execute Erase command
		if (FDL_Status==PFDL_ERR_ERASE) //if Erase error
		{
			 while(1); 		//FDL Erase fucntion failed, user may insert RESET function
		}
    	}  
   	FDL_Read();				//Execute Read command 
   	FDL_VersionChk=PFDL_GetVersionString();
    	FDL_status_chk();
    	PFDL_Close(); 				//Disable FDL library
}


void FDL_Blank_Chk(void)
{
	FDL_Request.index_u16=0x0000;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.bytecount_u16=Data_Count;			//Counter for number of bytes to be check
	FDL_Request.command_enu =PFDL_CMD_BLANKCHECK_BYTES;	//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk();					//Check status of FDL Handler
}

void FDL_status_chk(void)
{
	FDL_Status=PFDL_BUSY;
	while (FDL_Status==PFDL_BUSY)
	{
		FDL_Status=PFDL_Handler();
	}	
}

void FDL_Erase(void)
{
	FDL_Request.index_u16=0;  				//Assign the block 0 of data flash to be erase Data Flash 
								//0: block 0 (0xF1000 to 0xF13FF) 
								//1: block 1 (0xF1400 to 0xF17FF)
								//2: block 2 (0xF1800 to 0xF1BFF)
								//3: block 3 (0xF1C00 to 0xF1FFF)
	FDL_Request.command_enu =PFDL_CMD_ERASE_BLOCK;		//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk();					//Check status of FDL Handler
}

void FDL_Write(void)
{
	FDL_Request.index_u16=0x0000;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.data_pu08=Write_DataBuffer; 		//Assign the address of Write_DataBuffer as data to be stored in Data Flash
	FDL_Request.bytecount_u16=Data_Count;			//Counter for number of bytes to be write
	FDL_Request.command_enu =PFDL_CMD_WRITE_BYTES;		//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the Blank Check on data flash
	FDL_status_chk();					//Check status of FDL Handler	
}

void FDL_Verify(void)
{
	FDL_Request.index_u16=0x0000;  				//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.bytecount_u16=Data_Count;			//Counter for number of bytes to be verify
	FDL_Request.command_enu =PFDL_CMD_IVERIFY_BYTES;	//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the data verify on data flash
	FDL_status_chk();
}

void FDL_Read(void)
{
	FDL_Request.index_u16=0x0000;  				//Read from 0xF1000h; 
								//Start Address of Data Flash (For R5F100LE, Data Flash range from (0xF1000 - 0xF1FFF)
	FDL_Request.data_pu08=Read_DataBuffer; 			//Assign the address of Rea\d_DataBuffer to stored contents of  dedicated Data Flash area
	FDL_Request.bytecount_u16=Data_Count;			//Counter for number of bytes to be read
	FDL_Request.command_enu =PFDL_CMD_READ_BYTES;		//Assign the command to be execute by PFDL_Execute
	PFDL_Execute(&FDL_Request);				//Execute the read command on data flash
	FDL_status_chk();
}
//************************************************************************************************************* 

/* End user code. Do not edit comment generated here */
