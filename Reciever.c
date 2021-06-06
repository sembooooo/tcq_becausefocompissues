#include "reciever.h"
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

void Execute_Recieve(statemachine_t *sm)
{
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    int number;
    if( poll(&mypoll, 1, 5000) )
    {
        scanf("%d",&number );
        printf("Read number - %d\n", number);
    }
    else
    {
        sm->HasRxTimeoutOccured = 1;
    }
}

void updateState_Recieve(statemachine_t *sm)
{
    if(sm ->HasRxTimeoutOccured == 1)
    {
        sm->state = sm->listOfStates[State_ExitProgram];
    }
    else
    {
        sm->state = sm->listOfStates[State_CalculateMovingAverage];
    }
}


void Execute_CalculateMovingAverage(statemachine_t *sm)
{
    printf("Executing CalculateMovingAverage\n");
}

void updateState_CalculateMovingAverage(statemachine_t *sm)
{
    sm->state = sm->listOfStates[State_ExitProgram];
}


void Execute_Exit(statemachine_t *sm)
{
    printf("Executing Exit\n");
    exit(EXIT_SUCCESS);
}

void updateState_Exit(statemachine_t *sm)
{
}

void Execute_RecieverStatemachine(statemachine_t *this)
{
        this->state->Execute(this);
        this->state->updateState(this);
}

state_t Reciever ={ &Execute_Recieve, &updateState_Recieve };
state_t CalculateMovingAverage ={ &Execute_CalculateMovingAverage, &updateState_CalculateMovingAverage };
state_t Exit = {&Execute_Exit,&updateState_Exit};

statemachine_t Reciever_sm = {
    0,                                              /// HasRxTimeoutOccured
    {&Reciever , &CalculateMovingAverage, &Exit},   /// listOfStates
    0,                                               /// NULL
    &Execute_RecieverStatemachine
};

int main()
{

    Reciever_sm.state = &Reciever; 
    while(1)
    {
        Reciever_sm.Execute(&Reciever_sm);
    }

    return 0;
}
