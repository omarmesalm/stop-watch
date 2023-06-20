
#define F_CPU  8000000

#include <util/delay.h>

#define PORTA  (*(volatile unsigned char*)0x3B)
#define DDRA  (*(volatile unsigned char*)0x3A)
#define PINA  (*(volatile unsigned char*)0x39)

#define PORTC  (*(volatile unsigned char*)0x35)
#define DDRC  (*(volatile unsigned char*)0x34)
#define PINC  (*(volatile unsigned char*)0x33)

#define PORTD  (*(volatile unsigned char*)0x32)
#define DDRD  (*(volatile unsigned char*)0x31)
#define PIND  (*(volatile unsigned char*)0x30)


#define READ_BIT(reg, bit) ((reg>>bit)&1)
#define SET_BIT(reg, bit) (reg=reg|(1<<bit))
#define CLR_BIT(reg, bit) (reg=reg&~(1<<bit))
#define TOG_BIT(reg, bit) (reg=reg^(1<<bit))

void Kit_Segment_Display(char num);

typedef enum{
	run_mode,
	conf_mode
	}mode_status_t;



int main(void)
{
	DDRA = 0xFF;
	
    CLR_BIT(DDRD,2);
	CLR_BIT(DDRD,3);
	CLR_BIT(DDRD,4);
	
	SET_BIT(DDRC,5);
	SET_BIT(DDRC,6);
	SET_BIT(DDRC,7);
	
	char i, time=30, pause_resume=0, flag1=0,flag2=0,d0,d1,flag3=0;
	mode_status_t mode = run_mode;
	
    while (1) 
    {
		for(i=0; i<250; i++)
		{
			if(!READ_BIT(PIND,4))
			{
				if(flag1==0)
				{
					mode ^= 1;
					pause_resume=0;
					flag1=1;
				}
			}
			else
			{
				flag1 = 0;
			}
			
			Kit_Segment_Display(time);
			
			
			if(!READ_BIT(PIND,3))
			{
				if(flag2==0)
				{
					if(mode==run_mode)
					{
						pause_resume ^= 1;
						flag2 = 1;
						break;
					}
					else if (mode==conf_mode)
					{
						d0=time%10;
						d1=time-d0;
						time++;
						if(time==(d1+10))
						{
							time = d1;
						}
						flag2 = 1;
					}
					
				}
				
			}
			else
			{
				flag2 = 0;
			}
			
			if(!READ_BIT(PIND,2))
			{
				if(flag3==0)
				{
					if(mode==run_mode)
					{						
						time =30;
						pause_resume = 0;
					}
					else if(mode==conf_mode)
					{
						d0=time%10;
						time += 10;
						if(time>=100)
						{
							time = d0;
						}
						flag3=1;
					}
				}
			}
			else
			{
				flag3=0;
			}
			
			if(READ_BIT(PORTC,5) && i==249)
			{
				CLR_BIT(PORTC,5);
				pause_resume=0;
			}
		}
		
		if(pause_resume && mode==run_mode)
		{
			time--;
			if(time==0)
			{
				pause_resume ^= 1;
				SET_BIT(PORTC,5);
			}
		}
		
		
		
    }
}


void Kit_Segment_Display(char num)
{
	char SegmentArr[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
	char d0,d1;
	d0 = num%10;
	d1 = num/10;
	
	CLR_BIT(PORTC,7);
	PORTA=SegmentArr[d0]<<1;
	SET_BIT(PORTC,6);
	_delay_ms(1);
	CLR_BIT(PORTC,6);
	PORTA=SegmentArr[d1]<<1;
	SET_BIT(PORTC,7);
	_delay_ms(1);

}


