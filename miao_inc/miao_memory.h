/*(Ver.=0.96)
 * MIAO_memory.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_MEMORY_H_
#define MIAO_MEMORY_H_

#include "OSLW_define.h"
#include <stdlib.h>

#if MIAO_MEM_ALLOC_FAST
	#define MIAO_MEM_SIZE_MUL(LEN,MS) (((uint32_t)LEN) << (MS).MovNum )
	#define MIAO_MEM_SIZE_DIV(LEN,MS) (((uint32_t)LEN) >> (MS).MovNum)
	#define MIAO_MEM_SIZE_MOD(LEN,MS) (((uint32_t)LEN) &  (MS).ModNum)
#else
	#define MIAO_MEM_SIZE_MUL(LEN,MS) (((MS).SectionSize) * (LEN))
	#define MIAO_MEM_SIZE_DIV(LEN,MS) (((LEN) / ((MS).SectionSize)))
	#define MIAO_MEM_SIZE_MOD(LEN,MS) ((LEN) % ((MS).SectionSize))
#endif // MIAO_MEM_ALLOC_FAST


typedef struct MIAO_MEMORY_BASIC_STRUCT {

	lw_32(*init)
		(
			struct MIAO_MEMORY_BASIC_STRUCT *pMU,//存储器结构体
			void *pMem,//内存池地址
			lw_u16 lenMem,//内存片数量
			lw_u16 MemSize,//内存片大小(sizeof)
			uint32_t sizeofmem//sizeof(内存区域)的大小用来，验证内存大小是否对，一般sizeofmem>=计算得到的内存大小
			);

	void * (*Malloc)(struct MIAO_MEMORY_BASIC_STRUCT *pMU, uint32_t dsize);
	void * (*ReAlloc)(struct MIAO_MEMORY_BASIC_STRUCT *pMU, void *p, uint32_t dsize);
	void * (*Free)(struct MIAO_MEMORY_BASIC_STRUCT *pMU, void *p);
	void * (*Calloc)(struct MIAO_MEMORY_BASIC_STRUCT *pMU, uint32_t dsize);
	MiaoMemSize_t MemSectionSize;

	//相对概念 对于不同的分配方式概念不同
	uint32_t memMax;//内存总大小
	uint32_t memSurplus;//剩余内存数量

}MiaoMemoryBasic_t;

typedef MiaoMemoryBasic_t *lw_mem;



#if MIAO_MEMORY_IMPORT_MAP

typedef struct MIAO_MEMORY_MAP_STRUCT{

	MiaoMemoryBasic_t basic;

	MiaoExternMemory_t Mem;
	
	MiaoExternMemory_t MemInfo;
	
	void *pMemButtom,*pMemInfoButtom;//底部
		
}MiaoMemoryMap_t;

void * MiaoMemoryMapAlloc(MiaoMemoryMap_t *pMU,uint32_t dsize);
void * MiaoMemoryMapFree(MiaoMemoryMap_t *pMU,void *p);
void* MiaoMemoryMapReAlloc(MiaoMemoryMap_t *pMU, void *p, uint32_t dsize);
lw_32 MiaoMemoryMapInital(MiaoMemoryMap_t *pMU,
	void *pMem,
	lw_u16 lenMem,	
	lw_u16 MemSize,
	uint32_t sizeofmem
	);
#endif //MIAO_MEMORY_IMPORT_MAP


	
#if MIAO_MEMORY_IMPORT_SIMPLE	
	
typedef struct MIAO_MEMORY_SIMPLE_STRUCT {

	MiaoMemoryBasic_t basic;

	MiaoExternMemory_t Mem;

	size_t pMemNow,pMemButtom;//底部

}MiaoMemorySimple_t;


void * MiaoMemorySimpleAlloc(MiaoMemorySimple_t *pMU, uint32_t dsize);
void * MiaoMemorySimpleFree(MiaoMemorySimple_t *pMU, void *p);
void* MiaoMemorySimpleReAlloc(MiaoMemorySimple_t *pMU, void *p, uint32_t dsize);
lw_32 MiaoMemorySimpleInital(MiaoMemorySimple_t *pMU,
	void *pMem,
	lw_u16 lenMem,
	lw_u16 MemSize,
	uint32_t sizeofmem
);

#endif //MIAO_MEMORY_IMPORT_SIMPLE	

	
#if MIAO_MEMORY_IMPORT_LIST	
	
typedef struct MIAO_MEMORY_LIST_NODE_STRUCT {

	struct MIAO_TOOL_DLIST_NODE_CONNECT_STRUCT con;
	uint32_t len;

}MiaoMemoryListNode_t;


typedef struct MIAO_MEMORY_LIST_STRUCT {

	MiaoMemoryBasic_t basic;

	MiaoExternMemory_t Mem;

	void *pMemButtom;

	MiaoMemoryListNode_t *phead, *ptail;

}MiaoMemoryList_t;


void * MiaoMemoryListAlloc(MiaoMemoryList_t *pMU, uint32_t dsize);
void * MiaoMemoryListFree(MiaoMemoryList_t *pMU, void *p);
void* MiaoMemoryListReAlloc(MiaoMemoryList_t *pMU, void *p, uint32_t dsize);
lw_32 MiaoMemoryListInital(MiaoMemoryList_t *pMU,
	void *pMem,
	lw_u16 lenMem,
	lw_u16 MemSize,
	uint32_t sizeofmem
);
#endif //MIAO_MEMORY_IMPORT_LIST
	
	
#define MIAO_MEM_LIST_STD_SIZE(PMU,DSIZE) do{\
	lw_u16 _mod_res;\
	(DSIZE) += sizeof(MiaoMemoryListNode_t);\
	_mod_res=MIAO_MEM_SIZE_MOD((DSIZE) , (PMU)->basic.MemSectionSize);\
	(DSIZE) += (_mod_res?((PMU)->basic.MemSectionSize.SectionSize-_mod_res):0);\
}while(0)


#define MIAO_MEM_MAP_LEN(LEN,NUM) (((uint32_t)(LEN)*(NUM))+(sizeof(long **)*(NUM)))
#define MIAO_MEM_SIMPLE_LEN(LEN,NUM) ((uint32_t)(LEN)*(NUM))
#define MIAO_MEM_LIST_LEN(LEN,NUM) (((uint32_t)(LEN)*(NUM))+sizeof(MiaoMemoryListNode_t))



#define MIAO_MEM_MAP_DEFAULT {MiaoMemoryMapInital}
#define MIAO_MEM_SIMPLE_DEFAULT {MiaoMemorySimpleInital}
#define MIAO_MEM_LIST_DEFAULT {MiaoMemoryListInital}


#if MIAO_MEM_ALLOC_FAST

#define MIAO_MEM_MAP_DEF(NAME,LEN,NUM) MiaoMemoryMap_t NAME##_struct=MIAO_MEM_MAP_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
int _##NAME##_heap[MIAO_MEM_MAP_LEN(LEN,NUM)/sizeof(int)+1]; \
 MiaoMemoryBasic_t *p##NAME=(MiaoMemoryBasic_t *)(&(NAME##_struct));


#define MIAO_MEM_SIMPLE_DEF(NAME,LEN,NUM) MiaoMemorySimple_t NAME##_struct=MIAO_MEM_SIMPLE_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
int _##NAME##_heap[MIAO_MEM_SIMPLE_LEN(LEN,NUM)/sizeof(int)+1]; \
 MiaoMemoryBasic_t *p##NAME=(MiaoMemoryBasic_t *)(&(NAME##_struct));


#define MIAO_MEM_LIST_DEF(NAME,LEN,NUM) MiaoMemoryList_t NAME##_struct=MIAO_MEM_LIST_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
int _##NAME##_heap[MIAO_MEM_LIST_LEN(LEN,NUM)/sizeof(int)+1]; \
 MiaoMemoryBasic_t *p##NAME=(MiaoMemoryBasic_t *)(&(NAME##_struct));

#else
#define MIAO_MEM_MAP_DEF(NAME,LEN,NUM) MiaoMemoryMap_t NAME##_struct=MIAO_MEM_MAP_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
lw_u8 _##NAME##_heap[MIAO_MEM_MAP_LEN(LEN,NUM)]; \
 MiaoMemoryBasic_t *p##NAME=(MiaoMemoryBasic_t *)(&(NAME##_struct));


#define MIAO_MEM_SIMPLE_DEF(NAME,LEN,NUM) MiaoMemorySimple_t NAME##_struct=MIAO_MEM_SIMPLE_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
lw_u8 _##NAME##_heap[MIAO_MEM_SIMPLE_LEN(LEN,NUM)]; \
 MiaoMemoryBasic_t *p##NAME=(MiaoMemoryBasic_t *)(&(NAME##_struct));


#define MIAO_MEM_LIST_DEF(NAME,LEN,NUM) MiaoMemoryList_t NAME##_struct=MIAO_MEM_LIST_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
lw_u8 _##NAME##_heap[MIAO_MEM_LIST_LEN(LEN,NUM)]; \
 MiaoMemoryBasic_t *p##NAME=(MiaoMemoryBasic_t *)(&(NAME##_struct));

#endif //  MIAO_MEM_ALLOC_FAST






#define MIAO_MEM_INIT(NAME) do{p##NAME->init((void *)(p##NAME), (void *)(_##NAME##_heap), _##NAME##_page_num, _##NAME##_page_len, sizeof(lw_u8)*sizeof(_##NAME##_heap));}while(0)



#endif /*(Ver.=0.96) MIAO_MEMORY_H_ */
