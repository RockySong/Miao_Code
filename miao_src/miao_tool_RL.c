/*(Ver.=0.96)
 * MIAO_tool.c
 *
 *  Created on: 2017-11-27
 *      Author: slw
 */

#include "OSLW_include.h"
#if !(MIAO_SIMPLE_LEVEL >= 2)

#if MIAO_TOOL_IMPORT_RL || MIAO_TOOL_IMPORT_ALL
void MiaoToolRL_Termial(
	void *pRL,
	lw_u8 TermialJudgeFlag,
	ParaType RewardSumMax,
	ParaType RewardSumMin,
	ParaType *pStatusTermial,
	ParaType Threshold,//״̬�յ���ֵ ����㵱ǰ״̬��״̬�յ��ŷʽ����)
	uint32_t RoundMax
)
{
	MiaoToolRLBasic_t *_rl;
	MIAO_assert(!(pRL));

	_rl = pRL;

	_rl->isTermialFun = MiaoToolRL_isTermial;
	_rl->TermialJudgeFlag.all = TermialJudgeFlag;
	_rl->RewardSumMax = RewardSumMax;
	_rl->RewardSumMin = RewardSumMin;
	_rl->TermialJudgeFlag.bits.IsTermialFlag = 0;

	_rl->StatusTermialJudgeThreshold = Threshold;

	_rl->RoundMax = RoundMax;
	if (pStatusTermial)
	{
		MiaoToolMatrixInitial(&(_rl->StateTermial), 1, _rl->StateNow.col, pStatusTermial);
	}

}

lw_8 MiaoToolRL_isTermial(void *pRL)
{
	MiaoToolRLBasic_t *_rl;
	ParaType cmpres;
	MIAO_assert(!(pRL));

	_rl = pRL;
	if (_rl->TermialJudgeFlag.bits.IsTermialFlag)
	{
		return 1;
	}

	if (_rl->TermialJudgeFlag.bits.StatusNear)
	{
		cmpres = pMiaoToolMatrixLossSquare(NULL, &(_rl->StateNow), &(_rl->StateTermial));
		if (cmpres <= _rl->StatusTermialJudgeThreshold)
		{
			return 1;
		}
	}

	if (_rl->TermialJudgeFlag.bits.RoundOver)
	{
		if (_rl->RoundCount > _rl->RoundMax)
		{
			return 1;
		}
	}

	if (_rl->TermialJudgeFlag.bits.RewardUpOver)
	{
		if (_rl->RewardSum > _rl->RewardSumMax)
		{
			return 1;
		}
	}

	if (_rl->TermialJudgeFlag.bits.RewardDownOver)
	{
		if (_rl->RewardSum<_rl->RewardSumMin)
		{
			return 1;
		}
	}


	return 0;

}


static void MiaoToolRL_Born(MiaoToolRLBasic_t *rl)
{
	rl->RewardSum = _ParaFrom(0);
	rl->RoundCount = 0;
	rl->BornFunByUser(rl);
}



#if MIAO_TOOL_IMPORT_ALL || MIAO_TOOL_IMPORT_RL_Q_LEARNING


void MiaoToolQLearningParaInitial
(
    MiaoToolQLearning_t *pTQL,
    ParaType Ep,
    ParaType Lr,
    ParaType Gamma,
    MiaoToolRandomBasic_t *prand,
    MiaoMemoryBasic_t *pmem,
    pActEnvFunType pActfun,
    pBornFunType pBornfun
)
{

    MIAO_assert(!(pTQL));
    MIAO_assert(!(prand));
    MIAO_assert(!(pActfun));
    MIAO_assert(!(pBornfun));

    memset(pTQL,0,sizeof(MiaoToolQLearning_t));
    pTQL->basic.Ep=Ep;
    pTQL->Lr=Lr;
    pTQL->basic.basic.Gamma=Gamma;
    pTQL->basic.basic.pRand=prand;
    pTQL->basic.basic.ActEnvFun=(void *)pActfun;
    pTQL->basic.basic.BornFun= (void *)MiaoToolRL_Born;
	pTQL->basic.basic.BornFunByUser = (void *)pBornfun;

    pTQL->basic.basic.pmem = pmem;

    pTQL->basic.basic.LearnFun = (void *)MiaoToolQLearningLearnFun;
    pTQL->basic.basic.ChooseFun = (void *)MiaoToolQLearningChooseFun;
    pTQL->basic.basic.StateStoreFun = (void *)MiaoToolQLearningLearnStateStore;
    pTQL->basic.basic.isTermialFun = (void *)MiaoToolRL_isTermial;

}


void MiaoToolQLearningTableInitial
(
    MiaoToolQLearning_t *pTQL,
    lw_u16 state_row, lw_u16 state_col,
    lw_u16 action_row, lw_u16 action_col,
    void *pState,
    void *pAction,
    void *pTable,
    void *pStateNow,
    void *pStateTermial,
    lw_u16 *pactchoose
)

{
    MIAO_assert(!(pTQL));

    if (pTQL->basic.basic.pmem)
    {
        if (!pAction)
        {
            pAction = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(action_row*action_col));
        }

        if (!pState)
        {
            pState = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(state_row*state_col));
        }

        if (!pTable)
        {
            pTable = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(state_row*action_row));
        }

        if (!pStateNow)
        {
            pStateNow = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, (state_col * 1) * sizeof(lw_u16));
        }

        if (!pactchoose)
        {
            pactchoose = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, (action_col * 1) * sizeof(lw_u16));
        }


    }
    else;

    MiaoToolMatrixInitial(&(pTQL->basic.DataTable.ActionTable), action_row, action_col,pAction);
    MiaoToolMatrixInitial(&(pTQL->basic.DataTable.StateTable), state_row, state_col,pState);
    MiaoToolMatrixInitial(&(pTQL->basic.DataTable.Qtable), state_row, action_row,pTable);
    MiaoToolMatrixInitial(&(pTQL->basic.basic.StateNow), 1, state_col, pStateNow);
    MiaoToolMatrixInitial(&(pTQL->basic.basic.StateTermial), 1, state_col, pStateTermial);


    pMiaoToolMatrixSet(&(pTQL->basic.DataTable.Qtable), _ParaFrom(0),NULL);

    pTQL->basic.pActChoose = pactchoose;
}


MiaoToolQLearning_t* MiaoToolQLearningLearnStateStore(MiaoToolQLearning_t *pQRL,MiaoMat *state)
{

    MiaoToolQRLData_t *pTQRLDS;
    MiaoToolQRLBasic_t *pQRLB;
    ParaType *ps, *pq;

    MIAO_assert(!(pQRL));

    pQRLB = &(pQRL->basic);
    pTQRLDS = &(pQRL->basic.DataTable);

    if (pQRLB->basic.pmem == NULL)//�޷��ڴ����
    {
        MIAO_assert(1);
        return NULL;
    }
    if (state==NULL)
    {
        state = &(pQRLB->basic.StateNow);
    }

    if (state->col != pTQRLDS->StateTable.col)
    {
        MIAO_assert(1);
        return NULL;
    }


    ps = pQRLB->basic.pmem->ReAlloc(pQRLB->basic.pmem, pTQRLDS->StateTable.a,
                              PARA_MEM_CAL(pTQRLDS->StateTable.length + state->col)//����ԭ���Ĵ�С+1��
                             );

    pq = pQRLB->basic.pmem->ReAlloc(pQRLB->basic.pmem, pTQRLDS->Qtable.a,
                              PARA_MEM_CAL(pTQRLDS->Qtable.length + pTQRLDS->Qtable.col)//����ԭ���Ĵ�С+1��
                             );

    memcpy(ps + pTQRLDS->StateTable.length, state->a, PARA_MEM_CAL(state->col));
    memset(pq + pTQRLDS->Qtable.length, 0, PARA_MEM_CAL(pTQRLDS->Qtable.col));

    MiaoToolMatrixInitial(&(pTQRLDS->StateTable), pTQRLDS->StateTable.row + 1, pTQRLDS->StateTable.col, ps);//���ô�С
    MiaoToolMatrixInitial(&(pTQRLDS->Qtable), pTQRLDS->Qtable.row + 1, pTQRLDS->Qtable.col, pq);//���ô�С

    return pQRL;
}



MiaoToolQLearning_t* MiaoToolQLearningLearnFun(MiaoToolQLearning_t *pQRL)
{

    MiaoToolQRLData_t *pTQRLDS;
    MiaoToolQRLBasic_t *pQRLB;
	MiaoToolRLBasic_t *rl;
    ParaType *p,dmax;
    lw_u16 maxindex, row;
    MiaoMat mat;

    MIAO_assert(!(pQRL));

    pQRLB=&(pQRL->basic);
    pTQRLDS= &(pQRL->basic.DataTable);

	MIAO_RL_UPDATE(pQRL);

    /*(Ver.=0.96)
    mat.length = pTQRLDS->state.length;
    mat.a = pTQRLDS->state.a;

    pMiaoToolMatrixIndex(&mat, pQRLB->StateNow, &(pQRLB->StateNowIndex), &col);

    mat.length=pTQRLDS->action.length;
    mat.a=pTQRLDS->table.a+pTQRLDS->table.col*pQRLB->StateNowIndex;
    pMiaoToolMatrixMaxMin(&(mat),&dmax,&maxindex,NULL,NULL);
    */

    if (pMiaoToolMatrixFind(&(pTQRLDS->StateTable), &(pQRLB->basic.StateNow), 0, 0, &row) == MiaoMartixRes_NoFind)//û���ҵ�˵��Ҫ׷��״̬
    {

        row = pTQRLDS->StateTable.row;//�������һ��
        pQRLB->basic.StateStoreFun((MiaoToolRLBasic_t *)pQRL, NULL);
    }

    pQRLB->StateNowIndex = row;

    mat.length = pTQRLDS->Qtable.col;
    mat.a = pTQRLDS->Qtable.a + pTQRLDS->Qtable.col*row;

    pMiaoToolMatrixMaxMin(&(mat), &dmax, &maxindex, NULL, NULL);


    p = &(MIAO_TOOL_M_GET(pTQRLDS->Qtable,pQRLB->StateLastIndex,pQRLB->ActNowIndex));
    if(!(pQRLB->basic.isTermialFun((MiaoToolRLBasic_t *)pQRLB)))//�ж��Ƿ����յ�
        pQRL->Q_Now=_ParaAdd(pQRLB->basic.RewardNow,_ParaMul(pQRLB->basic.Gamma,dmax));
    else
        pQRL->Q_Now=pQRLB->basic.RewardNow;


    *p=_ParaAdd((*p),_ParaMul(pQRL->Lr,_ParaSub(pQRL->Q_Now,(*p))));

    //������һ�־���ֵ
    //pQRLB->StateLast = pQRLB->StateNow;
    pQRLB->StateLastIndex = pQRLB->StateNowIndex;
    //pQRLB->ActLast = pQRLB->ActNow;
    pQRLB->ActLastIndex = pQRLB->ActNowIndex;

    return pQRL;
}

#endif // MIAO_TOOL_IMPORT_ALL || MIAO_TOOL_IMPORT_RL_Q_LEARNING



#if MIAO_TOOL_IMPORT_ALL || MIAO_TOOL_IMPORT_RL_SARSA || MIAO_TOOL_IMPORT_RL_Q_LEARNING

MiaoToolQLearning_t* MiaoToolQLearningChooseFun(MiaoToolQLearning_t *pQRLB)
{

    ParaType drand, dmax;
    lw_u16 selrand, maxindex, state_index, row;
    MiaoToolQRLData_t *pTQRLDS;
	MiaoToolRLBasic_t *pRL;
    MiaoMat mat;

    MIAO_assert(!(pQRLB));

	pRL = &(pQRLB->basic.basic);
    drand = pRL->pRand->rand(pRL->pRand, _ParaFint(0), _ParaFint(1));
    pTQRLDS = &(pQRLB->basic.DataTable);


    //����state ��ѯstate��ID
    /*(Ver.=0.96)
    mat.col = pTQRLDS->action.col;
    mat.row = 1;
    mat.length = pTQRLDS->action.length;
    col = 1;
    pMiaoToolMatrixIndex(&(pTQRLDS->state), pQRLB->StateNow, &(pQRLB->StateNowIndex), &col);
    mat.a = pTQRLDS->table.a + pTQRLDS->table.col*pQRLB->StateNowIndex;
    */

    if (pMiaoToolMatrixFind(&(pTQRLDS->StateTable), &(pRL->StateNow), 0, 0, &row) == MiaoMartixRes_NoFind)//û���ҵ�˵��Ҫ׷��״̬
    {

        row = pTQRLDS->StateTable.row;//�������һ��
		pRL->StateStoreFun(pRL, NULL);
    }

    pQRLB->basic.StateNowIndex = row;

    mat.length = pTQRLDS->Qtable.col;
    mat.a = pTQRLDS->Qtable.a + pTQRLDS->Qtable.col*row;


    if (drand < pQRLB->basic.Ep)
    {

        //��ѯQ������Ϊ
        pMiaoToolMatrixMaxMin(&mat, &dmax, &maxindex, NULL, NULL);
        row = mat.length;
        //�õ�Q��������
        pMiaoToolMatrixIndex(&mat, dmax, pQRLB->basic.pActChoose, &row);
        //���ѡȡһ��Q������Ϊ
		selrand = pRL->pRand->randint(pRL->pRand, 0, row);
        pQRLB->basic.ActNowIndex = pQRLB->basic.pActChoose[selrand];


    }
    else
    {
        //ֱ�����ѡȡ��Ϊ
		selrand = pRL->pRand->randint(pRL->pRand, 0, pTQRLDS->Qtable.col);
        pQRLB->basic.ActNowIndex = selrand;

    }

    //������Ϊ��֮ǰ��״̬
    pRL->ActNow.a = pTQRLDS->ActionTable.a + pQRLB->basic.ActNowIndex*pTQRLDS->ActionTable.col;


    return pQRLB;
}
#endif // MIAO_TOOL_IMPORT_ALL || MIAO_TOOL_IMPORT_RL_SARSA || MIAO_TOOL_IMPORT_RL_Q_LEARNING


#if MIAO_TOOL_IMPORT_ALL || MIAO_TOOL_IMPORT_RL_SARSA

void MiaoToolSarsaLamberParaInitial
(
    MiaoToolSarsaLamber_t *pTQL,
    ParaType Ep,
    ParaType Lr,
    ParaType Gamma,
    MiaoToolRandomBasic_t *prand,
    MiaoMemoryBasic_t *pmem,
    ParaType lamber,
    pActEnvFunType pActfun,
    pBornFunType pBornfun
)
{
    MIAO_assert(!(pTQL));
    MIAO_assert(!(prand));
    MIAO_assert(!(pActfun));

    memset(pTQL, 0, sizeof(MiaoToolQLearning_t));
    pTQL->Ep = Ep;
    pTQL->Lr = Lr;
    pTQL->basic.basic.Gamma = Gamma;
    pTQL->basic.basic.pRand = prand;
    pTQL->basic.basic.pmem = pmem;
    pTQL->basic.basic.ActEnvFun = (void *)pActfun;
	pTQL->basic.basic.BornFun = (void *)MiaoToolRL_Born;
    pTQL->basic.basic.BornFunByUser = (void *)pBornfun;
    pTQL->Lamber = lamber;

    pTQL->basic.basic.LearnFun = (void *)MiaoToolSarsaLamberLearnFun;

    pTQL->basic.basic.ChooseFun = (void *)MiaoToolQLearningChooseFun;
    pTQL->basic.basic.StateStoreFun = (void *)MiaoToolQLearningLearnStateStore;
    pTQL->basic.basic.isTermialFun = (void *)MiaoToolRL_isTermial;

}

void MiaoToolSarsaLamberTableInitial
(
    MiaoToolSarsaLamber_t *pTQL,
    lw_u16 state_row, lw_u16 state_col,
    lw_u16 action_row, lw_u16 action_col,
    void *pState,
    void *pAction,
    void *pTable,
    void *pStateNow,
    void *pStateTermial,
    lw_u16 *pactchoose,
    void *pTrace
)

{
    MIAO_assert(!(pTQL));
    MiaoToolQLearningTableInitial
    (
        (MiaoToolQLearning_t *)pTQL,
        state_row, state_col,
        action_row, action_col,
        pState,
        pAction,
        pTable,
        pStateNow,
        pStateTermial,
        pactchoose
    );
    if (pTQL->basic.basic.pmem)
    {
        if (!pTrace)
        {
            pTrace = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(state_row*action_row));
        }
    }
    else;


    MiaoToolMatrixInitial(&(pTQL->Trace), state_row, action_row, pTrace);


    return;
}

MiaoToolSarsaLamber_t* MiaoToolSarsaLamberLearnFun(MiaoToolSarsaLamber_t *pTSL)
{
    MiaoToolQRLData_t *pTQRLDS;
    MiaoToolQRLBasic_t *pQRLB;
	MiaoToolRLBasic_t *pRL;
    ParaType pre, err, buf1, buf2, *p, *q;
    lw_u16 row;
    uint32_t num, i;
    MiaoMat mat;


    MIAO_assert(!(pTSL));

    pQRLB = &(pTSL->basic);
	pRL = &(pTSL->basic.basic);
    pTQRLDS = &(pTSL->basic.DataTable);
	MIAO_RL_UPDATE(pTSL);
    /*(Ver.=0.96)
        mat.length = pTQRLDS->state.length;
        mat.a = pTQRLDS->state.a;
        pMiaoToolMatrixIndex(&mat, pQRLB->StateNow, &(pQRLB->StateNowIndex), &col);

        pre = (MIAO_TOOL_M_GET(pTQRLDS->Qtable, pQRLB->StateLastIndex, pQRLB->ActLastIndex));
    	*/

    if (pMiaoToolMatrixFind(&(pTQRLDS->StateTable), &(pRL->StateNow), 0, 0, &row) == MiaoMartixRes_NoFind)//û���ҵ�˵��Ҫ׷��״̬
    {

        row = pTQRLDS->StateTable.row;//�������һ��
				pRL->StateStoreFun((MiaoToolRLBasic_t *)pTSL, NULL);
    }

    pQRLB->StateNowIndex = row;

    pre = (MIAO_TOOL_M_GET(pTQRLDS->Qtable, pQRLB->StateLastIndex, pQRLB->ActLastIndex));

    if (!(pRL->isTermialFun((MiaoToolRLBasic_t *)pQRLB)))
        pTSL->Q_Now = _ParaAdd(pRL->RewardNow, _ParaMul(pRL->Gamma, MIAO_TOOL_M_GET(pTQRLDS->Qtable, pQRLB->StateNowIndex, pQRLB->ActNowIndex)));
    else
		pTSL->Q_Now = pRL->RewardNow;

    err = _ParaSub(pTSL->Q_Now, pre);


    mat.length = pTQRLDS->Qtable.col;
    mat.a= &(MIAO_TOOL_M_GET(pTSL->Trace, pQRLB->StateLastIndex, 0));

    pMiaoToolMatrixSet(&(mat), _ParaFrom(0),NULL);
    MIAO_TOOL_M_GET(pTSL->Trace, pQRLB->StateLastIndex, pQRLB->ActLastIndex) = _ParaFrom(1);

    buf1 = _ParaMul(pTSL->Lr, err);
    buf2 = _ParaMul(pTSL->Lamber, pRL->Gamma);

    //Ϊ�˽�Լ�ռ�û��ʹ�þ�������
    num = pTQRLDS->Qtable.length;
    p = pTQRLDS->Qtable.a;
    q = pTSL->Trace.a;

    for ( i = 0; i < num; i++)
    {
        p[i] = _ParaAdd(p[i], _ParaMul(buf1, q[i]));
        q[i] = _ParaMul(q[i], buf2);
    }

    //������һ�־���ֵ
    pQRLB->ActLastIndex = pQRLB->ActNowIndex;
    pQRLB->StateLastIndex = pQRLB->StateNowIndex;

    return pTSL;

}

#endif // MIAO_TOOL_IMPORT_ALL || MIAO_TOOL_IMPORT_RL_SARSA


#if MIAO_TOOL_IMPORT_ALL || (MIAO_TOOL_IMPORT_RL_EXP_RE)

MiaoToolDQNetExpReplay_t *MiaoToolDQNetExpReplayAppend(MiaoToolDQNetExpReplay_t *pExpRe, lw_u16 Num, lw_u8 method)
{
	void *q;
	uint32_t len;
	ParaType ActIndex;
	MiaoToolDQNetExpReplayFrame_t *pexpref;


	MIAO_assert(!(pExpRe));

	/*(Ver.=0.96)
	//�ɰ汾 û�в���table
	pbase = ((lw_u8 *)(pExpRe->MemPool.pData) + Num*MIAO_TOOL_DQN_EXP_FRAME_SIZE(*pExpRe));
	//state last ����
	p = MIAO_TOOL_DQN_EXP_STATE_LAST(*pExpRe, pbase);
	q = (lw_u8 *)pExpRe->pTQL->StateLast.a;
	len = MIAO_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	MIAO_TOOL_MEMCPY(p, q, len);

	//������������(��ParaType����)
	ActIndex = _ParaFint(pExpRe->pTQL->ActNowIndex);
	q = (lw_u8 *)(&ActIndex);
	len = MIAO_TOOL_DQN_EXP_REWARD_SIZE(*pExpRe);
	MIAO_TOOL_MEMCPY(p, q, len);

	//����Reward
	q = (lw_u8 *)(&(pExpRe->pTQL->RewardNow));
	len = MIAO_TOOL_DQN_EXP_ACTION_SIZE(*pExpRe);
	MIAO_TOOL_MEMCPY(p, q, len);

	//����state now
	q = (lw_u8 *)pExpRe->pTQL->StateNow.a;
	len = MIAO_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	MIAO_TOOL_MEMCPY(p, q, len);
	*/

	//state last ����
	len = MIAO_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	q = pExpRe->pTQL->basic.StateLast.a;
	if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 1, q, len) != 0)
	{
		MIAO_assert(1);
	}

	//������������(��ParaType����)
	if (method)//�������0 ���� ֱ�ӱ���
	{
		q = pExpRe->pTQL->basic.ActNow.a;
		len = MIAO_TOOL_DQN_EXP_ACTION_SIZE(*pExpRe);
		if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 2, q, len) != 0)
		{
			MIAO_assert(1);
		}

	}
	else//�����0 ���� ��������
	{
		ActIndex = _ParaFint(pExpRe->pTQL->ActNowIndex);
		q = (&ActIndex);
		len = MIAO_TOOL_DQN_EXP_ACTION_SIZE(*pExpRe);
		if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 2, q, len) != 0)
		{
			MIAO_assert(1);
		}
	}


	//����Reward
	q = (&(pExpRe->pTQL->basic.RewardNow));
	len = MIAO_TOOL_DQN_EXP_REWARD_SIZE(*pExpRe);
	if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 3, q, len) != 0)
	{
		MIAO_assert(1);
	}
	//����state now
	q = pExpRe->pTQL->basic.StateNow.a;
	len = MIAO_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 4, q, len) != 0)
	{
		MIAO_assert(1);
	}

	if (pExpRe->PriorType == MiaoToolDQNetExpReplay_PER)//��������Ż�
	{
		pexpref = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), Num, 0, NULL);

		_MiaoToolDQNetExpReplayDelete(pExpRe, pexpref);

		pexpref->Importance = pExpRe->Rmax;

		_MiaoToolDQNetExpReplayInsert(pExpRe, pexpref);

	}



	/*(Ver.=0.96)
	//�ɰ汾 û�в���table
	pbase = ((lw_u8 *)(pExpRe->MemPool.pData) + (Num + 1)*MIAO_TOOL_DQN_EXP_FRAME_SIZE(*pExpRe));

	if (p != pbase)
	{
	MIAO_assert(1);
	}
	*/

	return pExpRe;

}


MiaoToolDQNetExpReplay_t *MiaoToolDQNetExpReplayMinMax(MiaoToolDQNetExpReplay_t *pExpRe)
{
	lw_32 num, i;
	ParaType loop_max;
	MiaoToolDListNodeConnect_t *pcon;
	MiaoToolDQNetExpReplayFrame_t *p;

	pcon = pExpRe->DivResult.pData;
	num = pExpRe->DivResult.uData;



	i = num;
	while (i<0 && !(pcon[i].pLast))
	{
		i--;
	}

	if (i < 0)
	{
		pExpRe->Min = pExpRe->Rmax;
	}
	else
	{
		p = (MiaoToolDQNetExpReplayFrame_t *)(pcon[i].pLast);
		pExpRe->Min = p->Importance;
	}

	i = 0;
	while (i< num && !(pcon[i].pNext))
	{
		i++;
	}

	if (i == num)
	{
		loop_max = pExpRe->Rmin;
	}
	else
	{
		p = (MiaoToolDQNetExpReplayFrame_t *)(pcon[i].pNext);
		loop_max = p->Importance;
	}

	pExpRe->Max = loop_max;

	return pExpRe;
}

MiaoToolDQNetExpReplay_t *_MiaoToolDQNetExpReplayInsert(MiaoToolDQNetExpReplay_t *pExpRe, MiaoToolDQNetExpReplayFrame_t *pExpReF)
{

	lw_32 hash_index;
	MiaoToolDListNodeConnect_t *pcon;
	MiaoToolDQNetExpReplayFrame_t *pLast, *pNext;

	MIAO_assert(!(pExpRe));
	MIAO_assert(!(pExpReF));


	//����1 ��Ҫ�Զ�λ����ϣֵ���㣩
	hash_index = _ParaInt(_ParaCeil(_ParaDiv(_ParaSub(pExpReF->Importance, pExpRe->Rmin), pExpRe->Div)));

	hash_index = hash_index > pExpRe->DivResult.uData ? pExpRe->DivResult.uData : hash_index;
	hash_index = hash_index < 0 ? 0 : hash_index;

	hash_index = pExpRe->DivResult.uData - hash_index;//��������

	pcon = pExpRe->DivResult.pData;

	//����2 ��ѯ����
	pcon += hash_index;
	pLast = (MiaoToolDQNetExpReplayFrame_t *)pcon;
	pNext = (MiaoToolDQNetExpReplayFrame_t *)pcon->pNext;
	while (pNext)
	{
		if (pNext->Importance <= pExpReF->Importance)//�ȸü���֡С
		{
			break;
		}
		pLast = (MiaoToolDQNetExpReplayFrame_t *)pLast->con.pNext;
		pNext = (MiaoToolDQNetExpReplayFrame_t *)pNext->con.pNext;
	}

	if (pNext == NULL)//������β��
	{
		MIAO_TOOL_DLIST_NODE_CONNECT(pLast, pExpReF);//����
		pcon->pLast = (MiaoToolDListNodeConnect_t *)pExpReF;
	}
	else
	{
		MIAO_TOOL_DLIST_NODE_INSERT(pLast, pExpReF);//����
	}

	pExpReF->pcon = pcon;

	//����3 ͳ����ֵ����

	pExpRe->Sum = _ParaAdd(pExpRe->Sum, pExpReF->Importance);

	/*(Ver.=0.96)
	if (pExpRe->pMax == NULL)
	{
	pExpRe->pMax = pExpReF;
	}
	else if (pExpRe->pMax->Importance <= pExpReF->Importance)
	{
	pExpRe->pMax = pExpReF;
	}
	else;

	if (pExpRe->pMin == NULL)
	{
	pExpRe->pMin = pExpReF;
	}
	else if (pExpRe->pMin->Importance > pExpReF->Importance)
	{
	pExpRe->pMin = pExpReF;
	}
	else;
	*/
	return pExpRe;
}



MiaoToolDQNetExpReplay_t *_MiaoToolDQNetExpReplayDelete(MiaoToolDQNetExpReplay_t *pExpRe, MiaoToolDQNetExpReplayFrame_t *pExpReF)
{
	lw_u8 _f_status;
//	lw_32 i, num;
	//MiaoToolDListNodeConnect_t *pcon;

	//pcon = pExpRe->DivResult.pData;
	//��ѯɾ��
	if (pExpReF->con.pLast == NULL && pExpReF->con.pNext == NULL)
	{
		return NULL;
	}
	_f_status = (((size_t)(pExpReF->pcon->pLast) == (size_t)pExpReF) << 1) | ((size_t)(pExpReF->pcon->pNext) == (size_t)pExpReF);
	switch (_f_status)
	{

	case 1:
	case 0:
		MIAO_TOOL_DLIST_NODE_DELECT(pExpReF);
		break;

	case 2://��β�� ����Ψһ
		pExpReF->pcon->pLast = pExpReF->con.pLast;
		pExpReF->con.pLast->pNext = NULL;
		break;

	case 3://Ψһ
		pExpReF->pcon->pLast = NULL;
		pExpReF->pcon->pNext = NULL;

		break;
	default:
		MIAO_assert(1);
		break;
	}

	//�⹳
	pExpReF->con.pLast = NULL;
	pExpReF->con.pNext = NULL;
	pExpReF->pcon = NULL;


	//ͳ����ֵ����

	pExpRe->Sum = _ParaSub(pExpRe->Sum, pExpReF->Importance);

	/*(Ver.=0.96)
	num = pExpRe->DivResult.uData;

	if (pExpRe->pMax ==pExpReF)
	{
	i = 0;
	while (i< num && !(pcon[i].pNext))
	{
	i++;
	}

	if (i == num)
	{
	pExpRe->pMax = NULL;
	}
	else
	{
	pExpRe->pMax = pcon[i].pNext;
	}

	}

	if (pExpRe->pMin == pExpReF)
	{
	i = num;
	while (i && !(pcon[i].pLast))
	{
	i--;
	}

	if (i == num)
	{
	pExpRe->pMax = NULL;
	}
	else
	{
	pExpRe->pMax = pcon[i].pNext;
	}

	}
	*/

	return pExpRe;
}



MiaoToolDQNetExpReplay_t *MiaoToolDQNetExpReplayUpdate(MiaoToolDQNetExpReplay_t *pExpRe, lw_u16 num)
{
	MiaoToolDQNetExpReplayFrame_t **ppsample;

	if (pExpRe->PriorType == MiaoToolDQNetExpReplay_PER)
	{
		ppsample = (MiaoToolDQNetExpReplayFrame_t **)(pExpRe->SampleResult.pData);
		while (num--)
		{
			_MiaoToolDQNetExpReplayDelete(pExpRe, *ppsample);

			(*ppsample)->Importance = (*ppsample)->_new_Importance;

			_MiaoToolDQNetExpReplayInsert(pExpRe, *ppsample);
			ppsample++;
		}

	}

	return pExpRe;
}



lw_u16 MiaoToolDQNetExpReplaySample(MiaoToolDQNetExpReplay_t *pExpRe)
{
	void **pp;
	lw_u16 len_div;
	lw_u16 n;
	lw_u16 drand = 0;
	lw_u16 num = 0;
	ParaType para_div, sample_th, now_Sum, now_min, loop_max, loop_min;
	MiaoToolDListNodeConnect_t *pcon;
	MiaoToolDQNetExpReplayFrame_t *pexpref;
	MiaoToolRLBasic_t *pRL;
	lw_u8 _loop_status;

	MIAO_assert(!(pExpRe));

	pcon = pExpRe->DivResult.pData;
	pp = (void **)pExpRe->SampleResult.pData;
	pRL = &(pExpRe->pTQL->basic);
	if (pExpRe->PriorType == MiaoToolDQNetExpReplay_PER)
	{
		para_div = _ParaDiv(pExpRe->Sum, _ParaFint(pExpRe->SampleResult.uData));
		now_min = pExpRe->Rmin;
		sample_th = pRL->pRand->rand(pRL->pRand, now_min, _ParaAdd(para_div, now_min));
		now_Sum = _ParaFrom(0);
		_loop_status = 1;
		loop_max = _ParaSub(pExpRe->Rmin, pExpRe->Epsi);
		loop_min = _ParaAdd(pExpRe->Rmax, pExpRe->Epsi);

		for (n = 0; n < pExpRe->DivResult.uData; n++)//ѭ����ϣ���ɢ��
		{
			pexpref = (MiaoToolDQNetExpReplayFrame_t *)(pcon[n].pNext);
			while (pexpref)//����ѭ��
			{
				//ѭ������
				now_Sum = _ParaAdd(now_Sum, pexpref->Importance);
				/*(Ver.=0.96)
				if (now_Sum >= sample_th && num < pExpRe->SampleResult.uData)
				{
				now_min = _ParaAdd(para_div, now_min);
				sample_th = pExpRe->pTQL->pRand->rand(pExpRe->pTQL->pRand, para_div + now_min, now_min);
				*pp++ = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), drand, 0, NULL);
				num++;
				}
				*/
				switch (_loop_status)
				{
				case 2:
					if (now_Sum >= _ParaAdd(now_min, para_div))
					{
						//��ǰ�ĺ�>=��һ��
						//˵���ո�Խ��
						now_min = _ParaAdd(para_div, now_min);
						sample_th = pRL->pRand->rand(pRL->pRand, now_min, _ParaAdd(para_div, now_min));
						_loop_status = 1;
					}
					else
					{
						break;
					}
				case 1:
					if (now_Sum >= sample_th)
					{
						*pp++ = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), drand, 0, NULL);
						num++;
						_loop_status = 2;
					}
					break;

				default:
					MIAO_assert(1);
					break;
				}

				//˳��������ֵ
				if (loop_max < pexpref->Importance)
				{
					loop_max = pexpref->Importance;
				}

				if (loop_min > pexpref->Importance)
				{
					loop_min = pexpref->Importance;
				}
				drand++;
				pexpref = (MiaoToolDQNetExpReplayFrame_t *)(pexpref->con.pNext);

			}//end while

		}//endfor
		pExpRe->Min = loop_min;
		pExpRe->Max = loop_max;

	}// end pExpRe->PriorType == MiaoToolDQNetExpReplay_PER
	else
	{
		len_div = pExpRe->MemTable.Row.uData / pExpRe->SampleResult.uData;
		for (n = 0; n < pExpRe->MemTable.Row.uData; n += len_div)
		{
			drand = pRL->pRand->randint(pRL->pRand, n, n + len_div);
			*pp++ = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), drand, 0, NULL);
			num++;
		}
	}

	return num;


}
#endif // MIAO_TOOL_IMPORT_ALL || (MIAO_TOOL_IMPORT_RL_EXP_RE)


#if MIAO_TOOL_IMPORT_ALL || (MIAO_TOOL_IMPORT_RL_DQN && MIAO_TOOL_IMPORT_NN && MIAO_TOOL_IMPORT_NN_BPnn && MIAO_TOOL_IMPORT_RL_EXP_RE)

void MiaoToolDQNetInitial(MiaoToolDQNet_t *pDQN)
{
    MIAO_assert(!(pDQN));

    memset(pDQN, 0, sizeof(MiaoToolDQNet_t));

	MiaoToolBPnnInit(&(pDQN->MainNet),1);
	MiaoToolBPnnInit(&(pDQN->TargetNet),1);
	pDQN->TargetNet.Train.Flag.NeedTrain = 1;

    pDQN->StoreMemFun = (void *)MiaoToolDQNetStoreMem;
    pDQN->basic.basic.ChooseFun = (void *)MiaoToolDQNetChoose;
    pDQN->basic.basic.LearnFun = (void *)MiaoToolDQNetLearning;

    pDQN->ExpReplay.AppendFun = (void *)MiaoToolDQNetExpReplayAppend;
    pDQN->ExpReplay.SampleFun = (void *)MiaoToolDQNetExpReplaySample;
    pDQN->ExpReplay.UpdateFun = (void *)MiaoToolDQNetExpReplayUpdate;
    pDQN->ExpReplay.MinMaxFun = (void *)MiaoToolDQNetExpReplayMinMax;
}


void MiaoToolDQNetParaInitial//DQN�ڴ��ʼ������
(
    MiaoToolDQNet_t *pDQN,//thisָ��
    ParaType Ep,//��ֵ
    ParaType Gamma,//˥������
    MiaoToolDQNetOptimNum Optim,
    MiaoToolRandomBasic_t *prand,//�����������pQRL->Ep
    MiaoMemoryBasic_t *pMem,//�ڴ�ָ��
    pActEnvFunType pActfun,//�������к���ָ��
    pBornFunType pBornfun//������ʼ������ָ��
)
{
    MIAO_assert(!(pDQN));
    MIAO_assert(!(prand));
	MIAO_assert(!(pMem));
    MIAO_assert(!(pActfun));
    MIAO_assert(!(pBornfun));

    pDQN->basic.Ep = Ep;
    pDQN->basic.basic.Gamma = Gamma;
    pDQN->Optim = Optim;
    pDQN->basic.basic.pRand = prand;
    pDQN->basic.basic.pmem = pMem;
    pDQN->basic.basic.ActEnvFun = pActfun;
	pDQN->basic.basic.BornFun = MiaoToolRL_Born;
    pDQN->basic.basic.BornFunByUser = pBornfun;

    pDQN->basic.basic.isTermialFun = MiaoToolRL_isTermial;

}

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
)
{
    MIAO_assert(!(pDQN));

    if (pDQN->basic.basic.pmem)
    {
        if (!pAction)
        {
            pAction = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(action_row*action_col));
        }

        if (!pStateNow)
        {
            pStateNow = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));
        }

        if (!pStateLast)
        {
            pStateLast = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));
        }

        if (!pStateTermial)
        {
            pStateTermial = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));
        }

        if (!pStateFactor)
        {
            pStateFactor = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));//
            MiaoToolMatrixInitial(&(pDQN->StateFactor), 1, state_col, pStateFactor);//
            pMiaoToolMatrixSet(&(pDQN->StateFactor), _ParaFint(1), NULL);//��ʼ��Ϊ1
        }
    }

    MiaoToolMatrixInitial(&(pDQN->basic.basic.StateNow), 1, state_col, pStateNow);
    MiaoToolMatrixInitial(&(pDQN->basic.basic.StateLast), 1, state_col, pStateLast);
    MiaoToolMatrixInitial(&(pDQN->basic.basic.StateTermial), 1, state_col, pStateTermial);

    MiaoToolMatrixInitial(&(pDQN->basic.DataTable.ActionTable), action_row, action_col, pAction);

    MiaoToolMatrixInitial(&(pDQN->StateFactor), 1, state_col, pStateFactor);

    pDQN->basic.DataTable.StateTable.col = state_col;

}

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
)
{
    MiaoMemoryBasic_t *pmem;
    lw_u16 col_kind[] = { sizeof(MiaoToolDQNetExpReplayFrame_t),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType) };
    MIAO_assert(!(pDQN));
    MIAO_assert(!(MemPoolLength));
    MIAO_assert(!(batch_size));

    pDQN->ExpReplay.pTQL = (MiaoToolQRLBasic_t *)pDQN;
    pDQN->ExpReplay.PriorType = PER;
    pDQN->NetCopyThreshold = copy_th;
    //�ڴ����
    pmem = pDQN->basic.basic.pmem;

    //������װ
    pDQN->ExpReplay.DivResult.uData = batch_size;
    pDQN->ExpReplay.SampleResult.uData = batch_size;
    //pDQN->ExpReplay.MemPool.uData = MemPoolLength;

    pDQN->ExpReplay.State_Col = pDQN->basic.DataTable.StateTable.col;
	pDQN->ExpReplay.Action_Col = pDQN->basic.DataTable.ActionTable.col;

    col_kind[1] *= pDQN->ExpReplay.State_Col;
    col_kind[4] *= pDQN->ExpReplay.State_Col;

	MiaoToolTableInitial(&(pDQN->ExpReplay.MemTable), MemPoolLength, col_kind, 5, pmem, NULL, NULL);

	pDQN->ExpReplay.SampleResult.pData = pmem->Malloc(pmem, 2 * pDQN->ExpReplay.SampleResult.uData * sizeof(void *));



    if (PER == MiaoToolDQNetExpReplay_PER)
    {
        //������װ

        pDQN->ExpReplay.Alpha = Alpha;
        pDQN->ExpReplay.Beta = Beta;
        pDQN->ExpReplay.Epsi = Epsi;

        Rmax = MIAO_TOOL_EXPRE_CAL_P(Rmax, pDQN->ExpReplay);
        Rmin = MIAO_TOOL_EXPRE_CAL_P(Rmin, pDQN->ExpReplay);

        pDQN->ExpReplay.Rmax = Rmax;
        pDQN->ExpReplay.Rmin = Rmin;

        pDQN->ExpReplay.Div = _ParaDiv(_ParaSub(Rmax, Rmin), _ParaFrom(batch_size));


        pDQN->ExpReplay.DivResult.pData = pmem->Malloc(pmem, pDQN->ExpReplay.DivResult.uData * sizeof(MiaoToolDListNodeConnect_t));

        memset(pDQN->ExpReplay.DivResult.pData, 0, pDQN->ExpReplay.DivResult.uData * sizeof(MiaoToolDListNodeConnect_t));


    }
    //pDQN->ExpReplay.MemPool.pData = pmem->Malloc(pmem, pDQN->ExpReplay.MemPool.uData*MIAO_TOOL_DQN_EXP_FRAME_SIZE(pDQN->ExpReplay));



}
void MiaoToolDQNetAppendInitial
(
    MiaoToolDQNet_t *pDQN,
    lw_u16 row,
    lw_u16 col,
	MiaoNNinitFunType pFun,
    MiaoToolRandomBasic_t *pTRB,
    ParaType initd1,ParaType initd2,
	MiaoToolNNLayerActFun_t *pTemplet
)
{
    MiaoMemoryBasic_t *pmem;
 

    MIAO_assert(!(pDQN));
	MIAO_assert(!(pFun));
    MIAO_assert(!(pTRB));

    pmem = (pDQN->basic.basic.pmem);

	MiaoToolBPnnFullConAppend(
		&(pDQN->MainNet),
		row, col,
		NULL, NULL,
		NULL, NULL,
		pFun, pTRB, initd1, initd2,
		pTemplet, pmem
	);

	MiaoToolBPnnFullConAppend(
		&(pDQN->TargetNet),
		row, col,
		NULL, NULL,
		NULL, NULL,
		NULL, NULL, initd1, initd2,
		pTemplet, pmem
	);


}


void MiaoToolDQNetTarinInitial(
    MiaoToolDQNet_t *pDQN,
    ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
)
{
    MIAO_assert(!(pDQN));
    MIAO_assert(!(loss));
	MiaoToolBPnnTrainInit
    (
        &(pDQN->MainNet),
        qref,
        pDQN->basic.basic.pmem,
		loss,
        nl
    );
}


void MiaoToolDQNetAllDataInit(MiaoToolDQNet_t *pDQN)
{

	MIAO_assert(!(pDQN));
	MiaoToolBPnnAllDataInit(&(pDQN->MainNet), pDQN->basic.basic.pmem);
	MiaoToolBPnnAllDataInit(&(pDQN->TargetNet), pDQN->basic.basic.pmem);
	MiaoToolBPnnCopy(&(pDQN->TargetNet), &(pDQN->MainNet));
}

MiaoToolDQNet_t* MiaoToolDQNetStoreMem(MiaoToolDQNet_t *pDQN)
{
    lw_u16 i = 0;

    MIAO_assert(!(pDQN));
    i = pDQN->ExpReplay.MemTable.Row.uData;

	pDQN->ExpReplay.AppendFun(&(pDQN->ExpReplay), pDQN->Count, 0);

    if (++(pDQN->Count) >= i)
    {
        pDQN->Count = 0;
    }
    if (pDQN->CountMax < i)
        pDQN->CountMax++;

    //״̬����
    pMiaoToolMatrixSet(&(pDQN->basic.basic.StateLast), 0, &(pDQN->basic.basic.StateNow));

    return pDQN;

}


MiaoToolDQNet_t* MiaoToolDQNetLearning(MiaoToolDQNet_t *pDQN)
{
    lw_u16 i, sample_len, actindex;
    ParaType *pStateLast, *pActIndex, *pReward, *pStateNow;
    ParaType dmax;
    ParaType ISweight;
    lw_u16  maxindex;
    MiaoToolQRLBasic_t *pQRLB;
    MiaoToolQRLData_t *pTQRLDS;
    MiaoMat mat;
    MiaoToolDQNetExpReplayFrame_t *pbase;
    void **ppsample;
    MIAO_assert(!(pDQN));

    pQRLB = &(pDQN->basic);
    pTQRLDS = &(pQRLB->DataTable);

	pDQN->StoreMemFun(pDQN);

	MIAO_RL_UPDATE(pDQN);

    if (pDQN->CountMax < pDQN->ExpReplay.MemTable.Row.uData)//û�д���
    {
        return pDQN;
    }

    if ((pDQN->Count_Copy >= pDQN->NetCopyThreshold))//�ж��Ƿ�������ֵ
    {
        pDQN->Count_Copy = 0;
        MiaoToolBPnnCopy(&(pDQN->TargetNet), &(pDQN->MainNet));//����������
    }
    else
    {
        pDQN->Count_Copy++;
    }


    sample_len = pDQN->ExpReplay.SampleFun(&(pDQN->ExpReplay));//�������
    ppsample = pDQN->ExpReplay.SampleResult.pData;
    for (i = 0; i < sample_len; i++)
    {
        pbase = (MiaoToolDQNetExpReplayFrame_t *)ppsample[i];//
        //�õ�����

        pStateLast = (ParaType *)MIAO_TOOL_DQN_EXP_STATE_LAST(pDQN->ExpReplay, pbase);
        pActIndex = (ParaType *)MIAO_TOOL_DQN_EXP_ACT(pDQN->ExpReplay, pbase);
        pReward = (ParaType *)MIAO_TOOL_DQN_EXP_R(pDQN->ExpReplay, pbase);
        pStateNow = (ParaType *)MIAO_TOOL_DQN_EXP_STATE_NOW(pDQN->ExpReplay, pbase);


        //qeval�������qnow ����qlast
        mat.length = pDQN->ExpReplay.State_Col;
        mat.a = pStateLast;
        //pMiaoToolMatrixSet(&(pDQN->MainNet.x), 0, &(mat));
        pMiaoToolMatrixDot(&(pDQN->MainNet.x), &(mat), &(pDQN->StateFactor));//���Թ�һ������


        //qtarget���� ��� qnext ����qnow
        mat.length = pDQN->ExpReplay.State_Col;
        mat.a = pStateNow;
        //pMiaoToolMatrixSet(&(pDQN->TargetNet.x), 0, &(mat));
        pMiaoToolMatrixDot(&(pDQN->TargetNet.x), &(mat), &(pDQN->StateFactor));//���Թ�һ������

		pDQN->TargetNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDQN->TargetNet));

		pDQN->MainNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDQN->MainNet));


        switch (pDQN->Optim)
        {
        case MiaoToolDQNetOptim_NormalDQN:
            pMiaoToolMatrixMaxMin(&(pDQN->TargetNet.y), &dmax, &maxindex, NULL, NULL);
            break;

        case MiaoToolDQNetOptim_DoubleDQN://����DoubleDQN�����Ż�
            pMiaoToolMatrixMaxMin(&(pDQN->MainNet.y), &dmax, &maxindex, NULL, NULL);
            dmax = pDQN->TargetNet.y.a[maxindex];
            break;

        default:
            MIAO_assert(1);
            break;
        }

        actindex = _ParaInt(*pActIndex);

        pMiaoToolMatrixSet(&(pDQN->MainNet.ref), 0, &(pDQN->MainNet.y));//ʹ���Ϊ0

		/*
        if (!(pQRLB->basic.isTermialFun((MiaoToolRLBasic_t *)pQRLB)))
        {
            pDQN->MainNet.ref.a[actindex] = (*pReward) + _ParaMul(pQRLB->basic.Gamma, dmax);
        }
        else
        {
            pDQN->MainNet.ref.a[actindex] = (*pReward);
        }
		*/
		pDQN->MainNet.ref.a[actindex] = (*pReward) + _ParaMul(pQRLB->basic.Gamma, dmax);
        //MiaoToolBPnnCalErr(&(pDQN->MainNet));
        //MiaoToolBPnnUpdate(&(pDQN->MainNet));


        if (pDQN->ExpReplay.PriorType == MiaoToolDQNetExpReplay_PER)
        {

            ISweight = _ParaPow(_ParaDiv(pDQN->ExpReplay.Min, pbase->Importance), pDQN->ExpReplay.Beta);
            pDQN->MainNet._nl_factor = ISweight * 1.5;

            MiaoToolBPnnCalErr(&(pDQN->MainNet));

            ISweight = _ParaPow(_ParaAdd(pDQN->MainNet.Error, pDQN->ExpReplay.Epsi),pDQN->ExpReplay.Alpha);

            ISweight = ISweight > pDQN->ExpReplay.Rmax ? pDQN->ExpReplay.Rmax : ISweight;
            ISweight = ISweight < pDQN->ExpReplay.Rmin ? pDQN->ExpReplay.Rmax : ISweight;

            pbase->_new_Importance = ISweight;

        }
        else
        {
			MiaoToolBPnnCalErr(&(pDQN->MainNet));//ѵ�������� ������������
        }

    }

    MiaoToolBPnnReview(&(pDQN->MainNet));

    pDQN->ExpReplay.UpdateFun(&(pDQN->ExpReplay), sample_len);


    /*(Ver.=0.96)
    //�ɰ汾
    for (i = 0; i < pDQN->CountMax; i++)
    {
        drand = pDQN->basic.basic.pRand->rand(pDQN->basic.basic.pRand, _ParaFint(1), _ParaFint(0));

        if (drand > pDQN->P)
        {
            continue;
        }
        pdata = pDQN->MemPool.a + (i << 2);

        pDQN->TargetNet.x.a[0] = pdata[3];//qtarget���� ��� qnext
        pDQN->MainNet.x.a[0] = pdata[0];//qeval�������qnow

        MiaoToolBPnnRunning(&(pDQN->TargetNet));
        MiaoToolBPnnRunning(&(pDQN->MainNet));

        pMiaoToolMatrixMaxMin(&(pDQN->TargetNet.y), &dmax, &maxindex, NULL, NULL);

        actindex = _ParaInt(pdata[1]);

        pMiaoToolMatrixSet(&(pDQN->MainNet.ref), 0, &(pDQN->MainNet.y));//ʹ���Ϊ0
        reward = pdata[2];
        if (pQRLB->StateNow != pQRLB->StateTermial)
        {
            pDQN->MainNet.ref.a[actindex] = reward + _ParaMul(pQRLB->Gamma, dmax);
        }
        else
        {
            pDQN->MainNet.ref.a[actindex] = reward;
        }

        MiaoToolBPnnCalErr(&(pDQN->MainNet));
        MiaoToolBPnnUpdate(&(pDQN->MainNet));
    }


    //������һ�־���ֵ
    pQRLB->StateLast = pQRLB->StateNow;
    pQRLB->StateLastIndex = pQRLB->StateNowIndex;
    pQRLB->ActLastIndex = pQRLB->ActNowIndex;
    */


	return pDQN;
}


MiaoToolDQNet_t* MiaoToolDQNetChoose(MiaoToolDQNet_t *pDQN)
{

    ParaType drand, dmax;
    lw_u16 selrand, row;
    MiaoMat mat;
    MiaoToolQRLBasic_t *pQRLB;
    MiaoToolQRLData_t *pTQRLDS;
	MiaoToolRLBasic_t *pRL;
    MIAO_assert(!(pDQN));

    pQRLB = &(pDQN->basic);
	pRL = &(pQRLB->basic);
	drand = pRL->pRand->rand(pRL->pRand, _ParaFint(0), _ParaFint(1));
    pTQRLDS = &(pQRLB->DataTable);



    if (drand < pQRLB->Ep)
    {
        //��װ����
        /*(Ver.=0.96)dmax= _ParaDiv(pQRLB->StateNow, pDQN->StateFactor);//��һ��
        pDQN->MainNet.x.a[0] = dmax;*/
        //pMiaoToolMatrixSet(&(pDQN->MainNet.x), 0, &(pQRLB->StateNow));
        pMiaoToolMatrixDot(&(pDQN->MainNet.x), &(pRL->StateNow), &(pDQN->StateFactor));//���Թ�һ������
        //��������
		pDQN->MainNet.Train.mini_batch_now = 1;
        MiaoToolBPnnForward(&(pDQN->MainNet));
		pDQN->MainNet.Train.AllBatchCount = 0;
		pDQN->MainNet.Train._batch_stream_count = 0;
        //��ѯQ������Ϊ
        MiaoToolMatrixMaxMin(&(pDQN->MainNet.y), &dmax, &selrand, NULL, NULL);

//		if (isnan(pDQN->MainNet.y.a[0]))
//		{
//			printf("------------\n-------------");
//		}

        pQRLB->ActNowIndex = selrand;


    }
    else
    {
        //ֱ�����ѡȡ��Ϊ
        selrand = pRL->pRand->randint(pRL->pRand, 0, pTQRLDS->ActionTable.row);
        pQRLB->ActNowIndex = selrand;

    }

    //������Ϊ
	pRL->ActNow.a = pTQRLDS->ActionTable.a + pQRLB->ActNowIndex*pTQRLDS->ActionTable.col;

    return pDQN;
}
#endif // MIAO_TOOL_IMPORT_ALL || (MIAO_TOOL_IMPORT_RL_DQN && MIAO_TOOL_IMPORT_NN && MIAO_TOOL_IMPORT_NN_BPnn)


#if MIAO_TOOL_IMPORT_ALL || (MIAO_TOOL_IMPORT_RL_DDPG && MIAO_TOOL_IMPORT_NN && MIAO_TOOL_IMPORT_NN_BPnn && MIAO_TOOL_IMPORT_RL_EXP_RE)


void MiaoToolDDPGradInitial(MiaoToolDDPGrad_t *pDDPG)//DDPG��ʼ�����������ȱ�����
{
	MIAO_assert(!(pDDPG));

	memset(pDDPG, 0, sizeof(MiaoToolDDPGrad_t));

	MiaoToolBPnnInit(&(pDDPG->CriticMainNet),1);
	MiaoToolBPnnInit(&(pDDPG->CriticTargetNet),1);
	MiaoToolBPnnInit(&(pDDPG->ActorMainNet),1);
	MiaoToolBPnnInit(&(pDDPG->ActorTargetNet),1);

	pDDPG->CriticTargetNet.Train.Flag.NeedTrain = 1;
	pDDPG->ActorTargetNet.Train.Flag.NeedTrain = 1;

	pDDPG->StoreMemFun = (void *)MiaoToolDDPGradStoreMem;
	pDDPG->PGradBasic.basic.ChooseFun = (void *)MiaoToolDDPGradChoose;
	pDDPG->PGradBasic.basic.LearnFun = (void *)MiaoToolDDPGradLearning;

	pDDPG->PGradBasic.ExpReplay.AppendFun = (void *)MiaoToolDQNetExpReplayAppend;
	pDDPG->PGradBasic.ExpReplay.SampleFun = (void *)MiaoToolDQNetExpReplaySample;
	pDDPG->PGradBasic.ExpReplay.UpdateFun = (void *)MiaoToolDQNetExpReplayUpdate;
	pDDPG->PGradBasic.ExpReplay.MinMaxFun = (void *)MiaoToolDQNetExpReplayMinMax;


}

void MiaoToolDDPGradParaInitial//DDPG������ʼ������
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	ParaType Gamma,//˥������
	MiaoToolRandomBasic_t *prand,//�����������
	MiaoMemoryBasic_t *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
)
{

	MIAO_assert(!(pDDPG));
	MIAO_assert(!(prand));
	MIAO_assert(!(pMem));
	MIAO_assert(!(pActfun));
	MIAO_assert(!(pBornfun));

	pDDPG->PGradBasic.basic.Gamma = Gamma;

	pDDPG->PGradBasic.basic.pRand = prand;
	pDDPG->PGradBasic.basic.pmem = pMem;
	pDDPG->PGradBasic.basic.ActEnvFun = (void *)pActfun;
	pDDPG->PGradBasic.basic.BornFun = (void *)MiaoToolRL_Born;
	pDDPG->PGradBasic.basic.BornFunByUser = (void *)pBornfun;

	pDDPG->PGradBasic.basic.isTermialFun = (void *)MiaoToolRL_isTermial;


}


void MiaoToolDDPGradInterfaceInitial
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	lw_u16 state_dim, //״̬ά��
	lw_u16 action_dim,//����ά��
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor,//״̬��һ������(��ΪNULL)
	ParaType ActionMax, ParaType ActionMin,
	ParaType ActionVar
)
{
	MIAO_assert(!(pDDPG));

	if (pDDPG->PGradBasic.basic.pmem)
	{
		if (!pAction)
		{
			pAction = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(1 * action_dim));
		}

		if (!pStateNow)
		{
			pStateNow = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
		}

		if (!pStateLast)
		{
			pStateLast = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
		}

		if (!pStateTermial)
		{
			pStateTermial = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
		}

		if (!pStateFactor)
		{
			pStateFactor = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));//
			MiaoToolMatrixInitial(&(pDDPG->PGradBasic.StateFactor), 1, state_dim, pStateFactor);//
			pMiaoToolMatrixSet(&(pDDPG->PGradBasic.StateFactor), _ParaFint(1), NULL);//��ʼ��Ϊ1
		}
	}

	MiaoToolMatrixInitial(&(pDDPG->PGradBasic.basic.StateNow), 1, state_dim, pStateNow);
	MiaoToolMatrixInitial(&(pDDPG->PGradBasic.basic.StateLast), 1, state_dim, pStateLast);
	MiaoToolMatrixInitial(&(pDDPG->PGradBasic.basic.StateTermial), 1, state_dim, pStateTermial);
	MiaoToolMatrixInitial(&(pDDPG->PGradBasic.basic.ActNow), 1, action_dim, pAction);

	MiaoToolMatrixInitial(&(pDDPG->PGradBasic.StateFactor), 1, state_dim, pStateFactor);

	pDDPG->PGradBasic.ActionMax = ActionMax;
	pDDPG->PGradBasic.ActionMin = ActionMin;
	pDDPG->ActionVar = ActionVar;

}

void MiaoToolDDPGradStoreMemInitial//DDPG�ڴ�����ʼ��
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	lw_u16 MemPoolLength,//���䳤��
	lw_u16 batch_size,//��ȡ�ļ��䳤��
	ParaType copy_data//���Ƶ���ֵ
)
{

	MiaoMemoryBasic_t *pmem;
	lw_u16 col_kind[] = { sizeof(MiaoToolDQNetExpReplayFrame_t),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType) };
	MIAO_assert(!(pDDPG));
	MIAO_assert(!(MemPoolLength));
	MIAO_assert(!(batch_size));

	pDDPG->PGradBasic.ExpReplay.pTQL = (MiaoToolQRLBasic_t *)pDDPG;
	pDDPG->PGradBasic.ExpReplay.PriorType = MiaoToolDQNetExpReplay_NoP;

	if (copy_data >_ParaFint(1))//>1 ��Ϊ��ֱ�Ӹ���ģʽ ������DQN
	{
		pDDPG->CopyMethod = MiaoToolDDPG_CopyMethod_Hard;
		pDDPG->Copy.HardReplace.Count_Copy = 0;
		pDDPG->Copy.HardReplace.NetCopyThreshold = (lw_u16)_ParaInt(copy_data);
	}
	else//���� �ȸ�DM�����еķ���
	{
		pDDPG->CopyMethod = MiaoToolDDPG_CopyMethod_Soft;
		pDDPG->Copy.tSoftReplace = copy_data;
	}


	//�ڴ����
	pmem = pDDPG->PGradBasic.basic.pmem;

	//������װ
	pDDPG->PGradBasic.ExpReplay.DivResult.uData = batch_size;
	pDDPG->PGradBasic.ExpReplay.SampleResult.uData = batch_size;
	


	//����ֵ�ط� ��ʼ��
	pDDPG->PGradBasic.ExpReplay.State_Col = pDDPG->PGradBasic.basic.StateNow.length;
	pDDPG->PGradBasic.ExpReplay.Action_Col = pDDPG->PGradBasic.basic.ActNow.length;

	//MiaoTable ��ʼ�� ����׼��
	col_kind[1] *= pDDPG->PGradBasic.ExpReplay.State_Col;
	col_kind[2] *= pDDPG->PGradBasic.ExpReplay.Action_Col;
	col_kind[4] *= pDDPG->PGradBasic.ExpReplay.State_Col;

	MiaoToolTableInitial(&(pDDPG->PGradBasic.ExpReplay.MemTable), MemPoolLength, col_kind, 5, pmem, NULL, NULL);

	pDDPG->PGradBasic.ExpReplay.SampleResult.pData = pmem->Malloc(pmem, 2 * pDDPG->PGradBasic.ExpReplay.SampleResult.uData * sizeof(void *));

}

/*
void MiaoToolDDPGradAppendInitial//DDPG������׷��,һ�λ�׷������������
(
	MiaoToolDDPGrad_t *pDDPG,//thisָ��
	MiaoToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	MiaoToolNNLayerActFun_t **AFlist,//�����
	ParaType *nl_list//ѧϰ����
)
{
	lw_u16 i;
	lw_u16 row, col;
	MiaoMemoryBasic_t *pmem;
	MiaoToolRandomBasic_t *pTRB;
	MiaoToolNNLayerFullCon_t *p1, *p2;
	MiaoToolBPnn_t *mn, *tn;

	MIAO_assert(!(pDDPG));
	MIAO_assert(!(layer_list));
	MIAO_assert(!(AFlist));
	MIAO_assert(!(nl_list));

	pmem = (pDDPG->PGradBasic.basic.pmem);
	pTRB = pDDPG->PGradBasic.basic.pRand;

	if (Kind == MiaoToolACtoActor)
	{
		row = pDDPG->PGradBasic.basic.StateNow.length;//ֱ���������
		layer_list[layer_num - 1] = pDDPG->PGradBasic.basic.ActNow.length;

		mn = &(pDDPG->ActorMainNet);
		tn = &(pDDPG->ActorTargetNet);

	}
	else if (Kind == MiaoToolACtoCritic)
	{
		row = pDDPG->PGradBasic.basic.StateNow.length + pDDPG->PGradBasic.basic.ActNow.length;//���ۼһὫ����Ҳ��������
		layer_list[layer_num - 1] = 1;

		mn = &(pDDPG->CriticMainNet);
		tn = &(pDDPG->CriticTargetNet);
	}
	else
	{
		MIAO_assert(1);
	}

	for (i = 0; i < layer_num; i++)
	{
		col = layer_list[i];

		p1 = MiaoToolBPnnLayerAppend_Dynamic(
			mn
			, AFlist[i]
			, row, col
			, pTRB
			, _ParaFrom(0.5), _ParaFrom(-0.5)
			, nl_list[i]
			, NULL, NULL, pmem
			, _ParaFrom(0)
		);

		p2 = MiaoToolBPnnLayerAppend_Dynamic(
			tn
			, AFlist[i]
			, row, col
			, pTRB
			, _ParaFrom(0.5), _ParaFrom(-0.5)
			, nl_list[i]
			, NULL, NULL, pmem
			, _ParaFrom(0)
		);

		pMiaoToolMatrixSet(&(p2->w), 0, &(p1->w));
		pMiaoToolMatrixSet(&(p2->BiasUN.bias), 0, &(p1->BiasUN.bias));

		row = col;

	}


}

void MiaoToolDDPGradLossInitial(
	MiaoToolDDPGrad_t *pDDPG,
	LossFunTYPE loss
)
{
	MiaoMemoryBasic_t *pmem;
	void *pref;

	MIAO_assert(!(pDDPG));
	MIAO_assert(!(loss));

	pmem = pDDPG->PGradBasic.basic.pmem;
	pref = pmem->Malloc(pmem, PARA_MEM_CAL(pDDPG->PGradBasic.basic.ActNow.length));

	MiaoToolBPnnLossInitial
	(
		&(pDDPG->ActorMainNet),
		pref,
		pDDPG->PGradBasic.basic.pRand,
		pDDPG->PGradBasic.basic.pmem,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		NULL,
		MiaoToolNNTrainSaveMethod_OnceSave,
		MiaoToolNNTrainUpdateMethod_Manu
	);


	pref = pmem->Malloc(pmem, PARA_MEM_CAL(pDDPG->PGradBasic.basic.ActNow.length));

	MiaoToolBPnnLossInitial
	(
		&(pDDPG->CriticMainNet),
		pref,
		pDDPG->PGradBasic.basic.pRand,
		pDDPG->PGradBasic.basic.pmem,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		loss,
		MiaoToolNNTrainSaveMethod_OnceSave,
		MiaoToolNNTrainUpdateMethod_Manu
	);
	
}
*/


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
)
{
	MiaoMemoryBasic_t *pmem;


	MIAO_assert(!(pDDPG));
	MIAO_assert(!(pFun));
	MIAO_assert(!(pTRB));

	pmem = (pDDPG->PGradBasic.basic.pmem);

	if (Kind== MiaoToolACtoActor)
	{
		MiaoToolBPnnFullConAppend(
			&(pDDPG->ActorMainNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			pFun, pTRB, initd1, initd2,
			pTemplet, pmem
		);

		MiaoToolBPnnFullConAppend(
			&(pDDPG->ActorTargetNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL, initd1, initd2,
			pTemplet, pmem
		);
	}
	else
	{
		MiaoToolBPnnFullConAppend(
			&(pDDPG->CriticMainNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			pFun, pTRB, initd1, initd2,
			pTemplet, pmem
		);

		MiaoToolBPnnFullConAppend(
			&(pDDPG->CriticTargetNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL, initd1, initd2,
			pTemplet, pmem
		);
	}




}


void MiaoToolDDPGradTarinInitial(
	MiaoToolDDPGrad_t *pDDPG,
	ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
)
{
	MIAO_assert(!(pDDPG));
	MIAO_assert(!(loss));
	MiaoToolBPnnTrainInit
	(
		&(pDDPG->ActorMainNet),
		qref,
		pDDPG->PGradBasic.basic.pmem,
		loss,
		nl
	);

	MiaoToolBPnnTrainInit
	(
		&(pDDPG->CriticMainNet),
		qref,
		pDDPG->PGradBasic.basic.pmem,
		loss,
		nl
	);
}


void MiaoToolDDPGradAllDataInit(MiaoToolDDPGrad_t *pDDPG)
{

	MIAO_assert(!(pDDPG));
	MiaoToolBPnnAllDataInit(&(pDDPG->ActorMainNet), pDDPG->PGradBasic.basic.pmem);
	MiaoToolBPnnAllDataInit(&(pDDPG->ActorTargetNet), pDDPG->PGradBasic.basic.pmem);
	MiaoToolBPnnAllDataInit(&(pDDPG->CriticMainNet), pDDPG->PGradBasic.basic.pmem);
	MiaoToolBPnnAllDataInit(&(pDDPG->CriticTargetNet), pDDPG->PGradBasic.basic.pmem);

	MiaoToolBPnnCopy(&(pDDPG->ActorTargetNet), &(pDDPG->ActorMainNet));
	MiaoToolBPnnCopy(&(pDDPG->CriticTargetNet), &(pDDPG->CriticMainNet));
}


MiaoToolDDPGrad_t* MiaoToolDDPGradChoose(MiaoToolDDPGrad_t *pDDPG)
{
	
	pMiaoToolMatrixDot(&(pDDPG->ActorMainNet.x), &(pDDPG->PGradBasic.basic.StateNow), &(pDDPG->PGradBasic.StateFactor));//���Թ�һ������

	//��������
	pDDPG->ActorMainNet.Train.mini_batch_now = 1;
	MiaoToolBPnnForward(&(pDDPG->ActorMainNet));
	pDDPG->ActorMainNet.Train.AllBatchCount = 0;
	pDDPG->ActorMainNet.Train._batch_stream_count = 0;

	MiaoToolMatrixToRandnChip(
		&(pDDPG->PGradBasic.basic.ActNow),
		&(pDDPG->ActorMainNet.y), 
		pDDPG->PGradBasic.basic.pRand,
		pDDPG->ActionVar,
		pDDPG->PGradBasic.ActionMin,
		pDDPG->PGradBasic.ActionMax
	);//��̬�ֲ������ �����޷�



	
	/*
	OU���� ����ʧ��
	MiaoMat m_rand, m_k;
	MiaoToolTable_t *ptable;
	ParaType temp = 1;

	ptable = &(pDDPG->ActorMainNet.Train.DeltTable);
	m_rand.a = ptable->ReadFun(ptable, 0, 0, NULL);
	m_rand.length = ptable->ColSize / sizeof(ParaType);

	MiaoToolMatrixRandomInitial(&m_rand, pDDPG->basic.pRand, 1, -1);

	MiaoToolMatrixToRandnChip(
		&(m_rand),
		&(m_rand),
		pDDPG->basic.pRand,
		pDDPG->ActionVar/1000,
		pDDPG->ActionMax,
		pDDPG->ActionMin
	);

	m_k.length = 1;
	m_k.a = &temp;



	_MiaoToolBPnnReviewOnce(&(pDDPG->ActorMainNet), 0, &m_k);

	pMiaoToolMatrixDot(&(pDDPG->ActorMainNet.x), &(pDDPG->basic.StateNow), &(pDDPG->StateFactor));//���Թ�һ������
																								  
	MiaoToolBPnnRunning(&(pDDPG->ActorMainNet));//��������

	temp = -1;

	_MiaoToolBPnnReviewOnce(&(pDDPG->ActorMainNet), 0, &m_k);//�����

	memset(ptable->Row.pData, 0, ptable->Row.uData * ptable->ColSize);

	
	MiaoToolMatrixToRandnChip(
		&(pDDPG->basic.ActNow),
		&(pDDPG->ActorMainNet.y),
		pDDPG->basic.pRand,
		pDDPG->ActionVar,
		pDDPG->ActionMax,
		pDDPG->ActionMin
	);//��̬�ֲ������ �����޷�
	*/

	return pDDPG;

}

MiaoToolDDPGrad_t* MiaoToolDDPGradStoreMem(MiaoToolDDPGrad_t *pDDPG)
{
	lw_u16 i = 0;

	MIAO_assert(!(pDDPG));
	i = pDDPG->PGradBasic.ExpReplay.MemTable.Row.uData;

	pDDPG->PGradBasic.ExpReplay.AppendFun(&(pDDPG->PGradBasic.ExpReplay), pDDPG->Count, 1);

	if (++(pDDPG->Count) >= i)
	{
		pDDPG->Count = 0;
	}
	if (pDDPG->CountMax < i)
		pDDPG->CountMax++;

	//״̬����
	pMiaoToolMatrixSet(&(pDDPG->PGradBasic.basic.StateLast), 0, &(pDDPG->PGradBasic.basic.StateNow));

	return pDDPG;


}

MiaoToolDDPGrad_t* MiaoToolDDPGradLearning(MiaoToolDDPGrad_t *pDDPG)
{
	lw_u16 i, sample_len, actindex;
	ParaType *pStateLast, *pAct, *pReward, *pStateNow, negbuf = _ParaFrom(1), qbuf;
	MiaoToolRLBasic_t *pRL;
	MiaoMat mat1, mat2, mat3;
	MiaoToolDQNetExpReplayFrame_t *pbase;
	void **ppsample;
	MiaoToolNNLayerFullCon_t *pTNL;

	MIAO_assert(!(pDDPG));

	pRL = &(pDDPG->PGradBasic.basic);
	

	pDDPG->StoreMemFun(pDDPG);

	MIAO_RL_UPDATE(pDDPG);

	if (pDDPG->CountMax < pDDPG->PGradBasic.ExpReplay.MemTable.Row.uData)//û�д���
	{
		return pDDPG;
	}


	//���㲽 ����
	sample_len = pDDPG->PGradBasic.ExpReplay.SampleFun(&(pDDPG->PGradBasic.ExpReplay));//�������
	ppsample = pDDPG->PGradBasic.ExpReplay.SampleResult.pData;
	
	for (i = 0; i < sample_len; i++)
	{
		pbase = (MiaoToolDQNetExpReplayFrame_t *)ppsample[i];//�õ�����

		pStateLast = (ParaType *)MIAO_TOOL_DQN_EXP_STATE_LAST(pDDPG->PGradBasic.ExpReplay, pbase);
		pAct = (ParaType *)MIAO_TOOL_DQN_EXP_ACT(pDDPG->PGradBasic.ExpReplay, pbase);
		pReward = (ParaType *)MIAO_TOOL_DQN_EXP_R(pDDPG->PGradBasic.ExpReplay, pbase);
		pStateNow = (ParaType *)MIAO_TOOL_DQN_EXP_STATE_NOW(pDDPG->PGradBasic.ExpReplay, pbase);


		//��һ�� �� StateNow ���� ActorTarget���� �õ�ActionNext 
		mat1.length = pRL->StateNow.length;
		mat1.a = pStateNow;
		pMiaoToolMatrixDot(&(pDDPG->ActorTargetNet.x), &(mat1), &(pDDPG->PGradBasic.StateFactor));//���Թ�һ������
		pDDPG->ActorTargetNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDDPG->ActorTargetNet));//��������


		//�ڶ��� ����ActionNext �� stateNow ���� CrticTarget ���� qtarget=R+GAMMA*CrticTarget.yout

		pMiaoToolMatrixJoin(&(pDDPG->CriticTargetNet.x), &(pDDPG->ActorTargetNet.x), &(pDDPG->ActorTargetNet.y));//״̬ƴ�Ӷ���
		pDDPG->CriticTargetNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDDPG->CriticTargetNet));//��������
		
		mat1.length = 1;
		mat1.a = &(pRL->Gamma);

		mat2.length = 1;
		mat2.a = pReward;

		pMiaoToolMatrixMPYA(&(pDDPG->CriticMainNet.ref), &(pDDPG->CriticTargetNet.y), &mat1, &mat2);//ref ����


		//������ ����ActionNow��stateLast ���� criticmain �õ�qmain �õ� TD_error ���򴫲�
		mat1.length = pRL->StateNow.length;
		mat1.a = pStateLast;

		pMiaoToolMatrixDot(&(pDDPG->ActorMainNet.x), &(mat1), &(pDDPG->PGradBasic.StateFactor));//���Թ�һ������ �������actor���������

		mat2.length = pRL->ActNow.length;
		mat2.a = pAct;

		pMiaoToolMatrixJoin(&(pDDPG->CriticMainNet.x), &(pDDPG->ActorMainNet.x), &(mat2));//״̬ƴ�Ӷ���
	
		pDDPG->CriticMainNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDDPG->CriticMainNet));//��������

		qbuf = pDDPG->CriticMainNet.y.a[0];

		MiaoToolBPnnCalErr(&(pDDPG->CriticMainNet));//���򴫲����

		/*
		������
		���Ĳ� ֱ�ӽ�-qmain��Ϊactormain����� ���򴫲�

		MiaoToolBPnnRunning(&(pDDPG->ActorMainNet));//����actormain����

		mat3.length = 1;
		mat3.a = &negbuf;
		pTNL = pDDPG->ActorMainNet.Net.pTail->Data.pData;

		

		pMiaoToolMatrixSet(&(pTNL->outErr),qbuf, NULL);//ֱ���������Ϊ-qmain
		
		MiaoToolBPnnTrain(&(pDDPG->ActorMainNet));//���򴫲�
		*/
		
		//���Ĳ� 
		//���� actor���� ���ҽ�A����������ΪC���������
		pDDPG->ActorMainNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDDPG->ActorMainNet));//����actormain����

		pMiaoToolMatrixJoin(&(pDDPG->CriticMainNet.x), &(pDDPG->ActorMainNet.x), &(pDDPG->ActorMainNet.y));//״̬ƴ�Ӷ���

		//���岽
		//����C���� ����qmain���� 
		pDDPG->CriticMainNet.Train.mini_batch_now = 1;
		MiaoToolBPnnForward(&(pDDPG->CriticMainNet));

		//������
		//����dq/da ȡ��dq/da
		//mat3.a=MiaoToolBPnnGradForInput(&(pDDPG->CriticMainNet));
		mat3.length = mat2.length;
		mat3.a += mat1.length;

		//���߲�
		//dq/daparam=dq/da*da/daparam
		mat1.length = 1;
		mat1.a = &negbuf;

		
		pMiaoToolMatrixDot(&(pDDPG->ActorMainNet.y), &mat3, &mat1);//�õ������ݶ�

		MiaoToolBPnnCalErr(&(pDDPG->ActorMainNet));//���򴫲�


	}

	//��8�� ������������
	MiaoToolBPnnReview(&(pDDPG->ActorMainNet));
	MiaoToolBPnnReview(&(pDDPG->CriticMainNet));

	//��9�� �����滻

	if (pDDPG->CopyMethod == MiaoToolDDPG_CopyMethod_Hard)
	{
		if ((pDDPG->Copy.HardReplace.Count_Copy >= pDDPG->Copy.HardReplace.NetCopyThreshold))//�ж��Ƿ�������ֵ
		{
			pDDPG->Copy.HardReplace.Count_Copy = 0;
			MiaoToolBPnnCopy(&(pDDPG->ActorTargetNet), &(pDDPG->ActorMainNet));//����������
			MiaoToolBPnnCopy(&(pDDPG->CriticTargetNet), &(pDDPG->CriticMainNet));//����������
		}
		else
		{
			pDDPG->Copy.HardReplace.Count_Copy++;
		}
	}
	else if (pDDPG->CopyMethod == MiaoToolDDPG_CopyMethod_Soft)
	{
		MiaoToolBPnnSoftReplace(&(pDDPG->ActorTargetNet), &(pDDPG->ActorMainNet), pDDPG->Copy.tSoftReplace);
		MiaoToolBPnnSoftReplace(&(pDDPG->CriticTargetNet), &(pDDPG->CriticMainNet), pDDPG->Copy.tSoftReplace);
	}
	else
	{
		MIAO_assert(1);
	}


	return pDDPG;
}
#endif // MIAO_TOOL_IMPORT_ALL || (MIAO_TOOL_IMPORT_RL_DDPG && MIAO_TOOL_IMPORT_NN && MIAO_TOOL_IMPORT_NN_BPnn && MIAO_TOOL_IMPORT_RL_EXP_RE)

//
//
//
//void MiaoToolPPOptimInitial(MiaoToolPPOptim_t *pPPO)//PPO��ʼ�����������ȱ�����
//{
//
//	MIAO_assert(!(pPPO));
//	memset(pPPO, 0, sizeof(MiaoToolPPOptim_t));
//
//	MiaoToolBPnnInitial(&(pPPO->CriticNet));
//	MiaoToolBPnnInitial(&(pPPO->ActorNewNet));
//	MiaoToolBPnnInitial(&(pPPO->ActorOldNet));
//
//
//
//	pPPO->StoreMemFun = MiaoToolPPOptimStoreMem;
//	pPPO->PGradBasic.basic.ChooseFun = MiaoToolPPOptimChoose;
//	pPPO->PGradBasic.basic.LearnFun = MiaoToolPPOptimLearning;
//
//	pPPO->PGradBasic.ExpReplay.AppendFun = MiaoToolDQNetExpReplayAppend;
//	pPPO->PGradBasic.ExpReplay.SampleFun = MiaoToolDQNetExpReplaySample;
//	pPPO->PGradBasic.ExpReplay.UpdateFun = MiaoToolDQNetExpReplayUpdate;
//	pPPO->PGradBasic.ExpReplay.MinMaxFun = MiaoToolDQNetExpReplayMinMax;
//
//
//}
//
//void MiaoToolPPOptimParaInitial//PPO�ڴ��ʼ������
//(
//	MiaoToolPPOptim_t *pPPO,//thisָ��
//	ParaType ClipE,
//	ParaType tau,
//	ParaType gamma,
//	ParaType beta,
//	MiaoToolRandomBasic_t *prand,//�����������
//	MiaoMemoryBasic_t *pMem,//�ڴ�ָ��
//	pActEnvFunType pActfun,//�������к���ָ��
//	pBornFunType pBornfun//������ʼ������ָ��
//
//)
//{
//	MIAO_assert(!(pPPO));
//	MIAO_assert(!(prand));
//	MIAO_assert(!(pMem));
//	MIAO_assert(!(pActfun));
//	MIAO_assert(!(pBornfun));
//
//	pPPO->ClipE = ClipE;
//	pPPO->GAE_gamma = gamma;
//	pPPO->GAE_tau = tau;
//	pPPO->beta_entropy = beta;
//
//	pPPO->PGradBasic.basic.pRand = prand;
//	pPPO->PGradBasic.basic.pmem = pMem;
//	pPPO->PGradBasic.basic.ActEnvFun = pActfun;
//	pPPO->PGradBasic.basic.BornFun = MiaoToolRL_Born;
//	pPPO->PGradBasic.basic.BornFunByUser = pBornfun;
//
//	pPPO->PGradBasic.basic.isTermialFun = MiaoToolRL_isTermial;
//
//
//}
//
//
//void MiaoToolPPOptimInterfaceInitial
//(
//	MiaoToolPPOptim_t *pPPO,//thisָ��
//	lw_u16 state_dim, lw_u16 action_dim,
//	void *pAction,//�����б��ڴ棨��ΪNULL��
//	void *pStateNow,//��ǰ״̬����ΪNULL��
//	void *pStateLast,//֮ǰ״̬����ΪNULL��
//	void *pStateTermial,//�ս�״̬����ΪNULL��
//	void *pStateFactor,//״̬��һ������(��ΪNULL)
//	ParaType ActionMax, ParaType ActionMin
//)
//{
//	MIAO_assert(!(pPPO));
//
//	if (pPPO->PGradBasic.basic.pmem)
//	{
//		if (!pAction)
//		{
//			pAction = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(1 * action_dim));
//		}
//
//		if (!pStateNow)
//		{
//			pStateNow = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
//		}
//
//		if (!pStateLast)
//		{
//			pStateLast = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
//		}
//
//		if (!pStateTermial)
//		{
//			pStateTermial = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
//		}
//
//		if (!pStateFactor)
//		{
//			pStateFactor = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));//
//			MiaoToolMatrixInitial(&(pPPO->PGradBasic.StateFactor), 1, state_dim, pStateFactor);//
//			pMiaoToolMatrixSet(&(pPPO->PGradBasic.StateFactor), _ParaFint(1), NULL);//��ʼ��Ϊ1
//		}
//	}
//
//	MiaoToolMatrixInitial(&(pPPO->PGradBasic.basic.StateNow), 1, state_dim, pStateNow);
//	MiaoToolMatrixInitial(&(pPPO->PGradBasic.basic.StateLast), 1, state_dim, pStateLast);
//	MiaoToolMatrixInitial(&(pPPO->PGradBasic.basic.StateTermial), 1, state_dim, pStateTermial);
//	MiaoToolMatrixInitial(&(pPPO->PGradBasic.basic.ActNow), 1, action_dim, pAction);
//
//	MiaoToolMatrixInitial(&(pPPO->PGradBasic.StateFactor), 1, state_dim, pStateFactor);
//
//	pPPO->PGradBasic.ActionMax = ActionMax;
//	pPPO->PGradBasic.ActionMin = ActionMin;
//
//}
//
//void MiaoToolPPOptimStoreMemInitial//PPO�ڴ�����ʼ��
//(
//	MiaoToolPPOptim_t *pPPO,//thisָ��
//	lw_u16 MemPoolLength//���䳤��
//)
//{
//	MiaoMemoryBasic_t *pmem;
//	lw_u16 col_kind[] = { sizeof(MiaoToolDQNetExpReplayFrame_t),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType) };
//	MIAO_assert(!(pPPO));
//	MIAO_assert(!(MemPoolLength));
//
//
//	pPPO->PGradBasic.ExpReplay.pTQL = (MiaoToolQRLBasic_t *)pPPO;
//	pPPO->PGradBasic.ExpReplay.PriorType = MiaoToolDQNetExpReplay_NoP;
//
//
//	//�ڴ����
//	pmem = pPPO->PGradBasic.basic.pmem;
//
//	//������װ
//	pPPO->PGradBasic.ExpReplay.DivResult.uData = 1;
//	pPPO->PGradBasic.ExpReplay.SampleResult.uData = MemPoolLength;
//
//	//����ֵ�ط� ��ʼ��
//	pPPO->PGradBasic.ExpReplay.State_Col = pPPO->PGradBasic.basic.StateNow.length;
//	pPPO->PGradBasic.ExpReplay.Action_Col = pPPO->PGradBasic.basic.ActNow.length;
//
//	//MiaoTable ��ʼ�� ����׼��
//	col_kind[1] *= pPPO->PGradBasic.ExpReplay.State_Col;
//	col_kind[2] *= pPPO->PGradBasic.ExpReplay.Action_Col;
//	col_kind[4] *= pPPO->PGradBasic.ExpReplay.State_Col;
//
//	MiaoToolTableInitial(&(pPPO->PGradBasic.ExpReplay.MemTable), MemPoolLength, col_kind, 5, pmem, NULL, NULL);
//
//	pPPO->PGradBasic.ExpReplay.SampleResult.pData = pmem->Malloc(pmem, 2 * pPPO->PGradBasic.ExpReplay.SampleResult.uData * sizeof(void *));
//
//
//}


void MiaoToolPPOptimAppendInitial//PPO������׷��,һ�λ�׷������������
(
	MiaoToolPPOptim_t *pPPO,//thisָ��
	MiaoToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	MiaoToolNNLayerActFun_t **AFlist,//�����
	ParaType *nl_list//ѧϰ����
)
{
	//lw_u16 i;
	//lw_u16 row, col;
	//MiaoMemoryBasic_t *pmem;
	//MiaoToolRandomBasic_t *pTRB;
	//MiaoToolNNLayerFullCon_t *p1, *p2;
	//MiaoToolBPnn_t *mn, *tn;

	//MIAO_assert(!(pPPO));
	//MIAO_assert(!(layer_list));
	//MIAO_assert(!(AFlist));
	//MIAO_assert(!(nl_list));

	//pmem = (pPPO->PGradBasic.basic.pmem);
	//pTRB = pPPO->PGradBasic.basic.pRand;

	//if (Kind == MiaoToolACtoActor)
	//{
	//	row = pPPO->PGradBasic.basic.StateNow.length;//ֱ���������
	//	layer_list[layer_num - 1] = pPPO->PGradBasic.basic.ActNow.length * 2;//ͬʱ��������ľ�ֵ�뷽��

	//	mn = &(pPPO->ActorNewNet);
	//	tn = &(pPPO->ActorOldNet);


	//	for (i = 0; i < layer_num; i++)
	//	{
	//		col = layer_list[i];

	//		p1 = MiaoToolBPnnLayerAppend_Dynamic(
	//			mn
	//			, AFlist[i]
	//			, row, col
	//			, pTRB
	//			, _ParaFrom(1), _ParaFrom(-1)
	//			, nl_list[i]
	//			, NULL, NULL, pmem
	//			, _ParaFrom(0)
	//		);

	//		p2 = MiaoToolBPnnLayerAppend_Dynamic(
	//			tn
	//			, AFlist[i]
	//			, row, col
	//			, pTRB
	//			, _ParaFrom(1), _ParaFrom(-1)
	//			, nl_list[i]
	//			, NULL, NULL, pmem
	//			, _ParaFrom(0)
	//		);

	//		pMiaoToolMatrixSet(&(p2->w), 0, &(p1->w));
	//		pMiaoToolMatrixSet(&(p2->BiasUN.bias), 0, &(p1->BiasUN.bias));

	//		row = col;

	//	}

	//}
	//else if (Kind == MiaoToolACtoCritic)
	//{
	//	row = pPPO->PGradBasic.basic.StateNow.length;
	//	layer_list[layer_num - 1] = 1;

	//	mn = &(pPPO->CriticNet);

	//	for (i = 0; i < layer_num; i++)
	//	{
	//		col = layer_list[i];

	//		p1 = MiaoToolBPnnLayerAppend_Dynamic(
	//			mn
	//			, AFlist[i]
	//			, row, col
	//			, pTRB
	//			, _ParaFrom(1), _ParaFrom(-1)
	//			, nl_list[i]
	//			, NULL, NULL, pmem
	//			, _ParaFrom(0)
	//		);
	//		row = col;

	//	}

	//}
	//else
	//{
	//	MIAO_assert(1);
	//}

	
}


MiaoToolPPOptim_t* MiaoToolPPOptimChoose(MiaoToolPPOptim_t *pPPO)
{


	return NULL;
}
MiaoToolPPOptim_t* MiaoToolPPOptimStoreMem(MiaoToolPPOptim_t *pPPO)
{


	return NULL;
}
MiaoToolPPOptim_t* MiaoToolPPOptimLearning(MiaoToolPPOptim_t *pPPO)
{



	return NULL;
}




#endif //MIAO_TOOL_IMPORT_RL || MIAO_TOOL_IMPORT_ALL

#endif // !(MIAO_SIMPLE_LEVEL >= 2)

