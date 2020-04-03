#ifndef MIAO_TOOL_SIGNAL_H_
#define MIAO_TOOL_SIGNAL_H_
#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_basic_math.h"
#include "OSLW_tool_random.h"
#include "OSLW_memory.h"

#if !(MIAO_SIMPLE_LEVEL >= 3)

typedef struct MIAO_TOOL_SYSTEM_BASIC_STRUCT {
	MiaoMat In;
	MiaoMat State;
	MiaoMat Out;

}MiaoToolSysBasic_t;





#endif // !(MIAO_SIMPLE_LEVEL >= 3)


#endif // !MIAO_TOOL_SIGNAL_H_
