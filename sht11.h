#ifndef __SHT11_H__
#define __SHT11_H__

/*************************
SHT11�������
**************************/

#define TEM_TEST 		0x03//�¶ȼ������
#define HUM_TEST  		0x05//ʪ�ȼ������
#define REG_READ  		0x07//���Ĵ���
#define REG_WRITE 		0x06//д�Ĵ���
#define FUNCTION_SET	0x01//����SHT11�Ĺ�������Ϊ8λ/ʪ�� 12λ�¶�
#define uchar unsigned char 
#define uint  unsigned int
/**************************
SHT11�˿ڶ���
***************************/
sbit SHT11_DATA=P2^0;
sbit SHT11_SCK=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
uchar flag_tempeture=0;  //��ʾ�¶�λ�õı�־
uchar flag_humidity=0;	 //��ʾʪ��λ�õı�־
uchar code str1[]={ 0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};//�¶�ͼ��
uchar code str6_sht11[]="%RH   "; 
uchar code str4_sht11[]="humi=";
uchar code str2_sht11[]="temp=";
uchar code str7_sht11[]="    ";//���û��Ҫ����ʾ
/***************************
��������:Delay()
��������:SHT11�ڲ���ʱ
****************************/
void Delay()
{	;
	;
}

/***************************
��������:Delay_Ms()
��������:SHT11���ȴ���ʱ
����˵��:11ms/55ms/210ms �ֱ��Ӧ8λ/12λ/14λ �������
��Ӧ���β�ΪN ����ʱNms
****************************/
void Delay_Ms(uint ms)
{
	uint i,j;
	for(i=ms;i>0;i--)
		for(j=112;j>0;j--);
}
/**************************
��������:SHT11����ʱ��
***************************/
void SHT11_Start()
{
/*	SHT11_SCK=1;
	SHT11_DATA=1;
	Delay();
	SHT11_DATA=0;
	Delay();
	SHT11_SCK=0;
	Delay();*/
	SHT11_SCK=1;
	Delay();
	SHT11_DATA=1;
}

/******************************
�������ƣ�SHT11_Sendbyte(uchar dat)
��������: ��SHT11����8bite����
******************************/
void SHT11_Sendbyte(uchar dat)
{
	uchar i;
	SHT11_SCK=0;
	Delay();
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		{
			SHT11_DATA=1;
			Delay();
		}
		else
		{
			SHT11_DATA=0;
			Delay();
		}
		dat=dat<<1;
		SHT11_SCK=1;
		Delay();
		SHT11_SCK=0;
	}
}
/*********************************
��������SHT11_Answer()��
��������:���SHT11����Ӧ�ź�(�ڵھŸ�ʱ������)
***********************************/
void SHT11_Answer()
{
	uchar i;
	SHT11_SCK=1;
	Delay();
	while(SHT11_DATA==1 && i<255)
			i++;
	SHT11_SCK=0;
	SHT11_DATA=1;
}

/************************************
�������ƣ�SHT11_Test_Finish()
��������:���SHT11��ʪ�ȼ���Ƿ����
*************************************/
void SHT11_Test_Finish()
{
	uchar i;
	while(SHT11_DATA==1 && i<255)
		i++;;
}

/************************************
�������ƣ�SHT11_Receivebyte()
��������:��SHT11����8bite����
*************************************/
uchar SHT11_Receivebyte() 
{	uchar i;
	uchar dat;
	SHT11_SCK=0;
	Delay();
	for(i=0;i<8;i++)
	{	
		SHT11_SCK=1;
		Delay();
		dat=dat<<1;
		if(SHT11_DATA)
		{
			dat=dat|0x01;
			Delay();
		}
		else
		{
			dat=dat&0xfe;
			Delay();
		}
	 	SHT11_SCK=0;
		Delay();
	}
	SHT11_DATA=1; //�ͷ���������
	return(dat);	
}

/***********************************
�������ƣ�MCU_Answer()
��������:��Ƭ����SHT11����Ӧ���ź�
*************************************/
void MCU_Answer()
{
	 SHT11_SCK=0;
	Delay();
	SHT11_DATA=0;
	Delay();
	SHT11_SCK=1;
	Delay();
	SHT11_SCK=0;
	Delay();
	SHT11_DATA=1; //�ͷ��������� ����ָ��ǳ���Ҫ ���ӵĻ����µ�Ƭ�����ܶ�ȡ��8λ
}
/***********************************
��������:SHT11_End()
����������8byte���ݺ󲿽���CRCУ����
************************************/
void SHT11_End()
{
	SHT11_DATA=1;
	SHT11_SCK=1;
/*	Delay();
	SHT11_SCK=0;
	Delay();*/
}
/*************************************
�������ƣ�void SHT11_Write_Register(uchar command ,uchar dat)
����˵������SHT11��״̬�Ĵ������ù���
			commandΪREG_WRITE 0x06д�Ĵ���
			datΪ ����SHT11�Ĺ��� �������ü�������λ��
***************************************/

void SHT11_Write_Register(uchar command ,uchar dat)
{
	SHT11_Start();
	
	SHT11_Sendbyte(command);
	SHT11_Answer();
	SHT11_Sendbyte(dat);
	SHT11_Answer();
}
/***************************************
�������ƣ�uchar SHT11_Read_Register(uchar command)
����˵����commandΪREG_READ  0x07//���Ĵ���
			����ֵΪ״̬�Ĵ�����ֵ
		 λ6��ʾ��ǰ�����һ�����ݺ��Դ�������
			��λ6Ϊ0ʱ����VDD>2.47V ��λ6Ϊ1ʱ����VDD<2.47V����������
		 λ0������ǰ�Ĳ����ֱ���
		 ��λ0Ϊ1ʱ�����������ȣ�8λ/ʪ�� 12λ�¶�
		 ��λ0Ϊ0ʱ�����������ȣ�12λʪ�� 14λ�¶� 
		 Ĭ��Ϊ0
*******************************************/
uchar SHT11_Read_Register(uchar command)
{
	uchar dat;
	SHT11_Start();
	SHT11_Sendbyte(command);
	SHT11_Answer();
	dat=SHT11_Receivebyte();
	SHT11_End();
	return(dat);
}
/***************************************
��������:SHT11_Measure(uchar command,uint time);
��������:����SHT11��⹦��,��������Ӧ�ļ����
����˵��:command�β������趨�¶ȼ�⻹��ʪ�ȼ��,
time�β������趨�������еĵȴ�ʱ�䣬��ȷ���������λ��
11ms/55ms/210ms �ֱ��Ӧ8λ/12λ/14λ 
****************************************/
uint SHT11_Measure(uchar command,uchar time)
{
	uint dat=0;
	uchar data_high,data_low;
	SHT11_Start();
	SHT11_Sendbyte(command);
	SHT11_Answer();
	Delay_Ms(time);
	SHT11_Test_Finish();
	data_high=SHT11_Receivebyte();
	MCU_Answer();
	data_low=SHT11_Receivebyte();
	SHT11_End();
	dat=(dat|data_high);
	dat=(dat<<8)|data_low;
	return(dat);
}
/****************************************
��������:Convert_Tempeture(uint dat);
��������:����⵽������ת��Ϊ��Ӧ���¶�����
����˵��:�¶�ת����ʽ--T=d1+d2*SOt
		  ��ʽ�еĲ���d1=-40,d2=0.01
		  ������14λ��������
*****************************************/
float SHT11_Convert_Tempeture14bit(uint dat)
{
	float tempeture1;
	tempeture1=-40+0.01*dat;
	if(tempeture1>100.0)
	{
		flag_tempeture=1;	
	}
	else if(tempeture1<0.0)
	{
		flag_tempeture=1;
	}
	else 
	{
		flag_tempeture=0;
	}
	return(tempeture1);
}

/*****************************************
��������:SHT11_Convert_Humidity(uint dat,float temp)
��������:����⵽������ת��Ϊ��Ӧ��ʪ������
����˵��:���ʪ��ת����ʽ-----RHline=C1+C2*SOrh+C3*SOrh*SOrh(������ݵ����Ի� SOrhΪ��Ƭ�����յ�������)
						 -----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
		  ��ʽ�еĲ���:C1=-4,C2=0,0405,C3=-0.0000028
					  t1=0.01,t2=0.00008
			������12λ��������
******************************************/
float SHT11_Convert_Humidity12bit(uint dat,float temp)
{
	float RHline,RHtrue;
	RHline=-4+0.0405*dat-0.0000028*dat*dat;
	RHtrue=(temp-25)*(0.01+0.00008*dat)+RHline;
	if(RHtrue<10.0)
	{
		flag_humidity=1;	
	}
	else 
	{
		flag_humidity=0;
	}
	return(RHtrue);
}

/****************************************
��������:Convert_Tempeture12bit(uint dat);
��������:����⵽������ת��Ϊ��Ӧ���¶�����
����˵��:�¶�ת����ʽ--T=d1+d2*SOt
		  ��ʽ�еĲ���d1=-40,d2=0.04
		  ������12λ��������
*****************************************/
float SHT11_Convert_Tempeture12bit(uint dat)
{
	float tempeture1;
	tempeture1=-40+0.04*dat;
	if(tempeture1>100.0)
	{
		flag_tempeture=1;	
	}
	else
	{
		flag_tempeture=0;
	}
	return(tempeture1);
}


/*****************************************
��������:SHT11_Convert_Humidity8bit(uint dat,float temp)
��������:����⵽������ת��Ϊ��Ӧ��ʪ������
����˵��:���ʪ��ת����ʽ-----RHline=C1+C2*SOrh+C3*SOrh*SOrh(������ݵ����Ի� SOrhΪ��Ƭ�����յ�������)
						 -----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
		  ��ʽ�еĲ���:C1=-4,C2=0,648,C3=-0.00072
					  t1=0.01,t2=0.00128
			������12λ��������
******************************************/
float SHT11_Convert_Humidity8bit(uint dat,float temp)
{
	float RHline,RHtrue;
	RHline=-4+0.648*dat-0.00072*dat*dat;
	RHtrue=(temp-25)*(0.01+0.00128*dat)+RHline;
	if(RHtrue<10.0)
	{
		flag_humidity=1;	
	}
	else 
	{
		flag_humidity=0;
	}
	return(RHtrue);
}

#endif