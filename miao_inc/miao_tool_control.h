/*(Ver.=0.96)
 * MIAO_tool.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef MIAO_TOOL_CONTROL_H_
#define MIAO_TOOL_CONTROL_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#if !(MIAO_SIMPLE_LEVEL >= 3)




typedef struct MIAO_TOOL_PID_CTRL_STRUCT{
 
 MiaoExternMemory_t ExtFun;
	
 MiaoParameter_t Para_Kp; 	
 MiaoParameter_t Para_Ki;
 MiaoParameter_t Para_Kd;
 MiaoParameter_t Para_Kc;
	
 ParaType K_up; 
 ParaType K_ui;
 ParaType K_ud; 		
 ParaType I_ref; 	
 ParaType I_fdb;
 ParaType K_e;
 ParaType K_eup;
 ParaType K_upresat;
 ParaType O_uout;  
 ParaType P_max;
 ParaType P_min;
 ParaType P_normal;
		
}MiaoToolPidCtrl_t;

ParaType MiaoToolPidCtrlCal(MiaoToolPidCtrl_t *p);

typedef struct MIAO_TOOL_SOGIPLL_STRUCT{
	MiaoExternMemory_t ExtFun;	
	MiaoParameter_t Para_PLLOutCorrect;
	
	ParaType K_alast,K_a,K_blast,K_b;
	ParaType K_mul,K_mullast;
	ParaType K_In1,K_In2,K_In1last;
	ParaType P_BaseFreq;
	lw_sf P_IrpFreq;
	
	ParaType I_input;
	
	ParaType O_BaseRad,O_RealRad;
		
	MiaoToolPidCtrl_t PllPid;
		
}MiaoToolSogiPll_t;


typedef struct MIAO_TOOL_PRCTRL_STRUCT{
	MiaoExternMemory_t ExtFun;
	MiaoParameter_t Para_Kp;	
	MiaoParameter_t Para_num[2];
	MiaoParameter_t Para_den[2];
	
	ParaType I_Ref,I_Fdb,O_Uout;

	ParaType P_max;
	ParaType P_min;
	
	ParaType K_Rout;
	ParaType K_e;
	
	ParaType K_yfia1;
	ParaType K_yfia2;
	ParaType K_xfia1;
	ParaType K_xfia2;	
	
}MiaoToolPrCtrl_t;

ParaType MiaoToolPrCtrlCal(MiaoToolPrCtrl_t *p);


#endif // !(MIAO_SIMPLE_LEVEL >= 3)
#endif

