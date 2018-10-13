#include "ucos_ii.h"
#include "os_cpu.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

uint32_t TestStart, TsetEnd;
__IO uint32_t TestCnt = 0;
__IO uint64_t TestCntTotal = 0;
void          App_TCBInitHook         (OS_TCB          *ptcb)
{
}

void          App_TaskCreateHook      (OS_TCB          *ptcb)
{
}

void          App_TaskDelHook         (OS_TCB          *ptcb)
{
}

void          App_TaskIdleHook        (void)
{
//	PWR_EnterSleepMode();

}

void          App_TaskStatHook        (void){
}

void          App_TaskSwHook          (void){
}

void          App_TimeTickHook        (void){
}
