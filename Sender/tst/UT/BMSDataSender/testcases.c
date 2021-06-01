/**
 * Note:
 * -----
 * I do not use Catch framework i instead use assert() for my unit test.
 * Reason being i can perform my testing locally but catch.hpp gives errors 
 * when executed locally.
 */

#include "../../../src/BMSDataSender.c"             /** Unit which sends the data */
#include <assert.h>                                 /* My unit test frame work */
#include <string.h>
#include "Mocks.h"


/**
 * *****************************************************
 * ******* This is not a testcase so skip this *********
 * *****************************************************
 * Environment_Initialization will be used by the python script which i
 * prepared as a hobby to save me from 
 * - writing main
 * - updating it with the newly added testcase
 * - running the .exe after compiling my testcase (equivalent to run in actions)
 * 
 * The python script shall call this function before every testcase.
 * All global variables are intialized to 0 in this function.
 */ 

static void Environment_Initialization(void)
{
    BMSDataTransmitter.TxControl.isTxStopRequested =0;
    BMSDataTransmitter.TxControl.isStopAfterNTransmissionRequested =0;
    BMSDataTransmitter.TxControl.NumberofTransmissionAllowed =0;
    print = &printf;
    Reset_all_print_mocks();
    strcpy(filename,"data.txt");
}

/**
 * Sender will read the data from a file and transmit the data onto console.
 * If the file is not found then the sender process shall exit.
 */ 

static void TC_ProcessShouldExitIfFileNotFound(void)
{
    strcpy(filename,"dat.txt");
    printf("TC_ProcessShouldExitIfFileNotFound\n");
    print = &print_Mock_FilenotfoundInstance;
    BatteryMonitoringSystemTransmitter_Main();
    assert(strcmp("Problem with File opening\n",Printf_FormartString)==0);
}


/**
 * The data is stored in the following order in the file
 *      temparature_1 chargerate_1
 *      temparature_2 chargerate_2
 *      temparature_3 chargerate_3
 *          ....
 *          ....
 * 
 * The data should also get printed in the same order on console
 *      temparature_1 chargerate_1
 *      temparature_2 chargerate_2
 *      temparature_3 chargerate_3
 *          ....
 *          ....
 */ 
static void TC_EvaluateParametersOrderPrintedOnConsole(void)
{
    double constants[BatteryParameter_TotalNumber] = {1.0,2.0};
    double epsilon = 0.0001;
    int condition;
    printf("TC_EvaluateParametersOrderPrintedOnConsole\n");
    print = &print_Mock_ForDataEvaluation;
    BatteryMonitoringSystemTransmitter_Main();
    assert(call_Printf == 1);
    printf("%f %f\n",printf_floatpar_data[BatteryParameter_Temparature],printf_floatpar_data[BatteryParameter_ChargeRate]);
    condition = ((printf_floatpar_data[BatteryParameter_Temparature] < (constants[BatteryParameter_Temparature]+epsilon)) &&
                (printf_floatpar_data[BatteryParameter_Temparature] > (constants[BatteryParameter_Temparature] - epsilon)));
    assert(condition);
    condition = ((printf_floatpar_data[BatteryParameter_ChargeRate] < (constants[BatteryParameter_ChargeRate]+epsilon)) &&
                (printf_floatpar_data[BatteryParameter_ChargeRate] > (constants[BatteryParameter_ChargeRate] - epsilon)));
    assert(condition);
}

/**
 * While fetching the values from ./data.txt file ,if End of the file is reached then the
 * reading should again start from the top of the file.
 * This allows us to send data on to console continously until user stops.
 * Temparature and chargerate are read together in a single read.
 * 
 * The data.txt file in this unittest is written in the following way
 * 1.0 2.0
 * 3.0 4.0
 * 5.0 6.0
 * 
 * So after 3rd read again 1.0 and 2.0 should be passed on to console (printf)
 *
 */ 
static void TC_EvaluateIfFilereadingisIteratedatEOF(void)
{
    printf("TC_EvaluateIfFilereadingisIteratedatEOF\n");
    int index;
    double datasamples[4][2] ={ {1.0,2.0},
                                {3.0,4.0},
                                {5.0,6.0},
                                {1.0,2.0}};
    print = &print_Mocks_ForFileIterationInstance;
    numberofcallsToRequestStop = 4;
    BatteryMonitoringSystemTransmitter_Main();
    assert(call_Printf == 4);
    for (index = 0; index <4 ; index ++)
    {
        assert(printf_floatpar_AlldataSamples[index][0] ==datasamples[index][0]);
        assert(printf_floatpar_AlldataSamples[index][1] ==datasamples[index][1]);
    }
}
/**
 * To Evaluate if the testdouble is assigned properly with printf or not
 */ 
static void TC_EvaluateIfPrintfIsAssigned(void)
{
    printf("TC_EvaluateIfPrintfIsAssigned\n");
    assert(print == &printf);
}


/**
 * We can also limit the number of transmission on to console
 * set BMSDataTxControl_t.NumberofTransmissionAllowed to desired number of transmissions
 * But to enable this feature one needs to set BMSDataTxControl_t.isStopAfterNTransmissionRequested to 1
 * 
 * This testcase evaluates if the transmission stops are defined number.
 */ 
static void TC_EvaluateIfTransmissionStopsAfterN(void)
{
    printf("TC_EvaluateIfTransmissionStopsAfterN\n");
    BMSDataTransmitter.TxControl.isStopAfterNTransmissionRequested = 1;
    BMSDataTransmitter.TxControl.NumberofTransmissionAllowed = 1000;
    print = &print_Mocks_ForNtransmissionsScenario;
    BatteryMonitoringSystemTransmitter_Main();
    assert(call_Printf == 1000);   
}


/**
 * This testcase evaluates if the transmission doesnt stop after defined transmissions
 * We set BMSDataTxControl_t.isStopAfterNTransmissionRequested = 0
 * but BMSDataTxControl_t.NumberofTransmissionAllowed = 1000 
 * The transmission shall continue after 1000th transmission
 */ 
static void TC_EvaluateIfTransmissionDoesntStopAfterN(void)
{
    printf("TC_EvaluateIfTransmissionDoesntStopAfterN\n");
    BMSDataTransmitter.TxControl.isStopAfterNTransmissionRequested = 0;
    BMSDataTransmitter.TxControl.NumberofTransmissionAllowed = 1000;
    numberofcallsToRequestStop = 2000;
    print = &print_Mocks_ForNtransmissionsScenario;
    BatteryMonitoringSystemTransmitter_Main();
    assert(call_Printf > 1000);
    assert(call_Printf == 2000);   
}


int main()
{
printf("*************************************\n");
printf("****Test Execution Started***********\n");
printf("*************************************\n");
Environment_Initialization(); 
TC_ProcessShouldExitIfFileNotFound(); 
Environment_Initialization(); 
TC_EvaluateIfPrintfIsAssigned();
Environment_Initialization(); 
TC_EvaluateParametersOrderPrintedOnConsole(); 
Environment_Initialization(); 
TC_EvaluateIfFilereadingisIteratedatEOF(); 
Environment_Initialization(); 
TC_EvaluateIfTransmissionStopsAfterN();
Environment_Initialization(); 
TC_EvaluateIfTransmissionDoesntStopAfterN();
printf("*************************************\n");
printf("****Test Execution Ended*************\n");
printf("*************************************\n");
return 0;
}
