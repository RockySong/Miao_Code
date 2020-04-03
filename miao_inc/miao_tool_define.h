/*(Ver.=0.96)
* MIAO_define.h
*
*  Created on: 2018-7-14
*      Author: slw
*/


#ifndef MIAO_TOOL_DEFINE_H_
#define MIAO_TOOL_DEFINE_H_

//ALL IMPORT
#define MIAO_TOOL_IMPORT_ALL 1

#define L1_L2_CACHE_OPTIM 1


//#define MIAO_USING_CBLAS 1


#if defined(MIAO_USING_CBLAS) && MIAO_USING_CBLAS

#include "cblas.h"
	
#if MIAO_GLOBAL_MATH_TYPE!=MIAO_GLOBAL_MATH_FLOAT

#error "Math type must be float"

#endif

#endif



//basic tool
#define MIAO_TOOL_IMPORT_SLIP_WIN 1
#define MIAO_TOOL_IMPORT_DLIST 1
#define MIAO_TOOL_IMPORT_HASH_LIST 1
#define MIAO_TOOL_IMPORT_TABLE 1

//哈希表 分类大小
#define MIAO_TOOL_HASH_LIST_MAX 20


//MATH TOOL 
#define MIAO_TOOL_IMPORT_MATH 1


// NN TOOL
#define MIAO_TOOL_IMPORT_NN 0 
#define MIAO_TOOL_IMPORT_NN_BPnn 0

#define MIAO_TOOL_NN_D_FRAME_C 'C'
#define MIAO_TOOL_NN_D_FRAME_F 'F'

#define MIAO_TOOL_NN_DATA_FRAME MIAO_TOOL_NN_D_FRAME_C

//RL
#define MIAO_TOOL_IMPORT_RL 1
#define MIAO_TOOL_IMPORT_RL_Q_LEARNING 1
#define MIAO_TOOL_IMPORT_RL_SARSA 1
#define MIAO_TOOL_IMPORT_RL_EXP_RE 1
#define MIAO_TOOL_IMPORT_RL_DQN 0
#define MIAO_TOOL_IMPORT_RL_DDPG 1

//RAND
#define MIAO_TOOL_IMPORT_RAND_CHAOS 0
#define MIAO_TOOL_IMPORT_RAND_WELL 0
#define MIAO_TOOL_IMPORT_RAND_MT 0


//正态分布随机数产生方法
#define MIAO_TOOL_RAND_NORM_BM 1
#define MIAO_TOOL_RAND_NORM_LLN 2
#define MIAO_TOOL_RAND_NORM_MP 3
#define MIAO_TOOL_RAND_NORM_METHOD MIAO_TOOL_RAND_NORM_BM

//大数定理 方法参数
#define MIAO_TOOL_RAND_NORM_LLN_NUM 25
#define MIAO_TOOL_RAND_NORM_LLN_DIV 1.4434

#define MIAO_DECLARE(KIND,NAME,PARA) KIND NAME PARA;typedef KIND (*p##NAME##FunType) PARA;extern p##NAME##FunType p##NAME;
#define MIAO_TOOL_FUN(KIND,NAME,PARA,RNAME) p##NAME##FunType p##NAME = RNAME;KIND NAME PARA


typedef enum {

	MiaoToolMatrixConvMethod_Valid = 'v',
	MiaoToolMatrixConvMethod_Full = 'f'

}MiaoToolMatrixConvMethodNUM;


#endif // MIAO_TOOL_DEFINE_H_
