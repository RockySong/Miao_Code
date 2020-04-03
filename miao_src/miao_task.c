/*(Ver.=0.96)
 * MIAO_task.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

#if MIAO_OS_ENABLE

//MiaoTask_t MiaoInlineTaskBoring,*pta_last=&MiaoInlineTaskBoring;

//MiaoTask_t MiaoInlineTaskParameter,*pta_1=&MiaoInlineTaskParameter;

//------------------------------------------
//<������>MiaoTaskInit</������>
//<����˵��>�����ʼ������</����˵��>
//<����˵��>pta:������ƿ�ָ��  priority:���ȼ�  name:����(����:MIAO_TASK_NAME_LENGTH_MAX) ExMemLen:�ⲿ�洢���� ExMemarr:�ⲿ�洢��ַ fun:������</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void MiaoTaskInit(
    MiaoTask_t *pta,
    lw_u8 priority,
    lw_8 *name,
    lw_u16 ExMemLen,
    void *ExMemarr,
    void (*fun)(struct MIAO_TASK_STRUCT *pta)
)
{

    MIAO_assert(!pta);
    MIAO_assert(!fun);

    memset(pta,0,sizeof(MiaoTask_t));//���������ƿ�
    pta->Priority=priority;//���ȼ�
    pta->TaskFun = fun;//������

	
#if !(MIAO_SIMPLE_MODE)	
		strncpy((char *)pta->Name,(char *)name,MIAO_TASK_NAME_LENGTH_MAX);//����
#endif		
	
#if MIAO_SIMPLE_LEVEL<3 && MIAO_GIFT_EN
    MiaoGiftConciergeInitial(&(pta->Concierge));//����ϵͳ��ʼ��
    pta->Concierge.pta=pta;//��������ָ��
    if(ExMemarr!=NULL && ExMemLen!=0)//��� �ⲿ�洢��������
    {
        pta->ExternMem.uData=ExMemLen;
        pta->ExternMem.pData=ExMemarr;
    }

#endif


}

//------------------------------------------
//<������>MiaoInlineTaskBoringExe</������>
//<����˵��>����������</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
#if MIAO_STEP_RUNNING

void MiaoInlineTaskBoringExe(MiaoTask_t *_pta)
{
    static MiaoTask_t *pta=NULL;

		
		
    pta = _pta;

		pta->pOS->InitialFlagGroup.all&=(~((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1)));//�����ʼ����־λ
		pta->pOS->ReadyFlagGroup.all|=((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1));		
		pta->TaskStatus=MiaoTaskStatus_Running;

		MiaoTaskDispatch(pta->pOS);

		pta->pOS->CurrentTaskIndex=MIAO_TASK_NUM_MAX-1;
		
#if !MIAO_SPEED_RUNNING						
		while (pta->TaskGroupFlag.CurrentStatus.all) {
				MIAO_LOW_POW_ENABLE();    //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
		}
		MIAO_LOW_POW_DISABLE();
#endif

		pta->TaskGroupFlag.CurrentStatus.all = 0x01;//���			
}

#else
void MiaoInlineTaskBoringExe(MiaoTask_t *_pta)
{
    static MiaoTask_t *pta;

    pta = _pta;
	
#if !(MIAO_SIMPLE_MODE)
    Miao_5_2(pta);//���� ���������ʼ�����
    for(;;)//��ѭ��
    {

        //while(!(pta_last->PrivateMem.m8[0])) {;} //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
		pta->pOS->CurrentTaskIndex = MIAO_TASK_NUM_MAX - 1;
#if !MIAO_SPEED_RUNNING			
        while (pta->TaskGroupFlag.CurrentStatus.all) {
            MIAO_LOW_POW_ENABLE();    //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
        }
        MIAO_LOW_POW_DISABLE();
#endif
				
				
        pta->TaskGroupFlag.CurrentStatus.all = 0x01;//���	
        MiaoTaskDispatch(pta->pOS);
    }

    Miao_2_5(pta);
#else//����ģʽ
		pta->pOS->InitialFlagGroup.all&=(~((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1)));//�����ʼ����־λ
		pta->pOS->ReadyFlagGroup.all|=((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1));		
		pta->TaskStatus=MiaoTaskStatus_Running;
		do
		{
			MiaoTaskDispatch(pta->pOS);

			pta->pOS->CurrentTaskIndex=MIAO_TASK_NUM_MAX-1;
			
#if !MIAO_SPEED_RUNNING						
			while (pta->TaskGroupFlag.CurrentStatus.all) {
					MIAO_LOW_POW_ENABLE();    //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
			}
			MIAO_LOW_POW_DISABLE();
#endif

			
			pta->TaskGroupFlag.CurrentStatus.all=1;//���			
		
		}while(1);
		
		
#endif
}

#endif

//------------------------------------------
//<������>MiaoInlineTaskParameterExe</������>
//<����˵��>��������������</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------

//!!!!!!!!!!!!Ϊ�˷�ֹ���ֱ������Ķϵ㲻֧�־ֲ��������� �Ƽ�����ʹ��ȫ�ֱ���!!!!!!!!!!!!!!!!!!!!!!!!
/*(Ver.=0.96)
MiaoParaCtrl_t *ppc_1;
MiaoGiftReceive_t *pgr_1;
MiaoGiftPostman_t *pp_1;
void MiaoInlineTaskParameterExe(MiaoTask_t *pta)
{

	MIAO_TASK_LOOP(pta_1)//ѭ����ʼ
	{
		MiaoGiftReceiveWait(pta_1);//�ȴ����ݽ���
		do
		{
			pgr_1=&(pta_1->Concierge.giftR);//�õ�������
			pp_1=pgr_1->pHead;//�õ��ʵ�Ա
			pp_1->Status=pp_1->Method;//�Ѿ�ǩ��

			//�ж������б��Ƿ��������б�
//			if(pp_1->pGU->PrivateUnit.ParaCmd.ListID > MIAO_CORE_PARA_CTRL_NUM)
//			{
//				continue;
//			}

#if !(MIAO_SIMPLE_MODE)

//			ppc_1=& pta_1->pOS->ParaCtrlList[pp_1->pGU->PrivateUnit.ParaCmd.ListID];
//			ppc_1->CmdExeFun(ppc_1,(void *)(pp_1->pGU));//ֱ�ӵ���ִ�к���MiaoParaCtrlCmdExe

		}while(pgr_1->MoveNextFun(pgr_1));//ѭ���б�

#else
			MiaoParaCtrlCmdExe(ppc_1,pp_1->pGU);
			//MiaoParaCtrlCmdExe(&(pta->pOS->ParaCtrl),& (pta->Concierge.giftR.pHead->pGU->PrivateUnit.ParaCmd));
		}while(MiaoGiftReceiveMoveNext(pgr_1));
#endif


	}

}
*/
//------------------------------------------
//<������>MiaoTaskSleep</������>
//<����˵��>����˯�ߺ���</����˵��>
//<����˵��>pta:this, n :˯�߽�����</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void MiaoTaskSleep(MiaoTask_t *pta,uint32_t n)
{


    MIAO_assert(!pta);

    if(n==0)
        return;
    pta->SleepCount=n;
    pta->TaskStatus=MiaoTaskStatus_Wait_ForSleep;
    MIAO_TASK_DISPATH_EXE(pta);
}

//------------------------------------------
//<������>MiaoTaskWaitGroupFlag</������>
//<����˵��>����ȴ���־�麯��</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void MiaoTaskWaitGroupFlag(MiaoTask_t *pta)
{

    MIAO_assert(!pta);

    pta->TaskStatus=MiaoTaskStatus_Wait_ForGroupFlag;//....
    MIAO_TASK_DISPATH_EXE(pta);

}


#if MIAO_GIFT_EN

//------------------------------------------
//<������>MiaoTaskWaitGiftReceice</������>
//<����˵��>���յȴ�����</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void MiaoTaskWaitGiftReceice(MiaoTask_t *pta)
{


    MIAO_assert(!pta);

    pta->TaskStatus=MiaoTaskStatus_Wait_ForGiftReceive;//����״̬Ϊ�ȴ��������
    MIAO_TASK_DISPATH_EXE(pta);
}


//------------------------------------------
//<������>MiaoTaskWaitGiftTransmit</������>
//<����˵��>���͵ȴ�����</����˵��>
//<����˵��>pta:this/����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void MiaoTaskWaitGiftTransmit(MiaoTask_t *pta)
{

    MIAO_assert(!pta);

    pta->TaskStatus=MiaoTaskStatus_Wait_ForGiftTransmit;//....
    MIAO_TASK_DISPATH_EXE(pta);

}


//------------------------------------------
//<������>MiaoTaskGiftTransmit</������>
//<����˵��>���﷢�ͺ���</����˵��>
//<����˵��>pta:this pta_aim:Ŀ������ָ�� ID:���ԱID pgu:���ﵥԪ��ָ�� method:����ģʽ</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
#if !(MIAO_SIMPLE_LEVEL >= 3)
MiaoTask_t *MiaoTaskGiftTransmit(MiaoTask_t *pta,MiaoTask_t *pta_aim,MiaoGiftPostman_t *pGP,MiaoGiftUnit_t *pgu,GiftPostmanStatusNUM method)
{

    MiaoGiftPostman_t *p;


    MIAO_assert(!pta);
    MIAO_assert(!pta_aim);
    MIAO_assert(!pgu);

    if (pGP)
    {
        p = pGP;//ȡ�ÿ��Ա
    }
    else if(pta)
    {
        //p = MiaoGiftTransmitIdle(&(pta->Concierge.giftT));//�������һ�����Ա
        MIAO_assert(1);
        if (!p)//���ȫ����������
        {
            MIAO_assert(1);
        }
    }
    else
    {
        MIAO_assert(1);
        return NULL;
    }

    p->node.Data.pData=pgu;//��������

    p->Method=method;//������ģʽ

    p->pSource = pta;
    p->pAim = pta_aim;

    MiaoGiftReceiveAppend(&(pta_aim->Concierge.giftR),p);

    return pta;//����this
}



MiaoGiftUnit_t * MiaoTaskGetOnceGift(MiaoTask_t *pta)
{

    MiaoGiftPostman_t *pGP;

    MIAO_assert(!(pta));
    pGP = (MiaoGiftPostman_t *)pta->Concierge.giftR.List.Head.con.pNext;
    if (pGP)
    {
        return pGP->node.Data.pData;
    }

    return NULL;
}


void MiaoTaskReceOnceGift(MiaoTask_t *pta)
{

    MiaoGiftPostman_t *pGP;

    MIAO_assert(!(pta));
    pGP = (MiaoGiftPostman_t *)pta->Concierge.giftR.List.Head.con.pNext;
    if (pGP)
    {
        pGP->Status = pGP->Method;
    }

    return ;
}
#endif //MIAO_GIFT_EN
#endif // !(MIAO_SIMPLE_LEVEL >= 3)

#if MIAO_SIMPLE_LEVEL==3 && MIAO_GIFT_EN
void * MiaoTaskDelivery(MiaoTask_t *pta,MiaoMemoryBasic_t *pmem,void *src,uint32_t len)
{
	
	void *p;
	MIAO_assert(!(pmem));
	MIAO_assert(!(src));
	len+=sizeof(MiaoToolDListNodeConnect_t);
	p=pmem->Malloc(pmem,len);
	if(!pmem) 
	{
		MIAO_assert(1);
		return NULL;
	}
	((MiaoToolDListNodeConnect_t *)p)->pLast=(void *)pmem;
	
	memcpy((void *)((size_t)p+sizeof(MiaoToolDListNodeConnect_t)),src,len);

	if(pta->Concierge.pLast ==NULL)//����Ϊ��
	{
		pta->Concierge.pNext=(MiaoToolDListNodeConnect_t *)(p);
		pta->Concierge.pLast=(MiaoToolDListNodeConnect_t *)(p);	
	}
	else
	{
		pta->Concierge.pLast->pNext=(MiaoToolDListNodeConnect_t *)(p);
		pta->Concierge.pLast=(MiaoToolDListNodeConnect_t *)(p);
	}
	
	
	return p;
}

#endif



MiaoGroupAllType MiaoTaskAuctionJudge(MiaoTask_t *pta,lw_8 task_price)
{
    register uint32_t data;

    if (task_price >=0 )
    {
        data = pta->Priority + task_price;
        data = data > (MIAO_TASK_NUM_MAX - 1) ? (MIAO_TASK_NUM_MAX - 1) : data;

    }
    else
    {
        data = ((MiaoGroupAllType)1 << (MIAO_TASK_NUM_MAX - 1)) - 1;
    }

    return (pta->pOS->ReadyFlagGroup.all) & (((MiaoGroupAllType)1<<data) - 1);

}


#if !(MIAO_SIMPLE_LEVEL >= 3)
lw_u16 MiaoTaskToc(MiaoTask_t *pta) {
	lw_u16 _res = pta->BackToSleep.bits.work_time_count; 
	pta->BackToSleep.bits.timer_start_flag = 0; 
	pta->BackToSleep.bits.work_time_count = 0;
	return _res;
}
#endif

#endif


