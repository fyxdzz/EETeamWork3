#include "sys.h"
#include "lcd.h"
#include "wave.h"

int k=0;
int j=0;

u16 sinedata[64]={255,254,252,249,245,239,233,225,217,207,197,186,174,162,150,137,124,112,99,87,75,64,53,43,34,26,19,13,8,4,1,0,
									0,1,4,8,13,19,26,34,43,53,64,75,87,99,112,124,137,150,162,174,186,197,207,217,225,233,239,245,249,252,254,255};
void DrawSineWave(void)
{
	for(j=0;j<6;j++)
	{
		for(k=0;k<32;k++)
		{
			LCD_DrawPoint(30+64*j+k,240-sinedata[k]/2);
			if(k)
			LCD_DrawLine(30+64*j+k-1,240-sinedata[k-1]/2,30+64*j+k,240-sinedata[k]/2);
		}
		for(k=32;k<64;k++)
		{
			LCD_DrawPoint(30+64*j+k,240-sinedata[k]/2);
			LCD_DrawLine(30+64*j+k-1,240-sinedata[k-1]/2,30+64*j+k,240-sinedata[k]/2);
		}
	}
}
