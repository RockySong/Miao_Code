/*(Ver.=0.96)
 * MIAO_tool.h
 *
 *  Created on: 2017-11-27
 *      Author: slw
 */

#ifndef MIAO_TOOL_RL_H_
#define MIAO_TOOL_RL_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_basic_math.h"
#include "OSLW_tool_random.h"
#include "OSLW_tool_NN.h"
#include "OSLW_memory.h"
#if !(MIAO_SIMPLE_LEVEL >= 2)

/*(Ver.=0.96)
数据结构:
例如	状态 3*2（一共3种状态 每一种状态2个特征量）
		动作 2*4 （一共4种动作 每一个动作2种操作）---由于C语言是按照先列再行的方式存储 所以对应的是转置 既动作矩阵实际是4*2 
		!!!!!!!!!!!!!!行代表有几个动作
		qtable 3*4
			/									\T
			|	|a0.0|	|a1.0|	|a2.0|	|a3.0|	|
			|	|a0.1|	|a1.1|	|a2.1|	|a3.1|	|
			\									/
			  ********************************
| s0.0 s0.0 | *	|q0 0|	|q0 1|	|q0 2|	|q0 3|	
| s1.0 s1.0 | *	|q1 0|	|q1 1|	|q1 2|	|q1 3|
| s2.0 s2.0 | *	|q2 0|	|q2 1|	|q2 2|	|q2 3|

*/
typedef struct MIAO_TOOL_Q_RL_DATA_STATIC_BASIC_STRUCT{
	MiaoMat StateTable;//状态
	MiaoMat ActionTable;//行为
	MiaoMat Qtable;//qtable
	
}MiaoToolQRLData_t;
	
typedef struct MIAO_TOOL_RL_BASIC_STRUCT {
	MiaoMemoryBasic_t *pmem;
	MiaoToolRandomBasic_t *pRand;

	MiaoMat ActNow;//当前(未来)行为
	MiaoMat StateNow;
	MiaoMat StateTermial;//状态终点
	MiaoMat StateLast;
	
	ParaType RewardNow;
	ParaType Gamma;


	ParaType RewardSum;//回报总和
	ParaType RewardSumMax, RewardSumMin;
	
	ParaType StatusTermialJudgeThreshold;//状态终点阈值 会计算当前状态与状态终点的欧式距离

	uint32_t RoundCount;//回合计数器
	uint32_t RoundMax;


	void(*ActEnvFun)(struct MIAO_TOOL_RL_BASIC_STRUCT *pRLB);
	void(*BornFun)(struct MIAO_TOOL_RL_BASIC_STRUCT *pRLB);
	void(*BornFunByUser)(struct MIAO_TOOL_RL_BASIC_STRUCT *pRLB);

	void * (*ChooseFun)(struct MIAO_TOOL_RL_BASIC_STRUCT *pRLB);
	void * (*LearnFun)(struct MIAO_TOOL_RL_BASIC_STRUCT *pRLB);
	void * (*StateStoreFun)(struct MIAO_TOOL_RL_BASIC_STRUCT *pQRLB, MiaoMat *state);
	lw_8(*isTermialFun)(struct MIAO_TOOL_RL_BASIC_STRUCT *pRLB);
	union 
	{
		struct
		{
			lw_u8 IsTermialFlag : 4;
			lw_u8 StatusNear : 1;
			lw_u8 RoundOver : 1;
			lw_u8 RewardUpOver : 1;
			lw_u8 RewardDownOver : 1;
		}bits;

		lw_u8 all;

	}TermialJudgeFlag;

}MiaoToolRLBasic_t;

/*
	SN 
	RO round over
	RUO reward up over
	RDO reward down over
*/


#define MiaoToolRLT_SN (1<<4)
#define MiaoToolRLT_RO (1<<5)
#define MiaoToolRLT_RUO (1<<6)
#define MiaoToolRLT_RDO (1<<7)




void MiaoToolRL_Termial(
	void *pRL,
	lw_u8 TermialJudgeFlag,
	ParaType RewardSumMax,
	ParaType RewardSumMin,
	ParaType *pStatusTermial,
	ParaType Threshold,//状态终点阈值 会计算当前状态与状态终点的欧式距离)
	uint32_t RoundMax
);

lw_8 MiaoToolRL_isTermial(void *pRL);

#define MIAO_RL_UPDATE(RL) 	do{((MiaoToolRLBasic_t *)(RL))->RewardSum = _ParaAdd(((MiaoToolRLBasic_t *)(RL))->RewardSum, ((MiaoToolRLBasic_t *)(RL))->RewardNow);\
	((MiaoToolRLBasic_t *)(RL))->RoundCount++;}while(0)

typedef struct MIAO_TOOL_Q_RL_BASIC_STRUCT{

	MiaoToolRLBasic_t basic;
	MiaoToolQRLData_t DataTable;
	ParaType Ep;

	lw_u16 ActNowIndex;//当前行为索引
	lw_u16 ActLastIndex;
	lw_u16 StateNowIndex;
	lw_u16 StateTermialIndex;
	lw_u16 StateLastIndex;
	lw_u16 *pActChoose;//待选择列表(当有相同数据时使用)


}MiaoToolQRLBasic_t;

typedef void (*pActEnvFunType)(struct MIAO_TOOL_Q_RL_BASIC_STRUCT *pQRLB);
typedef void (*pBornFunType)(struct MIAO_TOOL_Q_RL_BASIC_STRUCT *pQRLB);



/*(Ver.=0.96)
//Q-learing的顺序是先决策,再运行环境,再学习，所以是off-policy
//
//Slast------------>Snow-------->.........
//		Alast(Anow)		|
//						|
//						|
//						Learning(Slast,Anow,Snow)
//
*/



typedef struct MIAO_TOOL_Q_LEARNING_STRUCT{

	MiaoToolQRLBasic_t basic;

	ParaType Q_Pre;
	ParaType Q_Now;
	ParaType Lr;

		
}MiaoToolQLearning_t;


/*(Ver.=0.96)
//sarsa 是直接学习action 所以是on-policy
//
//Slast------------>Snow-------->.........
//		Alast			  Anow		
//							|
//							|
//							learing(Slast,Alast,Snow,Anow)
//
//
//lamber的作用是记录轨迹,如果收敛那么可以加速
// Trace: a<1
//		action	1	2	
//	state
//	1			0	1*a^4	4步之前
//	2			0	1*a^3	3步之前
//	3			0	1*a^2	2步之前
//	4			0	1*a^1	1步之前
//
//	Q矩阵+=Trace矩阵
//
*/

typedef struct MIAO_TOOL_SARSA_LAMBER_STRUCT {

	MiaoToolQRLBasic_t basic;


	ParaType Q_Pre;
	ParaType Q_Now;
	ParaType Ep;
	ParaType Lr;

	ParaType Lamber;
	MiaoMat Trace;
	


}MiaoToolSarsaLamber_t;




void MiaoToolQLearningParaInitial
(
	MiaoToolQLearning_t *pTQL,//this指针
	ParaType Ep,//阈值
	ParaType Lr,//学习速率
	ParaType Gamma,//衰减因子
	MiaoToolRandomBasic_t *prand,//随机数产生器
	MiaoMemoryBasic_t *pmem,//内存分配指针（可为NULL）
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
);

/*(Ver.=0.96)

void MiaoToolQLearningStaticInitial
	(
	MiaoToolQLearning_t *pTQL, //this指针
	lw_u16 row, lw_u16 col, //qtable行列
	ParaType *pState, //状态列表内存
	ParaType *pAction, //行为列表
	ParaType *pTable, //qtable
	lw_u16 *pactchoose//备选行为列表与行为列表相同大小
	);
	*/

void MiaoToolQLearningTableInitial
(
	MiaoToolQLearning_t *pTQL,//this指针
	lw_u16 state_row, lw_u16 state_col,//状态表的行列
	lw_u16 action_row, lw_u16 action_col,//行为表的行列
	void *pState,//状态列表内存（可为NULL）
	void *pAction,//行为列表（可为NULL,不推荐）
	void *pTable,//qtable（可为NULL）
	void *pStateNow,//状态列表内存（可为NULL,不推荐）
	void *pStateTermial,//状态终点,游戏终点(可为NULL，不推荐)
	lw_u16 *pactchoose//状态列表内存（可为NULL）
);


MiaoToolQLearning_t* MiaoToolQLearningChooseFun(MiaoToolQLearning_t *pQRLB);
MiaoToolQLearning_t* MiaoToolQLearningLearnStateStore(MiaoToolQLearning_t *pQRL, MiaoMat *state);
MiaoToolQLearning_t* MiaoToolQLearningLearnFun(MiaoToolQLearning_t *pQRL);




void MiaoToolSarsaLamberParaInitial
(
	//绝大部分与Qlearning相同
	MiaoToolSarsaLamber_t *pTQL,
	ParaType Ep,
	ParaType Lr,
	ParaType Gamma,
	MiaoToolRandomBasic_t *prand,
	MiaoMemoryBasic_t *pmem,
	ParaType lamber,//新增参数 Lamber
	pActEnvFunType pActfun,
	pBornFunType pBornfun
);

void MiaoToolSarsaLamberTableInitial
(
	//绝大部分与Qlearning相同
	MiaoToolSarsaLamber_t *pTQL,
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pState,
	void *pAction,
	void *pTable,
	void *pStateNow,
	void *pStateTermial,
	lw_u16 *pactchoose,
	void *pTrace//新增参数 轨迹（可为NULL）
);


MiaoToolSarsaLamber_t* MiaoToolSarsaLamberLearnFun(MiaoToolSarsaLamber_t *pQRL);


//------------------------------------------------------
//<经验值回放>
//------------------------------------------------------
typedef struct MIAO_TOOL_DQN_EXP_REPLAY_FRAME_STRUCT
{
	MiaoToolDListNodeConnect_t con, *pcon;
	ParaType Importance;
	ParaType _new_Importance;
	
}MiaoToolDQNetExpReplayFrame_t;


typedef enum {
	MiaoToolDQNetExpReplay_NoP = 0,
	MiaoToolDQNetExpReplay_PER
}MiaoToolDQNetExpReplayTypeNUM;


typedef struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT
{
	MiaoToolDQNetExpReplayTypeNUM PriorType;

	MiaoToolTable_t MemTable;
	MiaoExternMemory_t DivResult;
	MiaoExternMemory_t SampleResult;

	ParaType Rmin, Rmax;
	ParaType Sum;
	ParaType Div;
	

	ParaType Alpha, Beta, Epsi;

	lw_u16 State_Col;
	lw_u16 Action_Col;

	MiaoToolQRLBasic_t *pTQL;
	ParaType Min, Max;

	struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *(*AppendFun)(struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe,lw_u16 Num, lw_u8 method);
	struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *(*UpdateFun)(struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe,lw_u16 num);
	struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *(*MinMaxFun)(struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe);
	lw_u16 (*SampleFun)(struct MIAO_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe);

}MiaoToolDQNetExpReplay_t;

typedef MiaoToolDQNetExpReplay_t MiaoToolExpReplay_t;


#define MIAO_TOOL_EXPRE_CAL_P(ERR,EXPRE) _ParaPow(_ParaAdd((ERR), (EXPRE).Epsi), (EXPRE).Alpha)

MiaoToolDQNetExpReplay_t *MiaoToolDQNetExpReplayAppend(MiaoToolDQNetExpReplay_t *pExpRe,lw_u16 Num, lw_u8 method);
MiaoToolDQNetExpReplay_t *_MiaoToolDQNetExpReplayInsert(MiaoToolDQNetExpReplay_t *pExpRe, MiaoToolDQNetExpReplayFrame_t *pExpReF);
MiaoToolDQNetExpReplay_t *_MiaoToolDQNetExpReplayDelete(MiaoToolDQNetExpReplay_t *pExpRe, MiaoToolDQNetExpReplayFrame_t *pExpReF);
MiaoToolDQNetExpReplay_t *MiaoToolDQNetExpReplayUpdate(MiaoToolDQNetExpReplay_t *pExpRe,lw_u16 num);
MiaoToolDQNetExpReplay_t *MiaoToolDQNetExpReplayMinMax(MiaoToolDQNetExpReplay_t *pExpRe);
lw_u16 MiaoToolDQNetExpReplaySample(MiaoToolDQNetExpReplay_t *pExpRe);

#define MIAO_TOOL_DQN_EXP_STATE_SIZE(EXP__t) (sizeof(ParaType) *(EXP__t).State_Col)
#define MIAO_TOOL_DQN_EXP_REWARD_SIZE(EXP__t) (sizeof(ParaType))
#define MIAO_TOOL_DQN_EXP_ACTION_SIZE(EXP__t) (sizeof(ParaType)*(EXP__t).Action_Col)

#define MIAO_TOOL_DQN_EXP_FRAME_SIZE(EXP__t) (\
	sizeof(MiaoToolDQNetExpReplayFrame_t) +/*(Ver.=0.96)节点长度*/\
	(MIAO_TOOL_DQN_EXP_STATE_SIZE(EXP__t)<<1) + \
	MIAO_TOOL_DQN_EXP_REWARD_SIZE(EXP__t) + \
	MIAO_TOOL_DQN_EXP_ACTION_SIZE(EXP__t) \
)

#define MIAO_TOOL_DQN_EXP_STATE_LAST(EXP__t,PBASE) ((size_t)(PBASE)+sizeof(MiaoToolDQNetExpReplayFrame_t))

#define MIAO_TOOL_DQN_EXP_ACT(EXP__t,PBASE) ((size_t)(PBASE)+sizeof(MiaoToolDQNetExpReplayFrame_t)+MIAO_TOOL_DQN_EXP_STATE_SIZE(EXP__t))

#define MIAO_TOOL_DQN_EXP_R(EXP__t,PBASE) ((size_t)(PBASE)+sizeof(MiaoToolDQNetExpReplayFrame_t)+\
	MIAO_TOOL_DQN_EXP_STATE_SIZE(EXP__t)+\
	MIAO_TOOL_DQN_EXP_ACTION_SIZE(EXP__t)\
)

#define MIAO_TOOL_DQN_EXP_STATE_NOW(EXP__t,PBASE) ((size_t)(PBASE)+sizeof(MiaoToolDQNetExpReplayFrame_t)\
	+MIAO_TOOL_DQN_EXP_STATE_SIZE(EXP__t)\
	+MIAO_TOOL_DQN_EXP_REWARD_SIZE(EXP__t)\
	+MIAO_TOOL_DQN_EXP_ACTION_SIZE(EXP__t)\
)
//------------------------------------------------------
//</经验值回放>
//------------------------------------------------------



//------------------------------------------------------
//<DQN>
//------------------------------------------------------

typedef enum {
	MiaoToolDQNetOptim_NormalDQN = 0,
	MiaoToolDQNetOptim_DoubleDQN = 1
}MiaoToolDQNetOptimNum;

typedef struct MIAO_TOOL_DEEP_Q_NET_STRUCT
{
	MiaoToolQRLBasic_t basic;//基类
	MiaoToolBPnn_t MainNet;//Q现实
	MiaoToolBPnn_t TargetNet;//Q目标 异步更新

	MiaoToolDQNetExpReplay_t ExpReplay;//experience replay
	MiaoMat StateFactor; 
	MiaoToolDQNetOptimNum Optim;

	lw_u16 Count;
	lw_u16 CountMax;
	lw_u16 Count_Copy;
	lw_u16 NetCopyThreshold;//


	//MiaoMemoryBasic_t *pDyMem;//内存分配类

	struct MIAO_TOOL_DEEP_Q_NET_STRUCT* (*StoreMemFun)(struct MIAO_TOOL_DEEP_Q_NET_STRUCT *pQRLB);
	//struct MIAO_TOOL_DEEP_Q_NET_STRUCT* (*LearnFun)(struct MIAO_TOOL_DEEP_Q_NET_STRUCT *pQRLB);

}MiaoToolDQNet_t; 




MiaoToolDQNet_t* MiaoToolDQNetChoose(MiaoToolDQNet_t *pDQN);
MiaoToolDQNet_t* MiaoToolDQNetStoreMem(MiaoToolDQNet_t *pDQN);
MiaoToolDQNet_t* MiaoToolDQNetLearning(MiaoToolDQNet_t *pDQN);


void MiaoToolDQNetInitial(MiaoToolDQNet_t *pDQN);//DQN初始化函数，最先被调用


void MiaoToolDQNetParaInitial//DQN内存初始化函数
(
	MiaoToolDQNet_t *pDQN,//this指针
	ParaType Ep,//阈值
	ParaType Gamma,//衰减因子
	MiaoToolDQNetOptimNum Optim,
	MiaoToolRandomBasic_t *prand,//随机数产生器
	MiaoMemoryBasic_t *pMem,//内存指针
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
);


void MiaoToolDQNetInterfaceInitial
(
	MiaoToolDQNet_t *pDQN,//this指针
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pAction,//动作列表内存（可为NULL）
	void *pStateNow,//当前状态（可为NULL）
	void *pStateLast,//之前状态（可为NULL）
	void *pStateTermial,//终结状态（可为NULL）
	void *pStateFactor//状态归一化因子(可为NULL)
);

void MiaoToolDQNetStoreMemInitial//DQN内存分配初始化
(
	MiaoToolDQNet_t *pDQN,//this指针
	MiaoToolDQNetExpReplayTypeNUM PER,//是否继续PER优化
	lw_u16 MemPoolLength,//记忆长度
	lw_u16 batch_size,//抽取的记忆长度
	lw_u16 copy_th,//复制的阈值
	ParaType Rmax, ParaType Rmin,//大致范围（不优化状态下无意义）
	ParaType Alpha, ParaType Beta,//ab参数（不优化状态下无意义）
	ParaType Epsi
);



void MiaoToolDQNetAppendInitial
(
	MiaoToolDQNet_t *pDQN,
	lw_u16 row,
	lw_u16 col,
	MiaoNNinitFunType pFun,
	MiaoToolRandomBasic_t *pTRB,
	ParaType initd1, ParaType initd2,
	MiaoToolNNLayerActFun_t *pTemplet
);

void MiaoToolDQNetTarinInitial(
	MiaoToolDQNet_t *pDQN,
	ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
);

void MiaoToolDQNetAllDataInit(MiaoToolDQNet_t *pDQN);
//------------------------------------------------------
//</DQN>
//------------------------------------------------------



//------------------------------------------------------
//<DDPG>
//------------------------------------------------------

typedef enum
{
	MiaoToolACtoActor = 0,
	MiaoToolACtoCritic = 1,
}MiaoToolACtoNet;


typedef enum
{
	MiaoToolDDPG_CopyMethod_Hard = 0,
	MiaoToolDDPG_CopyMethod_Soft = 1,

}MiaoToolDDPG_CopyMethod;

typedef struct MIAO_TOOL_POLICY_GRAD_STRUCT {
	MiaoToolRLBasic_t basic;
	MiaoToolExpReplay_t ExpReplay;

	MiaoMat StateFactor;

	ParaType ActionMax, ActionMin;


}MiaoToolPGrad_t;


typedef struct MIAO_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT{

	MiaoToolPGrad_t PGradBasic;

	MiaoToolBPnn_t ActorMainNet, ActorTargetNet;
	MiaoToolBPnn_t CriticMainNet, CriticTargetNet;

	ParaType ActionVar;

	lw_u16 Count;
	lw_u16 CountMax;

	struct MIAO_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT* (*StoreMemFun)(struct MIAO_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT *pQRLB);

	union
	{
		ParaType tSoftReplace;
		struct
		{
			lw_u16 Count_Copy;
			lw_u16 NetCopyThreshold;
		}HardReplace;
	}Copy;

	MiaoToolDDPG_CopyMethod CopyMethod;

	
}MiaoToolDDPGrad_t;

MiaoToolDDPGrad_t* MiaoToolDDPGradChoose(MiaoToolDDPGrad_t *pDDPG);
MiaoToolDDPGrad_t* MiaoToolDDPGradStoreMem(MiaoToolDDPGrad_t *pDDPG);
MiaoToolDDPGrad_t* MiaoToolDDPGradLearning(MiaoToolDDPGrad_t *pDDPG);


void MiaoToolDDPGradInitial(MiaoToolDDPGrad_t *pDDPG);//DDPG初始化函数，最先被调用


void MiaoToolDDPGradParaInitial//DDPG内存初始化函数
(
	MiaoToolDDPGrad_t *pDDPG,//this指针
	ParaType Gamma,//衰减因子
	MiaoToolRandomBasic_t *prand,//随机数产生器
	MiaoMemoryBasic_t *pMem,//内存指针
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
);


void MiaoToolDDPGradInterfaceInitial
(
	MiaoToolDDPGrad_t *pDDPG,//this指针
	lw_u16 state_dim,lw_u16 action_dim,
	void *pAction,//动作列表内存（可为NULL）
	void *pStateNow,//当前状态（可为NULL）
	void *pStateLast,//之前状态（可为NULL）
	void *pStateTermial,//终结状态（可为NULL）
	void *pStateFactor,//状态归一化因子(可为NULL)
	ParaType ActionMax, ParaType ActionMin,
	ParaType ActionVar
);

void MiaoToolDDPGradStoreMemInitial//DDPG内存分配初始化
(
	MiaoToolDDPGrad_t *pDDPG,//this指针
	lw_u16 MemPoolLength,//记忆长度
	lw_u16 batch_size,//抽取的记忆长度
	ParaType copy_data//复制的阈值
);



void MiaoToolDDPGradAppendInitial
(
	MiaoToolDDPGrad_t *pDDPG,
	MiaoToolACtoNet Kind,
	lw_u16 row,
	lw_u16 col,
	MiaoNNinitFunType pFun,
	MiaoToolRandomBasic_t *pTRB,
	ParaType initd1, ParaType initd2,
	MiaoToolNNLayerActFun_t *pTemplet
);

void MiaoToolDDPGradTarinInitial(
	MiaoToolDDPGrad_t *pDDPG,
	ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
);
void MiaoToolDDPGradAllDataInit(MiaoToolDDPGrad_t *pDDPG);




//------------------------------------------------------
//</DDPG>
//------------------------------------------------------

//------------------------------------------------------
//<PPO>
//------------------------------------------------------
typedef struct MIAO_TOOL_PEOXIMAL_POLICY_OPTIM_STRUCT {
	MiaoToolPGrad_t PGradBasic;
	MiaoToolBPnn_t ActorNewNet, ActorOldNet;
	MiaoToolBPnn_t CriticNet;

	ParaType ClipE;
	ParaType GAE_tau;
	ParaType GAE_gamma;
	ParaType beta_entropy;

	lw_u16 Count;
	lw_u16 CountEpsion;


	struct MIAO_TOOL_PEOXIMAL_POLICY_OPTIM_STRUCT* (*StoreMemFun)(struct MIAO_TOOL_PEOXIMAL_POLICY_OPTIM_STRUCT *pQRLB);

}MiaoToolPPOptim_t;


MiaoToolPPOptim_t* MiaoToolPPOptimChoose(MiaoToolPPOptim_t *pPPO);
MiaoToolPPOptim_t* MiaoToolPPOptimStoreMem(MiaoToolPPOptim_t *pPPO);
MiaoToolPPOptim_t* MiaoToolPPOptimLearning(MiaoToolPPOptim_t *pPPO);


void MiaoToolPPOptimInitial(MiaoToolPPOptim_t *pPPO);//PPO初始化函数，最先被调用


void MiaoToolPPOptimParaInitial//PPO内存初始化函数
(
	MiaoToolPPOptim_t *pPPO,//this指针
	ParaType ClipE,
	ParaType tau,
	ParaType gamma,
	ParaType beta,
	MiaoToolRandomBasic_t *prand,//随机数产生器
	MiaoMemoryBasic_t *pMem,//内存指针
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
	
);


void MiaoToolPPOptimInterfaceInitial
(
	MiaoToolPPOptim_t *pPPO,//this指针
	lw_u16 state_dim, lw_u16 action_dim,
	void *pAction,//动作列表内存（可为NULL）
	void *pStateNow,//当前状态（可为NULL）
	void *pStateLast,//之前状态（可为NULL）
	void *pStateTermial,//终结状态（可为NULL）
	void *pStateFactor,//状态归一化因子(可为NULL)
	ParaType ActionMax, ParaType ActionMin
);

void MiaoToolPPOptimStoreMemInitial//PPO内存分配初始化
(
	MiaoToolPPOptim_t *pPPO,//this指针
	lw_u16 MemPoolLength//记忆长度
);


void MiaoToolPPOptimAppendInitial//PPO神经网络追加,一次会追加两个神经网络
(
	MiaoToolPPOptim_t *pPPO,//this指针
	MiaoToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	MiaoToolNNLayerActFun_t **AFlist,//激活函数
	ParaType *nl_list//学习速率
);


//------------------------------------------------------
//</PPO>
//------------------------------------------------------


#endif // !(MIAO_SIMPLE_LEVEL >= 2)

#endif
