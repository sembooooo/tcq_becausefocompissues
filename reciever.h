#pragma once

/**
 * MISRA 18.1 All structure or union types shall be complete at the end of a translation unit.
 * A complete declaration of the structure or union shall be included within any translation
 *  unit that refers to that structure.
 */  

typedef enum {
    State_Recieve = 0,
    State_CalculateMovingAverage,
    State_ExitProgram,
    State_TotalNumber
}States_t;

typedef struct sm statemachine_t;

typedef struct{
    void (*Execute)(statemachine_t *sm);
    void (*updateState)(statemachine_t *sm);
}state_t;

typedef struct sm
{
    int HasRxTimeoutOccured;
    state_t* listOfStates[State_TotalNumber];
    state_t *state;
    void (*Execute)( statemachine_t *this);
/// void (*Reset)(statemachine_t *this);    # can be used in init task or when CoToM revokes control
}statemachine_t;
