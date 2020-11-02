#include <xc.h>
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1


void main()
{
    int j,i;
    unsigned x, left, right;
    setup();
    while(1)
    {   
        if(PORTBbits.RB11)//if SW5 => none of the led works
        {
            PORTA = 0x00;
        }
        else
        {
            PORTA = 0x00;
            /*SW 0 and !(SW5) and !(S1) and !(S2) 
             *  count binary*/
            while(PORTFbits.RF3 && !(PORTBbits.RB11) &&!(PORTFbits.RF5) &&!(PORTFbits.RF4))
            {
                int val = 1;
                if(PORTDbits.RD15)
                {
                    val = -1;
                }
                PORTA = PORTA + val;
                for(j=0;j<getDelay();j++);//delay()
            }
            /* SW1 and !(SW5) and !(SW2) 
             * running led*/
            while(PORTFbits.RF5 && !(PORTBbits.RB11) && !(PORTFbits.RF4))
            {     

                 x = (PORTDbits.RD15) ? 0x80: 0x01; //choose the start side
                while(x < 0x81 && x > 0x0)
                {
                    if(!(PORTFbits.RF5)|| PORTBbits.RB11 ||PORTFbits.RF4) //if !(SW1) and !(SW2) and !(SW5) -> exit 
                    {
                        break;
                    }
                    //counter++;
                    PORTA = x;
                    if(PORTDbits.RD15) //!(SW3) right to left
                    {
                        x = x >> 1;
                    }
                    else // SW3 left to right
                    {
                        x = x << 1;
                    }
                    for(j=0;j<getDelay();j++);//delay()
                }
            }
            /*SW2 and !(SW5)
             *fan*/
            while(PORTFbits.RF4 && !(PORTBbits.RB11))
            {
                int i,counter;
                int steps_number = 4;
                counter = (PORTDbits.RD15) ? 4 : 0;
                left = (PORTDbits.RD15) ? 1 << 7 : 1 << 4;
                right = (PORTDbits.RD15) ? 1: 1 << 3;
                PORTA = left | right;
                do
                {
                    if(PORTDbits.RD15)
                    {
                        counter--;
                        left = left >> 1;
                        right = right << 1;
                    }
                    else
                    {
                        counter++;
                        left = left << 1;
                        right = right >> 1;
                    }
                    for(j=0;j<getDelay();j++);//delay()
                    PORTA = left | right;
                }
                while(counter > 0 && counter < steps_number && !(PORTBbits.RB11));
            }
        }
        if(PORTBbits.RB10)
        {
            unsigned int speaker_counter = 0;
            unsigned int check_var = 1;
            PORTA = 0xff;
            if (PORTBbits.RB10) //SW6
                if(speaker_counter++%check_var){
                    PORTBbits.RB14 = PORTBbits.RB14; //change RB14
                    for(j=0;j<getDelay();j++);//delay()
                }
                    
            check_var = check_var<<1;

        }
    }

}
int getDelay() // setting speed
{
    if(PORTDbits.RD14)
    {
        return 64000;
    }
    return 640000;
}
void setup()
{
    TRISA &= 0xff00;
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    TRISBbits.TRISB14 = 0; //speaker configured output
    ANSELBbits.ANSB14 = 0; //speaker disabled analog
}