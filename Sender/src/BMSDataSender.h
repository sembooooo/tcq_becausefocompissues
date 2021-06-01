#ifndef BMSDATASENDER_H__
#define BMSDATASENDER_H__

#define DATASAMPLES_FILENAME    "data.txt"


typedef enum{
    BatteryParameter_Temparature,
    BatteryParameter_ChargeRate,
    BatteryParameter_TotalNumber
} BatteryParameter_t;


typedef struct {
    int isTxStopRequested;
    int isStopAfterNTransmissionRequested;
    int NumberofTransmissionAllowed;
}BMSDataTxControl_t;

typedef struct {
    BMSDataTxControl_t TxControl;
    void (*TrasmitData)(BMSDataTxControl_t *TxControl);
}BMSDataTransmitter_t;

/* Designed For testing purposes */
extern void RequestToStopDataTransmission(void);

#endif  /*BMSDATASENDER_H__*/

