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
 ALIENTEK战舰STM32开发板实验3
 按键输入实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子
 @ALIENTEK
************************************************/
u16 FreqBase=10000;
u16 beishu=1;
u8 key;
u16 VOL=500;//恒定500mv电压
u16 delta_vol=125;//100mv步进电压
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
	KEY_Init();			  //初始化按键程序
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LCD_Init();
	Init_AD9834();
	AD9834_Select_Wave(0x2800);
	FSEL_0;
	AD9834_RESET_CLR;
	AD9834_Set_Freq(FREQ_0,beishu*FreqBase);
	AD9834_Set_Freq(FREQ_1,FreqBase*100);
	
	DAC_SetChannel1Data(DAC_Align_12b_R, delta_vol*i);//设置通道1DAC值
	DAC_SetChannel2Data(DAC_Align_12b_R, VOL);//通道2固定值为500
	
	POINT_COLOR=BLACK;//设置字体为黑色
	LCD_ShowString(60,00,200,16,16,"FREQ :         KHz");
	LCD_ShowString(60,30,200,16,16,"fudu :         mV");
	DrawSineWave();//显示正弦波
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
					if(state==0) AD9834_Write_16Bits(0x2080);//失能MCLK
					else AD9834_Write_16Bits(0x2880);
				}
				else 
				{
					if(state==0) AD9834_Write_16Bits(0x2000);//重新使能MCLK
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
					DAC_SetChannel1Data(DAC_Align_12b_R, delta_vol*i);//设置DAC值					
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
