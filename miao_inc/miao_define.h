/*(Ver.=0.96)
 * MIAO_define.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_DEFINE_H_
#define MIAO_DEFINE_H_


#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef char lw_8;
typedef unsigned char lw_u8;
typedef int16_t lw_16;
typedef uint16_t lw_u16;


typedef int32_t lw_32;
typedef uint32_t uint32_t;
typedef int64_t lw_64;
typedef uint64_t lw_u64;

typedef float lw_sf;
typedef double lw_df;

typedef intptr_t lw_ptr;

//--------------------------------
//<重要宏定义>
//--------------------------------

//操作系统使能
#define MIAO_OS_ENABLE 1

//操作系统时间片
#define MIAO_CORE_TICK_MS 1

//操作系统精简等级
#define MIAO_SIMPLE_LEVEL 1

//操作系统高速运行
#define MIAO_SPEED_RUNNING 0

//操作系统步进运行
#define MIAO_STEP_RUNNING 0

//精简模式
#if MIAO_SIMPLE_LEVEL
	#define MIAO_SIMPLE_MODE 1
#else
	#define MIAO_SIMPLE_MODE 0
#endif // MIAO_SIMPLE_LEVEL

//CPU任务数量
#define MIAO_TASK_NUM_MAX 32

#define DEBUG
#if !defined(DEBUG)
#define MIAO_assert(is_error) ((void)0)
#else
#define MIAO_assert(is_error)   if((is_error)) {while(1);}
#endif

#if MIAO_SIMPLE_MODE
#define _MIAO_SAVE(A) (setjmp((A)))
#define _MIAO_LOAD(A,B,PTA) ((PTA)->TaskFun(PTA))
typedef lw_u16 _BP;
#else
#define _MIAO_SAVE(A) (setjmp((A)))
#define _MIAO_LOAD(A,B,PTA) (longjmp((A),(B)))
typedef jmp_buf _BP;
#endif

#if MIAO_STEP_RUNNING && MIAO_SIMPLE_MODE==0
 #error "STEP RUNNING MOST BE IN SIMPLE"
#endif


//--------------------------------
//</重要宏定义>
//--------------------------------


#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifndef PI
	#define PI 3.14159265358979f
#endif


//parameter define
typedef lw_32 fixpoint;

#define MIAO_GLOBAL_MATH_Q 1
#define MIAO_GLOBAL_MATH_FLOAT 2
#define MIAO_GLOBAL_MATH_DOUBLE 3
#define MIAO_GLOBAL_MATH_TYPE MIAO_GLOBAL_MATH_FLOAT

#define   FP_A_TO_FP_B(A,B,X)  ((A >= B) ? ((fixpoint) X >> (A - B)):((fixpoint) X << (B - A)))
#define MIAO_GLOBAL_MATH_Q_FORM 24
#define MIAO_PARA_NAME_LENGTH 5

#define MIAO_PARA_NORMAL_LENGTH 2

#define MIAO_PARA_LIST_LENGRH 2

#define MIAO_PAPA_CTRL_NAME_LENGTH 5

typedef enum{

	ParaUnitStatus_Free=0x00,
	ParaUnitStatus_Lock=0x01,
	ParaUnitStatus_Protect=0x02
	
	

}ParaUnitStatusNUM;

typedef enum{
	
	ParaOrder_IO_Prec=0,
	ParaOrder_IO_Normal=1,
	ParaOrder_IO_Data=2,
	
	ParaOrder_I_SelfUp,
	ParaOrder_I_SelfDown,
	ParaOrder_O_Draw,
	
	ParaOrder_I_UnlockSave,
	ParaOrder_I_UnlockNSave,
	ParaOrder_I_lock
}MiaoParaOrderNUM;

typedef enum{
	
	ParaLOrder_Once=0,
	ParaLOrder_More=1,
	
}MiaoParaListOrderNUM;


//<宏组说明>数据类型定义 定义参数所用的类型</宏组说明>
#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	 typedef fixpoint ParaType;
	 #include "IQmathLib.h"
    #define MIAO_GLOBAL_MATH_MAX 0X7FFFFFFFUL
    #define MIAO_GLOBAL_MATH_MIN 0X80000000UL
    #define MIAO_GLOBAL_MATH_DELT 1UL
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT
	 typedef float ParaType;
		//#include "arm_math.h"
		#include "math.h"
        #define MIAO_GLOBAL_MATH_MAX 3.2E38f
        #define MIAO_GLOBAL_MATH_MIN -3.2E38f
        #define MIAO_GLOBAL_MATH_DELT 1.2E-38f
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
	 typedef double ParaType;
		#include "math.h"
    #define MIAO_GLOBAL_MATH_MAX 1.7E308
    #define MIAO_GLOBAL_MATH_MIN -1.7E308
    #define MIAO_GLOBAL_MATH_DELT 2.2E-308
#else
	#error "MATH TYPE IS NOT DEFINED"
#endif
//<宏组说明>数据类型定义 定义参数所用的算法</宏组说明>
#if MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_Q
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMul(A,B) (_IQ24mpy(A,B))
	#define _ParaDiv(A,B) (_IQ24div(A,B))
	#define _ParaInt(A) (_IQ24int(A))
	#define _ParaFrac(A) (_IQ24frac(A))
	#define _ParaFint(A) (((ParaType)(A))<<MIAO_GLOBAL_MATH_Q_FORM)
	#define _ParaToF(A) (_IQ24toF(A))
	#define _ParaFrom(A) (_IQ24(A))
	#define _ParaSin(A) (_IQ24sin(A))
	#define _ParaCos(A) (_IQ24cos(A))
	#define _ParaTan(A) (_IQ24tan(A))
	#define _ParaLn(A) (_IQ24(logf((lw_sf)_IQ26toF(A))))
  #define _ParaLog(A) (_IQ26div(_ParaLn(A),_ParaLn(_IQ24(10))))
	#define _ParaSqrt(A) (_IQ24sqrt(A))
	#define _ParaAbs(A) (_IQ24abs(A))
    #define _ParaPow(A,B) (A==0?0:(_IQ24exp(_IQ24mpy((B),_ParaLn(A)))))
    #define _ParaCeil(A) (_ParaInt((A))+1)
    #define _ParaExp(A) (_IQ24exp(A))
	#define PARA_LEN 4
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_FLOAT
	#define _ParaAdd(A,B) ((lw_sf)(A)+(lw_sf)(B))
	#define _ParaSub(A,B) ((lw_sf)(A)-(lw_sf)(B))
	#define _ParaMul(A,B) ((lw_sf)(A)*(lw_sf)(B))
	#define _ParaDiv(A,B) ((lw_sf)(A)/(lw_sf)(B))
	#define _ParaInt(A) ((long)(A))
	#define _ParaFrac(A) ((A)-long(A))
	#define _ParaFint(A)	((lw_sf)A)
	#define _ParaToF(A) ((lw_sf)(A))
	#define _ParaFrom(A) ((lw_sf)(A))
	#define _ParaSin(A) (sinf(A))
	#define _ParaCos(A) (cosf(A))
	#define _ParaTan(A) (sinf(A)/cosf(A))		
	#define _ParaSqrt(A) (sqrtf(A))
	#define _ParaExp(A) (_MiaoToolMathExp1024(A))
	#define _ParaAbs(A) (A > 0.0f?A:-A)		
	#define _ParaPow(A,B) (powf((A),(B)))
	#define _ParaCeil(A) (ceilf((A)))
	#define _ParaLn(A) (logf((A)))
	#define _ParaLog(A) (logf10((A)))
	#define PARA_LEN 4
	
#elif MIAO_GLOBAL_MATH_TYPE==MIAO_GLOBAL_MATH_DOUBLE
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMul(A,B) ((A)*(B))
	#define _ParaDiv(A,B) ((A)/(B))
	#define _ParaInt(A) ((long)(A))
	#define _ParaFrac(A) ((A)-long(A))
	#define _ParaFint(A)	((lw_df)(A))
	#define _ParaToF(A) ((lw_df)(A))	
	#define _ParaFrom(A)	((lw_df)(A))	
	#define _ParaSin(A) (sin(A))
	#define _ParaCos(A) (cos(A))
	#define _ParaTan(A) (tan(A))	
	#define _ParaSqrt(A) (sqrt(A))
	#define _ParaExp(A) (exp(A))
	#define _ParaAbs(A) (A > 0.0f?A:-A)
	#define _ParaPow(A,B) (pow((A),(B)))
	#define _ParaCeil(A) (ceil((A)))
	#define _ParaLn(A) (log((A)))
	#define _ParaLog(A) (log10((A)))
	#define PARA_LEN 8
#else
	#error "MATH TYPE IS NOT DEFINED"
#endif

#if PARA_LEN == 8
	#define PARA_MEM_CAL(D) ((uint32_t)(D)*sizeof(ParaType))
#elif PARA_LEN == 4
	#define PARA_MEM_CAL(D) ((uint32_t)(D)*sizeof(ParaType))	
#endif // PARA_LEN == 8


//task define
#define TASK_PRIVATE_MEMORY_SECTION_LENGTH 2

#define MIAO_TASK_BREAKPOINT_DEEP 1

	
#if MIAO_SIMPLE_MODE
#undef MIAO_TASK_BREAKPOINT_DEEP
#define MIAO_TASK_BREAKPOINT_DEEP 1
#endif	
	
#define MIAO_TASK_NAME_LENGTH_MAX 4

#if MIAO_TASK_NAME_LENGTH_MAX<3
#error "LENGTH OF TASK NAME IS TOO SHORT"
#endif

typedef struct MIAO_EXTERN_MEMORY_STRUCT{
	void *pData;
	uint32_t uData;
}MiaoExternMemory_t;


//core define
#define MIAO_CORE_PARA_PAGE_NUM 2

#if MIAO_CORE_PARA_PAGE_NUM <2
	#error "parameter num is to little"
#endif

#if MIAO_SIMPLE_LEVEL >= 3
#define MIAO_CPU_USAGE_MAX ((lw_u8)(200))
#else
#define MIAO_CPU_USAGE_MAX ((lw_u16)(1000))
#endif // MIAO_SIMPLE_LEVEL >= 3


//gift define

#define MIAO_GIFT_EN 1

#define MIAO_GIFT_UNIT_STR_LENGTH 8

#define MIAO_GIFT_UNIT_MAX 3

#define MIAO_GIFT_POSTMAN_MAX 2

#if MIAO_GIFT_POSTMAN_MAX > MIAO_TASK_NUM_MAX
	#error "postman is too much"
#endif

//communication
#define MIAO_TASK_COMMUN_ENABLE 0

#define MIAO_COMMUN_TX_LENGTH 20

#define MIAO_COMMUN_RX_LENGTH 8

#define	MIAO_GIFT_COMMUN_RXD_LENGTH 8

#define MIAO_COMMUN_ADDRESS 1

#define MIAO_COMMUN_READY_LEN_LIST_LENGTH 3


//show deifne
#define MIAO_TASK_SHOW_ENABLE 0

#define MIAO_SHOW_NO_KEEP 1
#define MIAO_SHOW_HAVE_KEEP 2

#define MIAO_SHOW_SCREEN MIAO_SHOW_NO_KEEP

#define MIAO_SHOW_WIN_LENGTH 3

#define MIAO_SHOW_MOVE_CURSOR_FREE 1
#define MIAO_SHOW_MOVE_WIN_FREE 2

#define MIAO_SHOW_MOVE_METHOD MIAO_SHOW_MOVE_WIN_FREE

#define MIAO_SHOW_MOVE_CURSOR_POS (MIAO_SHOW_WIN_LENGTH-1)/2

#if MIAO_SHOW_MOVE_CURSOR_POS >= MIAO_SHOW_WIN_LENGTH
	#error "Curosr position is over"
#endif

#define MIAO_SHOW_BUFFER_LEN 10

#define MIAO_SHOW_PAGE_LIST_LEN 3

#define MIAO_SHOW_CMD_UP 18
#define MIAO_SHOW_CMD_DOWN 17
#define MIAO_SHOW_CMD_TURN 12
#define MIAO_SHOW_CMD_LOCK 15
#define MIAO_SHOW_CMD_CAN 24
#define MIAO_SHOW_CMD_NEXT '\n'
//timer define
#define MIAO_TIMER_CTRL_LIST_LEN 5


#if MIAO_SIMPLE_LEVEL >= 3 && MIAO_TIMER_CTRL_LIST_LEN > 3

#undef MIAO_TIMER_CTRL_LIST_LEN
#define MIAO_TIMER_CTRL_LIST_LEN 3

#endif

//memory define
#define MIAO_MEMORY_EN 1

//IMPORT MAP MALLOC
#define MIAO_MEMORY_IMPORT_MAP 1
//IMPORT SIMPLE MALLOC
#define MIAO_MEMORY_IMPORT_SIMPLE 1
//IMPORT LIST MALLOC
#define MIAO_MEMORY_IMPORT_LIST 1

#define MIAO_MEM_ALLOC_FAST 0

typedef struct MIAO_MEM_SIZE_STRUCT {
#if MIAO_MEM_ALLOC_FAST
	uint32_t ModNum;
	lw_u16 MovNum;
#endif
	lw_u16 SectionSize;

}MiaoMemSize_t;


//tool define
typedef struct MIAO_TOOL_DLIST_NODE_CONNECT_STRUCT {
	struct MIAO_TOOL_DLIST_NODE_CONNECT_STRUCT *pLast, *pNext;
}MiaoToolDListNodeConnect_t;


//String Debug Define
//是否使能StringDebug
#define MIAO_STR_DBG_EN 1

#define MIAO_STR_DBG_SIMPLE_MODE 0

//StringDebug 输出字符串大小
#define MIAO_STR_DBG_OUT_LEN 100
//StringDebug 名字字符串大小
#define MIAO_STR_DBG_NAME_LEN 10
//StringDebug 数值字符串大小
#define MIAO_STR_DBG_DATA_LEN 200

//StringDebug 内存大小
#define MIAO_STR_DBG_MEM_SIZE 16
#define MIAO_STR_DBG_MEM_LEN 100

//StringDebug 哈希表大小
#define MIAO_STR_DBG_HASH_LEN 8

//2号精简模式下DATA的数量
#define MIAO_STR_DBG_S2_DATA_LEN 20


#if MIAO_TASK_NUM_MAX<=8
	#define __MIAO_LOG2_TASK_NUM 3
	typedef lw_u8 MiaoGroupAllType;
#elif MIAO_TASK_NUM_MAX>8 && MIAO_TASK_NUM_MAX<=16
	#define __MIAO_LOG2_TASK_NUM 4
	typedef lw_u16 MiaoGroupAllType;
#elif MIAO_TASK_NUM_MAX>16 && MIAO_TASK_NUM_MAX<=32
	#define __MIAO_LOG2_TASK_NUM 5
	typedef uint32_t MiaoGroupAllType;
#elif MIAO_TASK_NUM_MAX>32 && MIAO_TASK_NUM_MAX<=64
	#define __MIAO_LOG2_TASK_NUM 6
	typedef lw_u64 MiaoGroupAllType;
#else
	#error "TASK NUM IS OVER"
#endif


#if MIAO_SIMPLE_LEVEL == 2 && MIAO_TASK_NUM_MAX>32
	#error "TASK NUM IS OVER PLEASE DEFINE MIAO_SIMPLE_LEVEL OR MIAO_TASK_NUM_MAX AGAIN"
#endif // MIAO_SIMPLE_LEVEL == 2 && MIAO_TASK_NUM_MAX>32

#if MIAO_SIMPLE_LEVEL ==3 && MIAO_TASK_NUM_MAX>16
	#error "TASK NUM IS OVER PLEASE DEFINE MIAO_SIMPLE_LEVEL OR MIAO_TASK_NUM_MAX AGAIN"
#endif // MIAO_SIMPLE_LEVEL ==3 && MIAO_TASK_NUM_MAX>16


#if MIAO_SIMPLE_LEVEL ==4 && MIAO_TASK_NUM_MAX>8
	#error "TASK NUM IS OVER PLEASE DEFINE MIAO_SIMPLE_LEVEL OR MIAO_TASK_NUM_MAX AGAIN"
#endif // MIAO_SIMPLE_LEVEL ==4 && MIAO_TASK_NUM_MAX>8


#define _MIAO_2_POW(NUM) ((MiaoGroupAllType)1<<NUM)
#define _MIAO_IS_2_POW(n) ((n)==((n)&~(n)+1))
//------------------------------------------
//<共用体名>MiaoCoreGroupUN</共用体名>
//<共用体功能>内核标志组共用体 根据MIAO_TASK_NUM_MAX确定长度</共用体功能>
//------------------------------------------
typedef union MIAO_CORE_GROUP_UNION{
	MiaoGroupAllType all;
#if MIAO_TASK_NUM_MAX<=8
	struct{
		lw_u8 bit0:1;
		lw_u8 bit1:1;
		lw_u8 bit2:1;
		lw_u8 bit3:1;
		lw_u8 bit4:1;
		lw_u8 bit5:1;
		lw_u8 bit6:1;
		lw_u8 bit7:1;
	}bits;
#elif MIAO_TASK_NUM_MAX>8 && MIAO_TASK_NUM_MAX<=16
	struct{
		lw_u16 bit0:1;
		lw_u16 bit1:1;
		lw_u16 bit2:1;
		lw_u16 bit3:1;
		lw_u16 bit4:1;
		lw_u16 bit5:1;
		lw_u16 bit6:1;
		lw_u16 bit7:1;
		lw_u16 bit8:1;
		lw_u16 bit9:1;
		lw_u16 bit10:1;
		lw_u16 bit11:1;
		lw_u16 bit12:1;
		lw_u16 bit13:1;
		lw_u16 bit14:1;
		lw_u16 bit15:1;
	}bits;
#elif MIAO_TASK_NUM_MAX>16 && MIAO_TASK_NUM_MAX<=32
	struct{
		uint32_t bit0 : 1;
		uint32_t bit1 : 1;
		uint32_t bit2 : 1;
		uint32_t bit3 : 1;
		uint32_t bit4 : 1;
		uint32_t bit5 : 1;
		uint32_t bit6 : 1;
		uint32_t bit7 : 1;
		uint32_t bit8 : 1;
		uint32_t bit9 : 1;
		uint32_t bit10 : 1;
		uint32_t bit11 : 1;
		uint32_t bit12 : 1;
		uint32_t bit13 : 1;
		uint32_t bit14 : 1;
		uint32_t bit15 : 1;
		uint32_t bit16 : 1;
		uint32_t bit17 : 1;
		uint32_t bit18 : 1;
		uint32_t bit19 : 1;
		uint32_t bit20 : 1;
		uint32_t bit21 : 1;
		uint32_t bit22 : 1;
		uint32_t bit23 : 1;
		uint32_t bit24 : 1;
		uint32_t bit25 : 1;
		uint32_t bit26 : 1;
		uint32_t bit27 : 1;
		uint32_t bit28 : 1;
		uint32_t bit29 : 1;
		uint32_t bit30 : 1;
		uint32_t bit31 : 1;
	}bits;
#elif MIAO_TASK_NUM_MAX>32 && MIAO_TASK_NUM_MAX<=64
	struct{
		lw_u64 bit0 : 1;
		lw_u64 bit1 : 1;
		lw_u64 bit2 : 1;
		lw_u64 bit3 : 1;
		lw_u64 bit4 : 1;
		lw_u64 bit5 : 1;
		lw_u64 bit6 : 1;
		lw_u64 bit7 : 1;
		lw_u64 bit8 : 1;
		lw_u64 bit9 : 1;
		lw_u64 bit10 : 1;
		lw_u64 bit11 : 1;
		lw_u64 bit12 : 1;
		lw_u64 bit13 : 1;
		lw_u64 bit14 : 1;
		lw_u64 bit15 : 1;
		lw_u64 bit16 : 1;
		lw_u64 bit17 : 1;
		lw_u64 bit18 : 1;
		lw_u64 bit19 : 1;
		lw_u64 bit20 : 1;
		lw_u64 bit21 : 1;
		lw_u64 bit22 : 1;
		lw_u64 bit23 : 1;
		lw_u64 bit24 : 1;
		lw_u64 bit25 : 1;
		lw_u64 bit26 : 1;
		lw_u64 bit27 : 1;
		lw_u64 bit28 : 1;
		lw_u64 bit29 : 1;
		lw_u64 bit30 : 1;
		lw_u64 bit31 : 1;
		lw_u64 bit32 : 1;
		lw_u64 bit33 : 1;
		lw_u64 bit34 : 1;
		lw_u64 bit35 : 1;
		lw_u64 bit36 : 1;
		lw_u64 bit37 : 1;
		lw_u64 bit38 : 1;
		lw_u64 bit39 : 1;
		lw_u64 bit40 : 1;
		lw_u64 bit41 : 1;
		lw_u64 bit42 : 1;
		lw_u64 bit43 : 1;
		lw_u64 bit44 : 1;
		lw_u64 bit45 : 1;
		lw_u64 bit46 : 1;
		lw_u64 bit47 : 1;
		lw_u64 bit48 : 1;
		lw_u64 bit49 : 1;
		lw_u64 bit50 : 1;
		lw_u64 bit51 : 1;
		lw_u64 bit52 : 1;
		lw_u64 bit53 : 1;
		lw_u64 bit54 : 1;
		lw_u64 bit55 : 1;
		lw_u64 bit56 : 1;
		lw_u64 bit57 : 1;
		lw_u64 bit58 : 1;
		lw_u64 bit59 : 1;
		lw_u64 bit60 : 1;
		lw_u64 bit61 : 1;
		lw_u64 bit62 : 1;
		lw_u64 bit63 : 1;
	}bits;
#else
	#error "TASK NUM IS OVER"
#endif
}MiaoCoreGroupUN;


#endif /*(Ver.=0.96) MIAO_DEFINE_H_ */


