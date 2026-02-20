/*
 * File:   newmain.c
 * Author: Mohanapriya
 *
 * Created on 22 December, 2025, 8:37 PM
 */

/*#include <xc.h>
#include<stdio.h>
#include"adc.h"
#include"clcd.h"
#include"mkp.h"
#include"rtc.h"
#include"i2c.h"

void init_config()
{    
    TRISB = 0x00;       // LEDs output
    PORTB = 0x00;
     
    //TRISC |= 0x0F; // Keypad input 
    //TRISD = 0x00; // SSD data 
    //TRISA &= 0xF0; // SSD select pins RA0-RA3 
    init_adc();
    init_clcd();
    init_matrix_keypad();
    init_i2c();
    init_ds1307();
}
void main(void) 
{
    const char *gear_table[] = {"N", "1", "2", "3", "4", "5", "R"};
    unsigned int adc_reg_val;
    unsigned char key,buf[5],speed=0,gear_index=0;
    unsigned long result = 0; 

    unsigned char clock_reg[3];
    unsigned char calender_reg[4];
    unsigned char time[9];

    init_config();
    while (1)
    {
        adc_reg_val = read_adc(CHANNEL4);
        if(gear_index > 0)
        {
        result = (adc_reg_val / 10.23) * 60;
        sprintf(buf,"SP:%03d", speed);
        clcd_print("speed",LINE1(0));
        clcd_print(buf, LINE2(0));
        }
        key = read_switches(STATE_CHANGE);

        if(key == MK_SW1)        // gear ++
        {
            if(gear_index < 6)
                gear_index++;
        }
        else if(key == MK_SW2)   // gear --
        {
            if(gear_index > 0)
                gear_index--;
        }
        clcd_print("                ",LINE1(0));
        clcd_print("                ",LINE2(0));
        clcd_print(gear_table[gear_index] , LINE1(7));
        clcd_print((char*)gear_table[gear_index], LINE2(5));
        void display_time(void)
        {
	clcd_print(time, LINE2(2));

	if (clock_reg[0] & 0x40)
	{
		if (clock_reg[0] & 0x20)
		{
			clcd_print("PM", LINE2(12));
		}
		else
		{
			clcd_print("AM", LINE2(12));
		}
	}
}
        static void get_time(void)
        {
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}
        clcd_print("RTC",LINE1(10));
            
        #if 1
		get_time();
		display_time();
       #else
		get_date();
		display_date();
       #endif
        
        
    return;
   }
}*/
#include <xc.h>
#include "newmain.h"

pos_t state;

unsigned char key;

void init_config(void) {
    init_clcd();
    init_i2c();
    init_ds1307();
    init_adc();
    init_uart();
    init_matrix_keypad();
    //init_EEPROM();

}

void main(void) {
    init_config();

    while (1) {

        key = read_switches(STATE_CHANGE);
        switch (state) {
            case dashboard:

                view_dashboard();
                break;

            case main_menu:
                view_menu();
                break;

            case viewlog:
                view_log();
                break;
                
            case downloadlog:
                download_log();
                break;
                
            case clearlog:
                clear_log();
                break;

            case setlog:
                set_log();
                break;

        }
    }

}
