
#pragma once

extern char Printf_FormartString[100];
extern int call_Printf;
extern double printf_floatpar_data[2];
extern double printf_floatpar_AlldataSamples[4][2];
extern int numberofcallsToRequestStop;
extern int print_Mock_FilenotfoundInstance(char *Par_s);
extern int print_Mock_ForDataEvaluation(char *Par_s,...);
extern int print_Mocks_ForFileIterationInstance(char *Par_s, double temp,double chargerate);
extern int print_Mocks_ForNtransmissionsScenario(char *Par_s,double temp, double chargerate);
extern void Reset_all_print_mocks(void);