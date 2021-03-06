#include "DS1302.h"

sbit CE		=	P3^5;	 //RST
sbit SCLK	=	P3^6;	
sbit SDA	=	P3^4;	 //DSIO

unsigned char clockdata[8];
unsigned char clockdataBCD[8];
unsigned char BRAMdata[31];

/**************************************************************************************************************/
//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
unsigned char code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
unsigned char code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2013年1月1日星期二10点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
//unsigned char TIME[7] = {0, 0x42, 0x10, 0x10, 0x01, 0x03, 0x19};

/*******************************************************************************
* 函 数 名         : Ds1302Write
* 函数功能		   : 向DS1302命令（地址+数据）
* 输    入         : addr,dat
* 输    出         : 无
*******************************************************************************/

/*void Ds1302Write(unsigned char addr, unsigned char dat)
{
	unsigned char n;
	CE = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	CE = 1; //然后将RST(CE)置高电平。
	_nop_();

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
		SDA = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//写入8位数据
	{
		SDA = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	CE = 0;//传送数据结束
	_nop_();
}*/

/*******************************************************************************
* 函 数 名         : Ds1302Read
* 函数功能		   : 读取一个地址的数据
* 输    入         : addr
* 输    出         : dat
*******************************************************************************/
 /*
unsigned char Ds1302Read(unsigned char addr)
{
	unsigned char n,dat,dat1;
	CE = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	CE = 1;//然后将RST(CE)置高电平。
	_nop_();

	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		SDA = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = SDA;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}

	CE = 0;
	_nop_();	//以下为DS1302复位的稳定时间,必须的。
	SCLK = 1;
	_nop_();
	SDA = 0;
	_nop_();
	SDA = 1;
	_nop_();
	return dat;	
}  */
   
/*******************************************************************************
* 函 数 名         : Ds1302Init
* 函数功能		   : 初始化DS1302.
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

/*void Ds1302Init()
{
	unsigned char n;
	Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}*/

/*******************************************************************************
* 函 数 名         : Ds1302ReadTime
* 函数功能		   : 读取时钟信息
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

/*void Ds1302ReadTime()
{
	unsigned char n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}	*/


/*****************************************************************************************************************/



//向DS1302写一个字节的数据
void DS1302_SPIwritebyte( unsigned char dat )
{
	unsigned char i;

	for ( i = 0; i < 8; i++ )
	{	
		SCLK = 0;
		SDA = ( dat >> i ) & 0x01;
				
		_nop_();
		SCLK = 1;
	}
}

//从DS1302读一个字节的数据
unsigned char DS1302_SPIreadbyte( )
{
	unsigned char i, tmp;
	
	tmp  = 0;
	for ( i = 0; i < 8; i++ )
	{
		if ( SDA )
			tmp = tmp | 0x80;
		tmp = tmp >> 1;

		SCLK = 1;
		_nop_();
		_nop_();
		SCLK = 0;
	}

	SDA = 0; _nop_(); _nop_();			 //不加会闪屏或显示85错误

	return tmp;
}

//向DS1302写一个字节的数据到地址为address的单元（寄存器）
void DS1302_writeybyte( unsigned char address, unsigned char dat )
{
	CE = 0; _nop_();; 
	SCLK = 0; _nop_();
	SDA = 0; _nop_();
	CE = 1; _nop_();

	DS1302_SPIwritebyte( address );
	DS1302_SPIwritebyte( dat );	
	
	CE = 0; _nop_();_nop_();
	SCLK = 1; _nop_();_nop_();
	SDA = 1; _nop_();_nop_();	 

}

//从DS1302的address的单元（寄存器）读一个字节的数据
unsigned char DS1302_readbyte( unsigned char address )
{
	unsigned char tmp;

	tmp = 0;

	CE = 0; _nop_(); 
	SCLK = 0; _nop_();
	SDA = 0; _nop_();
	CE = 1; _nop_();

	DS1302_SPIwritebyte( address );
	tmp =  DS1302_SPIreadbyte( );

	CE = 0; _nop_();_nop_();
	SCLK = 1; _nop_();_nop_();
	SDA = 1; _nop_();_nop_();

	return tmp;
}


//突发模式写时钟数据，数据存放在clockdataBCD[8]中
void DS1302_writeclockburst( )
{	 
	 unsigned char i;
	 
	 //取消写保护
	 DS1302_unlock();

	 CE = 0; _nop_();; 
	SCLK = 0; _nop_();
	SDA = 0; _nop_();
	CE = 1; _nop_();

	//写入突发模式写寄存器地址
	DS1302_SPIwritebyte( DS1302_CLOCKBURST + DS1302_WRITE );

	for ( i = 0; i < 8; i++ )	 //连续写入8个字节数据
		DS1302_SPIwritebyte( clockdataBCD[i] );	
	
	CE = 0; _nop_();_nop_();
	SCLK = 1; _nop_();_nop_();
	SDA = 1; _nop_();_nop_();
	 
	 	 	
	 //开启写保护
	 DS1302_lock();
}


//突发模式读时钟数据，数据存放在clockdataBCD[8]中
void DS1302_readclockburst( )
{
	unsigned char i;
	 
	 //取消写保护
	DS1302_unlock();

	CE = 0; _nop_();; 
	SCLK = 0; _nop_();
	SDA = 0; _nop_();
	CE = 1; _nop_();

	//写入突发模式写寄存器地址
	DS1302_SPIwritebyte( DS1302_CLOCKBURST + DS1302_READ );

	for ( i = 0; i < 8; i++ )	  //连续读取8个字节数据
		clockdataBCD[i] = DS1302_SPIreadbyte( );	
	
	CE = 0; _nop_();_nop_();
	SCLK = 1; _nop_();_nop_();
	SDA = 1; _nop_();_nop_(); 
	 	 	
	 //开启写保护
	 DS1302_lock();
}


//突发模式写ram数据，数据存放在BRAMdata[31]中
void DS1302_writeramburst( )
{
	 unsigned char i;
	 
	 //取消写保护
	 DS1302_unlock();

	 CE = 0; _nop_();; 
	SCLK = 0; _nop_();
	SDA = 0; _nop_();
	CE = 1; _nop_();

	//写入突发模式写寄存器地址
	DS1302_SPIwritebyte( DS1302_BRAMBURST + DS1302_WRITE );

	for ( i = 0; i < 31; i++ )	 //连续写入31个字节数据
		DS1302_SPIwritebyte( BRAMdata[i] );	
	
	CE = 0; _nop_();_nop_();
	SCLK = 1; _nop_();_nop_();
	SDA = 1; _nop_();_nop_();	 
	 	 	
	//开启写保护
	DS1302_lock();	
}

//突发模式读ram数据，数据存放在BRAMdata[31]中
void DS1302_readramburst( )
{
	unsigned char i;
	 
	 //取消写保护
	DS1302_unlock();

	CE = 0; _nop_();; 
	SCLK = 0; _nop_();
	SDA = 0; _nop_();
	CE = 1; _nop_();

	//写入突发模式写寄存器地址
	DS1302_SPIwritebyte( DS1302_BRAMBURST + DS1302_READ );

	for ( i = 0; i < 31; i++ )	 //连续读取31个字节数据
		BRAMdata[i] = DS1302_SPIreadbyte( );	
	
	CE = 0; _nop_();_nop_();
	SCLK = 1; _nop_();_nop_();
	SDA = 1; _nop_();_nop_();
	 
	 	 	
	//开启写保护
	DS1302_lock();
}


//上锁（开启写保护）
void DS1302_lock()
{
	DS1302_writeybyte( DS1302_CONTROL +  DS1302_WRITE, 0x80 );
}

//解锁（取消写保护）
void DS1302_unlock()
{
	DS1302_writeybyte( DS1302_CONTROL +  DS1302_WRITE, 0x00 );
}


//获得秒钟数据，输出为二进码
unsigned char DS1302_getsecond()				  //秒
{
	unsigned char tmp;

	//读取秒数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_SECOND + DS1302_READ );
	
	//(tmp & 0x70) >> 4得到秒的十位部分，(tmp & 0x0F)得到秒的个位部分
	return ( ((tmp & 0x70) >> 4) * 10 + (tmp & 0x0F) );
}

//获得分钟数据，输出为二进码
unsigned char DS1302_getminute()				  //分
{
	unsigned char tmp;

	//读取分数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_MINUTE + DS1302_READ );
	
	//(tmp & 0x70) >> 4得到分的十位部分，(tmp & 0x0F)得到分的个位部分
	return ( ((tmp & 0x70) >> 4) * 10 + (tmp & 0x0F) );
}

//获得小时数据，输出为二进码，24进制
unsigned char DS1302_gethour()					  //时
{
	unsigned char tmp;

	//读取小时数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_HOUR + DS1302_READ );

	//最高位【7】区分12进制、24进制
	if ( tmp & 0x80 ) //==========================12小时制
		//(( tmp & 0x20 ) >> 5)区分AM(0)、PM(1),0x20对应第5位,PM需要加12, AM不需要加12
		//( tmp & 0x10 ) >> 4)得到时的十位部分，0x10对应第4位
		//(tmp & 0x0F)得到时的个位部分  
		return ( (( tmp & 0x20 ) >> 5) * 12 + (( tmp & 0x10 ) >> 4) * 10  + ( tmp & 0x0F ) ) ;	
	else			//==========================24小时制
		//( tmp & 0x20 ) >> 5)得到时的第二个十位部分，(( tmp & 0x10 ) >> 4)得到时的第一个十位部分，(tmp & 0x0F)得到时的个位部分
	    return ( (( tmp & 0x20 ) >> 5) * 20 + (( tmp & 0x10 ) >> 4) * 10  + ( tmp & 0x0F ) ) ;
}

//获得日期
unsigned char DS1302_getdate()					  //日
{
	unsigned char tmp;

	//读取日期数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_DATE + DS1302_READ );
	
	//(tmp & 0x30) >> 4得到日期的十位部分，(tmp & 0x0F)得到日期的个位部分
	return ( ((tmp & 0x30) >> 4) * 10 + (tmp & 0x0F) );	
}

//获得月份
unsigned char DS1302_getmonth()				  //月
{
	unsigned char tmp;

	//读取月份数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_MONTH + DS1302_READ );
	
	//(tmp & 0x10) >> 4得到月的十位部分，(tmp & 0x0F)得到月的个位部分
	return ( ((tmp & 0x10) >> 4) * 10 + (tmp & 0x0F) );	
}

//获得星期
unsigned char DS1302_getweekday()				  //周
{
	unsigned char tmp;

	//读取星期数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_WEEKDAY + DS1302_READ );
	
	//(tmp & 0x07)得到星期几
	return ( tmp & 0x07 );	
}

//获得年分
unsigned char DS1302_getyear()					  //年
{
	unsigned char tmp;

	//读取星期数据，tmp为BCD码
	tmp = DS1302_readbyte ( DS1302_YEAR + DS1302_READ );
	
	//高4位为年的十位部分，低4位为年的个位部分
	return ( ((tmp & 0xF0) >> 4) * 10 + (tmp & 0x0F) );
}



//设置时钟---秒数据，输入为二进码
void DS1302_setsecond( unsigned char second )	  //秒
{
	if ( second > 59 )
		return;
	
	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_SECOND + DS1302_WRITE，需要转换为BCD码
	//(second/10)得到秒的十位，( second %10 )得到秒的个位
	DS1302_writeybyte( DS1302_SECOND + DS1302_WRITE, (((second/10) & 0x07) << 4) | ( second %10 ) );
	
	//上锁
	DS1302_lock();
}

//设置时钟---分数据，输入为二进码
void DS1302_setminute( unsigned char minute )	  //分
{
	if ( minute > 59 )
		return;
	
	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_MINUTE + DS1302_WRITE，需要转换为BCD码
	//(second/10)得到分的十位，( second %10 )得到分的个位
	DS1302_writeybyte( DS1302_MINUTE + DS1302_WRITE, (((minute/10) & 0x07) << 4) | ( minute %10 ) );
	
	//上锁
	DS1302_lock();
}

//设置时钟---时数据，输入为24小时进制二进码 
void DS1302_sethour( unsigned char hour )		  //时
{
	unsigned char tmp;	

	tmp = 0;

	//获得DS1302当前的小时进制
	if ( DS1302_readbyte ( DS1302_HOUR + DS1302_READ ) & 0x80 )						 //12小时制
	{
		tmp = tmp | 0x80;				//还是12小时制
		if ( hour > 12 )				//PM
		{
			tmp = tmp | 0x20;			//第5位为1表示PM,为0表示AM（省略）
		   	hour = hour - 12;
		}				
	}
	else									 //24小时制
	{
		if ( hour > 20 )
		{
			tmp = tmp | 0x20;				//第二个十
		   	hour = hour - 20;
		}
	}
	
	if ( hour > 9 )				//设置12小时制的十位或24小时制的第一个十;
			tmp = tmp | 0x10;		//第4位

	tmp = tmp | (( hour % 10 ) & 0x0F);	//个位为低4位	

	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_HOUR + DS1302_WRITE，tmp已经转换为BCD码
	DS1302_writeybyte( DS1302_HOUR + DS1302_WRITE, tmp );
	
	//上锁
	DS1302_lock();	
}

//设置时钟---日期数据，输入为二进码
void DS1302_setdate( unsigned char date )		  //日
{
	if ( date > 31 )
		return;
	
	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_DATE + DS1302_WRITE，需要转换为BCD码
	//(date/10)得到日期的十位，( date %10 )得到日期的个位
	DS1302_writeybyte( DS1302_DATE + DS1302_WRITE, (((date/10) & 0x03) << 4) | ( date %10 ) );
	
	//上锁
	DS1302_lock();
}

//设置时钟---月份数据，输入为二进码
void DS1302_setmonth( unsigned char month )		//月
{
	if ( month > 12 || month < 1 )
		return;
	
	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_MONTH + DS1302_WRITE，需要转换为BCD码
	//(month/10)得到月的十位，( month %10 )得到月的个位
	DS1302_writeybyte( DS1302_MONTH + DS1302_WRITE, (((month/10) & 0x01) << 4) | ( month % 10 ) );
	
	//上锁
	DS1302_lock();	
}

//设置时钟---星期数据，输入为二进码
void DS1302_setweekday( unsigned char weekday )  //周
{
   	if ( weekday > 7 || weekday < 1 )			//检查数据有效性
		return;
	
	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_WEEKDAY + DS1302_WRITE，需要转换为BCD码
	//(weekday & 0x07)得到星期值
	DS1302_writeybyte( DS1302_WEEKDAY + DS1302_WRITE, ( weekday & 0x07 ) );
	
	//上锁
	DS1302_lock();
}

//设置时钟---年数据，输入为二进码
void DS1302_setyear( unsigned char year )	  	//年
{
	if ( year > 99 )			//检查数据有效性
		return;

	//先解锁
	DS1302_unlock();

	//写入数据，地址为DS1302_YEAR + DS1302_WRITE，需要转换为BCD码
	//(weekday & 0x07)得到星期值
	DS1302_writeybyte( DS1302_YEAR + DS1302_WRITE, ( ((year/10) & 0x0F) << 4) | ((year % 10) & 0x0F) );
	
	//上锁
	DS1302_lock();
}



//设置DS1302的时间，输入数据存放在clockdata数组中
void DS1302_settime( )
{
	 DS1302_setsecond( clockdata[0] );	  		//秒
	 DS1302_setminute( clockdata[1] );	  		//分
	 DS1302_sethour( clockdata[2] );		  //时
	 DS1302_setdate( clockdata[3] );		  //日
	 DS1302_setmonth( clockdata[4] );	  		//月
	 DS1302_setyear( clockdata[5] );		  //年
	 DS1302_setweekday( clockdata[6] );  		//周
	 		
}

//获取DS1302的时间，输出数据存放在clockdata数组中 
void DS1302_gettime( )
{
	clockdata[0]  = DS1302_getsecond();				  //秒
	clockdata[1]  = DS1302_getminute();				  //分
	clockdata[2]  = DS1302_gethour();					  //时
	clockdata[3]  = DS1302_getdate();					  //日
	clockdata[4]  = DS1302_getmonth();				  //月
	clockdata[5]  = DS1302_getyear();					  //年
	clockdata[6]  = DS1302_getweekday();				  //周	
}
