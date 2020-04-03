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
���ݽṹ:
����	״̬ 3*2��һ��3��״̬ ÿһ��״̬2����������
		���� 2*4 ��һ��4�ֶ��� ÿһ������2�ֲ�����---����C�����ǰ����������еķ�ʽ�洢 ���Զ�Ӧ����ת�� �ȶ�������ʵ����4*2 
		!!!!!!!!!!!!!!�д����м�������
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
	MiaoMat StateTable;//״̬
	MiaoMat ActionTable;//��Ϊ
	MiaoMat Qtable;//qtable
	
}MiaoToolQRLData_t;
	
typedef struct MIAO_TOOL_RL_BASIC_STRUCT {
	MiaoMemoryBasic_t *pmem;
	MiaoToolRandomBasic_t *pRand;

	MiaoMat ActNow;//��ǰ(δ��)��Ϊ
	MiaoMat StateNow;
	MiaoMat StateTermial;//״̬�յ�
	MiaoMat StateLast;
	
	ParaType RewardNow;
	ParaType Gamma;


	ParaType RewardSum;//�ر��ܺ�
	ParaType RewardSumMax, RewardSumMin;
	
	ParaType StatusTermialJudgeThreshold;//״̬�յ���ֵ ����㵱ǰ״̬��״̬�յ��ŷʽ����

	uint32_t RoundCount;//�غϼ�����
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
	ParaType Threshold,//״̬�յ���ֵ ����㵱ǰ״̬��״̬�յ��ŷʽ����)
	uint32_t RoundMax
);

lw_8 MiaoToolRL_isTermial(void *pRL);

#define MIAO_RL_UPDATE(RL) 	do{((MiaoToolRLBasic_t *)(RL))->RewardSum = _ParaAdd(((MiaoToolRLBasic_t *)(RL))->RewardSum, ((MiaoToolRLBasic_t *)(RL))->RewardNow);\
	((MiaoToolRLBasic_t *)(RL))->RoundCount++;}while(0)

typedef struct MIAO_TOOL_Q_RL_BASIC_STRUCT{

	MiaoToolRLBasic_t basic;
	MiaoToolQRLData_t DataTable;
	ParaType Ep;

	lw_u16 ActNowIndex;//��ǰ��Ϊ����
	lw_u16 ActLastIndex;
	lw_u16 StateNowIndex;
	lw_u16 StateTermialIndex;
	lw_u16 StateLastIndex;
	lw_u16 *pActChoose;//��ѡ���б�(������ͬ����ʱʹ��)


}MiaoToolQRLBasic_t;

typedef void (*pActEnvFunType)(struct MIAO_TOOL_Q_RL_BASIC_STRUCT *pQRLB);
typedef void (*pBornFunType)(struct MIAO_TOOL_Q_RL_BASIC_STRUCT *pQRLB);



/*(Ver.=0.96)
//Q-learing��˳�����Ⱦ���,�����л���,��ѧϰ��������off-policy
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
//sarsa ��ֱ��ѧϰaction ������on-policy
//
//Slast------------>Snow-------->.........
//		Alast			  Anow		
//							|
//							|
//							learing(Slast,Alast,Snow,Anow)
//
//
//lamber�������Ǽ�¼�켣,���������ô���Լ���
// Trace: a<1
//		action	1	2	
//	state
//	1			0	1*a^4	4��֮ǰ
//	2			0	1*a^3	3��֮ǰ
//	3			0	1*a^2	2��֮ǰ
//	4			0	1*a^1	1��֮ǰ
//
//	Q����+=Trace����
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
	MiaoToolQLearning_t *pTQL,//thisָ��
	ParaType Ep,//��ֵ
	ParaType Lr,//ѧϰ����
	ParaType Gamma,//˥������
	MiaoToolRandomBasic_t *prand,//�����������
	MiaoMemoryBasic_t *pmem,//�ڴ����ָ�루��ΪNULL��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
);

/*(Ver.=0.96)

void MiaoToolQLearningStaticInitial
	(
	MiaoToolQLearning_t *pTQL, //thisָ��
	lw_u16 row, lw_u16 col, //qtable����
	ParaType *pState, //״̬�б��ڴ�
	ParaType *pAction, //��Ϊ�б�
	ParaType *pTable, //qtable
	lw_u16 *pactchoose//��ѡ��Ϊ�б�����Ϊ�б���ͬ��С
	);
	*/

void MiaoToolQLearningTableInitial
(
	MiaoToolQLearning_t *pTQL,//thisָ��
	lw_u16 state_row, lw_u16 state_col,//״̬�������
	lw_u16 action_row, lw_u16 action_col,//��Ϊ�������
	void *pState,//״̬�б��ڴ棨��ΪNULL��
	void *pAction,//��Ϊ�б���ΪNULL,���Ƽ���
	void *pTable,//qtable����ΪNULL��
	void *pStateNow,//״̬�б��ڴ棨��ΪNULL,���Ƽ���
	void *pStateTermial,//״̬�յ�,��Ϸ�յ�(��ΪNULL�����Ƽ�)
	lw_u16 *pactchoose//״̬�б��ڴ棨��ΪNULL��
);


MiaoToolQLearning_t* MiaoToolQLearningChooseFun(MiaoToolQLearning_t *pQRLB);
MiaoToolQLearning_t* MiaoToolQLearningLearnStateStore(MiaoToolQLearning_t *pQRL, MiaoMat *state);
MiaoToolQLearning_t* MiaoToolQLearningLearnFun(MiaoToolQLearning_t *pQRL);




void MiaoToolSarsaLamberParaInitial
(
	//���󲿷���Qlearning��ͬ
	MiaoToolSarsaLamber_t *pTQL,
	ParaType Ep,
	ParaType Lr,
	ParaType Gamma,
	MiaoToolRandomBasic_t *prand,
	MiaoMemoryBasic_t *pmem,
	ParaType lamber,//�������� Lamber
	pActEnvFunType pActfun,
	pBornFunType pBornfun
);

void MiaoToolSarsaLamberTableInitial
(
	//���󲿷���Qlearning��ͬ
	MiaoToolSarsaLamber_t *pTQL,
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pState,
	void *pAction,
	void *pTable,
	void *pStateNow,
	void *pStateTermial,
	lw_u16 *pactchoose,
	void *pTrace//�������� �켣����ΪNULL��
);


MiaoToolSarsaLamber_t* MiaoToolSarsaLamberLearnFun(MiaoToolSarsaLamber_t *pQRL);


//------------------------------------------------------
//<����ֵ�ط�>
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
	sizeof(MiaoToolDQNetExpReplayFrame_t) +/*(Ver.=0.96)�ڵ㳤��*/\
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
//</����ֵ�ط�>
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
	MiaoToolQRLBasic_t basic;//����
	MiaoToolBPnn_t MainNet;//Q��ʵ
	MiaoToolBPnn_t TargetNet;//QĿ�� �첽����

	MiaoToolDQNetExpReplay_t ExpReplay;//experience replay
	MiaoMat StateFactor; 
	MiaoToolDQNetOptimNum Optim;

	lw_u16 Count;
	lw_u16 CountMax;
	lw_u16 Count_Copy;
	lw_u16 NetCopyThreshold;//


	//MiaoMemoryBasic_t *pDyMem;//�ڴ������

	struct MIAO_TOOL_DEEP_Q_NET_STRUCT* (*StoreMemFun)(struct MIAO_TOOL_DEEP_Q_NET_STRUCT *pQRLB);
	//struct MIAO_TOOL_DEEP_Q_NET_STRUCT* (*LearnFun)(struct MIAO_TOOL_DEEP_Q_NET_STRUCT *pQRLB);

}MiaoToolDQNet_t; 




MiaoToolDQNet_t* MiaoToolDQNetChoose(MiaoToolDQNet_t *pDQN);
MiaoToolDQNet_t* MiaoToolDQNetStoreMem(MiaoToolDQNet_t *pDQN);
MiaoToolDQNet_t* MiaoToolDQNetLearning(MiaoToolDQNet_t *pDQN);


void MiaoToolDQNetInitial(MiaoToolDQNet_t *pDQN);//DQN��ʼ�����������ȱ�����


void MiaoToolDQNetParaInitial//DQN�ڴ��ʼ������
(
	MiaoToolDQNet_t *pDQN,//thisָ��
	ParaType Ep,//��ֵ
	ParaType Gamma,//˥������
	MiaoToolDQNetOptimNum Optim,
	MiaoToolRandomBasic_t *prand,//�����������
	MiaoMemoryBasic_t *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
);


void MiaoToolDQNetInterfaceInitial
(
	MiaoToolDQNet_t *pDQN,//thisָ��
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor//״̬��һ������(��ΪNULL)
);

void MiaoToolDQNetStoreMemInitial//DQN�ڴ�����ʼ��
(
	MiaoToolDQNet_t *pDQN,//thisָ��
	MiaoToolDQNetExpReplayTypeNUM PER,//�Ƿ����PER�Ż�
	lw_u16 MemPoolLength,//���䳤��
	lw_u16 batch_size,//��ȡ�ļ��䳤��
	lw_u16 copy_th,//���Ƶ���ֵ
	ParaType Rmax, ParaType Rmin,//���·�Χ�����Ż�״̬�������壩
	ParaType Alpha, ParaType Beta,//ab���������Ż�״̬�������壩
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


void MiaoToolDDPGradInitial(MiaoToolDDPGrad_t *pDDPG);//DDPG��ʼ�����������ȱ�����


void MiaoToolDDPGradParaInitial//DDPG�ڴ��ʼ������
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	ParaType Gamma,//˥������
	MiaoToolRandomBasic_t *prand,//�����������
	MiaoMemoryBasic_t *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
);


void MiaoToolDDPGradInterfaceInitial
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	lw_u16 state_dim,lw_u16 action_dim,
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor,//״̬��һ������(��ΪNULL)
	ParaType ActionMax, ParaType ActionMin,
	ParaType ActionVar
);

void MiaoToolDDPGradStoreMemInitial//DDPG�ڴ�����ʼ��
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	lw_u16 MemPoolLength,//���䳤��
	lw_u16 batch_size,//��ȡ�ļ��䳤��
	ParaType copy_data//���Ƶ���ֵ
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


void MiaoToolPPOptimInitial(MiaoToolPPOptim_t *pPPO);//PPO��ʼ�����������ȱ�����


void MiaoToolPPOptimParaInitial//PPO�ڴ��ʼ������
(
	MiaoToolPPOptim_t *pPPO,//thisָ��
	ParaType ClipE,
	ParaType tau,
	ParaType gamma,
	ParaType beta,
	MiaoToolRandomBasic_t *prand,//�����������
	MiaoMemoryBasic_t *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
	
);


void MiaoToolPPOptimInterfaceInitial
(
	MiaoToolPPOptim_t *pPPO,//thisָ��
	lw_u16 state_dim, lw_u16 action_dim,
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor,//״̬��һ������(��ΪNULL)
	ParaType ActionMax, ParaType ActionMin
);

void MiaoToolPPOptimStoreMemInitial//PPO�ڴ�����ʼ��
(
	MiaoToolPPOptim_t *pPPO,//thisָ��
	lw_u16 MemPoolLength//���䳤��
);


void MiaoToolPPOptimAppendInitial//PPO������׷��,һ�λ�׷������������
(
	MiaoToolPPOptim_t *pPPO,//thisָ��
	MiaoToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	MiaoToolNNLayerActFun_t **AFlist,//�����
	ParaType *nl_list//ѧϰ����
);


//------------------------------------------------------
//</PPO>
//------------------------------------------------------


#endif // !(MIAO_SIMPLE_LEVEL >= 2)

#endif
