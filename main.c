#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "adc.h"
#include "beep.h"
#include "AD9834.h"
#include "usart.h"
#include "stm32f10x.h"
#include "wave.h"
/************************************************
 ALIENTEKս��STM32������ʵ��3
 ��������ʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ��
 @ALIENTEK
************************************************/
u16 FreqBase=10000;
u16 beishu=1;
u8 key;
u16 VOL=500;//�㶨500mv��ѹ
u16 delta_vol=125;//100mv������ѹ
u8 i=1;
u8 state=1;
u8 zhuangtai=1;
u16 freq=0;
u16 fudu=0;
extern int k;
extern int j;
extern u16 sinedata[64];
int main(void)
{
	
	
	delay_init();
	KEY_Init();			  //��ʼ����������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LCD_Init();
	Init_AD9834();
	AD9834_Select_Wave(0x2800);
	FSEL_0;
	AD9834_RESET_CLR;
	AD9834_Set_Freq(FREQ_0,beishu*FreqBase);
	AD9834_Set_Freq(FREQ_1,FreqBase*100);
	
	DAC_SetChannel1Data(DAC_Align_12b_R, delta_vol*i);//����ͨ��1DACֵ
	DAC_SetChannel2Data(DAC_Align_12b_R, VOL);//ͨ��2�̶�ֵΪ500
	
	POINT_COLOR=BLACK;//��������Ϊ��ɫ
	LCD_ShowString(60,00,200,16,16,"FREQ :         KHz");
	LCD_ShowString(60,30,200,16,16,"fudu :         mV");
	DrawSineWave();//��ʾ���Ҳ�
	while(1)
	{
		key=KEY_Scan(0);
		if(key)
		{
			switch(key)
			{
				
				case WKUP_PRES:				
				state=!state;
				if(state==0) AD9834_Write_16Bits(0x2000);
				else AD9834_Write_16Bits(0x2800);
				break;
				
				case KEY1_PRES:
				zhuangtai=!zhuangtai;
				if(zhuangtai==0) 
				{
					if(state==0) AD9834_Write_16Bits(0x2080);//ʧ��MCLK
					else AD9834_Write_16Bits(0x2880);
				}
				else 
				{
					if(state==0) AD9834_Write_16Bits(0x2000);//����ʹ��MCLK
					else AD9834_Write_16Bits(0x2800);
				}
				break;
				
				case KEY0_PRES:
				if(beishu<100)
				{
					beishu++;
					AD9834_Set_Freq(FREQ_0,beishu*FreqBase);
					AD9834_Set_Freq(FREQ_1,FreqBase*100);
				}	break;
				
				case KEY2_PRES:
				if(i<10)
				{
					i++;
					DAC_SetChannel1Data(DAC_Align_12b_R, delta_vol*i);//����DACֵ					
				}	break;
				default: break;
			}
		}		
		else delay_ms(10);
		
		if(state==0)			freq=10*beishu;
		else							freq=1000;
		LCD_ShowxNum(124,00,freq,6,16,0);
		if(state==0)
		{			
			fudu=550;
			if(beishu>1&&beishu<=20)	fudu=550-(beishu/10)*4;
			else if(beishu>20&&beishu<=40)	fudu=542-(beishu-20)/10*6;
			else if(beishu>40&&beishu<=60)  fudu=530-(beishu-40)/10*4;
			else if(beishu>60&&beishu<=80)  fudu=522-(beishu-60)/10*8;
			else if(beishu>80&&beishu<=100)  fudu=506-(beishu-80)/10*10;
		}
		else fudu=486;
		LCD_ShowxNum(124,30,fudu,3,16,0);     	
		}
//	
	
}
