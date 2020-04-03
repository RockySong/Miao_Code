/*(Ver.=0.96)
 * MIAO_tool.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"


#if MIAO_TOOL_IMPORT_SLIP_WIN || MIAO_TOOL_IMPORT_ALL
MiaoToolSlipwin_t* MiaoToolSlipwinAppend(MiaoToolSlipwin_t *psw,void *pd,lw_u8 num)
{
    register lw_8 *p,*q;
    register uint32_t n,n1;

    if(psw->MDataLockFlag)//如果锁存
    {
        p=(lw_8 *)psw->pBufferData+psw->BufferLength;//得到当前缓冲区首地址
        q=(lw_8 *)pd;
        n=num;
        MIAO_TOOL_MEMCPY(p,q,n);//复制黏贴
        psw->BufferLength+=num;

    }
    else
    {
        n=num+psw->BufferLength;//存入数据长度==新存数据长度+缓冲区数据长度
        p=(lw_8 *)psw->pMasterData;
        q=p+n;
        n1=psw->MDataLen_forSize_t-n;
        MIAO_TOOL_MEMCPY(p,q,n1);//先滑窗

        n=psw->BufferLength;
        q=psw->pBufferData;
        MIAO_TOOL_MEMCPY(p,q,n);//再填入缓冲器数据 如果缓冲器长0 则不执行
        psw->BufferLength=0;

        q=(lw_8 *)pd;

        while((num)--)//复制黏贴新数据
            *(p)++=*(q)++;




    }
    return psw;
}
#endif //MIAO_TOOL_IMPORT_SLIP_WIN


#if !(MIAO_SIMPLE_LEVEL >= 4)
#if MIAO_TOOL_IMPORT_DLIST || MIAO_TOOL_IMPORT_ALL
void MiaoToolDListInitial(MiaoToolDList_t *pDL,lw_u8 len,void *p)
{


    MIAO_assert(!pDL);
    memset(pDL,0,sizeof(MiaoToolDList_t));
    pDL->Head.Key.uData=0;
    pDL->pTail=&pDL->Head;
    pDL->Head.Data.uData=len;
    pDL->Head.Data.pData=p;
    pDL->MaxLen++;
    pDL->NowLen++;
    pDL->AppendFUN=MiaoToolDListAppend;
    pDL->IndexOfFUN=MiaoToolDListIndexOf;
    pDL->InsertFUN=MiaoToolDListInsert;
    pDL->RemoveFUN=MiaoToolDListRemove;


    return;
}



lw_8 MiaoToolDListCmpId(MiaoToolDListNode_t *pDLN,lw_u16 data,lw_u16 count,void *pdata)
{
    return pDLN->Key.uData == data;
}

lw_8 MiaoToolDListCmpCount(MiaoToolDListNode_t *pDLN, lw_u16 data, lw_u16 count, void *pdata)
{

    return count == data;
}


MiaoToolDListNode_t* MiaoToolDListIndexOf(MiaoToolDList_t *pDL, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT)
{
    register MiaoToolDListNode_t *pDLN;
    lw_u16 count=0;

    MIAO_assert(!(pDL));
    pDLN = &pDL->Head;
    /*(Ver.=0.96)
        if(mode== MiaoToolDListIndexMode_byId)
        {
            while(!(pDLN->Id == id)) pDLN= (MiaoToolDListNode_t *)pDLN->con.pNext;
            if(pDLN->Id == id)return pDLN;
            else return NULL;
        }
        else if(mode == MiaoToolDListIndexMode_byNum && id < pDL->NowLen)
        {
            while(count++ != id) pDLN=(MiaoToolDListNode_t *)(pDLN->con.pNext);
            return pDLN;
        }
        else
            return NULL;
    */

    if (pCmpFun)//如果定义判断函数
    {
        do
        {
            if (pCmpFun(pDLN, data, count, pdata))//如果判断函数返回1
            {
                break;
            }
            count++;
            pDLN = (MiaoToolDListNode_t *)(pDLN->con.pNext);

        } while (pDLN);

        return pDLN;
    }
    else
    {
        return NULL;
    }


}


MiaoToolDListNode_t* MiaoToolDListAppend(struct MIAO_TOOL_DLIST_STRUCT *pDL,MiaoToolDListNode_t *pDLN)
{

    MIAO_assert(!(pDL));
    MIAO_assert(!(pDLN));

    /*(Ver.=0.96)
    pDL->pTail->con.pNext=(MiaoToolDListNodeConnect_t *)pDLN;//尾部的下一个为新的
    pDLN->con.pLast=(MiaoToolDListNodeConnect_t *)pDL->pTail;//新节点的上一个为原来尾部
    */

    MIAO_TOOL_DLIST_NODE_CONNECT(pDL->pTail, pDLN)

    pDL->pTail=pDLN;
    pDLN->Key.uData =pDL->MaxLen++;//ID赋予
    //pDLN->con.pNext=NULL;
    pDL->NowLen++;

    return pDLN;



}
MiaoToolDListNode_t* MiaoToolDListInsert
(struct MIAO_TOOL_DLIST_STRUCT *pDL,MiaoToolDListNode_t *pDLN, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT)
{
    register MiaoToolDListNode_t *pDLNbuf;
    MIAO_assert(!(pDL));
    MIAO_assert(!(pDLN));

    pDLNbuf=pDL->IndexOfFUN(pDL,pCmpFun,data,pdata);
    if(!(pDLNbuf))
    {
        if(pDLNbuf != pDL->pTail)
        {
            //pDLNbuf1=(MiaoToolDListNode_t *)(pDLNbuf->con.pNext);//得到下一个
            pDLN->Key.uData =pDL->MaxLen++;//ID赋予
            pDL->NowLen++;
            /*(Ver.=0.96)
            pDLNbuf->con.pNext=(MiaoToolDListNodeConnect_t *)pDLN;//上一个的下一个为新的
            pDLN->con.pLast=(MiaoToolDListNodeConnect_t *)pDLNbuf;//新的的上一个为原来的

            pDLN->con.pNext=(MiaoToolDListNodeConnect_t *)pDLNbuf1;//二号对接
            pDLNbuf1->con.pLast=(MiaoToolDListNodeConnect_t *)pDLN;*/
            MIAO_TOOL_DLIST_NODE_INSERT(pDLNbuf, pDLN);
        }
        else
        {
            pDL->AppendFUN(pDL,pDLN);
        }
        return pDLN;

    }
    else
        return NULL;


}



//内置函数
MiaoToolDListNode_t* _MiaoToolDListRemoveNode
(struct MIAO_TOOL_DLIST_STRUCT *pDL, MiaoToolDListNode_t *pDLNbuf)
{

    if((pDLNbuf) && pDLNbuf != &(pDL->Head))//0号为head 不可以释放
    {
        if(pDLNbuf == pDL->pTail)
        {
            pDLNbuf->con.pLast->pNext=NULL;//尾部指针清零
            pDL->pTail=(MiaoToolDListNode_t *)pDLNbuf->con.pLast;//尾巴更新
        }
        else
        {
            /*(Ver.=0.96)
            pDLNbuf->con.pLast->pNext=pDLNbuf->con.pNext;//上一个的下一个为下一个
            pDLNbuf->con.pNext->pLast=pDLNbuf->con.pLast;//下一个的上一个为上一个
            */
            MIAO_TOOL_DLIST_NODE_DELECT(pDLNbuf)
        }
        pDL->NowLen--;

        return (MiaoToolDListNode_t *)pDLNbuf->con.pNext;

    }


    return (MiaoToolDListNode_t *)pDLNbuf->con.pNext;


}

MiaoToolDListNode_t* MiaoToolDListRemove
(struct MIAO_TOOL_DLIST_STRUCT *pDL, MIAO_TOOL_DLIST_FUN_PARA_DEFAULT)
{
    MiaoToolDListNode_t *pDLNbuf;
    MIAO_assert(!(pDL));
    pDLNbuf = pDL->IndexOfFUN(pDL, pCmpFun, data, pdata);
    /*(Ver.=0.96)
        if((pDLNbuf) && data !=0)//0号为head 不可以释放
        {
            if(pDLNbuf == pDL->pTail)
            {
                pDLNbuf->con.pLast->pNext=NULL;//尾部指针清零
                pDL->pTail=(MiaoToolDListNode_t *)pDLNbuf->con.pLast;//尾巴更新
            }
            else
            {
                #if 0
                pDLNbuf->con.pLast->pNext=pDLNbuf->con.pNext;//上一个的下一个为下一个
                pDLNbuf->con.pNext->pLast=pDLNbuf->con.pLast;//下一个的上一个为上一个
								#endif
                MIAO_TOOL_DLIST_NODE_DELECT(pDLNbuf)
            }
            pDL->NowLen--;

            return (MiaoToolDListNode_t *)pDLNbuf->con.pNext;

        }
        else
            return NULL;
    */

    if((pDLNbuf) && data !=0)//0号为head 不可以释放
    {
        return _MiaoToolDListRemoveNode(pDL,pDLNbuf);

    }
    else
        return NULL;


}
#endif //MIAO_TOOL_IMPORT_DLIST




#if MIAO_TOOL_IMPORT_HASH_LIST || MIAO_TOOL_IMPORT_ALL
lw_u8 HashCalStringBKDR(void *pData)
{
    register lw_u8 *str = (lw_u8 *)pData;
    register lw_u8 hash = 0;
    register lw_u8 ch ;

    MIAO_assert(!(pData));

    while ((ch=(lw_u8)(*str++)))
    {
        hash += ((hash << 7) + (hash << 1) + hash + ch);//hash=hash*131+ch;
    }

    return hash;
}

lw_8 HashCmpString(void *p1, void *p2)
{
    return strcmp((lw_8 *)p1, (lw_8 *)p2);
}



void MiaoToolHashInitial(MiaoToolHash_t *pH, lw_u8 length, HashCalFunType pHashFun, HashKeyCmpFunType pHashKeyCmpFun)
{

    MIAO_assert(!(pH));
    MIAO_assert(!(pHashFun));
    MIAO_assert(!(pHashKeyCmpFun));
    MIAO_assert(!(length));

    if (length > MIAO_TOOL_HASH_LIST_MAX)
    {
        MIAO_assert(1);
        length = MIAO_TOOL_HASH_LIST_MAX;
    }


    memset(pH, 0, sizeof(MiaoToolHash_t));

    pH->HashListLength = length;
    pH->HashCalFun = pHashFun;
    pH->HashKeyCmp = pHashKeyCmpFun;

    pH->SelectFun = MiaoToolHashSelect;
    pH->AppendFun = MiaoToolHashAppend;
    pH->DeleteFun = MiaoToolHashDelete;

    return;
}




lw_u8 MiaoToolHashSelect(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey, lw_u8 *pIndexResult, MiaoToolDListNode_t **ppDLN)
{
    lw_u8 key_index;
    MiaoToolDListNode_t *pDLN = NULL;
    MiaoToolDListNode_t *pDLNlast = NULL;

    MIAO_assert(!(pH));
    MIAO_assert(!(pKey));

    key_index = pH->HashCalFun(pKey) % pH->HashListLength;

    pDLN = pH->pHashList[key_index];
    while (pDLN)
    {
        if (!(pH->HashKeyCmp(pDLN->Key.pData,pKey)))
        {
            break;
        }
        pDLNlast = pDLN;
        pDLN = (MiaoToolDListNode_t *)(pDLN->con.pNext);

    }
    if (pIndexResult)
        *pIndexResult = key_index;



    if (pDLN)//判断是否找到
    {
        if (ppDLN)//要不要返回
        {
            *ppDLN = pDLN;
        }
        return 1;
    }
    else//没有找到返回前一个节点
    {
        if (ppDLN)
        {
            *ppDLN = pDLNlast;
        }
        return 0;
    }
    return 0;

}

MiaoToolDListNode_t* MiaoToolHashAppend(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey, MiaoToolDListNode_t *pDLN)
{
    lw_u8 key_index;
    lw_u8 res;
    MiaoToolDListNode_t *pdln;

    MIAO_assert(!(pH));
    MIAO_assert(!(pKey));
    MIAO_assert(!(pDLN));

    res = pH->SelectFun(pH, pKey, &key_index,&pdln);
    if (res)//不满足追加条件
    {
        MIAO_assert(1);
        return NULL;

    }
    else//满足追加条件
    {
        //pdln = pH->pHashList[key_index];
        if (pdln)//如果不是空
        {
            MIAO_TOOL_DLIST_NODE_CONNECT(pdln, pDLN);
        }
        else//为空 直接对应数组填入
        {
            pH->pHashList[key_index] = pDLN;
						pDLN->con.pLast = NULL;
        }
        pDLN->con.pNext = NULL;
        pDLN->Key.pData = pKey;

    }
    return pDLN;

}

MiaoToolDListNode_t* MiaoToolHashDelete(struct MIAO_TOOL_HASH_STRUCT *pH, void *pKey)
{
    lw_u8 key_index;
    lw_u8 res;
    MiaoToolDListNode_t *pdln;

    MIAO_assert(!(pH));
    MIAO_assert(!(pKey));

    res = pH->SelectFun(pH, pKey, &key_index, &pdln);
    if (res)//满足删除条件
    {
        if (pdln->con.pLast)//如果有上一个 说明不是头部
        {
            MIAO_TOOL_DLIST_NODE_DELECT(pdln);
        }
        else
        {
            pH->pHashList[key_index] = (MiaoToolDListNode_t *)(pdln->con.pNext);
            if (pdln->con.pNext)//如果有下一个
            {
                pdln->con.pNext->pLast = NULL;//清除前一个
            }
        }
        return pdln;
    }
    else
    {
        MIAO_assert(1);
        return NULL;
    }

}
#endif //MIAO_TOOL_IMPORT_HASH_LIST

#endif

#if !(MIAO_SIMPLE_LEVEL >= 4)

#if MIAO_TOOL_IMPORT_TABLE || MIAO_TOOL_IMPORT_ALL
void MiaoToolTableInitial(MiaoToolTable_t *pT, lw_u16 RowLen, lw_u16 *pCol, lw_u16 ColLen, MiaoMemoryBasic_t *pmem, void *pTable,uint32_t *pColReal)
{

    uint32_t col_size = 0;
    uint32_t *pSize;
    lw_u16 _len = 0;

    MIAO_assert(!(pT));
    MIAO_assert(!(pmem));

    pT->Col.uData = ColLen;
    pT->Row.uData = RowLen;

	if (pColReal)
	{
		pSize = pT->Col.pData = pColReal;

		while (ColLen--)
		{
			col_size += *pSize++;
		}

	}
	else
	{
		pSize = pT->Col.pData = pmem->Malloc(pmem, sizeof(uint32_t) * ColLen);
		while (ColLen--)
		{
			_len = *pCol;
			*pSize = (uint32_t)(_len);
			col_size += *pSize;
			pSize++;
			pCol++;
		}
	}
    



    pT->ColSize = col_size;

    if (pTable)//如果定义了pTable 采用静态分配
    {
        pT->Row.pData = pTable;
    }
    else//全部采用动态分配
    {
        pT->Row.pData = pmem->Malloc(pmem, (RowLen * pT->ColSize));
    }

	if (pT->Row.pData == NULL)
	{
		MIAO_assert(1);
	}

	memset(pT->Row.pData, 0, (RowLen * pT->ColSize));
    

    pT->pmem = pmem;

    pT->ReadFun = MiaoToolTableRead;
    pT->WriteFun = MiaoToolTableWrite;


}


void * MiaoToolTableRead(struct MIAO_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, lw_u16 *pLen)
{
    void *p;
    uint32_t *pSize;
    size_t col_size = 0;

    MIAO_assert(!(pT));

    row = row < 0 ? row + pT->Row.uData : row;
    col = col < 0 ? col + pT->Col.uData : col;

    if (row < 0 || col < 0)//彻底超出维度
    {
        return NULL;
    }

    if (col >= (lw_32)(pT->Col.uData))//列数太大 一旦固定不在修改
    {
        return NULL;
    }

    if (row >= (lw_32)(pT->Row.uData))//行数太大
    {
        p = pT->pmem->ReAlloc(pT->pmem, pT->Row.pData, (row  * pT->ColSize));
        if (p==NULL)
        {
            return p;
        }
        pT->Row.uData = row;
    }


    pSize = pT->Col.pData;
    while (col--)
    {
        col_size += *pSize++;
    }

    if (pLen)
    {
        *pLen = *pSize;
    }

    return (void *)((size_t)(pT->Row.pData) + (row * pT->ColSize) + col_size);

}



lw_32 MiaoToolTableWrite(struct MIAO_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, void *psrc, lw_u16 len)
{
    MiaoMemSize_t *pSize;
    lw_u16 _len;
    void *p;

    p = MiaoToolTableRead(pT, row, col, &_len);
    if (p ==NULL )
    {
        MIAO_assert(1);
        return (lw_32)((uint32_t)1 << 31);
    }
    if (len <= _len)
    {
        memcpy(p, psrc, len);
    }
    else
    {
        memcpy(p, psrc, _len);
    }

    return (lw_32)_len - (lw_32)len;
}
#endif //MIAO_TOOL_IMPORT_TABLE

#endif // !(MIAO_SIMPLE_LEVEL >= 3)
