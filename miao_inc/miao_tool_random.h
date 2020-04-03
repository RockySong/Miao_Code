/*(Ver.=0.96)
 * MIAO_tool.h
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#ifndef MIAO_TOOL_RANDOM_H_
#define MIAO_TOOL_RANDOM_H_


#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#if !(MIAO_SIMPLE_LEVEL >= 3)

typedef struct MIAO_TOOL_RANDOM_BASIC_STRUCT{

	ParaType Rmin, Rmax;

	ParaType(*rand)(void *pRand, ParaType Rmin, ParaType Rmax);
	lw_32(*randint)(void *pRand, lw_32 Rmin, lw_32 Rmax);
	ParaType(*randn)(void *pRand, ParaType Cent, ParaType Var);
	void(*Seed)(void *pRand, uint32_t seed);
#if MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM
	ParaType _normal_last;
	lw_u8 _normal_gen_flag;

#elif MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_LLN

#elif MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_MP
	ParaType _normal_last;
	lw_u8 _normal_gen_flag;
#else
	
#endif // MIAO_TOOL_RAND_NORM_METHOD == MIAO_TOOL_RAND_NORM_BM

}MiaoToolRandomBasic_t;
ParaType MiaoToolRandomNormal(MiaoToolRandomBasic_t *pRB, ParaType Cent, ParaType Var);


typedef struct MIAO_TOOL_RANDOM_CHAOS_LOGISTIC_STRUCT{
	MiaoToolRandomBasic_t basic;	
	
	lw_df u,x;	
}MiaoToolRandomChaosLogistic_t;

extern MiaoToolRandomChaosLogistic_t MiaoTRCLog;
extern MiaoToolRandomBasic_t *pRCLog;
ParaType MiaoToolRandomChaosLogisticFun(void *pRand,ParaType Rmin,ParaType Rmax);
lw_32 MiaoToolRandomChaosLogisticFunInt(void *pRand,lw_32 Rmin,lw_32 Rmax);


typedef struct MIAO_TOOL_RANDOM_CHAOS_TENT_STRUCT{
	MiaoToolRandomBasic_t basic;	
	
	lw_df a,x;
}MiaoToolRandomChaosTent_t;

extern MiaoToolRandomChaosTent_t MiaoTRCTent;
extern MiaoToolRandomBasic_t *pRCTent;
ParaType MiaoToolRandomChaosTentFun(void *pRand,ParaType Rmin,ParaType Rmax);
lw_32 MiaoToolRandomChaosTentFunInt(void *pRand,lw_32 Rmin,lw_32 Rmax);


typedef struct MIAO_TOOL_RANDOM_CHAOS_KENT_STRUCT{
	MiaoToolRandomBasic_t basic;	
	
	lw_df a,x;
}MiaoToolRandomChaosKent_t;

extern MiaoToolRandomChaosKent_t MiaoTRCKent;
extern MiaoToolRandomBasic_t *pRCKent;
ParaType MiaoToolRandomChaosKentFun(void *pRand,ParaType Rmin,ParaType Rmax);
lw_32 MiaoToolRandomChaosKentFunInt(void *pRand,lw_32 Rmin,lw_32 Rmax);




typedef struct MIAO_TOOL_RANDOM_LCG_STRUCT {
	MiaoToolRandomBasic_t basic;
	uint32_t rand_x;
	uint32_t rand_a;
	uint32_t rand_c;
	uint32_t rand_mod;
}MiaoToolRandomLCG_t;

extern MiaoToolRandomLCG_t MiaoTRLcg;
extern MiaoToolRandomBasic_t *pRLcg;

ParaType MiaoToolRandomLCGFun(void *pRand, ParaType Rmin, ParaType Rmax);
lw_32 MiaoToolRandomLCGFunInt(void *pRand, lw_32 Rmin, lw_32 Rmax);

typedef struct MIAO_TOOL_RANDOM_WELL512_STRUCT {
	MiaoToolRandomBasic_t basic;
	uint32_t _state[16];
	uint32_t _index;
}MiaoToolRandomWELL512_t;


ParaType MiaoToolRandomWELL512Fun(void *pRand, ParaType Rmin, ParaType Rmax);
lw_32 MiaoToolRandomWELL512FunInt(void *pRand, lw_32 Rmin, lw_32 Rmax);
void MiaoToolRandomWELL512Seed(MiaoToolRandomWELL512_t *pRand, uint32_t seed);



typedef struct MIAO_TOOL_RANDOM_MT_STRUCT {
	MiaoToolRandomBasic_t basic;
	uint32_t _mt[624];
	lw_u16 _index;
}MiaoToolRandomMT_t;


ParaType MiaoToolRandomMTFun(void *pRand, ParaType Rmin, ParaType Rmax);
lw_32 MiaoToolRandomMTFunInt(void *pRand, lw_32 Rmin, lw_32 Rmax);
void MiaoToolRandomMTSeed(MiaoToolRandomMT_t *pRand, uint32_t seed);


void MiaoToolMatrixRandomInitial(MiaoMat *m,void *pRand,ParaType Rmin,ParaType Rmax);
void *MiaoToolMatrixToRandnChip(MiaoMat *s, MiaoMat *m, MiaoToolRandomBasic_t *pRand, ParaType var, ParaType Rmin, ParaType Rmax);


#endif // !(MIAO_SIMPLE_LEVEL >= 3)

#endif

