#include <REGX52.H>
#include "IR.h"
#include "AT24C02.h"
#include "LCD1602.h"
#include "dk.h"

struct time
{
    u8 Mon;
    u8 Day;
    u8 Hour;
    u8 Min;
    u8 Sec;
    u8 y1;
    u8 y2;
    u16 Year;
};
struct time clock;

