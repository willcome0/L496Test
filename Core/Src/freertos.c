/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "common.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* 定义服务 */
osThreadId LED3TaskHandle;
osThreadId OLEDTaskHandle;          // OLED 显示内容
osThreadId OLEDRefreshHandle;       // OLED 显示刷新
osThreadId ReadTaskStateHandle;     // 显示任务状态

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartLED3Task(void const * argument);
void OLEDShowTask(void const * argument);
void OLEDRefreshTask(void const * argument);
void ReadTaskStateTask(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 32);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    osThreadDef(LED3Task,       StartLED3Task,      osPriorityNormal,   0, 32);
    osThreadDef(OLEDShow,       OLEDShowTask,       osPriorityNormal,   0, 128);    // OLED 显示内容
    osThreadDef(OLEDRefresh,    OLEDRefreshTask,    osPriorityNormal,   0, 64);     // OLED 显示刷新
    osThreadDef(ReadTaskState,  ReadTaskStateTask,  osPriorityNormal,   0, 128);    // 显示任务状态
    
    
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    LED3TaskHandle      = osThreadCreate(osThread(LED3Task),        NULL);
    OLEDTaskHandle      = osThreadCreate(osThread(OLEDShow),        NULL);      // OLED 显示内容
    OLEDRefreshHandle   = osThreadCreate(osThread(OLEDRefresh),     NULL);      // OLED 显示刷新
    ReadTaskStateHandle = osThreadCreate(osThread(ReadTaskState),   NULL);      // 显示任务状态
    
    /* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    
    for (;;)
    {
        osDelay(10);
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        
//        osDelay(100);
//        printf("Temp:91.79 \r\n");
//        printf("Hum:109.23 \n");
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartLED3Task(void const * argument)
{
    for (;;)
    {
        osDelay(250);
        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        
    }
}

void OLEDShowTask(void const * argument)
{
    for (;;)
    {
        osDelay(40);
        static uint32_t count = 0;
        char str[20] = {0};           // 这里是个疑问，用*str就不行
        sprintf(str, "计数:%d", count++);
        OLED_Show_StrAll(0, 0, str, 1);

        sprintf(str, "优先级:%d", (int)uxTaskPriorityGet(OLEDTaskHandle));
        OLED_Show_StrAll(0, 15, str, 1);
    }
}



void OLEDRefreshTask(void const * argument)
{
    for (;;)
    {
        osDelay(40);    // 大概25帧
        OLED_Refresh_Gram();
    }
}

void ReadTaskStateTask(void const * argument)
{
//    char *pcWriteBuffer = 0;
    for (;;)
    {
        osDelay(10);
        if (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == GPIO_PIN_SET)
        {
            // 为什么大于4个任务就完全不显示
//            vTaskList((char *)&pcWriteBuffer);
//    //        printf("U\t\tU\tU\tU\tU\r\n");
//            printf("任务名      运行状态  优先级  剩余栈 任务序号\r\n");
//            printf(pcWriteBuffer);
//            printf("\r\n");
//            
//    //        printf("\r\nUU\t\tUU\t\tUU\r\n");
//            printf("任务名      运行次数         使用率\r\n");
//            vTaskGetRunTimeStats((char *)&pcWriteBuffer);
//            printf(pcWriteBuffer);
//            printf("\r\n");
            uint32_t TotalRunTime = 0;
            UBaseType_t ArraySize = uxTaskGetNumberOfTasks();   // 获取任务数量
            TaskStatus_t *StatusArray = pvPortMalloc(ArraySize * sizeof(TaskStatus_t)); // 申请内存
            if (StatusArray != NULL)
            {
                ArraySize = uxTaskGetSystemState((TaskStatus_t *)StatusArray,
                                                 (UBaseType_t   )ArraySize,
                                                 (uint32_t *    )&TotalRunTime);
                
                for (UBaseType_t i=0; i<ArraySize; i++)
                {
                    printf("%-15s \t %-6d \t %-6d\r\n",
                           StatusArray[i].pcTaskName,
                           (int)StatusArray[i].uxCurrentPriority,
                           (int)StatusArray[i].xTaskNumber);
                }
            }
            vPortFree(StatusArray);
            
            while (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == GPIO_PIN_SET);
        }
    }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
