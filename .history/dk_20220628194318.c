#include <regx52.h>

unsigned char ntb[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
	0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0X76};
// 0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F、H的显示码

void Delay(unsigned int xms); //延时函数声明

/**
 * @brief  数码管显示
 * @param  location 显示的地址
 * @param  num 要显示的数据
 * @retval 无
 */
void Nt(unsigned char location, num) //数码管显示： 位置 数据
{
	switch (location)
	{
	case 1:
		P2_4 = 1;
		P2_3 = 1;
		P2_2 = 1;
		break;
	case 2:
		P2_4 = 1;
		P2_3 = 1;
		P2_2 = 0;
		break;
	case 3:
		P2_4 = 1;
		P2_3 = 0;
		P2_2 = 1;
		break;
	case 4:
		P2_4 = 1;
		P2_3 = 0;
		P2_2 = 0;
		break;
	case 5:
		P2_4 = 0;
		P2_3 = 1;
		P2_2 = 1;
		break;
	case 6:
		P2_4 = 0;
		P2_3 = 1;
		P2_2 = 0;
		break;
	case 7:
		P2_4 = 0;
		P2_3 = 0;
		P2_2 = 1;
		break;
	case 8:
		P2_4 = 0;
		P2_3 = 0;
		P2_2 = 0;
		break;
	}
	P0 = ntb[num];
	Delay(1);
	P0 = 0x00;
}

/**
 * @brief  延时函数
 * @param  xms 延时时间
 * @retval 无
 */
void Delay(unsigned int xms) //延时函数： 时间
{
	unsigned char i, j;
	while (xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j)
				;
		} while (--i);
	}
}
/**
 * @brief  获取独立按键键码
 * @param  无
 * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
 */
unsigned char Key()
{
	unsigned char KeyNumber = 0;

	if (P3_1 == 0)
	{
		Delay(20);
		while (P3_1 == 0)
			;
		Delay(20);
		KeyNumber = 1;
	}
	if (P3_0 == 0)
	{
		Delay(20);
		while (P3_0 == 0)
			;
		Delay(20);
		KeyNumber = 2;
	}
	if (P3_2 == 0)
	{
		Delay(20);
		while (P3_2 == 0)
			;
		Delay(20);
		KeyNumber = 3;
	}
	if (P3_3 == 0)
	{
		Delay(20);
		while (P3_3 == 0)
			;
		Delay(20);
		KeyNumber = 4;
	}

	return KeyNumber;
}

/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
			如果按键按下不放，程序会停留在此函数，松手的一瞬间，返回按键键码，没有按键按下时，返回0
  */
unsigned char MatrixKey()
{
	unsigned char KeyNumber = 0;

	P1 = 0xFF;
	P1_3 = 0;
	if (P1_7 == 0)
	{
		Delay(20);
		while (P1_7 == 0)
			;
		Delay(20);
		KeyNumber = 1;
	}
	if (P1_6 == 0)
	{
		Delay(20);
		while (P1_6 == 0)
			;
		Delay(20);
		KeyNumber = 5;
	}
	if (P1_5 == 0)
	{
		Delay(20);
		while (P1_5 == 0)
			;
		Delay(20);
		KeyNumber = 9;
	}
	if (P1_4 == 0)
	{
		Delay(20);
		while (P1_4 == 0)
			;
		Delay(20);
		KeyNumber = 13;
	}

	P1 = 0xFF;
	P1_2 = 0;
	if (P1_7 == 0)
	{
		Delay(20);
		while (P1_7 == 0)
			;
		Delay(20);
		KeyNumber = 2;
	}
	if (P1_6 == 0)
	{
		Delay(20);
		while (P1_6 == 0)
			;
		Delay(20);
		KeyNumber = 6;
	}
	if (P1_5 == 0)
	{
		Delay(20);
		while (P1_5 == 0)
			;
		Delay(20);
		KeyNumber = 10;
	}
	if (P1_4 == 0)
	{
		Delay(20);
		while (P1_4 == 0)
			;
		Delay(20);
		KeyNumber = 14;
	}

	P1 = 0xFF;
	P1_1 = 0;
	if (P1_7 == 0)
	{
		Delay(20);
		while (P1_7 == 0)
			;
		Delay(20);
		KeyNumber = 3;
	}
	if (P1_6 == 0)
	{
		Delay(20);
		while (P1_6 == 0)
			;
		Delay(20);
		KeyNumber = 7;
	}
	if (P1_5 == 0)
	{
		Delay(20);
		while (P1_5 == 0)
			;
		Delay(20);
		KeyNumber = 11;
	}
	if (P1_4 == 0)
	{
		Delay(20);
		while (P1_4 == 0)
			;
		Delay(20);
		KeyNumber = 15;
	}

	P1 = 0xFF;
	P1_0 = 0;
	if (P1_7 == 0)
	{
		Delay(20);
		while (P1_7 == 0)
			;
		Delay(20);
		KeyNumber = 4;
	}
	if (P1_6 == 0)
	{
		Delay(20);
		while (P1_6 == 0)
			;
		Delay(20);
		KeyNumber = 8;
	}
	if (P1_5 == 0)
	{
		Delay(20);
		while (P1_5 == 0)
			;
		Delay(20);
		KeyNumber = 12;
	}
	if (P1_4 == 0)
	{
		Delay(20);
		while (P1_4 == 0)
			;
		Delay(20);
		KeyNumber = 16;
	}

	return KeyNumber;
}

/**
 * @brief  定时器0初始化，1毫秒@12.000MHz
 * @param  无
 * @retval 无
 */
void Timer0_Init() //定时器： 初始化
{
	TMOD = 0x01; // 定时/计数器的工作方式寄存器，确定工作方式和功能
	TF0 = 0;
	TR0 = 1;
	TH0 = 64535 / 256;
	TL0 = 64535 % 256;
	ET0 = 1;
	EA = 1; //总中断允许位
	PT0 = 0;
}

/*定时器中断函数模板
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;

	}
}
*/
