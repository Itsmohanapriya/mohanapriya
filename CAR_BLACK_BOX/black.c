#include"newmain.h"
#include <xc.h>

extern unsigned char key;
const unsigned char *menu[] = {"View Log", "Download Log", "Clear Log ", "Set Time"};
unsigned char gear = 0;
unsigned char collision_state;
unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];
unsigned char data[12];
unsigned char adder;
unsigned int rpm_set, prev_rpm = 0;
unsigned char i, j, addres;
unsigned char log_event[10][16];
unsigned int hr, min, sec, count = 0, event_count, index, delay, flag = 0, delay1;

static void get_time(void) {
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

void store_event(void) {
    get_time();
    for (int i = 0; i < 8; i++) 
    {
        data[i] = time[i];
    }


    //data[8] = 'G';
    if (gear == 0) 
    {
        data[9] = 'N';
    } 
    else if (gear == 6) 
    {
        data[9] = 'R';
    } 
    else if (gear == 10) 
    {
        data[9] = 'C';
    } 
    else 
    {
        data[9] = gear + '0';
    }
    data[10] = (rpm_set) / 10 % 10 + '0';
    data[11] = (rpm_set) % 10 + '0';

    for (i = 0; i < 12; i++) {
        if (adder == 120)
            adder = 0;
        //write_EEPROM(adder, data[i]);
        adder++;
    }
    //    if (event_count < 10) {
    //        event_count++;
    //    } else {
    //        event_count = 0;
    //    }
    event_count++;
}

void gear_position() {
    if (collision_state == 1) 
    {
        if (key == 1 || key == 2) 
        {
            gear = 0;
            collision_state = 0;
            store_event();
        }
        return;
    }
    if (key == 1) 
    {
        if (gear < 6) 
        {
            gear++;
            store_event();
            //for(int delay = 50;delay--;);
        }
    }
    if (key == 2) 
    {
        if (gear > 0) 
        {
            gear--;
            store_event();
            //for(int delay = 50;delay--;);
        }
    }
    if (key == 3) 
    {
        gear = 10;
        collision_state = 1;
        store_event();
        return;
        //afor(int delay = 100;delay--;);
    }
}

void view_dashboard(void) 
{
    unsigned int adc_reg_val;
    clcd_print("TIME     G   SP", LINE1(0));
    adc_reg_val = read_adc(CHANNEL4);
    rpm_set = (adc_reg_val) / 10.3;

    if (rpm_set >= prev_rpm + 5 || rpm_set + 5 <= prev_rpm) 
    {
        store_event();
        prev_rpm = rpm_set;
    }
    get_time();
    clcd_print(time, LINE2(0));

    //GEAR
    gear_position();
    clcd_putch('G', LINE2(9));
    if (gear == 0) 
    {
        clcd_putch('N', LINE2(10));
    } 
    else if (gear == 6) 
    {
        clcd_putch('R', LINE2(10));
    } 
    else if (gear == 10) 
    {
        //clcd_print("   ",LINE2(10));
        clcd_putch('C', LINE2(10));
    } 
    else 
    {
        clcd_putch(gear + '0', LINE2(10));
    }

    //SPEED
    clcd_putch((rpm_set) / 10 % 10 + '0', LINE2(14));
    clcd_putch(rpm_set % 10 + '0', LINE2(15));

    if (key == 11) 
    {
        state = main_menu;
    }
}

void view_menu(void) {
    if (key == 2 && count < 3) 
    {
        count++;
    }
    if (key == 1 && count > 0) 
    {
        count--;
    }
    if (!count) 
    {
        clcd_putch('*', LINE1(0));
        clcd_print(menu[count], LINE1(1));
        clcd_putch(' ', LINE2(0));
        clcd_print(menu[count + 1], LINE2(1));
    } 
    else 
    {
        clcd_putch(' ', LINE1(0));
        clcd_print(menu[count - 1], LINE1(1));
        clcd_putch('*', LINE2(0));
        clcd_print(menu[count], LINE2(1));
    }
    if (key == 11 && count == 0) 
    {
        index=0;
        state = viewlog;
    }
    if (key == 11 && count == 1) 
    {
        state = downloadlog;
    }
    if (key == 11 && count == 2) 
    {
        state = clearlog;
    }
    if (key == 11 && count == 3) 
    {
        state = setlog;
    }
    if (key == 12) 
    {
        state = dashboard;
        CLEAR_DISP_SCREEN;
        key=0;
    }

}

void view_log(void) 
{
    read_event();
    clcd_print("N  TIME    G SP", LINE1(0));
    int end = 0;
    if (event_count > 10) 
    {
        end = 10;
    } 
    else 
    {
        end = event_count;
    }
    if (key == 2 && index < end - 1) 
    {
        index++;
    }
    if (key == 1 && index > 0) 
    {
        index--;
    }
    if (event_count == 0) 
    {
        clcd_print("NO LOGS STORED", LINE2(0));
    } 
    else 
    {
        int dis=((end-1)-index);
        clcd_putch(index + '0', LINE2(0));
        clcd_putch(' ',LINE2(1));
        clcd_print(log_event[dis], LINE2(2));
    }
    if (key == 12) 
    {
        state = main_menu;
        CLEAR_DISP_SCREEN;
    }

}

void read_event(void) 
{
    int end;
    if (event_count > 10) 
    {
        addres = ((event_count %10)*12);
        end = 10;
    } 
    else 
    {
        addres = 0;
        end = event_count;
    }
    for (i = 0; i < end; i++) 
    {
        for (j = 0; j < 15; j++) 
        {
            if (j == 8 || j == 11) 
            {
                log_event[i][j] = ' ';
            } 
            else if (j == 14) 
            {
                log_event[i][j] = '\0';
            } 
            else 
            {
                log_event[i][j] = read_internal_eeprom(addres);
                addres++;
                if (addres >= 120) 
                {
                    addres = 0;
                }
            }
        }
    }
}

void clear_log(void) 
{
    clcd_print("CLEAR SUCCESS", LINE1(0));
    event_count = 0;
    adder = 0;

    __delay_ms(1000);
    state = main_menu;
}

void download_log(void) 
{
    read_event();
    clcd_print("                ",LINE2(0));
    clcd_print("DOWNLOAD SUCCESS", LINE1(0));
    int end;
    if (event_count > 10) 
    {
        end = 10;
    } 
    else 
    {
        end = event_count;
    }
    if (event_count == 0) 
    {
        clcd_print("NO LOGS PRESENT", LINE1(0));
        state = main_menu;
        CLEAR_DISP_SCREEN;
    } 
    else 
    {
        while (delay++ >= 100) {
            delay = 0;
            state = main_menu;
            puts("\n------------------");
            puts("\n\r");
            puts("N TIME     EV SP\n\r");

            puts("------------------");
            for (int i = 0; i < end; i++) 
            {
                puts("\n\r");
                putch((i) + '0');
                putch(' ');
                puts(log_event[i]);
            }
            puts("\n\r");
            puts("------------------\n\r");
        }
    }
}

void print_clcd(void) {
    clcd_putch(hr / 10 + '0', LINE2(4));
    clcd_putch(hr % 10 + '0', LINE2(5));

    clcd_putch(':', LINE2(6));

    clcd_putch(min / 10 + '0', LINE2(7));
    clcd_putch(min % 10 + '0', LINE2(8));

    clcd_putch(':', LINE2(9));

    clcd_putch(sec / 10 + '0', LINE2(10));
    clcd_putch(sec % 10 + '0', LINE2(11));

}

void set_log(void) {
    clcd_print("    HH:MM:SS   ", LINE1(0));
    clcd_print("    ", LINE2(0));
    clcd_print("  ", LINE2(12));

    if (flag == 0) 
    {
        hr = ((time[0] - '0')*10) + (time[1] - '0');
        min = ((time[3] - '0')*10) + (time[4] - '0');
        sec = ((time[6] - '0')*10) + (time[7] - '0');
        flag++;
    }
    if (key == 2) 
    {
        if (++flag == 4) 
        {
            flag = 1;
        }
    }
    if (key == 1) 
    {
        if (flag == 1) 
        {
            if (++hr == 24) 
            {
                hr = 0;
            }
        } 
        else if (flag == 2) 
        {
            if (++min == 60) 
            {
                min = 0;
            }
        } 
        else if (flag == 3) 
        {
            if (++sec == 60) 
            {
                sec = 0;
            }
        }
    }
    if (flag == 1) 
    {
        if (delay1++ <= 500) 
        {
            print_clcd();
        } 
        else if (delay1++ <= 1000) 
        {
            clcd_putch(0XFF, LINE2(4));
            clcd_putch(0XFF, LINE2(5));
        } 
        else 
        {
            delay1 = 0;
        }
    }
    if (flag == 2) 
    {
        if (delay1++ <= 500) 
        {
            print_clcd();
        } 
        else if (delay1++ <= 1000) 
        {
            clcd_putch(0XFF, LINE2(7));
            clcd_putch(0XFF, LINE2(8));
        } 
        else 
        {
            delay1 = 0;
        }
    }
    if (flag == 3) 
    {
        if (delay1++ <= 500) 
        {
            print_clcd();
        } 
        else if (delay1++ <= 1000) 
        {
            clcd_putch(0XFF, LINE2(10));
            clcd_putch(0XFF, LINE2(11));
        } 
        else 
        {
            delay1 = 0;
        }
    }
    if (key == 11) 
    {
        write_ds1307(HOUR_ADDR, (((hr / 10) << 4) | (hr % 10)));
        write_ds1307(MIN_ADDR, (((min / 10) << 4) | (min % 10)));
        write_ds1307(SEC_ADDR, (((sec / 10) << 4) | (sec % 10)));

        state = main_menu;
    }
    if (key == 12) 
    {
        state = main_menu;
    }
}


