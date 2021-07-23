
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#define PBASE 0x3F000000





extern void dummy ( unsigned int );

#define ARM_TIMER_CTL   (PBASE+0x0000B408)
#define ARM_TIMER_CNT   (PBASE+0x0000B420)
#define GPFSEL0         (PBASE+0x00200000)
#define GPFSEL1         (PBASE+0x00200004)
#define GPFSEL2         (PBASE+0x00200008)
#define GPSET0          (PBASE+0x0020001C)
#define GPCLR0          (PBASE+0x00200028)
#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)

#define AUX_ENABLES     (PBASE+0x00215004)
#define AUX_MU_IO_REG   (PBASE+0x00215040)
#define AUX_MU_IER_REG  (PBASE+0x00215044)
#define AUX_MU_IIR_REG  (PBASE+0x00215048)
#define AUX_MU_LCR_REG  (PBASE+0x0021504C)
#define AUX_MU_MCR_REG  (PBASE+0x00215050)
#define AUX_MU_LSR_REG  (PBASE+0x00215054)
#define AUX_MU_MSR_REG  (PBASE+0x00215058)
#define AUX_MU_SCRATCH  (PBASE+0x0021505C)
#define AUX_MU_CNTL_REG (PBASE+0x00215060)
#define AUX_MU_STAT_REG (PBASE+0x00215064)
#define AUX_MU_BAUD_REG (PBASE+0x00215068)

extern void Put32(unsigned int address , unsigned int data);
extern unsigned int Get32(unsigned int address );





//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//------------------------------------------------------------------------
unsigned int uart_lcr ( void )
{
    return(Get32(AUX_MU_LSR_REG));
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(Get32(AUX_MU_LSR_REG)&0x01) break;
    }
    return(Get32(AUX_MU_IO_REG)&0xFF);
}
//------------------------------------------------------------------------
unsigned int uart_check ( void )
{
    if(Get32(AUX_MU_LSR_REG)&0x01) return(1);
    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int c )
{
    while(1)
    {
        if(Get32(AUX_MU_LSR_REG)&0x20) break;
    }
    Put32(AUX_MU_IO_REG,c);
}
//------------------------------------------------------------------------
void uart_flush ( void )
{
    while(1)
    {
        if((Get32(AUX_MU_LSR_REG)&0x100)==0) break;
    }
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    Put32(AUX_ENABLES,1);
    Put32(AUX_MU_IER_REG,0);
    Put32(AUX_MU_CNTL_REG,0);
    Put32(AUX_MU_LCR_REG,3);
    Put32(AUX_MU_MCR_REG,0);
    Put32(AUX_MU_IER_REG,0);
    Put32(AUX_MU_IIR_REG,0xC6);
    Put32(AUX_MU_BAUD_REG,270);
    ra=Get32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    Put32(GPFSEL1,ra);
    Put32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    Put32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    Put32(GPPUDCLK0,0);
    Put32(AUX_MU_CNTL_REG,3);
}
//------------------------------------------------------------------------
void  timer_init ( void )
{
    //0xF9+1 = 250
    //250MHz/250 = 1MHz
    Put32(ARM_TIMER_CTL,0x00F90000);
    Put32(ARM_TIMER_CTL,0x00F90200);
}
//-------------------------------------------------------------------------
unsigned int timer_tick ( void )
{
    return(Get32(ARM_TIMER_CNT));
}
//-------------------------------------------------------------------------
void leds_off ( void )
{
#define GPFSEL3 0x3F20000C
#define GPFSEL4 0x3F200010
#define GPSET1  0x3F200020
#define GPCLR1  0x3F20002C
    unsigned int ra;

    ra=Get32(GPFSEL4);
    ra&=~(7<<21);
    ra|=1<<21;
    Put32(GPFSEL4,ra);

    ra=Get32(GPFSEL3);
    ra&=~(7<<15);
    ra|=1<<15;
    Put32(GPFSEL3,ra);

    Put32(GPCLR1,1<<(47-32));
    Put32(GPCLR1,1<<(35-32));
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

void leds_on ( void )
{
#define GPFSEL3 0x3F20000C
#define GPFSEL4 0x3F200010
#define GPSET1  0x3F200020
#define GPCLR1  0x3F20002C
    unsigned int ra;
    ra=Get32(GPFSEL4);
    ra&=~(7<<21);
    ra|=1<<21;
    Put32(GPFSEL4,ra);
    ra=Get32(GPFSEL3);
    ra&=~(7<<15);
    ra|=1<<15;
    Put32(GPFSEL3,ra);
    Put32(GPSET1,1<<(47-32));
    Put32(GPSET1,1<<(35-32));
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

void uart_send_str(char *ptr)
{
	while(*ptr!=0)
		uart_send((int)*ptr++);
}






void jtag_enable(void)
{
    
    unsigned int ra;

        //for jtag

    //alt4 = 0b011 3
    //alt5 = 0b010 2


    Put32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    Put32(GPPUDCLK0,(1<<4)|(1<<22)|(1<<24)|(1<<25)|(1<<27));
    for(ra=0;ra<150;ra++) dummy(ra);
    Put32(GPPUDCLK0,0);

    ra=Get32(GPFSEL0);
    ra&=~(7<<12); //gpio4
    ra|=2<<12; //gpio4 alt5 ARM_TDI
    Put32(GPFSEL0,ra);

    ra=Get32(GPFSEL2);
    ra&=~(7<<6); //gpio22
    ra|=3<<6; //alt4 ARM_TRST
    ra&=~(7<<12); //gpio24
    ra|=3<<12; //alt4 ARM_TDO
    ra&=~(7<<15); //gpio25
    ra|=3<<15; //alt4 ARM_TCK
    ra&=~(7<<21); //gpio27
    ra|=3<<21; //alt4 ARM_TMS
    Put32(GPFSEL2,ra);

//ARM_TRST      22 GPIO_GEN3 P1-15 IN  (22 ALT4)
//ARM_TDO     5/24 GPIO_GEN5 P1-18 OUT (24 ALT4)
//ARM_TCK    13/25 GPIO_GEN6 P1-22 OUT (25 ALT4)
//ARM_TDI     4/26 GPIO_GCLK P1-7   IN ( 4 ALT5)
//ARM_TMS    12/27 CAM_GPIO  S5-11 OUT (27 ALT4)
}
//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------