#include "reg52.h"	 //此文件中定义了单片机的一些特殊功能寄存器
#include "IR.h"		 //红外函数头文件
#include "dk.h"		 //常用函数
#include "UART.h"	 //串口初始化
#include "Buzzer.h"	 //蜂鸣器使用函数
#include "LCD1602.h" //LCD1602相关使用函数
#include "AT24C02.h" //掉电记忆使用寄存器相关函数
//全局变量
struct ir IRs; //红外相关变量
struct ks kn;  //键位码相关变量
struct time
{
	u16 Year;
	u8 Mon;
	u8 Day;
	u8 Hour;
	u8 Min;
	u8 Sec;
	u8 y1;
	u8 y2;
	u8 week;
};
struct time clock = {2022, 6, 27, 16, 39, 50, 0, 0}; //时间相关变量
struct ck											 //闹钟相关变量
{
	u8 hour;
	u8 min;
};
struct ck alarm = {8, 0};
struct md //模式码相关变量
{
	u8 time;
	u8 alarm;
	u8 TimeSetSelect;
	u8 AlarmSetSelect;
	u8 TimeSetFlashFlag;
	u8 Count;
};
struct md mod;
//声明函数
void IRs_int();	  //红外命令解析
void showtime();  //展示时间
void ReadTime();  //读取at24c02数据
void WriteTime(); //写入at24c02数据
void TimeBase();  //时间进制
void TimeSet();	  //时钟设置
void clock_set(); //闹钟设置
void UART_int();
//主函数入口
void main()
{
	//初始化
	IrInit();	   //红外初始化
	LCD_Init();	   // LCD1602初始化
	Timer0_Init(); //定时器初始化
	UART_Init();   //串口初始化
	//静态显示
	LCD_ShowString(1, 5, "-");
	LCD_ShowString(1, 8, "-");
	LCD_ShowString(2, 3, ":");
	LCD_ShowString(2, 6, ":");
	//从AT24C02读取时间
	ReadTime();
	while (1)
	{
		UART_int()
			IRs_int(); //红外数据解析初始化
		//基姆拉尔森计算公式——计算星期数
		clock.week = (clock.Day + 2 * clock.Mon + 3 * (clock.Mon + 1) / 5 + clock.Year + clock.Year / 4 - clock.Year / 100 + clock.Year / 400 + 1) % 7;
		if (kn.KeyNum == 1 || kn.Num == 69) //按键1按下或者红外按下mode
		{
			if (mod.time == 0) //模式0到模式1
			{
				mod.time = 1;
				mod.TimeSetSelect = 0; //时钟设置选择位
			}						   //功能切换
			else if (mod.time == 1)	   //模式1到模式2
			{
				mod.time = 2;
				mod.AlarmSetSelect = 0; //闹钟设置选择位
			}
			else if (mod.time == 2) //模式2到模式0
			{
				mod.time = 0;
			}
		}
		//模式判断
		switch (mod.time) //根据不同的功能执行不同的函数
		{
		case 0:
			showtime();	 //展示时钟
			TimeBase();	 //计算时间进制
			WriteTime(); //将时间数据写入AT24C02
			break;
		case 1:
			TimeSet(); // mode1:时间设置
			break;
		case 2:
			clock_set(); // mode2:闹钟设置
			break;
		}
		// 闹钟判断
		if (alarm.hour == clock.Hour && alarm.min == clock.Min) //当闹钟时间与时钟时间相等时响
		{
			Buzzer_Time(100);
		}
	}
}

/**
 * @brief  串口数据解析初始化
 */
void UART_int()
{
	switch (kn.test)
	{
	case 128:
		kn.nums = 0;
		break;
	case 129:
		kn.nums = 1;
		break;
	case 130:
		kn.nums = 2;
		break;
	case 131:
		kn.nums = 3;
		break;
	case 132:
		kn.nums = 4;
		break;
	case 133:
		kn.nums = 5;
		break;
	case 134:
		kn.nums = 6;
		break;
	case 135:
		kn.nums = 7;
		break;
	case 136:
		kn.nums = 8;
		break;
	case 137:
		kn.nums = 9;
		break;
	}
	// kn.nums = Identify_UNum(kn.test); //转化成数字
	// kn.Ua_command = kn.nums;
}

/**
 * @brief  红外数据解析初始化
 */
void IRs_int()
{
	IRs.IR_sum = (IRs.IrValue[2] / 16) * 10 + (IRs.IrValue[2] % 16); //键码值
	IRs.IR_num = Identify_Num(IRs.IR_sum);							 //转化成数字
	IRs.IrValue[2] = 0;												 //置零
	kn.Num = IRs.IR_num;											 //获取红外command
	kn.KeyNum = Key();												 //获取独立按键键位数据
	kn.MatrixKey = MatrixKey();										 //获取矩阵按键键位数据
}

/**
 * @brief  闹钟设置
 */
void clock_set()
{
	LCD_ShowString(2, 11, "alarms");	   //模式显示
	if (kn.MatrixKey == 2 || kn.Num == 96) //按键2按下
	{

		mod.AlarmSetSelect++;	 //设置选择位加1
		mod.AlarmSetSelect %= 2; //越界清零
	}
	//所在位置时间增加
	if (kn.MatrixKey == 3 || kn.Num == 32) //按键3按下
	{
		switch (mod.AlarmSetSelect)
		{
		case 0:
			alarm.hour++;
			if (alarm.hour > 23)
			{
				alarm.hour = 0;
			}
			break;
		case 1:
			alarm.min++;
			if (alarm.min > 59)
			{
				alarm.min = 0;
			}
			break;
		}
	}
	//所在位置时间减少
	if (kn.MatrixKey == 4 || kn.Num == 23) //按键4按下
	{
		switch (mod.AlarmSetSelect)
		{
		case 0:
			alarm.hour--;
			if (alarm.hour > 23)
			{
				alarm.hour = 23;
			}
			break;
		case 1:
			alarm.min--;
			if (alarm.min > 60)
			{
				alarm.min = 59;
			}
			break;
		}
	}
	//更新显示，根据mod.AlarmSetSelect和mod.TimeSetFlashFlag判断可完成闪烁功能
	if (mod.AlarmSetSelect == 0 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 1, "  ");
	}
	else
	{
		LCD_ShowNum(2, 1, alarm.hour, 2);
	}
	if (mod.AlarmSetSelect == 1 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 4, "  ");
	}
	else
	{
		LCD_ShowNum(2, 4, alarm.min, 2);
	}
	//静态显示
	LCD_ShowNum(1, 1, clock.Year, 4);
	LCD_ShowNum(1, 6, clock.Mon, 2);
	LCD_ShowNum(1, 9, clock.Day, 2);
	LCD_ShowNum(2, 7, clock.Sec, 2);
}

/**
 * @brief  时间设置
 */
void TimeSet()
{
	LCD_ShowString(2, 11, "change");	//模式显示
	if (kn.KeyNum == 2 || kn.Num == 96) //按键2按下
	{

		mod.TimeSetSelect++;	//设置选择位加1
		mod.TimeSetSelect %= 6; //越界清零
	}
	//所在位置时间增加
	if (kn.KeyNum == 3 || kn.Num == 32) //按键3按下
	{
		switch (mod.TimeSetSelect)
		{
		case 0:
			clock.Year++;
			break;
		case 1:
			clock.Mon++;
			if (clock.Mon > 12)
			{
				clock.Mon = 1;
			}
			break;
		case 2:
			clock.Day++;
			break;
		case 3:
			clock.Hour++;
			if (clock.Hour > 23)
			{
				clock.Hour = 0;
			}
			break;
		case 4:
			clock.Min++;
			if (clock.Min > 59)
			{
				clock.Min = 0;
			}
			break;
		case 5:
			clock.Sec++;
			break;
		}
	}
	//所在位置时间减少
	if (kn.KeyNum == 4 || kn.Num == 23) //按键4按下
	{
		switch (mod.TimeSetSelect)
		{
		case 0:
			clock.Year--;
			break;
		case 1:
			clock.Mon--;
			if (clock.Mon == 0)
			{
				clock.Mon = 12;
			}
			break;
		case 2:
			clock.Day--;
			if (clock.Day == 0)
			{
				clock.Day = 28;
			}
			break;
		case 3:
			clock.Hour--;
			if (clock.Hour > 23)
			{
				clock.Hour = 23;
			}
			break;
		case 4:
			clock.Min--;
			if (clock.Min > 60)
			{
				clock.Min = 59;
			}
			break;
		case 5:
			clock.Sec--;
			break;
		}
	}
	//更新显示，根据mod.TimeSetSelect和mod.TimeSetFlashFlag判断可完成闪烁功能
	if (mod.TimeSetSelect == 0 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 1, "    ");
	}
	else
	{
		LCD_ShowNum(1, 1, clock.Year, 4);
	}
	if (mod.TimeSetSelect == 1 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 6, "  ");
	}
	else
	{
		LCD_ShowNum(1, 6, clock.Mon, 2);
	}
	if (mod.TimeSetSelect == 2 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(1, 9, "  ");
	}
	else
	{
		LCD_ShowNum(1, 9, clock.Day, 2);
	}
	if (mod.TimeSetSelect == 3 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 1, "  ");
	}
	else
	{
		LCD_ShowNum(2, 1, clock.Hour, 2);
	}
	if (mod.TimeSetSelect == 4 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 4, "  ");
	}
	else
	{
		LCD_ShowNum(2, 4, clock.Min, 2);
	}
	if (mod.TimeSetSelect == 5 && mod.TimeSetFlashFlag == 1)
	{
		LCD_ShowString(2, 7, "  ");
	}
	else
	{
		LCD_ShowNum(2, 7, clock.Sec, 2);
	}
}

/**
 * @brief  时间显示与模式显示
 */
void showtime()
{
	//静态显示
	LCD_ShowNum(1, 1, clock.Year, 4);
	LCD_ShowNum(1, 6, clock.Mon, 2);
	LCD_ShowNum(1, 9, clock.Day, 2);
	LCD_ShowNum(2, 1, clock.Hour, 2);
	LCD_ShowNum(2, 4, clock.Min, 2);
	LCD_ShowNum(2, 7, clock.Sec, 2);
	//模式判断
	LCD_ShowString(2, 11, "normal");
	//闰平年判断
	if ((clock.Year % 4 == 0 && clock.Year % 100 != 0) || clock.Year % 400 == 0) //判断闰平年
	{
		LCD_ShowString(1, 16, "R");
	}
	else
	{
		LCD_ShowString(1, 16, "P");
	}
	//当天星期数显示
	// LCD_ShowNum(1, 12, clock.week, 2);
	LCD_ShowNum(1, 12, kn.test, 5);
}

/**
 * @brief  时间进制
 */
void TimeBase()
{
	static u8 t; //闰平年判断位
	//大于60 clock.Min++
	if (clock.Sec >= 60)
	{
		clock.Min++;
		clock.Sec = 0;
	}
	//大于60 clock.Hour++
	if (clock.Min >= 60)
	{
		clock.Hour++;
		clock.Min = 0;
	}
	//大于24 clock.Day++
	if (clock.Hour >= 24)
	{
		clock.Hour = 0;
		clock.Day++;
	}
	//重置闰年判断条件
	t = 0;
	//闰年判断
	if ((clock.Year % 4 == 0 && clock.Year % 100 != 0) || clock.Year % 400 == 0)
	{
		t = 1;
	}
	//判断闰年月份情况
	if (t == 1)
	{
		if (clock.Day > 29 && clock.Mon == 2)
		{
			clock.Mon++;
			clock.Day = 1;
		}
		else if (clock.Day > 30 && (clock.Mon == 4 || clock.Mon == 6 || clock.Mon == 9 || clock.Mon == 11))
		{
			clock.Mon++;
			clock.Day = 1;
		}
		else if (clock.Day > 31 && (clock.Mon == 1 || clock.Mon == 3 || clock.Mon == 5 || clock.Mon == 7 || clock.Mon == 8 || clock.Mon == 10 || clock.Mon == 12))
		{
			clock.Mon++;
			clock.Day = 1;
		}
	}
	//判断平年月份情况
	if (t != 1)
	{
		if (clock.Day > 28 && clock.Mon == 2)
		{
			clock.Mon++;
			clock.Day = 1;
		}
		else if (clock.Day > 31 && (clock.Mon == 1 || clock.Mon == 3 || clock.Mon == 5 || clock.Mon == 7 || clock.Mon == 8 || clock.Mon == 10 || clock.Mon == 12))
		{
			clock.Mon++;
			clock.Day = 1;
		}
		else if (clock.Day > 30 && (clock.Mon == 4 || clock.Mon == 6 || clock.Mon == 9 || clock.Mon == 11))
		{
			clock.Mon++;
			clock.Day = 1;
		}
	}
	//判断月份是否进入下一年
	if (clock.Mon > 12)
	{
		clock.Year++;
		clock.Mon = 1;
	}
	//重开世纪超过4位数
	if (clock.Year > 9999)
	{
		clock.Year = 0;
		clock.Mon = 0;
		clock.Day = 0;
		clock.Hour = 0;
		clock.Min = 0;
		clock.Sec = 0;
	}
}

/**
 * @brief  读取红外数值的中断函数
 */
void ReadIr() interrupt 0
{
	u8 j, k;
	u16 err; //延迟计量数
	IRs.Time = 0;
	IR_delay(700);
	if (IRIN == 0) //确认是否真的接收到正确的信号
	{

		err = 1000; // 1000*10us=10ms,超过说明接收到错误的信号
		/*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/
		while ((IRIN == 0) && (err > 0)) //等待前面9ms的低电平过去
		{
			IR_delay(1);
			err--;
		}
		if (IRIN == 1) //如果正确等到9ms低电平
		{
			err = 500;
			while ((IRIN == 1) && (err > 0)) //等待4.5ms的起始高电平过去
			{
				IR_delay(1);
				err--;
			}
			for (k = 0; k < 4; k++) //共有4组数据
			{
				for (j = 0; j < 8; j++) //接收一组数据
				{

					err = 60;
					while ((IRIN == 0) && (err > 0)) //等待信号前面的560us低电平过去
					{
						IR_delay(1);
						err--;
					}
					err = 500;
					while ((IRIN == 1) && (err > 0)) //计算高电平的时间长度。
					{
						IR_delay(10); // 0.1ms
						IRs.Time++;
						err--;
						if (IRs.Time > 30)
						{
							return;
						}
					}
					IRs.IrValue[k] >>= 1; // k表示第几组数据
					if (IRs.Time >= 8)	  //如果高电平出现大于565us，那么是1
					{
						IRs.IrValue[k] |= 0x80; //接收的第1个数为高电平;在第二个for循环中，数据会向右移8次
					}
					IRs.Time = 0; //用完时间要重新赋值
				}
			}
		}
		if (IRs.IrValue[2] != ~IRs.IrValue[3]) //与反码验证是否正确
		{
			return;
		}
	}
}

/**
 * @brief  中断函数
 */
void Timer0_Routine() interrupt 1 //中断函数,一般放在main.c里
{
	static unsigned int T0Count; //在中断函数内是局部变量，中断函数外是全局变量
	static unsigned int T1Count;
	TH0 = 64535 / 256; //赋初值
	TL0 = 64535 % 256;
	T0Count++; //定时秒钟
	T1Count++; //闪烁计时
	//定时器过一秒 clock.Sec++
	if (T0Count >= 1000)
	{
		T0Count = 0;
		clock.Sec++;
		TimeBase();
	}
	if (T1Count >= 500)
	{
		mod.TimeSetFlashFlag = !mod.TimeSetFlashFlag; //取反
	}
}
//串口中断
void UART_Routine() interrupt 4
{
	if (RI == 1) //如果接收标志位为1，接收到了数据
	{
		kn.test = SBUF;		 //读取数据
		UART_SendByte(SBUF); //将受到的数据发回串口
		RI = 0;				 //接收标志位清0
	}
}

/**
 * @brief  读取at24c02时间数据
 */
void ReadTime()
{
	clock.y1 = AT24C02_ReadByte(0);
	Delay(5);
	clock.y2 = AT24C02_ReadByte(1);
	Delay(5);
	clock.Mon = AT24C02_ReadByte(2);
	Delay(5);
	clock.Day = AT24C02_ReadByte(3);
	Delay(5);
	clock.Hour = AT24C02_ReadByte(4);
	Delay(5);
	clock.Min = AT24C02_ReadByte(5);
	Delay(5);
	clock.Sec = AT24C02_ReadByte(6);
	Delay(5);
	alarm.hour = AT24C02_ReadByte(7);
	Delay(5);
	alarm.min = AT24C02_ReadByte(8);
	Delay(5);
	clock.Year = (clock.y1 * 100) + clock.y2; //计算clock.Year
}

/**
 * @brief  写入at24c02时间数据
 */
void WriteTime()
{
	clock.y2 = (clock.Year % 100); //拆分clock.Year到clock.y1，clock.y2方便写入
	clock.y1 = (clock.Year / 100);
	AT24C02_WriteByte(0, clock.y1);
	Delay(5);
	AT24C02_WriteByte(1, clock.y2);
	Delay(5);
	AT24C02_WriteByte(2, clock.Mon);
	Delay(5);
	AT24C02_WriteByte(3, clock.Day);
	Delay(5);
	AT24C02_WriteByte(4, clock.Hour);
	Delay(5);
	AT24C02_WriteByte(5, clock.Min);
	Delay(5);
	AT24C02_WriteByte(6, clock.Sec);
	Delay(5);
	AT24C02_WriteByte(7, alarm.hour);
	Delay(5);
	AT24C02_WriteByte(8, alarm.min);
	Delay(5);
}
