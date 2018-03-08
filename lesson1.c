#include <reg52.h> 
#include "SHT11.h"  
#include<stdio.h>
#include<intrins.h>
unsigned char wendu1[5];
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71,
0x76,0x79,0x38,0x3f,0};
uchar despcom;
uchar led;
uchar DispData[4]={0,1,2,3};
uchar code tab[]={
0xfe,0xfd,0xfb,0xf7,0xef};
void  DispConvert(uint Temp2Con) 
{
	
	wendu1[0] = table[Temp2Con/10000];
	wendu1[1] = table[Temp2Con%10000/1000];
	wendu1[2] = table[Temp2Con%1000/100];
	wendu1[3] = table[Temp2Con%100/10];
	wendu1[4] = table[Temp2Con%10];
}

void init()
{
	

	TMOD=0x21;//设置定时器1为工作方式2
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	SM0=0;
	SM1=1;
	REN=1;

	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;

	EA=1;
	ET0=1;

	TR0=1;
	led=0xFE;

}
void main()
{
	uint temp;
	uint dat;
	uint Hum;
	init();
	while(1)
	{								   
	
		SHT11_Write_Register(REG_WRITE,FUNCTION_SET);
		
		temp=SHT11_Measure(TEM_TEST,0x37);
		
		temp=SHT11_Convert_Tempeture12bit(temp);
		dat=SHT11_Measure(HUM_TEST,0x0b); 

		Hum=SHT11_Convert_Humidity8bit(dat,temp);
	


	
		DispConvert(temp); 
		P1=~P1;	
	}
}

void timer0() interrupt 1
{
	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;
	dula=1;
	P0=wendu1[despcom];
	dula=0;
	P0=0xff;
	wela=1;
	P0=tab[despcom];
	wela=0;
	despcom++;
	if(despcom>=5)
		despcom=0;
}