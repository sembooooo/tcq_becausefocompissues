#include <stdio.h>
#include <assert.h>
#include "BMSDataSender.h"

///> Test double
int (*print)(const char *format, ...) = &printf;
char filename[10] = "data.txt";



void CalculateIsTxStopRequested(BMSDataTxControl_t * TxControlPtr)
{
    if(TxControlPtr->isStopAfterNTransmissionRequested == 1)   
    {
        TxControlPtr->NumberofTransmissionAllowed--;
        if(TxControlPtr->NumberofTransmissionAllowed == 0)
        {
            TxControlPtr->isTxStopRequested = 1;
        }
    }  
}

void TransmitDataFromFileToConsole(BMSDataTxControl_t * TxControlPtr)
{
    float ParameterData[BatteryParameter_TotalNumber];
    FILE *fptr = fopen(filename,"r");   
    if(fptr == NULL)
    {
        print("Problem with File opening\n");   
        return; 
    }
    while(TxControlPtr->isTxStopRequested == 0)
    {
            if(fscanf(fptr,"%f %f",&ParameterData[BatteryParameter_Temparature],
                                &ParameterData[BatteryParameter_ChargeRate] ) == EOF)
            {
                rewind(fptr);
            }
            else
            {
                print("%f %f\n",ParameterData[BatteryParameter_Temparature],ParameterData[BatteryParameter_ChargeRate]);
               CalculateIsTxStopRequested(TxControlPtr);
            }

    }
    fclose(fptr);

}

BMSDataTransmitter_t BMSDataTransmitter ={
                                            {0,0,0},
                                        &TransmitDataFromFileToConsole};

void BatteryMonitoringSystemTransmitter_Main(void)
{
    if(BMSDataTransmitter.TxControl.isStopAfterNTransmissionRequested == 1) 
    {
        assert(BMSDataTransmitter.TxControl.NumberofTransmissionAllowed > 0);
    }        
   BMSDataTransmitter.TrasmitData(&BMSDataTransmitter.TxControl);
}

void RequestToStopDataTransmission(void)
{   
    BMSDataTransmitter.TxControl.isTxStopRequested = 1;
}

#ifdef MAIN
/**
 * This main function will only get compiled if this MAIN macro is defined during compilatin
 */ 
int main()
{
    /**
     *  As we cannot run the process forever in github actions i have restricted to 1000 data transmissions
     *  For infinite transmissions set BMSDataTransmitter.TxControl.isStopAfterNTransmissionRequested = 0;
     **/
    BMSDataTransmitter.TxControl.isStopAfterNTransmissionRequested = 1;
    BMSDataTransmitter.TxControl.NumberofTransmissionAllowed = 10000;
    BatteryMonitoringSystemTransmitter_Main();
    return 0;
}
#endif
