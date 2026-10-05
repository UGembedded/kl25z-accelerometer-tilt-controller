
#include "PWM.h"
#include "MKL25Z4.h"
#include "board.h"
#include "i2c.h"
#include <stdio.h>  // for sprintf
#include <math.h>   // for fabs
// #include "LEDDriver.h"






#include "UART0_TXRX2.h"

/******************************************************************************
* Constants and macros
******************************************************************************/

//MMA8451Q Registers

#define STATUS_REG            0x00		// STATUS Register 

#define OUT_X_MSB_REG         0x01		// [7:0] are 8 MSBs of the 14-bit X-axis sample
#define OUT_X_LSB_REG         0x02		// [7:2] are the 6 LSB of 14-bit X-axis sample
#define OUT_Y_MSB_REG         0x03		// [7:0] are 8 MSBs of the 14-bit Y-axis sample
#define OUT_Y_LSB_REG         0x04		// [7:2] are the 6 LSB of 14-bit Y-axis sample
#define OUT_Z_MSB_REG         0x05		// [7:0] are 8 MSBs of the 14-bit Z-axis sample
#define OUT_Z_LSB_REG         0x06		// [7:2] are the 6 LSB of 14-bit Z-axis sample

#define F_SETUP_REG           0x09    	// F_SETUP FIFO Setup Register 
#define TRIG_CFG_REG          0x0A    	// TRIG_CFG Map of FIFO data capture events 
#define SYSMOD_REG            0x0B    	// SYSMOD System Mode Register 
#define INT_SOURCE_REG        0x0C    	// INT_SOURCE System Interrupt Status Register 
#define WHO_AM_I_REG          0x0D    	// WHO_AM_I Device ID Register 
#define XYZ_DATA_CFG_REG      0x0E    	// XYZ_DATA_CFG Sensor Data Configuration Register 
#define HP_FILTER_CUTOFF_REG  0x0F    	// HP_FILTER_CUTOFF High Pass Filter Register 

#define PL_STATUS_REG         0x10    	// PL_STATUS Portrait/Landscape Status Register 
#define PL_CFG_REG            0x11    	// PL_CFG Portrait/Landscape Configuration Register 
#define PL_COUNT_REG          0x12    	// PL_COUNT Portrait/Landscape Debounce Register 
#define PL_BF_ZCOMP_REG       0x13    	// PL_BF_ZCOMP Back/Front and Z Compensation Register 
#define P_L_THS_REG           0x14    	// P_L_THS Portrait to Landscape Threshold Register 

#define FF_MT_CFG_REG         0x15    	// FF_MT_CFG Freefall and Motion Configuration Register 
#define FF_MT_SRC_REG         0x16    	// FF_MT_SRC Freefall and Motion Source Register 
#define FT_MT_THS_REG         0x17    	// FF_MT_THS Freefall and Motion Threshold Register 
#define FF_MT_COUNT_REG       0x18    	// FF_MT_COUNT Freefall Motion Count Register 

#define TRANSIENT_CFG_REG     0x1D    	// TRANSIENT_CFG Transient Configuration Register 
#define TRANSIENT_SRC_REG     0x1E    	// TRANSIENT_SRC Transient Source Register 
#define TRANSIENT_THS_REG     0x1F    	// TRANSIENT_THS Transient Threshold Register 
#define TRANSIENT_COUNT_REG   0x20    	// TRANSIENT_COUNT Transient Debounce Counter Register 

#define PULSE_CFG_REG         0x21    	// PULSE_CFG Pulse Configuration Register 
#define PULSE_SRC_REG         0x22    	// PULSE_SRC Pulse Source Register 
#define PULSE_THSX_REG        0x23    	// PULSE_THS XYZ Pulse Threshold Registers 
#define PULSE_THSY_REG        0x24
#define PULSE_THSZ_REG        0x25
#define PULSE_TMLT_REG        0x26    	// PULSE_TMLT Pulse Time Window Register 
#define PULSE_LTCY_REG        0x27    	// PULSE_LTCY Pulse Latency Timer Register 
#define PULSE_WIND_REG        0x28    	// PULSE_WIND Second Pulse Time Window Register 

#define ASLP_COUNT_REG        0x29    	// ASLP_COUNT Auto Sleep Inactivity Timer Register 

#define CTRL_REG1             0x2A    	// CTRL_REG1 System Control 1 Register 
#define CTRL_REG2             0x2B    	// CTRL_REG2 System Control 2 Register 
#define CTRL_REG3             0x2C    	// CTRL_REG3 Interrupt Control Register 
#define CTRL_REG4             0x2D    	// CTRL_REG4 Interrupt Enable Register 
#define CTRL_REG5             0x2E    	// CTRL_REG5 Interrupt Configuration Register 

#define OFF_X_REG             0x2F    	// XYZ Offset Correction Registers 
#define OFF_Y_REG             0x30
#define OFF_Z_REG             0x31

//MMA8451Q 7-bit I2C address

#define MMA845x_I2C_ADDRESS   0x1D		// SA0 pin = 1 -> 7-bit I2C address is 0x1D 


/******************************************************************************
* Global variables
******************************************************************************/

unsigned char AccData[6];
short Xout_14_bit, Yout_14_bit, Zout_14_bit;
float Xout_g, Yout_g, Zout_g;
char DataReady;
char Xoffset, Yoffset, Zoffset;
volatile int app_error = APP_OK; // Driver error; inspect in the debugger.

/******************************************************************************
* Functions
******************************************************************************/

void MCU_Init(void);
void Accelerometer_Init(void);
void readAccXYZ(void);
void LED_init(void);

/******************************************************************************
* Main
******************************************************************************/  

char buf [100];   // UART buffer 
int main (void)
{

	DataReady = 0;

	MCU_Init(); // Clock/timebase setup must precede UART and PWM.
	if (app_error != APP_OK) { while (1) { } }
	LED_init();
	app_error = UART0_init();  // Initialized UART0, 57600 baud
	if (app_error != APP_OK) { while (1) { } }
	sendHelloWorld();
	Accelerometer_Init();
	if (app_error != APP_OK) {
		sendStr("Accelerometer init failed\r\n",
		        sizeof("Accelerometer init failed\r\n") - 1);
		while (1) { }
	}
	sendStr("Init OK2\r\n", sizeof("Init OK2\r\n") - 1);

	
	
	
	
  	while(1)
    {
	readAccXYZ();
		}
	
}


void LED_init(void) {
	// Keep the system clock established by startup and checked in MCU_Init().
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
    SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_TPMSRC_MASK)
                | SIM_SOPT2_TPMSRC(1);

    TPM2->SC = TPM_SC_CMOD(1) | TPM_SC_PS(0);
    TPM2->MOD = 16383;

    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    PORTB->PCR[18] = PORT_PCR_MUX(3); // Configure PTB18 as PWM output
    PORTB->PCR[19] = PORT_PCR_MUX(3); // Configure PTB19 as PWM output

    PTB->PDDR |= (1 << 18) | (1 << 19); // Set PTB18 and PTB19 as outputs

    TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	PORTD->PCR[blue_led] &=~PORT_PCR_MUX_MASK;
	PORTD->PCR[blue_led] |=PORT_PCR_MUX(4);

	// Preserve PLLFLLSEL: Board_Init selected the source for these clocks.

	TPM0->MOD = 16383;

	TPM0->SC=TPM_SC_PS(1);
	TPM0->CONF |=TPM_CONF_DBGMODE(3);

	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	TPM0->CONTROLS[1].CnV = 0;

	TPM0->SC |=TPM_SC_CMOD(1);
}

/******************************************************************************
* MCU initialization function
******************************************************************************/ 

void MCU_Init(void)
{
    app_error = Board_Init();
    if (app_error != APP_OK) { return; }
    app_error = i2c_init();
    // No PTA14/PORTA interrupt: readAccXYZ() polls the sensor's status.
}

/******************************************************************************
* Accelerometer initialization function
******************************************************************************/ 

void Accelerometer_Init (void)
{
	unsigned char reg_val = 0;
	int errCode;
	// sendStr("Bf i2cwrite\r\n",13);
	errCode=i2c_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x40);		// Reset all registers to POR values
	int n = sprintf(buf, "Acc Init() errCode=%d\r\n", errCode);
	sendStr(buf,n);
	app_error = errCode;
	if (app_error != APP_OK) { return; }
    Board_DelayMs(2U); // Allow the sensor reset to complete before polling.
	do		// Wait for the RST bit to clear 
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, CTRL_REG2) & 0x40;
        app_error = I2C_LastError;
        if (app_error != APP_OK) { return; } 
	} 	while (reg_val);
	sendStr("CTRL_REG2 & 0x40 reg_val OK\r\n",
            sizeof("CTRL_REG2 & 0x40 reg_val OK\r\n") - 1);
	
	app_error = i2c_WriteRegister(MMA845x_I2C_ADDRESS, XYZ_DATA_CFG_REG, 0x00);		// +/-2g range -> 1g = 16384/4 = 4096 counts 
    if (app_error != APP_OK) { return; }
	app_error = i2c_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x02);		// High Resolution mode
    if (app_error != APP_OK) { return; }
	app_error = i2c_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x3D);	// ODR = 1.56Hz, Reduced noise, Active mode	
    if (app_error != APP_OK) { return; }
}

/******************************************************************************
* Simple read xyz acceleration from MMA8451Q
******************************************************************************/ 

void readAccXYZ (void)
{
	int n;
	unsigned char reg_val = 0;
    if (app_error != APP_OK) { return; }
	while (!reg_val)		// Wait for a first set of data		 
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, STATUS_REG) & 0x08;
        app_error = I2C_LastError;
        if (app_error != APP_OK) { return; } 
		//n=sprintf(buf,"reg_val=%d\r\n",reg_val);
	    //sendStr(buf,n); 
	} 	
	//int n=printf(buf,"readAccXYZ() reg_val=%d\r\n",reg_val);
	// sendStr(buf,n); 
	
	
	app_error = I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);		// Read data output registers 0x01-0x06  
	
    if (app_error != APP_OK) { return; }

	Xout_14_bit = ((short) (AccData[0]<<8 | AccData[1])) >> 2;		// Compute 14-bit X-axis output value
	Yout_14_bit = ((short) (AccData[2]<<8 | AccData[3])) >> 2;		// Compute 14-bit Y-axis output value
	Zout_14_bit = ((short) (AccData[4]<<8 | AccData[5])) >> 2;		// Compute 14-bit Z-axis output value

	float acceleration_x = (float)(((short)(AccData[0] << 8 | AccData[1])) >> 2) / 4096.0f;
	float acceleration_y = (float)(((short)(AccData[2] << 8 | AccData[3])) >> 2) / 4096.0f;
	float acceleration_z = (float)(((short)(AccData[4] << 8 | AccData[5])) >> 2) / 4096.0f;

	acceleration_x=fabs(acceleration_x);
	acceleration_y=fabs(acceleration_y);
	acceleration_z=fabs(acceleration_z);

	acceleration_x=round(acceleration_x * 100.0) / 100.0;
	acceleration_y=round(acceleration_y * 100.0) / 100.0;
	acceleration_z=round(acceleration_z * 100.0) / 100.0;

	if(acceleration_x >= 1) {
		TPM2->CONTROLS[0].CnV = TPM2->MOD + 1U;
	} else {
		TPM2->CONTROLS[0].CnV = (TPM2->MOD + 1U) * acceleration_x;
	}

	if(acceleration_y >= 1) {
		TPM2->CONTROLS[1].CnV = TPM2->MOD + 1U;
	} else {
		TPM2->CONTROLS[1].CnV = (TPM2->MOD + 1U) * acceleration_y;
	}

	if(acceleration_z >= 1) {
		TPM0->CONTROLS[1].CnV = TPM0->MOD + 1U;
	} else {
		TPM0->CONTROLS[1].CnV = (TPM0->MOD + 1U) * acceleration_z;
	}
	n = sprintf(buf, "x=%f ", acceleration_x); 
	sendStr(buf, n);
	n = sprintf(buf, "y=%f ", acceleration_y); 
	sendStr(buf, n);
	n = sprintf(buf, "z=%f \r\n", acceleration_z); 
	sendStr(buf, n);
	
}
