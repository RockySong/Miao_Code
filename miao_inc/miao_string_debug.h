#ifndef _MIAO_STRING_DEBUG_HEADER_
#define _MIAO_STRING_DEBUG_HEADER_
#include "OSLW_memory.h"
#include "OSLW_tool.h"

#if MIAO_SIMPLE_LEVEL<4 && MIAO_STR_DBG_EN

typedef void(*FunType_0)();
typedef void(*FunType_1)(void *a1);
typedef void(*FunType_2)(void *a1, void *a2);
typedef void(*FunType_3)(void *a1, void *a2, void *a3);
typedef void(*FunType_4)(void *a1, void *a2, void *a3, void *a4);
typedef void(*FunType_5)(void *a1, void *a2, void *a3, void *a4, void *a5);
typedef void(*FunType_6)(void *a1, void *a2, void *a3, void *a4, void *a5, void *a6);
typedef void(*FunType_7)(void *a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7);
typedef void(*FunType_8)(void *a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7, void *a8);
typedef void(*FunType_9)(void *a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7, void *a8, void *a9);



typedef struct MIAO_STRING_DEBUG_DATA_STRUCT{
	void *pdata;
	lw_u8 *name;
	lw_u8 kind;
}MiaoStrDbgData_t;



typedef struct MIAO_STRING_DEBUG_STRUCT{

#if MIAO_STR_DBG_SIMPLE_MODE == 0	
	MiaoToolHash_t DebugHash;
	MiaoMemoryMap_t DebugMem;
	lw_u8 DebugHeap[MIAO_MEM_MAP_LEN(MIAO_STR_DBG_MEM_SIZE, MIAO_STR_DBG_MEM_LEN)];
#elif MIAO_STR_DBG_SIMPLE_MODE==1
	MiaoToolHash_t DebugHash;
	lw_u8 DebugHeap[16];
#elif MIAO_STR_DBG_SIMPLE_MODE==2
	MiaoStrDbgData_t Data[MIAO_STR_DBG_S2_DATA_LEN];
	lw_u8 count;	
#endif	
	
	union 
	{
		lw_u8 OutStr[MIAO_STR_DBG_OUT_LEN];
		struct 
		{
			lw_u8 NameStr[MIAO_STR_DBG_NAME_LEN];
			lw_u8 DataStr[MIAO_STR_DBG_DATA_LEN];

		} UsingStr;
	}StrDbgAllStr;
	
	
}MiaoStrDbg_t;

void MiaoStrDbgInitial(struct MIAO_STRING_DEBUG_STRUCT *pSD);
lw_u8 * MiaoStrDbgFun(struct MIAO_CORE_STRUCT *pOS, lw_u8 *instr);

#if MIAO_STR_DBG_SIMPLE_MODE == 0	

#define STRING_DEBUG_VER "v1.0"

lw_u8 *_StringDebugSelectInfo(lw_8 c);
lw_u8 * _StringDebugFindChar(lw_8 *name, lw_8 c);
lw_u8 _StringDebugFindCharNum(lw_8 *name, lw_8 c);
uint32_t _StringDebugMemCalSq(lw_u8 *data, lw_u8 *info, lw_u8 *pNum);
uint32_t _StringDebugMemLengthPl(lw_u8 *data, lw_u8 *info, lw_sf *pstart, lw_sf *pdelt, lw_u8 *pNum);
void * _StringDebugFind(MiaoToolHash_t *ph, lw_8 *name, lw_8 *pkind, lw_u8 *pindex, MiaoToolDListNode_t **ppdln, lw_u8 *pdata_sel_kind);
void *_StringDebugString2Number(MiaoMemoryBasic_t *pmem, lw_u8 *str, lw_u8 *info, lw_u8 *outstr, uint32_t *pdsize, lw_u8 *pdlen, lw_u8 *pstatus);
lw_u8 _StringDebugSliceDEF(lw_u8 *instr, lw_u8 *name, lw_u8 *pkind, lw_u8 *data);
lw_u8 _StringDebugSliceUPDATE(lw_u8 *instr, lw_u8 *name, lw_u8 *data);
lw_u8 _StringDebugSliceSELECT(lw_u8 *instr, lw_u8 *name, lw_u8 *pkind);
lw_u8 _StringDebugSliceFUN(lw_u8 *instr, lw_u8 *name, lw_u8 *data);
void *_SD_MemCpy(void *dst, void *src, uint32_t len);
lw_u8 *_SD_Data2Str(lw_u8 *str, void *data, lw_u8 *form, lw_u8 *buf, lw_u8 *info);
lw_u8 * _SD_HashNode2Str(lw_u8 *str, MiaoToolDListNode_t *pdln);
lw_8 * StringDebug(MiaoToolHash_t *ph, MiaoMemoryBasic_t *pmem, lw_8 *instr, lw_8 *outstr, lw_8 *data1, lw_8 *data2);


#elif MIAO_STR_DBG_SIMPLE_MODE==1

#define STRING_DEBUG_VER "v1.0S"

lw_u8 *_StringDebugSelectInfo(lw_8 c);
lw_u8 * _StringDebugFindChar(lw_8 *name, lw_8 c);
lw_u8 _StringDebugFindCharNum(lw_8 *name, lw_8 c);
lw_u8 _StringDebugSliceUPDATE(lw_u8 *instr, lw_u8 *name, lw_u8 *data);
lw_u8 _StringDebugSliceSELECT(lw_u8 *instr, lw_u8 *name, lw_u8 *pkind);
lw_u8 _StringDebugSliceFUN(lw_u8 *instr, lw_u8 *name, lw_u8 *data);
void *_SD_MemCpy(void *dst, void *src, uint32_t len) ;
lw_u8 *_SD_Data2Str(lw_u8 *str, void *data, lw_u8 *form,lw_u8 *buf,lw_u8 *info);
lw_u8 * _SD_HashNode2Str(lw_u8 *str, MiaoToolDListNode_t *pdln);
void MiaoStrDbgRun(struct MIAO_STRING_DEBUG_STRUCT *pSD,lw_u8 *instr);
void *_StringDebugString2NumberSimple(void *pmem, lw_u8 *str, lw_u8 *info,lw_u8 *outstr);
lw_8 * StringDebug(MiaoToolHash_t *ph, void *pm, lw_8 *instr, lw_8 *outstr, lw_8 *data1, lw_8 *data2);


#elif MIAO_STR_DBG_SIMPLE_MODE==2

#define STRING_DEBUG_VER "v1.0SS"

lw_8 * StringDebug(MiaoStrDbg_t *pSD, lw_8 *instr, lw_8 *outstr, lw_8 *data1, lw_8 *data2);
void * _StringDebugFind_S2(MiaoStrDbg_t *pSD,lw_8 *name, lw_8 *pkind);
lw_u8 *_StringDebugSelectInfo(lw_8 c);

#endif

#if MIAO_STR_DBG_SIMPLE_MODE==0 || MIAO_STR_DBG_SIMPLE_MODE==1

#define STR_DBG_DEF(DATA,NAME) const char DATA##_name[]=NAME;MiaoToolDListNode_t DATA##_node;
#define STR_DBG_INC(_t,DATA,TYPE_S,LEN) do{_t.DebugHash.AppendFun(&(_t.DebugHash), (void *)(DATA##_name), &(DATA##_node));DATA##_node.Data.uData = TYPE_S+((lw_u16)LEN<<8);DATA##_node.Data.pData = &DATA;}while(0)

#elif MIAO_STR_DBG_SIMPLE_MODE==2

#define STR_DBG_DEF(DATA,NAME) const char DATA##_name[]=NAME;
#define STR_DBG_INC(_t,DATA,TYPE_S,LEN) do{	_t.Data[_t.count].kind=TYPE_S;_t.Data[_t.count].pdata=&DATA;_t.Data[_t.count].name=(void *)DATA##_name;_t.count++;}while(0)

#endif


#define SD_Str2I LwStr2I
#define SD_Str2F LwStr2F
#define SD_Sprintf LwSprintf




#endif

#endif

