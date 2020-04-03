#ifndef MIAO_TOOL_STRING_H_
#define MIAO_TOOL_STRING_H_
#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_memory.h"

#if !(MIAO_SIMPLE_LEVEL >= 4)


lw_sf MiaoToolStringTolw_sf(const lw_u8 *str);
lw_32 MiaoToolStringToInt(const lw_u8 *str);

lw_u8* MiaoToolString_itoa(lw_32 value, lw_u8 *str, lw_32 radix);
lw_u8* MiaoToolString_utoa(uint32_t value, lw_u8 *str, lw_32 radix);
lw_u8* MiaoToolString_gcvt(lw_sf value, lw_32 ndigit, lw_u8 *buf);
void* MiaoToolStringVSprintf(lw_8 *buf, const lw_8 *fmt, va_list args);
void* MiaoToolStringSprintf(lw_8 *buf, const lw_8 *fmt, ...);

#define LwStr2F(S) MiaoToolStringTolw_sf(S)
#define LwStr2I(S) MiaoToolStringToInt(S)
#define LwI2Str(S,D) MiaoToolString_itoa(S,D,10)
#define LwF2Str(S,D) MiaoToolString_gcvt(D,4,S)
#define LwSprintf MiaoToolStringSprintf

#define _LW_STR_SKIP_FIND(SS,S,C) 	do{(SS) = (S);while ( *(S) && *(S)++ != (C));(S)[-1] = 0;}while(0)
#define _LW_STR_SKIP_COPY(SS,S,C) 	do{lw_u8 *_buf=(SS);while ( *(S) && *(S) != (C)){*(_buf)++=*(S)++;}(_buf)[0] = 0;}while(0)

#endif // !(MIAO_SIMPLE_LEVEL >= 3)


#endif // !MIAO_TOOL_STRING_H_
