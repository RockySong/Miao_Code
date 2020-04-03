/*(Ver.=0.96)
* MIAO_tool_string.c
*
*  Created on: 2018-08-22
*      Author: slw
*/

#include "OSLW_include.h"
#if !(MIAO_SIMPLE_LEVEL >= 4)



lw_sf MiaoToolStringTolw_sf(const lw_u8 *str)
{
	lw_sf s = _ParaFrom(0.0);
	lw_sf d = _ParaFrom(10.0);
	uint32_t jishu = 0;

	lw_u8 falg = 0;

	MIAO_assert(!(str));

	while (*str == ' ')
	{
		str++;
	}

	if (*str == '-')//记录数字正负
	{
		falg = 1;
		str++;
	}

	if (!(*str >= '0'&&*str <= '9'))//如果一开始非数字则退出，返回0.0
		return s;

	while (*str >= '0'&&*str <= '9'&&*str != '.')//计算小数点前整数部分
	{
		s = s*10.0f + *str - '0';
		str++;
	}

	if (*str == '.')//以后为小数部分
		str++;

	while (*str >= '0'&&*str <= '9')//计算小数部分
	{
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}

	if (*str == 'e' || *str == 'E')//考虑科学计数法
	{
		str++;
		if (*str == '+')
		{
			str++;
			while (*str >= '0'&&*str <= '9')
			{
				jishu = jishu * 10 + *str - '0';
				str++;
			}
			while (jishu > 0)
			{
				s *= 10;
				jishu--;
			}
		}
		if (*str == '-')
		{
			str++;
			while (*str >= '0'&&*str <= '9')
			{
				jishu = jishu * 10 + *str - '0';
				str++;
			}
			while (jishu > 0)
			{
				s /= 10;
				jishu--;
			}
		}
	}

	return s*(falg ? -1.0f : 1.0f);
}


lw_32 MiaoToolStringToInt(const lw_u8 *str)
{
	lw_32 Ret_Integer = 0;
	lw_u8 Integer_sign = 0;

	MIAO_assert(!(str));

	while (*str == ' ')
	{
		str++;
	}


	if (*str == '-')
	{
		Integer_sign = 1;
	}

	if (*str == '-' || *str == '+')
	{
		str++;
	}

	while (*str >= '0' && *str <= '9')
	{
		Ret_Integer = Ret_Integer * 10 + *str - '0';
		str++;
	}
	

	return Integer_sign ?  -Ret_Integer : Ret_Integer;
}


static const lw_u8 _str_list[] = "0123456789abcdef";

lw_u8* MiaoToolString_itoa(lw_32 value, lw_u8 *str, lw_32 radix) 
{
	lw_8 reverse[20];
	lw_8 *p = reverse;
	lw_32 sign = (value >= 0) ? 1 : 0;

	value = (value >= 0) ? value : -value;
	*p++ = '\0';
	while (value >= 0) {
		*p++ = _str_list[value%radix];
		value /= radix;
		if (value == 0) break;
	}

	if (!sign) {
		*p = '-';
	}
	else {
		p--;
	}

	while (p >= reverse) {
		*str++ = *p--;
	}

	return str;
}


lw_u8* MiaoToolString_utoa(uint32_t value, lw_u8 *str, lw_32 radix) 
{
	lw_8 reverse[20];
	lw_8 *p = reverse;

	*p++ = '\0';
	while (value != 0) {
		*p++ = _str_list[value%radix];
		value /= radix;
		if (value == 0) break;
	}
	p--;

	while (p >= reverse) {
		*str++ = *p--;
	}

	return str;
}


lw_u8* MiaoToolString_gcvt(lw_sf value, lw_32 ndigit, lw_u8 *buf) 
{
	lw_8 tmpbuf[40];
	lw_32 int_part = (int)value;

	lw_sf folat_part = value - int_part;
	lw_8 *p=tmpbuf;

	if(value<0)
	{
	    *p++='-';
	    int_part=-int_part;
	}

	if (folat_part < 0)
	{
	    folat_part = -folat_part;
	}
	MiaoToolString_itoa(int_part, p, 10);

	while (*p != '\0') p++;

	*p++ = '.';

	while (ndigit > 0 && folat_part > 0.0000001) {
		*p++ = (int)(folat_part * 10) + '0';
		folat_part = (folat_part * 10) - (int)(folat_part * 10);
		ndigit--;
	}

	*p = '\0';
	strcpy(buf, tmpbuf);
	return buf;
}

void* MiaoToolStringVSprintf(lw_8 *buf, const lw_8 *fmt, va_list args)
{
	lw_8 *p;
	va_list p_next_arg = args;

	for (p = buf; *fmt; fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}
		fmt++;  // *fmt = '%'
		switch (*fmt) {
		case 'd':
			MiaoToolString_itoa(va_arg(p_next_arg, int), p, 10);
			p += strlen(p);
			break;
		case 'x':
			MiaoToolString_utoa(va_arg(p_next_arg, unsigned int), p, 16);
			p += strlen(p);
			break;
		case 'c':
			*p++ = va_arg(p_next_arg, char);
			break;
		case 's':
			*p = '\0';
			strcat(p, va_arg(p_next_arg, char *));
			p += strlen(p);
			break;
		case 'f':
			MiaoToolString_gcvt(va_arg(p_next_arg, lw_sf), 6, p);
			p += strlen(p);
			break;
		default:
			break;
		}
	}
	*p = '\0';
	return p;
}

void* MiaoToolStringSprintf(lw_8 *buf, const lw_8 *fmt, ...)
{
	lw_u8 *p;
	va_list args;
	va_start(args, fmt);

	p=MiaoToolStringVSprintf(buf, fmt, args);

	va_end(args);
	return p;
}


#endif // !(MIAO_SIMPLE_LEVEL >= 3)
