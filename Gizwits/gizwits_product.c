/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart2.h"

extern int MQ2_flag,MQ5_flag,MQ9_flag;
extern uint16_t MQ2,MQ5,MQ9;
extern int MQ2_threshold,MQ5_threshold,MQ9_threshold;
extern int beep_flag;

static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_beep_flag:
        currentDataPoint.valuebeep_flag = dataPointPtr->valuebeep_flag;
        GIZWITS_LOG("Evt: EVENT_beep_flag %d \n", currentDataPoint.valuebeep_flag);
        if(0x01 == currentDataPoint.valuebeep_flag)
        {
          //user handle
        }
        else
        {
          //user handle    
        }
        break;


      case EVENT_smoke_threshold:
        currentDataPoint.valuesmoke_threshold = dataPointPtr->valuesmoke_threshold;
        GIZWITS_LOG("Evt:EVENT_smoke_threshold %d\n",currentDataPoint.valuesmoke_threshold);
        //user handle
        break;
      case EVENT_MQ9_threshold:
        currentDataPoint.valueMQ9_threshold = dataPointPtr->valueMQ9_threshold;
        GIZWITS_LOG("Evt:EVENT_MQ9_threshold %d\n",currentDataPoint.valueMQ9_threshold);
        //user handle
        break;
      case EVENT_MQ2_threshold:
        currentDataPoint.valueMQ2_threshold = dataPointPtr->valueMQ2_threshold;
        GIZWITS_LOG("Evt:EVENT_MQ2_threshold %d\n",currentDataPoint.valueMQ2_threshold);
        //user handle
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{

    currentDataPoint.valueMQ2_data =MQ2;//Add Sensor Data Collection
    currentDataPoint.valueMQ9_data =MQ9;//Add Sensor Data Collection
    currentDataPoint.valueMQ5_data =MQ5;//Add Sensor Data Collection
    currentDataPoint.valueMQ2_flag =MQ2_flag;//Add Sensor Data Collection
    currentDataPoint.valueMQ9_flag =MQ9_flag;//Add Sensor Data Collection
    currentDataPoint.valueMQ5_flag =MQ5_flag;//Add Sensor Data Collection

	  MQ2_threshold=currentDataPoint.valuesmoke_threshold;//设置烟雾阈值
		MQ5_threshold=currentDataPoint.valueMQ2_threshold;//设置煤气阈值
		MQ9_threshold=currentDataPoint.valueMQ9_threshold;
		beep_flag=currentDataPoint.valuebeep_flag;
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valuebeep_flag = ;
      currentDataPoint.valuesmoke_threshold = ;
      currentDataPoint.valueMQ9_threshold = ;
      currentDataPoint.valueMQ2_threshold = ;
      currentDataPoint.valueMQ2_data = ;
      currentDataPoint.valueMQ9_data = ;
      currentDataPoint.valueMQ5_data = ;
      currentDataPoint.valueMQ2_flag = ;
      currentDataPoint.valueMQ9_flag = ;
      currentDataPoint.valueMQ5_flag = ;
    */

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
         USART_SendData(USART3, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
			while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          USART_SendData(USART3, 0x55);//STM32 test demo
					while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        }
    }


    
    return len;
}


