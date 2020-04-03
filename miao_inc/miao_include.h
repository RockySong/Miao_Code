/*(Ver.=0.96)
 * MIAO_include.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef MIAO_INCLUDE_H_
#define MIAO_INCLUDE_H_


#include <setjmp.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "OSLW_define.h"

#include "OSLW_task.h"
#include "OSLW_gift.h"
#include "OSLW_parameter.h"
#include "OSLW_memory.h"
#include "OSLW_timer.h"
#include "OSLW_tool.h"
#include "OSLW_communication.h"
#include "OSLW_show.h"
#include "OSLW_string_debug.h"
#include "OSLW_core.h"

/*(Ver.=0.96)
移植步骤
0)添加文件与路径

1)修改宏定义

2)实例化内核
MiaoCore_t myos;

3)定时器回调函数
MiaoClockCallBack(&myos);

4)实例任务
MiaoTask_t mytask;

5)内核任务函数
void Mytask(MiaoTask_t *_pta)
{
	static MiaoTask_t *pta;
	static lw_u16 num=0;

	pta=_pta;
	MiaoJiJi(_pta);
	for(;;)	
	{
		MiaoSleepSec(pta,1,0);
	}
	MiaoWeiJi(_pta);

}
	
6)初始化内核 追加任务
MiaoCoreInitial(&myos);
MiaoTaskInit(&mytask,3,"T1",0,NULL,Mytask);
myos.TaskAppendFun(&myos,&mytask);

7)启动os
myos.StartFun(&myos);

*/


#endif /*(Ver.=0.96) MIAO_INCLUDE_H_*/
