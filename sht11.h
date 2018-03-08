#ifndef __SHT11_H__
#define __SHT11_H__

/*************************
SHT11相关命令
**************************/

#define TEM_TEST 		0x03//温度检测命令
#define HUM_TEST  		0x05//湿度检测命令
#define REG_READ  		0x07//读寄存器
#define REG_WRITE 		0x06//写寄存器
#define FUNCTION_SET	0x01//设置SHT11的工作精度为8位/湿度 12位温度
#define uchar unsigned char 
#define uint  unsigned int
/**************************
SHT11端口定义
***************************/
sbit SHT11_DATA=P2^0;
sbit SHT11_SCK=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
uchar flag_tempeture=0;  //显示温度位置的标志
uchar flag_humidity=0;	 //显示湿度位置的标志
uchar code str1[]={ 0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00};//温度图标
uchar code str6_sht11[]="%RH   "; 
uchar code str4_sht11[]="humi=";
uchar code str2_sht11[]="temp=";
uchar code str7_sht11[]="    ";//清除没不要的显示
/***************************
函数名称:Delay()
函数功能:SHT11内部延时
****************************/
void Delay()
{	;
	;
}

/***************************
函数名称:Delay_Ms()
函数功能:SHT11检测等待延时
函数说明:11ms/55ms/210ms 分别对应8位/12位/14位 测量结果
对应的形参为N 则延时Nms
****************************/
void Delay_Ms(uint ms)
{
	uint i,j;
	for(i=ms;i>0;i--)
		for(j=112;j>0;j--);
}
/**************************
函数功能:SHT11启动时序
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
函数名称：SHT11_Sendbyte(uchar dat)
函数功能: 向SHT11发送8bite数据
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
函数名称SHT11_Answer()：
函数功能:检测SHT11的响应信号(在第九个时钟周期)
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
函数名称：SHT11_Test_Finish()
函数功能:检测SHT11温湿度检测是否完毕
*************************************/
void SHT11_Test_Finish()
{
	uchar i;
	while(SHT11_DATA==1 && i<255)
		i++;;
}

/************************************
函数名称：SHT11_Receivebyte()
函数功能:从SHT11接收8bite数据
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
	SHT11_DATA=1; //释放数据总线
	return(dat);	
}

/***********************************
函数名称：MCU_Answer()
函数功能:单片机向SHT11发送应答信号
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
	SHT11_DATA=1; //释放数据总线 这条指令非常重要 不加的话导致单片机不能读取低8位
}
/***********************************
函数名称:SHT11_End()
当接收两个8byte数据后部接收CRC校验码
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
函数名称：void SHT11_Write_Register(uchar command ,uchar dat)
函数说明：向SHT11的状态寄存器设置功能
			command为REG_WRITE 0x06写寄存器
			dat为 设置SHT11的功能 可以设置检测的数据位数
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
函数名称：uchar SHT11_Read_Register(uchar command)
函数说明：command为REG_READ  0x07//读寄存器
			返回值为状态寄存器的值
		 位6显示当前检测完一次数据后电源供电情况
			当位6为0时表明VDD>2.47V 当位6为1时表明VDD<2.47V即电量不足
		 位0表明当前的测量分辨率
		 当位0为1时表明测量精度：8位/湿度 12位温度
		 当位0为0时表明测量精度：12位湿度 14位温度 
		 默认为0
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
函数名称:SHT11_Measure(uchar command,uint time);
函数功能:设置SHT11检测功能,并返回相应的检测结果
函数说明:command形参用于设定温度检测还是湿度检测,
time形参用于设定检测过程中的等待时间，以确定检测结果的位数
11ms/55ms/210ms 分别对应8位/12位/14位 
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
函数名称:Convert_Tempeture(uint dat);
函数功能:将检测到的数据转化为相应的温度数据
函数说明:温度转换公式--T=d1+d2*SOt
		  公式中的参数d1=-40,d2=0.01
		  适用于14位测量精度
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
函数名称:SHT11_Convert_Humidity(uint dat,float temp)
函数功能:将检测到的数据转化为相应的湿度数据
函数说明:相对湿度转换公式-----RHline=C1+C2*SOrh+C3*SOrh*SOrh(检测数据的线性化 SOrh为单片机接收到的数据)
						 -----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
		  公式中的参数:C1=-4,C2=0,0405,C3=-0.0000028
					  t1=0.01,t2=0.00008
			适用于12位测量精度
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
函数名称:Convert_Tempeture12bit(uint dat);
函数功能:将检测到的数据转化为相应的温度数据
函数说明:温度转换公式--T=d1+d2*SOt
		  公式中的参数d1=-40,d2=0.04
		  适用于12位测量精度
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
函数名称:SHT11_Convert_Humidity8bit(uint dat,float temp)
函数功能:将检测到的数据转化为相应的湿度数据
函数说明:相对湿度转换公式-----RHline=C1+C2*SOrh+C3*SOrh*SOrh(检测数据的线性化 SOrh为单片机接收到的数据)
						 -----RHtrue=(tempeture-25)*(t1+t2*SOrh)+RHline
		  公式中的参数:C1=-4,C2=0,648,C3=-0.00072
					  t1=0.01,t2=0.00128
			适用于12位测量精度
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