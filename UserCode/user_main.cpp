#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>

int *numPtr  = nullptr;
int *numPtr2 = nullptr;

int testNum;

void SysInit()
{
    extern uint32_t _sbss_d1, _sbss_d2, _sbss_d3, _sbss_itcm;
    extern uint32_t _ebss_d1, _ebss_d2, _ebss_d3, _ebss_itcm;

    auto clearMem = [](void *startPos, void *endPos) {
        memset(startPos, 0, (uint32_t)endPos - (uint32_t)startPos);
    };

    clearMem(&_sbss_d1, &_ebss_d1);
    clearMem(&_sbss_d2, &_ebss_d2);
    clearMem(&_sbss_d3, &_ebss_d3);
    clearMem(&_sbss_itcm, &_ebss_itcm);
}

void StartDefaultTask(void *argument)
{
    (void)argument;
    int stackNum = 0;

    (void)stackNum;

    numPtr  = new int(0);
    numPtr2 = (int *)pvPortMalloc(sizeof(int));

    for (;;) {
        (*numPtr)++;
        (*numPtr2)++;
        testNum = *numPtr2;
        stackNum++;
        vTaskDelay(100);
    }
}
