/*(Ver.=0.96)
 * MIAO_memory.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

lw_32 MiaoMemorySizeInitial(MiaoMemSize_t *ps, lw_u16 len)
{
#if MIAO_MEM_ALLOC_FAST
	int i = 15;
	len = len < sizeof(int) ? sizeof(int) : len;
	do
	{
		if (len & ((lw_u16)1 << i ))
		{
			break;
		}

	} while (--i);

	
	if (!_MIAO_IS_2_POW(len))//不是2^n 要再+1
	{
		i++;
	}
	i = i > 15 ? 15 : i;//限幅

	ps->MovNum = i;
	ps->ModNum = (((uint32_t)1) << (i)) - 1;
	ps->SectionSize = MIAO_MEM_SIZE_MUL(1, *ps);
#else
    ps->SectionSize = len;
		return 0;
#endif // MIAO_MEM_ALLOC_FAST

}

void * MiaoMemoryGlobalCalloc(struct MIAO_MEMORY_BASIC_STRUCT *pMU, uint32_t dsize)
{

	void *p;
	MIAO_assert(!(pMU));
	p = pMU->Malloc(pMU, dsize);

	MIAO_assert(!(p));

	memset(p, 0, dsize);

	return p;
}

#if MIAO_MEMORY_IMPORT_MAP

lw_32 MiaoMemoryMapInital
(
    MiaoMemoryMap_t *pMU,//存储器结构体
    void *pMem,//内存池地址 总长度=lenMem*(MemSize+sizeof(void *))
    lw_u16 lenMem,//内存片数量
    lw_u16 MemSize,//内存片大小(sizeof)
    uint32_t sizeofmem
)
{
	void *pMemInfo = pMem;
    uint32_t reallen;
    lw_u16 lenMemInfo;

    MIAO_assert(!(pMU));
    MIAO_assert(!(pMem));
    MIAO_assert(!(MemSize));
    MIAO_assert(!(sizeofmem));


    lenMemInfo=lenMem;
	MiaoMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize) + lenMem * sizeof(pMU->Mem.pData);
	if (reallen > sizeofmem)//重新划分片大小
	{
		//MIAO_assert(1);//此情况为异常 需要提醒用户
		lenMem = sizeofmem / (pMU->basic.MemSectionSize.SectionSize + sizeof(pMU->Mem.pData));
		reallen = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize) + lenMem * sizeof(pMU->Mem.pData);
	}


	pMU->Mem.pData = (void *)((size_t)pMem + (lenMem * sizeof(pMU->Mem.pData)));
    pMU->Mem.uData=lenMem;//为了编程方便没有使用真长度
    pMU->MemInfo.pData=pMemInfo;
    pMU->MemInfo.uData=lenMemInfo;//为了编程方便没有使用真长度
	pMU->pMemInfoButtom = (void *)((size_t)pMU->MemInfo.pData + pMU->MemInfo.uData * sizeof(pMU->Mem.pData));


    pMU->basic.Malloc=(void *)MiaoMemoryMapAlloc;
    pMU->basic.Free=(void *)MiaoMemoryMapFree;
    pMU->basic.ReAlloc = (void *)MiaoMemoryMapReAlloc;
	pMU->basic.Calloc = (void *)MiaoMemoryGlobalCalloc;
    pMU->basic.memMax = lenMem;//内存片总大小
    pMU->basic.memSurplus = pMU->basic.memMax;//剩余大小

	pMU->pMemButtom = (void *)((size_t)pMem + reallen);

	memset(pMem, 0, reallen);

	return reallen - sizeofmem;

}

void* MiaoMemoryMapAlloc(MiaoMemoryMap_t *pMU,uint32_t dsize)
{

    size_t parr,phead= (size_t)NULL;
    void **pinfoarr;
    uint32_t len;
    lw_u16 count=0;


    //lw_u16 *pMemButtom=(lw_u16 *)pMU->Mem.pData+pMU->Mem.Data*pMU->MemMapSize,*pMemInfoButtom=(lw_u16 *)pMU->MemInfo.pData+pMU->MemInfo.Data*sizeof(pMU->Mem.pData);

    MIAO_assert(!(pMU));


    parr = (size_t)(pMU->Mem.pData);
    pinfoarr = pMU->MemInfo.pData;
    if (dsize == 0)
    {
        return NULL;
    }

    //len = dsize / pMU->MemSectionSize;
    len = MIAO_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);

    //得到比用户预订内存要大的内存片数量
    /*(Ver.=0.96)
    if(len * pMU->MemSectionSize < dsize)
    	len++;
    	*/

	//得到比用户预订内存要大的内存片数量
	if (MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize) < dsize)
		len++;

    if (pMU->basic.memSurplus < len)//没有足够的内存 不用找了
    {
        return NULL;
    }

    do {
        //如果该内存无人使用
        if((*pinfoarr)==NULL)
        {
            count++;//计数器++
            if(phead == (size_t)(NULL))//如果之前没有找到过
                phead=parr;//保存下内存片头部
        }
        else//如果已经有人使用
        {
            count=0;//计数器清零
            phead= (size_t)NULL;//头部释放
        }
        if(count==len)//如果找到了足够数量的内存片
        {
            while (count--) *pinfoarr-- = (void *)(phead);//将内存片赋予
            count=len;//计数器恢复长度
            break;
        }


    } while((parr+=pMU->basic.MemSectionSize.SectionSize) < (size_t)(pMU->pMemButtom) && (size_t)(pinfoarr++) < (size_t)(pMU->pMemInfoButtom));

    if(count == len)//计数器长度 == 要求长度
    {
        pMU->basic.memSurplus -= len;
        return (void *)phead;
    }
    else
    {
        return NULL;
    }

    //return NULL;

}

//------------------------------------------
//<函数名>_MiaoMemoryMapFind</函数名>
//<功能说明>映射形内存分配 查找函数</功能说明>
//<输入说明>pmu 内存分配器 p:申请的内存 pLen(可为NULL):当前指针的内存 pNum(可为NULL):当前指针的内存+之后的空闲内存片数量 </输入说明>
//<输出说明>lw_32 内存片对应的索引 -1表示没有该内存</输出说明>
//------------------------------------------
static lw_32 _MiaoMemoryMapFind(MiaoMemoryMap_t *pMU, void *p,lw_u16 *pLen,lw_u16 *pNum)
{
    lw_u16 count = 0, len = 0;
    lw_u16 headernum;
    void **pinfoarr;

    MIAO_assert(!(pMU));
    MIAO_assert(!(p));

    //不在范围之内 找不到
    if
    (
        p < pMU->Mem.pData
        ||
        p > pMU->pMemButtom
    )
    {
        return (lw_32)-1;
    }

    //headernum = (size_t)((size_t)p - (size_t)(pMU->Mem.pData))/((size_t)(pMU->MemSectionSize));//计算偏移
	headernum = (lw_u16)(MIAO_MEM_SIZE_DIV((size_t)((size_t)p - (size_t)(pMU->Mem.pData)), (pMU->basic.MemSectionSize))); //计算偏移
    if (pNum && pLen)
    {
        pinfoarr = pMU->MemInfo.pData;
        pinfoarr += headernum;
        while ((*pinfoarr == p) || (*pinfoarr==NULL) && (pinfoarr != pMU->pMemInfoButtom))
        {
            if (*pinfoarr == p)
            {
                len++;
            }
            count++;
            pinfoarr++;
        }

        *pNum = count;
        *pLen = len;
    }
    return (lw_32)headernum;

}

void * MiaoMemoryMapFree(MiaoMemoryMap_t *pMU,void *p)
{
    /*(Ver.=0.96)
    	lw_u8 **pinfoarr=pMU->MemInfo.pData;
    	lw_u8 **pMemInfoButtom;
    	lw_u8 FindFlag = 0;
    	uint32_t count = 0;

    	//不在范围之内 不可以释放
    	if
    	(
    		p < pMU->Mem.pData
    		||
    		p > pMU->pMemButtom
    	)
    	{
    		return NULL;
    	}

    	do{
    		//如果该内存的使用者是p
    		if(*pinfoarr == p)
    		{
    			*pinfoarr = NULL;//恢复为空
    			count++;
    			FindFlag = 1;//找寻到标志位为1
    		}
    		else if(FindFlag)//如果没有找到且标志位为1 说明查找完毕
    		{
    			break;
    		}

    	}while((pinfoarr++) < (lw_u8 **)(pMU->pMemInfoButtom));

    	pMU->basic.memSurplus += count;
    	*/

    lw_32 findres;
    uint32_t count = 0;
    void **pinfoarr;

    MIAO_assert(!(pMU));


    findres = _MiaoMemoryMapFind(pMU, p, NULL,NULL);
    if (findres < 0)//没有找到
    {
        return NULL;
    }
    pinfoarr = ((void **)pMU->MemInfo.pData)+ findres;

    while (*pinfoarr == p)
    {
        *pinfoarr = NULL;//恢复为空
        count++;
        pinfoarr++;
    }
    pMU->basic.memSurplus += count;


    return p;

}

void* MiaoMemoryMapReAlloc(MiaoMemoryMap_t *pMU,void *p, uint32_t dsize)
{
    lw_u16 count = 0, len, nowlen;
    lw_32 index;
    void **pinfoarr;
    void *pAlloc;

    MIAO_assert(!(pMU));


    if (p==NULL)//直接分配内存
    {
        return MiaoMemoryMapAlloc(pMU, dsize);
    }

    if (dsize == 0)//直接释放内存
    {
        return MiaoMemoryMapFree(pMU, p);
    }

    index = _MiaoMemoryMapFind(pMU, p, &nowlen, &count);
    if (index < 0 || count ==0)
    {
        MIAO_assert(1);
        return NULL;
    }

    //len = dsize / pMU->MemSectionSize;
    len = MIAO_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);
    //得到比用户预订内存要大的内存片数量
    /*(Ver.=0.96)
    if (len * pMU->MemSectionSize < dsize)
    	len++;
    */

	//得到比用户预订内存要大的内存片数量
	if (MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize) < dsize)
		len++;
    if (count >= len)//不需要重新分配
    {

        if (nowlen > len)//当前长度大于要求长度 需要释放多余长度
        {
            pinfoarr = ((void **)pMU->MemInfo.pData) + index + nowlen - 1;//尾部
            count = nowlen - len;
            pMU->basic.memSurplus += count;
            do
            {
                *pinfoarr-- = NULL;
            } while (--count);

        }
        else if (nowlen < len)//当前长度小于要求长度 申请长度
        {
            pinfoarr = ((void **)pMU->MemInfo.pData) + index + len - 1;//尾部
            count = len - nowlen;
            pMU->basic.memSurplus -= count;
            do
            {
                *pinfoarr-- = p;
            } while (--count);


        }
        else;//不需要处理
        return p;
    }
    else//重新分配
    {
        pAlloc = MiaoMemoryMapAlloc(pMU, dsize);//分配
        memcpy(pAlloc, p, MIAO_MEM_SIZE_MUL(nowlen, pMU->basic.MemSectionSize));//拷贝内存数据

        //释放数据 不用调用free 节约时间
        pinfoarr = ((void **)pMU->MemInfo.pData) + index;

        count = 0;
        while (*pinfoarr == p)
        {
            *pinfoarr = NULL;//恢复为空
            count++;
            pinfoarr++;
        }
        pMU->basic.memSurplus += count;

        return pAlloc;
    }

   // MIAO_assert(1);

   // return NULL;

}

#endif //MIAO_MEMORY_IMPORT_MAP



#if MIAO_MEMORY_IMPORT_SIMPLE

lw_32 MiaoMemorySimpleInital(MiaoMemorySimple_t *pMU,
                             void *pMem,
                             lw_u16 lenMem,
                             lw_u16 MemSize,
                             uint32_t sizeofmem
                            )
{
    uint32_t reallen;
    MIAO_assert(!(pMU));
    MIAO_assert(!(pMem));


    pMU->basic.Malloc = (void *)MiaoMemorySimpleAlloc;
    pMU->basic.Free = (void *)MiaoMemorySimpleFree;
    pMU->basic.ReAlloc = (void *)MiaoMemorySimpleReAlloc;
	pMU->basic.Calloc = (void *)MiaoMemoryGlobalCalloc;

    //pMU->MemSectionSize = MemSize;
    MiaoMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MemSize*lenMem;

	if (reallen > sizeofmem)
	{
		MIAO_assert(1);//此情况为异常 需要提醒用户
		lenMem = sizeofmem / pMU->basic.MemSectionSize.SectionSize;
		reallen = MemSize*lenMem;
	}



    pMU->basic.memSurplus = pMU->basic.memMax = lenMem;

    pMU->Mem.pData = pMem;
    pMU->pMemNow=(size_t)pMem;
    pMU->Mem.uData = lenMem;


	pMU->pMemButtom = (size_t)pMem + reallen;

	memset(pMem, 0, reallen);

	return reallen - sizeofmem;
}


void * MiaoMemorySimpleAlloc(MiaoMemorySimple_t *pMU, uint32_t dsize)
{
    uint32_t len, offsetlen = 0;
    void *pbuf = NULL;
    MIAO_assert(!(pMU));

    //len = dsize / pMU->MemSectionSize;
    len = MIAO_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);
    //得到比用户预订内存要大的内存片数量
    /*(Ver.=0.96)
    if (len * pMU->MemSectionSize < dsize)
    	len++;
    */
    //offsetlen = len* pMU->MemSectionSize;//得到内存实际偏移量

	//得到比用户预订内存要大的内存片数量
	if (MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize) < dsize)
		len++;

    offsetlen = MIAO_MEM_SIZE_MUL(len, pMU->basic.MemSectionSize);//得到内存实际偏移量

    if (pMU->pMemNow + offsetlen < pMU->pMemButtom)
    {
        pbuf = (void *)(pMU->pMemNow);
        pMU->pMemNow += offsetlen;
        pMU->basic.memSurplus -= len;
        return pbuf;

    }
    else
    {
        return NULL;
    }

}


void * MiaoMemorySimpleFree(MiaoMemorySimple_t *pMU, void *p)
{
    MIAO_assert(1);
    //该内存分配不应该释放 需要提醒用户

    return NULL;

}

void* MiaoMemorySimpleReAlloc(MiaoMemorySimple_t *pMU, void *p, uint32_t dsize)
{
    MIAO_assert(1);
    //该内存分配不应该重新分配 需要提醒用户

    return NULL;
}

#endif //MIAO_MEMORY_IMPORT_SIMPLE



#if MIAO_MEMORY_IMPORT_LIST

lw_32 MiaoMemoryListInital(MiaoMemoryList_t *pMU,
                           void *pMem,
                           lw_u16 lenMem,
                           lw_u16 MemSize,
                           uint32_t sizeofmem
                          )
{
    uint32_t reallen;
    //lw_32 data_return;
    uint32_t dsize = 0;
//    lw_u16 len;
    //MiaoMemoryListNode_t *ptail;

    MIAO_assert(!(pMU));
    MIAO_assert(!(pMem));
    MIAO_assert(!(MemSize));
    MIAO_assert(!(sizeofmem));


    pMU->basic.Malloc = (void *)MiaoMemoryListAlloc;
    pMU->basic.Free = (void *)MiaoMemoryListFree;
    pMU->basic.ReAlloc = (void *)MiaoMemoryListReAlloc;
	pMU->basic.Calloc = (void *)MiaoMemoryGlobalCalloc;
    //pMU->MemSectionSize = MemSize;

    MiaoMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MIAO_MEM_SIZE_MUL (lenMem, pMU->basic.MemSectionSize)+  sizeof(MiaoMemoryListNode_t);
	if (reallen > sizeofmem)
	{
		//MIAO_assert(1);//此情况为异常 需要提醒用户
		lenMem = (sizeofmem - sizeof(MiaoMemoryListNode_t)) / pMU->basic.MemSectionSize.SectionSize;
		reallen = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize) + sizeof(MiaoMemoryListNode_t);
	}


    pMU->Mem.uData = lenMem;
    pMU->Mem.pData = pMem;
	pMU->basic.memSurplus = pMU->basic.memMax = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize);
	pMU->pMemButtom = (void *)((size_t)pMem + reallen);
	memset(pMem, 0, reallen);
	pMU->basic.memSurplus = pMU->basic.memMax = MIAO_MEM_SIZE_MUL(lenMem, pMU->basic.MemSectionSize);


    pMU->ptail = (MiaoMemoryListNode_t *)((size_t)pMU->pMemButtom - sizeof(MiaoMemoryListNode_t));
    pMU->phead = pMem;

    MIAO_TOOL_DLIST_NODE_CONNECT(pMU->phead, pMU->ptail);

    MIAO_MEM_LIST_STD_SIZE(pMU, dsize);//内存大小规范化

    pMU->phead->len = dsize;
    pMU->basic.memSurplus -= dsize;

	return reallen - sizeofmem;

}


void * MiaoMemoryListAlloc(MiaoMemoryList_t *pMU, uint32_t dsize)
{

    MiaoMemoryListNode_t *p, *q;
    void *preturn = NULL;
		register size_t buf1;

    MIAO_assert(!(pMU));
    MIAO_assert(!(dsize));
    /*(Ver.=0.96)
    MemSectionSize = pMU->MemSectionSize;
    dsize += sizeof(MiaoMemoryListNode_t);
    len = dsize / MemSectionSize;

    //得到比用户预订内存要大的内存片数量
    if ((uint32_t)len * MemSectionSize < dsize)
    	len++;

    dsize = (uint32_t)len *MemSectionSize;
    */

    if (dsize == 0)
    {
        return NULL;
    }


    MIAO_MEM_LIST_STD_SIZE(pMU, dsize);//内存大小规范化

    if (pMU->basic.memSurplus < dsize)//没有足够的内存 不用找了
    {
        return NULL;
    }

    p = pMU->phead;
    do
    {

        if (p < pMU->phead || p>pMU->ptail )//p越界代表异常
        {
            MIAO_assert(1);
            return NULL;
        }
				buf1=((size_t)(p->con.pNext) - (size_t)(p) - p->len);
        if (buf1 >= dsize)//代表有剩余
        {
            q = (MiaoMemoryListNode_t *)(((size_t)p) + p->len);
            /*(Ver.=0.96)
            			q->con.pLast = (MiaoToolDListNodeConnect_t *)p;
            			q->con.pNext = (MiaoToolDListNodeConnect_t *)p->con.pNext;

            			p->con.pNext->pLast = (MiaoToolDListNodeConnect_t *)q;
            			p->con.pNext = (MiaoToolDListNodeConnect_t *)q;
            */
            MIAO_TOOL_DLIST_NODE_INSERT(p, q);

            q->len = dsize;
            preturn = (void *)((size_t)q + sizeof(MiaoMemoryListNode_t));
            pMU->basic.memSurplus -= dsize;

            break;
        }

        p = (MiaoMemoryListNode_t *)(p->con.pNext);

    } while (p->con.pNext);


    return preturn;

}


static lw_8 _MiaoMemoryListJudgeIllegal(MiaoMemoryList_t *pMU, MiaoMemoryListNode_t *pnode)
{

    MIAO_assert(!pMU);
    MIAO_assert(!pnode);

    if (
        ((size_t)(pnode) < (size_t)(pMU->phead) || (size_t)(pnode) > (size_t)(pMU->ptail))
        ||
        ((size_t)(pnode->con.pNext) < (size_t)(pMU->phead) || (size_t)(pnode->con.pNext) > (size_t)(pMU->ptail))
        ||
        ((size_t)(pnode->con.pLast) < (size_t)(pMU->phead) || (size_t)(pnode->con.pLast) > (size_t)(pMU->ptail))
    )//判定范围
    {
        MIAO_assert(1);
        return 1;
    }

    //if (((size_t)pnode - (size_t)(pMU->phead)) % pMU->MemSectionSize)//判定是否对齐
    if (MIAO_MEM_SIZE_MOD(((size_t)pnode - (size_t)(pMU->phead)), pMU->basic.MemSectionSize)) //判定是否对齐
    {
        MIAO_assert(1);
        return 1;
    }

    return 0;

}

void * MiaoMemoryListFree(MiaoMemoryList_t *pMU, void *p)
{
    MiaoMemoryListNode_t *pnode=NULL;
    //lw_u16 len;
    //uint32_t Length;

    MIAO_assert(!(pMU));
    MIAO_assert(!(p));

    pnode = (MiaoMemoryListNode_t *)((size_t)p - sizeof(MiaoMemoryListNode_t));

    if (_MiaoMemoryListJudgeIllegal(pMU,pnode))
    {
        MIAO_assert(1);
        return NULL;
    }

    pMU->basic.memSurplus += pnode->len;

    MIAO_TOOL_DLIST_NODE_DELECT(pnode);

    pnode->con.pLast = NULL;
    pnode->con.pNext = NULL;
    pnode->len = 0;

    return pnode;

}

void* MiaoMemoryListReAlloc(MiaoMemoryList_t *pMU, void *p, uint32_t dsize)
{
    MiaoMemoryListNode_t *pnode = NULL;
    uint32_t _dsize;
    uint32_t reallen;
    void *preturn = NULL;

    MIAO_assert(!(pMU));

    _dsize = dsize;

    if (p == NULL)//直接分配内存
    {
        return MiaoMemoryListAlloc(pMU, _dsize);
    }

    if (dsize == 0)//直接释放内存
    {
        return MiaoMemoryListFree(pMU, p);
    }

    pnode = (MiaoMemoryListNode_t *)((size_t)p - sizeof(MiaoMemoryListNode_t));//偏移得到链表节点

    _dsize = dsize;//保存之前大小

    if (_MiaoMemoryListJudgeIllegal(pMU, pnode))//节点合法性判断
    {
        MIAO_assert(1);
        return NULL;
    }

    MIAO_MEM_LIST_STD_SIZE(pMU, dsize);//长度规范化

    reallen = (size_t)(pnode->con.pNext) - (size_t)pnode;//得到真实长度

    if (dsize <= reallen)//内存足够
    {
        pMU->basic.memSurplus += pnode->len;
        pMU->basic.memSurplus -= dsize;
        pnode->len = dsize;
        preturn = p;
    }
    else
    {
        preturn = MiaoMemoryListAlloc(pMU, _dsize);//重新申请

        memcpy(preturn, p, pnode->len);//复制内容

        //释放节点
        pMU->basic.memSurplus += pnode->len;
        MIAO_TOOL_DLIST_NODE_DELECT(pnode);

        pnode->con.pLast = NULL;
        pnode->con.pNext = NULL;
        pnode->len = 0;

    }

    return preturn;

}

#endif //MIAO_MEMORY_IMPORT_LIST