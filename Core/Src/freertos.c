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
osThreadId LED3TaskHandle;
osThreadId OLEDTaskHandle;
osThreadId OLEDRefreshHandle;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartLED3Task(void const * argument);
void OLEDShowTask(void const * argument);
void OLEDRefreshTask(void const * argument);

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
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    osThreadDef(LED3Task,       StartLED3Task,      osPriorityNormal,   0, 32);
    osThreadDef(OLEDShow,       OLEDShowTask,       osPriorityRealtime, 0, 128);    // OLED 显示内容
    osThreadDef(OLEDRefresh,    OLEDRefreshTask,    osPriorityNormal,   0, 64);     // OLED 显示刷新
    
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    LED3TaskHandle = osThreadCreate(osThread(LED3Task), NULL);
    OLEDTaskHandle = osThreadCreate(osThread(OLEDShow), NULL);
    OLEDRefreshHandle = osThreadCreate(osThread(OLEDRefresh), NULL);

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
    uint8_t pcWriteBuffer[100];
    uint8_t TaskName[16];
    uint8_t TaskCount = 0;
    uint8_t process = 0;
    for (;;)
    {
        osDelay(10);
        vTaskList((char *)&pcWriteBuffer);
        printf("U\t\tU\tU\tU\tU\r\n");
        printf(pcWriteBuffer);
        printf("\r\n");
        
        osDelay(10);
        printf("\r\nUU\t\tUU\t\tUU\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        
//        for (uint8_t i=0; i<strlen(pcWriteBuffer); i++)
//        {
//            switch (process)
//            {
//                case 0: // 获取任务名
//                    if (pcWriteBuffer[i] == ' ' || pcWriteBuffer[i] == '\t')
//                    {
//                        TaskName[i] = 0;
//                        process++;
//                        break;
//                    }
//                    TaskName[i] = pcWriteBuffer[i];
//                    break;
//                    
//                case 1:
//                    if ()
//            }
//            
//        }
        
        printf("\r\n开始");
        printf(TaskName);
        printf("结束\r\n");
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
        uint8_t str[15] = {0};
        sprintf(str, "MyCount:%d", count++);
        OLED_Show_StrAll(0, 0, str, 1);

        sprintf(str, "Priority:%d", (int)uxTaskPriorityGet(OLEDTaskHandle));
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
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
