#ifndef OS_CBK_H
#define OS_CBK_H


/********************************* Includes **********************************/

/**************************** Constants and Types ****************************/

/********************************* Functions *********************************/
void Scheduler_init(void);
void Scheduler_destroy(void);
void Scheduler_MainFunction(void);
void DcmDsl_RxIndicationTrigger(void);
void DoIP_TxTrigger();


#endif //OS_CBK_H
