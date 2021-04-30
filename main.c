/*
 *
 * Created: 2019-03-22 09:32:30
 * Author : svni
 */ 
//****main.c****//

#define F_CPU 16000000UL
#include "lcd.h"
#include "i2c.h"
#include "font.h"
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "sonar.h"

#include "MPU6050_res_define.h"							// Include MPU6050 register define file 
#include "I2C_Master_H_file.h"							// Include I2C Master header file 

void MPU6050_Init();	
void Read_RawValue();
void MPU_Start_Loc();

float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;

/***********************************************************************************
 * Name: MPU6050_Init
 * Purpose: To initialize MPU
 * Input: void
 * Returns: void
 * To call: I2C_Start_Wait(), I2C_Write(), I2C_Stop()
************************************************************************************/
void MPU6050_Init()										// Gyro initialization function 
{
	_delay_ms(150);										// Power up time >100ms 
	I2C_Start_Wait(0xD0);								// Start with device write address 
	I2C_Write(SMPLRT_DIV);								// Write to sample rate register 
	I2C_Write(0x07);									// 1KHz sample rate 
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								// Write to power management register 
	I2C_Write(0x01);									// X axis gyroscope reference frequency 
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									// Write to Configuration register 
	I2C_Write(0x00);									// Fs = 16MHz */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								// Write to Gyro configuration register 
	I2C_Write(0x18);									// Full scale range +/- 2000 degree/C 
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								// Write to interrupt enable register 
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);								// I2C start with device write address 
	I2C_Write(ACCEL_XOUT_H);							// Write start location address from where to read 
	I2C_Repeated_Start(0xD1);							// I2C start with device read address 
}

/************************************************************************
*Namn: Read_RawValue                                                    *
*                                                                       *
*Syfte: Reads x, y and z raw values from MPU-6050						*
*                                                                       *
*Indata: void                                                               *
*                                                                       *
*Utdata: void								                            *
*                                                                       *
*Anropar:MPU_Start_Loc(), I2C_Read_Ack(), I2C_Stop()                    *
************************************************************************/
void Read_RawValue()
{
	MPU_Start_Loc();									
	Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());     // Read acceleration x value
	Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());     // Read acceleration y value
	Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());     // Read acceleration z value
	Temperature = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

/************************************************************************
*Namn: main                                                             *
*                                                                       *
*Syfte: needs to be here, init the oled and print the x, y and z values *
*	    (accelerometer) on it. And also print distance from ultra sonic *
*       sensor		     	                                            *
*Indata: void											                *
*								                                        *
*Utdata: a int								                            *
*                                                                       *
*Anropar: Read_RawValue(), MPU6050_Init()				                *
************************************************************************/
int main()
{
	int distance_in_cm=0;     // initialize display, cursor off
	char a[16];
	
	char d[10], b[10], c[10], float_[10];
	float Xa,Ya,Za,t;
	float Xg=0,Yg=0,Zg=0;
	I2C_Init();				// Initialize I2C 
	MPU6050_Init();			// Initialize MPU6050 
    lcd_init(LCD_DISP_ON);	
    lcd_clrscr();

	while(1)
	{	
        distance_in_cm=read_sonar();
        //lcd_clrscr();

        if (distance_in_cm == TRIG_ERROR)
        {
	      lcd_gotoxy(0,0);
	      lcd_puts("ERROR!!!");
	      _delay_ms(DELAY_BETWEEN_TESTS/2);
	      lcd_clrscr();
	      _delay_ms(DELAY_BETWEEN_TESTS/2);
	
         }
        else if (distance_in_cm == ECHO_ERROR)
        {
	      lcd_gotoxy(0,0);
	      lcd_puts("CLEAR!!!");
	      _delay_ms(DELAY_BETWEEN_TESTS);
	      lcd_clrscr();
         }
        else
        {
	      lcd_gotoxy(0,0);
	      lcd_puts("Distance(cm):    ");
	      itoa(distance_in_cm,a,10);
	      lcd_gotoxy(13,0);
	      lcd_puts(a);
	      //_delay_ms(DELAY_BETWEEN_TESTS);
	      //lcd_clrscr();
         }

                Read_RawValue();

                Xa = Acc_x/16384.0;	   // Divide raw value by sensitivity scale factor to get real values
                Ya = Acc_y/16384.0;
                Za = Acc_z/16384.0;
                
                Xg = Gyro_x/16.4;
                Yg = Gyro_y/16.4;
                Zg = Gyro_z/16.4;

                t = (Temperature/340.00)+36.53;	   //Convert temperature in ?/c using formula
				
				dtostrf( Xa, 3, 2, float_ );
				sprintf(d,"x = %s g\t",float_);
				lcd_gotoxy(0,2);
				//lcd_puts("x=       ");   // display Xa value
				lcd_gotoxy(0,2);
				lcd_puts(d);
				
                dtostrf( Ya, 3, 2, float_ );
				
                sprintf(b,"y = %s g\t",float_);
                lcd_gotoxy(0,3);
                //lcd_puts("y=       ");   // display Ya value
                lcd_gotoxy(0,3);
                lcd_puts(b);
				
				dtostrf( Za, 3, 2, float_ );
				sprintf(c,"z = %s g\t",float_);
				lcd_gotoxy(0,4);
				//lcd_puts("z=       ");   // display Za value
				lcd_gotoxy(0,4);
				lcd_puts(c);
				_delay_ms(200);
	}
	return 0;
}





/*
int main(){
	int distance_in_cm=0;     // initialize display, cursor off
	char a[16];
	
	lcd_init(LCD_DISP_ON);  //INITIALIZE LCD
	lcd_clrscr();
	
	while(1)
	{
		distance_in_cm=read_sonar();
		if (distance_in_cm == TRIG_ERROR)
		{
			lcd_gotoxy(0,0);
			lcd_puts("ERROR!!!");
			_delay_ms(DELAY_BETWEEN_TESTS/2);
			lcd_clrscr();
			_delay_ms(DELAY_BETWEEN_TESTS/2);
			
		}
		else if (distance_in_cm == ECHO_ERROR)
		{
			lcd_gotoxy(0,0);
			lcd_puts("CLEAR!!!");
			_delay_ms(DELAY_BETWEEN_TESTS);
			lcd_clrscr();
		}
		else
		{
			lcd_gotoxy(0,0);
			lcd_puts("Distance(cm):    ");
			itoa(distance_in_cm,a,10);
			lcd_gotoxy(13,0);
			lcd_puts(a);
			//_delay_ms(DELAY_BETWEEN_TESTS);
			//lcd_clrscr();
		}
	}
	return 0;
}
*/

















