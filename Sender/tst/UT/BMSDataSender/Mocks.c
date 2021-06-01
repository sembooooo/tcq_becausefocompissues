#include "../../../src/BMSDataSender.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "Mocks.h"

char Printf_FormartString[100];
int call_Printf;
double printf_floatpar_data[2];
double printf_floatpar_AlldataSamples[4][2];
int numberofcallsToRequestStop;

int print_Mock_FilenotfoundInstance(char *Par_s)
{
    strcpy(Printf_FormartString,Par_s);
    return 0;
}

// Lesson learnt : floatgets coverted into double when passed through va_args
// controversy here
int print_Mock_ForDataEvaluation(char *Par_s,...)
{
    double temp;
    double chargerate;
    va_list ap;
    va_start(ap,Par_s);
    temp = va_arg(ap,double);
    chargerate = va_arg(ap,double);
    va_end(ap);
    call_Printf++;

    strcpy(Printf_FormartString,Par_s);
    printf_floatpar_data[0] =temp;
    printf_floatpar_data[1] =chargerate;
    RequestToStopDataTransmission();
    return 0;
}

int print_Mocks_ForFileIterationInstance(char *Par_s, double temp,double chargerate)
{
    static int index =0;
    call_Printf++;
    strcpy(Printf_FormartString,Par_s);
    printf_floatpar_AlldataSamples[index][0] = temp;
    printf_floatpar_AlldataSamples[index][1] = chargerate;
    index ++;
    if(call_Printf == numberofcallsToRequestStop)
        RequestToStopDataTransmission();
    return 0;
}

int print_Mocks_ForNtransmissionsScenario(char *Par_s,double temp, double chargerate)
{
    call_Printf++;
    
    if((call_Printf == numberofcallsToRequestStop)&& (numberofcallsToRequestStop!= 0))
    {
        RequestToStopDataTransmission();
    }
    return 0;
}
void Reset_all_print_mocks(void)
{
    int index = 0;
    
    memset(Printf_FormartString,0,100);
    
    call_Printf = 0;
    
    printf_floatpar_data[0] = 0.0;
    printf_floatpar_data[1] = 0.0;

    for(index =0; index <4 ; index++)
    {
        printf_floatpar_AlldataSamples[index][0] = 0.0;
        printf_floatpar_AlldataSamples[index][1] = 0.0;
    }
    numberofcallsToRequestStop = 0;
}