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
//<函数名>MiaoTaskInit</函数名>
//<功能说明>任务初始化函数</功能说明>
//<输入说明>pta:任务控制块指针  priority:优先级  name:名字(长度:MIAO_TASK_NAME_LENGTH_MAX) ExMemLen:外部存储长度 ExMemarr:外部存储地址 fun:任务函数</输入说明>
//<输出说明>void</输出说明>
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

    memset(pta,0,sizeof(MiaoTask_t));//清空任务控制块
    pta->Priority=priority;//优先级
    pta->TaskFun = fun;//任务函数

	
#if !(MIAO_SIMPLE_MODE)	
		strncpy((char *)pta->Name,(char *)name,MIAO_TASK_NAME_LENGTH_MAX);//名字
#endif		
	
#if MIAO_SIMPLE_LEVEL<3 && MIAO_GIFT_EN
    MiaoGiftConciergeInitial(&(pta->Concierge));//门卫系统初始化
    pta->Concierge.pta=pta;//门卫所属指针
    if(ExMemarr!=NULL && ExMemLen!=0)//如果 外部存储器有意义
    {
        pta->ExternMem.uData=ExMemLen;
        pta->ExternMem.pData=ExMemarr;
    }

#endif


}

//------------------------------------------
//<函数名>MiaoInlineTaskBoringExe</函数名>
//<功能说明>空闲任务函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
#if MIAO_STEP_RUNNING

void MiaoInlineTaskBoringExe(MiaoTask_t *_pta)
{
    static MiaoTask_t *pta=NULL;

		
		
    pta = _pta;

		pta->pOS->InitialFlagGroup.all&=(~((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1)));//清除初始化标志位
		pta->pOS->ReadyFlagGroup.all|=((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1));		
		pta->TaskStatus=MiaoTaskStatus_Running;

		MiaoTaskDispatch(pta->pOS);

		pta->pOS->CurrentTaskIndex=MIAO_TASK_NUM_MAX-1;
		
#if !MIAO_SPEED_RUNNING						
		while (pta->TaskGroupFlag.CurrentStatus.all) {
				MIAO_LOW_POW_ENABLE();    //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
		}
		MIAO_LOW_POW_DISABLE();
#endif

		pta->TaskGroupFlag.CurrentStatus.all = 0x01;//清除			
}

#else
void MiaoInlineTaskBoringExe(MiaoTask_t *_pta)
{
    static MiaoTask_t *pta;

    pta = _pta;
	
#if !(MIAO_SIMPLE_MODE)
    Miao_5_2(pta);//声明 空闲任务初始化完成
    for(;;)//死循环
    {

        //while(!(pta_last->PrivateMem.m8[0])) {;} //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
		pta->pOS->CurrentTaskIndex = MIAO_TASK_NUM_MAX - 1;
#if !MIAO_SPEED_RUNNING			
        while (pta->TaskGroupFlag.CurrentStatus.all) {
            MIAO_LOW_POW_ENABLE();    //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
        }
        MIAO_LOW_POW_DISABLE();
#endif
				
				
        pta->TaskGroupFlag.CurrentStatus.all = 0x01;//清除	
        MiaoTaskDispatch(pta->pOS);
    }

    Miao_2_5(pta);
#else//极简模式
		pta->pOS->InitialFlagGroup.all&=(~((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1)));//清除初始化标志位
		pta->pOS->ReadyFlagGroup.all|=((MiaoGroupAllType)1<<(MIAO_TASK_NUM_MAX-1));		
		pta->TaskStatus=MiaoTaskStatus_Running;
		do
		{
			MiaoTaskDispatch(pta->pOS);

			pta->pOS->CurrentTaskIndex=MIAO_TASK_NUM_MAX-1;
			
#if !MIAO_SPEED_RUNNING						
			while (pta->TaskGroupFlag.CurrentStatus.all) {
					MIAO_LOW_POW_ENABLE();    //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
			}
			MIAO_LOW_POW_DISABLE();
#endif

			
			pta->TaskGroupFlag.CurrentStatus.all=1;//清除			
		
		}while(1);
		
		
#endif
}

#endif

//------------------------------------------
//<函数名>MiaoInlineTaskParameterExe</函数名>
//<功能说明>参数管理任务函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------

//!!!!!!!!!!!!为了防止部分编译器的断点不支持局部变量保存 推荐尽量使用全局变量!!!!!!!!!!!!!!!!!!!!!!!!
/*(Ver.=0.96)
MiaoParaCtrl_t *ppc_1;
MiaoGiftReceive_t *pgr_1;
MiaoGiftPostman_t *pp_1;
void MiaoInlineTaskParameterExe(MiaoTask_t *pta)
{

	MIAO_TASK_LOOP(pta_1)//循环开始
	{
		MiaoGiftReceiveWait(pta_1);//等待数据介绍
		do
		{
			pgr_1=&(pta_1->Concierge.giftR);//得到接收器
			pp_1=pgr_1->pHead;//得到邮递员
			pp_1->Status=pp_1->Method;//已经签收

			//判断请求列表是否长于已有列表
//			if(pp_1->pGU->PrivateUnit.ParaCmd.ListID > MIAO_CORE_PARA_CTRL_NUM)
//			{
//				continue;
//			}

#if !(MIAO_SIMPLE_MODE)

//			ppc_1=& pta_1->pOS->ParaCtrlList[pp_1->pGU->PrivateUnit.ParaCmd.ListID];
//			ppc_1->CmdExeFun(ppc_1,(void *)(pp_1->pGU));//直接调用执行函数MiaoParaCtrlCmdExe

		}while(pgr_1->MoveNextFun(pgr_1));//循环列表

#else
			MiaoParaCtrlCmdExe(ppc_1,pp_1->pGU);
			//MiaoParaCtrlCmdExe(&(pta->pOS->ParaCtrl),& (pta->Concierge.giftR.pHead->pGU->PrivateUnit.ParaCmd));
		}while(MiaoGiftReceiveMoveNext(pgr_1));
#endif


	}

}
*/
//------------------------------------------
//<函数名>MiaoTaskSleep</函数名>
//<功能说明>任务睡眠函数</功能说明>
//<输入说明>pta:this, n :睡眠节拍数</输入说明>
//<输出说明>void</输出说明>
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
//<函数名>MiaoTaskWaitGroupFlag</函数名>
//<功能说明>任务等待标志组函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void MiaoTaskWaitGroupFlag(MiaoTask_t *pta)
{

    MIAO_assert(!pta);

    pta->TaskStatus=MiaoTaskStatus_Wait_ForGroupFlag;//....
    MIAO_TASK_DISPATH_EXE(pta);

}


#if MIAO_GIFT_EN

//------------------------------------------
//<函数名>MiaoTaskWaitGiftReceice</函数名>
//<功能说明>接收等待函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void MiaoTaskWaitGiftReceice(MiaoTask_t *pta)
{


    MIAO_assert(!pta);

    pta->TaskStatus=MiaoTaskStatus_Wait_ForGiftReceive;//任务状态为等待礼物接收
    MIAO_TASK_DISPATH_EXE(pta);
}


//------------------------------------------
//<函数名>MiaoTaskWaitGiftTransmit</函数名>
//<功能说明>发送等待函数</功能说明>
//<输入说明>pta:this/输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void MiaoTaskWaitGiftTransmit(MiaoTask_t *pta)
{

    MIAO_assert(!pta);

    pta->TaskStatus=MiaoTaskStatus_Wait_ForGiftTransmit;//....
    MIAO_TASK_DISPATH_EXE(pta);

}


//------------------------------------------
//<函数名>MiaoTaskGiftTransmit</函数名>
//<功能说明>礼物发送函数</功能说明>
//<输入说明>pta:this pta_aim:目标任务指针 ID:快递员ID pgu:礼物单元的指针 method:发送模式</输入说明>
//<输出说明>void</输出说明>
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
        p = pGP;//取得快递员
    }
    else if(pta)
    {
        //p = MiaoGiftTransmitIdle(&(pta->Concierge.giftT));//随机分配一个快递员
        MIAO_assert(1);
        if (!p)//如果全部被用上了
        {
            MIAO_assert(1);
        }
    }
    else
    {
        MIAO_assert(1);
        return NULL;
    }

    p->node.Data.pData=pgu;//带上礼物

    p->Method=method;//告诉他模式

    p->pSource = pta;
    p->pAim = pta_aim;

    MiaoGiftReceiveAppend(&(pta_aim->Concierge.giftR),p);

    return pta;//返回this
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

	if(pta->Concierge.pLast ==NULL)//链表为空
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


