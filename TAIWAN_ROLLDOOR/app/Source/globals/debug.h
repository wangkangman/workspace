#ifndef _DEBUG_DEFINE_H
#define _DEBUG_DEFINE_H

/**
  * @brief  define send port
  *   
  */
#define UART0_DEBUG

/**
  * @brief  define which module should be debuged
  *   
  */
#if 1
#define START_STEP_DEBUG
#define START_READEEP_DEBUG
#define POWER_ON_DEBUG
#define STANDBY_DEBUG
#define LIMITSET_DEBUG
#define TRAIN_DEBUG
#define EXCUTE_DEBUG
#define POWER_OFF_DEBUG
//#define REMOTE_DEBUG
#endif

#endif /* _GLOBALS_H */
