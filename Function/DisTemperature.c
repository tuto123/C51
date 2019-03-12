/*******************************************************************************

* 函数功能名:   温度显示

*******************************************************************************/

#include"lcd.h"
#include"temp.h"
#include"lcd1602.h"
uchar danwei[] = "T:";
void DisDate();
void LcdDisplay(int);
void DisTime();
void DisTemperture();
void DisDate();
/*******************************************************************************
* 函数名         : Distemperture()
* 函数功能		 : 显示温度
*******************************************************************************/

void DisTemperture()
{

//	LcdInit();			 //初始化LCD1602
//	LCD1602_Initialize();
    LCD1602_PrintString(2,1,danwei);
	LcdWriteCom(0xC5);	//写地址 80表示初始地址
	LcdWriteData('C');
//	DisTime();					  //显示时间
		    
//	DisClock();					  //显示闹钟序号
    
//	DisDate();					  //显示日期 
	
//	while(1)
//	{

		LcdDisplay(Ds18b20ReadTemp());
		
        
//	}
}

/*******************************************************************************
* 函数名         : LcdDisplay()
* 函数功能		 : LCD显示读取到的温度
*******************************************************************************/

void LcdDisplay(int temp) 	 //lcd显示
{
    
  	unsigned char  datas[] = {0, 0, 0, 0, 0}; //定义数组
	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
	  	LcdWriteCom(0xC2);		//写地址 80表示初始地址
		SBUF='-';//将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
	    LcdWriteData('-');  		//显示负
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由�0.5，还是在小数点后面。
 
  	}
 	else						 //当温度值为正数
  	{			
	  	LcdWriteCom(0xC2);		//写地址 C0表示初始地址
	    LcdWriteData('+'); 		//显示正
		SBUF='+';//将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
//	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
//	datas[3] = temp % 100 / 10;
//	datas[4] = temp % 10;

/*	LcdWriteCom(0xC1);		  //写地址 80表示初始地址
	LcdWriteData('0'+datas[0]); //百位 
	SBUF = '0'+datas[0];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;	*/
	
	LcdWriteCom(0xC3);		 //写地址 80表示初始地址
	LcdWriteData('0'+datas[1]); //十位
	SBUF = '0'+datas[1];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

	LcdWriteCom(0xC4);		//写地址 80表示初始地址
	LcdWriteData('0'+datas[2]); //个位 
	SBUF = '0'+datas[2];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

/*	LcdWriteCom(0xC3);		//写地址 80表示初始地址
	LcdWriteData('.'); 		//显示 ‘.’
	SBUF = '.';//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;	 */

/*	LcdWriteCom(0xC3);		 //写地址 80表示初始地址
	LcdWriteData('0'+datas[3]); //显示小数点  
	SBUF = '0'+datas[3];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

	LcdWriteCom(0xC4);		 //写地址 80表示初始地址
	LcdWriteData('0'+datas[4]); //显示小数点 
	SBUF = '0'+datas[4];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;*/
/*	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];//将接收到的数据放入到发送寄存器
		while (!TI);			         //等待发送数据完成
		TI = 0;
	} */

	 
}



