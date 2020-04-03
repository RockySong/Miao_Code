/*(Ver.=0.96)
 * MIAO_gift.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

#if MIAO_OS_ENABLE && MIAO_GIFT_EN

#if !(MIAO_SIMPLE_LEVEL >= 3)
//------------------------------------------
//<函数名>MiaoGiftConciergeInitial</函数名>
//<功能说明>门卫管理系统 初始化</功能说明>
//<输入说明>pgc:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------


void MiaoGiftConciergeInitial(MiaoGiftConcierge_t *pgc)
{

    MIAO_assert(!pgc);
    memset(pgc,0,sizeof(MiaoGiftConcierge_t));//初始化清零
    MiaoToolDListInitial(&(pgc->giftR.List), 0, NULL);
#if !(MIAO_SIMPLE_MODE)
    pgc->giftR.MoveNextFun=MiaoGiftReceiveMoveNext;//双向链表移动成员函数
    pgc->giftR.SlipNextFun=MiaoGiftReceiveSlipNext;//滑动函数
    pgc->giftR.AppendFun=MiaoGiftReceiveAppend;//追加成员函数
#endif
    pgc->giftR.pGC=pgc;//接收器 指针

    //MiaoGiftTransmitInitial(&(pgc->giftT));

    // pgc->giftT.pGC=pgc;//发送器 指针

}


//逐渐放弃 采用内存分配机制代替
#if 0
void MiaoGiftTransmitInitial(MiaoGiftTransmit_t *pgt)
{
    int n=0;

    MIAO_assert(!pgt);
#if !(MIAO_SIMPLE_MODE)
    pgt->IdleFun=MiaoGiftTransmitIdle;//得到空闲成员索引函数
#endif

    for(; n<MIAO_GIFT_POSTMAN_MAX; n++) //快递员初始化
    {
        pgt->PostmanList[n].PostmanID = n;
        pgt->PostmanList[n].Status = GiftPostmanStatus_Free;//设置状态为自由
        pgt->PostmanList[n].pGT = pgt;//快递员 所属指针初始化
    }

}
#endif


//------------------------------------------
//<函数名>MiaoGiftReceiveAppend</函数名>
//<功能说明>礼物发送函数</功能说明>
//<输入说明>pgr:this pgp:快递员指针</输入说明>
//<输出说明>双向链表头部</输出说明>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveAppend(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t *pgp)
{

    MIAO_assert(!pgr);
    MIAO_assert(!pgp);

#if 0

    if(!(pgr->GiftNum))//判断当前礼物数量是否==0
    {   //为0

        pgr->pHead=pgr->pTail=pgp;//首尾相同
        pgr->pHead->pPrior=NULL;//上一个为空
    }
    else
    {   //不为0
        pgr->pTail->pNext=pgp;//当前尾部的下一个接上
        pgp->pPrior=pgr->pTail;//pgp的前一个为当前尾部
        pgr->pTail=pgp;	//更新尾部
    }

    pgp->pGT->PostmanFlag.all|=((lw_u64)1<<pgp->ID);//发送标志组置起

    pgp->Status=GiftPostmanStatus_InDelivery;//快递员状态为正在投递
    pgr->pTail->pNext=NULL;//尾部下一个为空
    pgr->GiftMax++;//最大数++
    pgr->GiftNum++;//礼物数量++

    return pgr->pHead;//返回头部
#endif // 0

    pgr->List.AppendFUN(&(pgr->List), &(pgp->node));

    //逐渐放弃 采用内存分配机制代替
#if 0
    if(pgp->pGT)//说明是有主邮递员
    {
        pgp->pGT->PostmanFlag.all |= ((lw_u64)1 << pgp->PostmanID);//发送标志组置起
        if(pgp->Status!=GiftPostmanStatus_Free)//如果快递员 状态为自由 才可以放入等待链表
        {
            //如果不为自由
            pgr->pGC->pta->pOS->CoreError=MiaoCoreError_GiftStatusError;//将内核错误标志置起
            return NULL;//返回NULL
        }

    }
#endif

    pgp->Status = GiftPostmanStatus_InDelivery;//快递员状态为正在投递
    return pgp;

}


//------------------------------------------
//<函数名>MiaoGiftReceiveMoveNext</函数名>
//<功能说明>礼物接收器移动 用于 do{...}while(..move..)循环</功能说明>
//<输入说明>pgr:this</输入说明>
//<输出说明>当前头部</输出说明>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveMoveNext(MiaoGiftReceive_t *pgr)
{

    MiaoGiftPostman_t *p,*q;
//    MiaoMemoryBasic_t *pmem;

    MIAO_assert(!pgr);

    if(pgr->List.NowLen<=1)//如果数量已经为0 不用再移动
    {
        return NULL;
    }
#if 0
    p=pgr->pHead;//取得同步
    if(pgr->GiftNum==1)//如果数量已经==1
    {
        pgr->pHead=NULL;//释放头部
        pgr->pTail=NULL;//释放尾部
        q=NULL;//返回NULL 因为采用do {}while() 循环
    }
    else
    {
        pgr->pHead=p->pNext;//头部下一个
        q=pgr->pHead;//返回当前头部
    }
    pgr->GiftNum--;//礼物数量--

    if(p->Status == p->Method)//如果之前任务状态 与 模式系统
    {
        p->pGT->PostmanFlag.all &= ~((lw_u64)1<<p->ID);//清除发送标志
        p->Status=GiftPostmanStatus_Free;//并且状态为自由

    }
#endif // 0


    p = (MiaoGiftPostman_t *)pgr->List.Head.con.pNext;//取得除了头结点的第一个

    if (!p)
    {
        return NULL;
    }
    //逐渐放弃 采用内存分配机制代替
#if 0
    if (p->Status == p->Method && (p->pGT))//如果任务状态 与 目标模式一致 且 邮递员有主
    {
        p->pGT->PostmanFlag.all &= ~((lw_u64)1 << p->PostmanID);//清除发送标志
        p->Status = GiftPostmanStatus_Free;//并且状态为自由

    }
#endif

    q = (MiaoGiftPostman_t *)(pgr->List.RemoveFUN(&(pgr->List), MiaoToolDListCmpCount, 1, NULL));


    return q;
}



//------------------------------------------
//<函数名>MiaoGiftReceiveSlipNext</函数名>
//<功能说明>礼物接收器移动 用于 do{...}while(..slip(,&pgp)..)循环</功能说明>
//<输入说明>pgr:this,ppGP:指向指针的指针</输入说明>
//<输出说明>修改之后的指针值</输出说明>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveSlipNext(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t **ppGP)
{
    MiaoGiftPostman_t *p;

    MIAO_assert(!pgr);
    MIAO_assert(!ppGP);
    MIAO_assert(!(*ppGP));

    p=(MiaoGiftPostman_t *)((*ppGP)->node.con.pNext);

    /*(Ver.=0.96)
    if(_MiaoToolDListRemoveNode(&(pgr->List),(MiaoToolDListNode_t *)(*ppGP)))
    {
    	return NULL;
    }
    */



    if((*ppGP)->Method == (*ppGP)->Status && (*ppGP)->pmem)
    {
        _MiaoToolDListRemoveNode(&(pgr->List),(MiaoToolDListNode_t *)(*ppGP));

        if(!((*ppGP)->pmem->Free((*ppGP)->pmem,(*ppGP))))//如果释放失败
        {

            (*ppGP)=p;
            return p;

        }
    }

    (*ppGP)=p;
    return *ppGP;
}


//------------------------------------------
//<函数名>MiaoGiftReceiveForeah</函数名>
//<功能说明>遍历礼物/功能说明>
//<输入说明>pgr:this,ppGP:指向指针的指针</输入说明>
//<输出说明>下一个礼物</输出说明>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftReceiveForeah(MiaoGiftReceive_t *pgr,MiaoGiftPostman_t **ppGP)
{
    MiaoGiftPostman_t *p;

    MIAO_assert(!pgr);
    MIAO_assert(!ppGP);
    MIAO_assert(!(*ppGP));

    p=(MiaoGiftPostman_t *)((*ppGP)->node.con.pNext);
	
		if((*ppGP) != (MiaoGiftPostman_t *)(&(pgr->List.Head)))
		{
			if((*ppGP)->Method == (*ppGP)->Status && (*ppGP)->pmem)
			{
					_MiaoToolDListRemoveNode(&(pgr->List),(MiaoToolDListNode_t *)(*ppGP));

					if(!((*ppGP)->pmem->Free((*ppGP)->pmem,(*ppGP))))//如果释放失败
					{

							(*ppGP)=p;
							return p;

					}
			}
			 
		}
		(*ppGP)=p;
    return *ppGP;
}

//逐渐放弃 采用内存分配机制代替
#if 0
//------------------------------------------
//<函数名>MiaoGiftTransmitIdleIndex</函数名>
//<功能说明>得到空闲邮递员函数</功能说明>
//<输入说明>pgt:this</输入说明>
//<输出说明>空闲邮递员的ID 如果返回-1表示没有空闲</输出说明>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftTransmitIdle(MiaoGiftTransmit_t *pgt)
{

    lw_u64 sta=pgt->PostmanFlag.all;

    lw_8 n=0;

    while(((1<<n)&sta) && n<MIAO_GIFT_POSTMAN_MAX) n++;//循环判断邮递员 如果判断数量超过 或者找到了 就停止循环

    return n<MIAO_GIFT_POSTMAN_MAX? (pgt->PostmanList+n):NULL;

}
#endif


//------------------------------------------
//<函数名>MiaoGiftAllocPostman</函数名>
//<功能说明>申请空闲邮递员</功能说明>
//<输入说明>pmem:内存分配函数</输入说明>
//<输出说明>NUL表示没有</输出说明>
//------------------------------------------
MiaoGiftPostman_t * MiaoGiftAllocPostman(MiaoMemoryBasic_t *pmem)
{

    MiaoGiftPostman_t *pgp;

    MIAO_assert(!pmem);

    pgp=(MiaoGiftPostman_t *)pmem->Malloc(pmem,sizeof(MiaoGiftPostman_t));

    if(!pgp)
    {
        return pgp;
    }

    pgp->pmem=pmem;
    pgp->Method=GiftPostmanStatus_Received;


    return pgp;

}

#endif // !(MIAO_SIMPLE_LEVEL >= 3)

#if MIAO_SIMPLE_LEVEL ==3

void * MiaoGiftReceiveMoveNext(MiaoToolDListNodeConnect_t *pgr)
{
	MiaoToolDListNodeConnect_t *p;
	MiaoMemoryBasic_t *pmem;
	if(pgr->pLast == NULL || pgr->pNext == NULL)
	{
		return NULL;
	}
	
	p=pgr->pNext;
	pmem=(MiaoMemoryBasic_t *)(p->pLast);
	
	if(pgr->pLast == pgr->pNext)//只要一个消息
	{
		
		pgr->pLast=NULL;
		pgr->pNext=NULL;
		pmem->Free(pmem,p);
		
		return NULL;
	}
	else
	{
		pgr->pNext=p->pNext;
		pmem->Free(pmem,p);
		return 	pgr->pNext;	
	}
	
	
//	return NULL;
}

#endif


#endif
