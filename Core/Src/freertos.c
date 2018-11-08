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
#include "string.h"
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
/* ������� */
osThreadId LED3TaskHandle;
osThreadId OLEDTaskHandle;          // OLED ��ʾ����
osThreadId OLEDRefreshHandle;       // OLED ��ʾˢ��
osThreadId ReadTaskStateHandle;     // ��ʾ����״̬

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
    osThreadDef(LED3Task,       StartLED3Task,      osPriorityNormal,       0, 32);
    osThreadDef(OLEDShow,       OLEDShowTask,       osPriorityAboveNormal,  0, 128);    // OLED ��ʾ����
    osThreadDef(OLEDRefresh,    OLEDRefreshTask,    osPriorityNormal,       0, 64);     // OLED ��ʾˢ��
    osThreadDef(ReadTaskState,  ReadTaskStateTask,  osPriorityNormal,       0, 128);    // ��ʾ����״̬
    
    
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    LED3TaskHandle      = osThreadCreate(osThread(LED3Task),        NULL);
    OLEDTaskHandle      = osThreadCreate(osThread(OLEDShow),        NULL);      // OLED ��ʾ����
    OLEDRefreshHandle   = osThreadCreate(osThread(OLEDRefresh),     NULL);      // OLED ��ʾˢ��
    ReadTaskStateHandle = osThreadCreate(osThread(ReadTaskState),   NULL);      // ��ʾ����״̬
    
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
        osDelay(500);
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
//        char str[20] = {0};           // �����Ǹ����ʣ���*str�Ͳ���
//        sprintf(str, "����:%d", count++);
//        OLED_Show_StrAll(0, 0, str, 1);
        
        OLED_Show_Pic(11, 25, IMG64[(count++/15)%5], 64, 64, 1);
        switch ((count++/15)%5)
        {
            case 0: OLED_Show_StrAll(23, 95, 16, "�� ��", 1);  break;
            case 1: OLED_Show_StrAll(23, 95, 16, "Ӱ ��", 1);  break;
            case 2: OLED_Show_StrAll(23, 95, 16, "�� ��", 1);  break;
            case 3: OLED_Show_StrAll(23, 95, 16, "�� Ϸ", 1);  break;
            case 4: OLED_Show_StrAll(23, 95, 16, "�� ��", 1);  break;
            
        }
        
        
        OLED_Refresh_Gram();
    }
}



void OLEDRefreshTask(void const * argument)
{
    for (;;)
    {
        osDelay(40);    // ���25֡
//        OLED_Refresh_Gram();
    }
}

char RunTimeInfo[512];
void ReadTaskStateTask(void const * argument)
{
//    char *pcWriteBuffer = 0;
    for (;;)
    {
        osDelay(10);
        if (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == GPIO_PIN_SET)
        {
            // Ϊʲô����4���������ȫ����ʾ
//            vTaskList((char *)&pcWriteBuffer);
//    //        printf("U\t\tU\tU\tU\tU\r\n");
//            printf("������      ����״̬  ���ȼ�  ʣ��ջ �������\r\n");
//            printf(pcWriteBuffer);
//            printf("\r\n");
//            
//    //        printf("\r\nUU\t\tUU\t\tUU\r\n");
            
            
            TaskHandle_t TaskHandle;        // ������
            TaskStatus_t TaskStatus;        // ������Ϣ�ṹ��
            
            uint32_t TotalRunTime = 0;      
            UBaseType_t ArraySize = uxTaskGetNumberOfTasks();   // ��ȡ��������
            TaskStatus_t *StatusArray = pvPortMalloc(ArraySize * sizeof(TaskStatus_t)); // �����ڴ�
            if (StatusArray != NULL)
            {
                ArraySize = uxTaskGetSystemState((TaskStatus_t *)StatusArray,
                                                 (UBaseType_t   )ArraySize,
                                                 (uint32_t *    )&TotalRunTime);
                
                printf("%-15s   %-8s   %-8s   %-8s   %-8s   %-8s   %-10s \r\n",
                       "������",
                       "�����ȼ�",
                       "��ǰ����",
                       "�������",
                       "����״̬",
                       "ջʷСֵ",
                       "��ջ����ַ");
                for (UBaseType_t i=0; i<ArraySize; i++)
                {
                    TaskHandle = xTaskGetHandle(StatusArray[i].pcTaskName); // ��ȡ������
                    vTaskGetInfo((TaskHandle_t      )TaskHandle,
                                 (TaskStatus_t *    )&TaskStatus,
                                 (BaseType_t        )10,
                                 (eTaskState        )eInvalid);

                    char stateStr[8] = {0};
                    switch (TaskStatus.eCurrentState)
                    {
                        case eRunning:  sprintf(stateStr, "����");  break;
                        case eReady:    sprintf(stateStr, "����");  break;
                        case eBlocked:  sprintf(stateStr, "����");  break;
                        case eSuspended:sprintf(stateStr, "��ͣ");  break;
                        case eDeleted:  sprintf(stateStr, "ɾ��");  break;
                        case eInvalid:  sprintf(stateStr, "��Ч");  break;
                    }
                    
                    printf("%-15s   %-8d   %-8d   %-8d   %-8s   %-8d   %-#10x \r\n",
                           StatusArray[i].pcTaskName,               // ������
                           (int)TaskStatus.uxBasePriority,          // ��������ȼ�
                           (int)TaskStatus.uxCurrentPriority,       // ����ǰ���ȼ�
                           (int)TaskStatus.xTaskNumber,             // �������
                           stateStr,                                // ��������״̬
                           (int)TaskStatus.usStackHighWaterMark,    // �����ջ��ʷ��Сֵ
                           (int)TaskStatus.pxStackBase              // �����ջ����ַ
                           
                           );
                }
                printf("\r\n");
                printf("������          ����ʱ��        ʹ����\r\n");
                memset(RunTimeInfo, 0, 400);
                vTaskGetRunTimeStats(RunTimeInfo);
                printf("%s\r\n", RunTimeInfo);
            }
            vPortFree(StatusArray);
            
            
            
            while (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == GPIO_PIN_SET);
        }
    }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
