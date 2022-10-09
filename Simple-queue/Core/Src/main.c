/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "string.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

UART_HandleTypeDef huart2;

//osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
//void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**************** TASK HANDLERS ***********************/
xTaskHandle Sender_HPT_Handler;
xTaskHandle Sender_LPT_Handler;
xTaskHandle Receiver_Handler;

/**************** QUEUE HANDLER ***********************/
xQueueHandle SimpleQueue;
/**************** TASK FUNCTIONS ***********************/
void Sender_HPT_Task (void *argument);
void Sender_LPT_Task (void *argument);
void Receiver_Task (void *argument);

uint8_t Rx_data;




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /************************* Create Integer Queue ****************************/
   SimpleQueue = xQueueCreate(5, sizeof (int)); // creation of a queue which can store 5 integers
   if (SimpleQueue == 0)  // Queue not created like there is an error
   {
 	  char *str = "Unable to create Integer Queue\n\n";
 	  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);
   }
   else
   {
 	  char *str = "Integer Queue Created successfully\n\n";
 	  HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);
   }


   /****************************** TASK RELATED ******************************/

    xTaskCreate(Sender_HPT_Task, "HPT_SEND", 128, NULL, 3, &Sender_HPT_Handler);
    xTaskCreate(Sender_LPT_Task, "LPT_SEND", 128, (void *)111 , 2, &Sender_LPT_Handler);

    xTaskCreate(Receiver_Task, "Receive", 128, NULL, 1, &Receiver_Handler);


    HAL_UART_Receive_IT(&huart2, &Rx_data, 1);

    vTaskStartScheduler();
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
//  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
//  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
//  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PI1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Sender_HPT_Task (void *argument)
{
	int i=222;
	uint32_t TickDelay = pdMS_TO_TICKS(2000);
	while (1)
	{
		char *str = "Entered SENDER_HPT Task\n about to SEND a number to the queue\n\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);

		if (xQueueSend(SimpleQueue, &i, portMAX_DELAY) == pdPASS)
		{
			char *str2 = " Successfully sent the number to the queue\nLeaving SENDER_HPT Task\n\n\n";
			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen (str2), HAL_MAX_DELAY);
		}

		vTaskDelay(TickDelay);
	}
}

void Sender_LPT_Task (void *argument)
{
	int ToSend;
	uint32_t TickDelay = pdMS_TO_TICKS(1000);
	while (1)
	{
		ToSend = (int) argument;
		char *str = "Entered SENDER_LPT Task\n about to SEND a number to the queue\n\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);

		xQueueSend(SimpleQueue, &ToSend, portMAX_DELAY);

		char *str2 = " Successfully sent the number to the queue\nLeaving SENDER_LPT Task\n\n\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen (str2), HAL_MAX_DELAY);

		vTaskDelay(TickDelay);
	}
}

void Receiver_Task (void *argument)
{
	int received=0;
	uint32_t TickDelay = pdMS_TO_TICKS(3000);
	while (1)
	{
		char str[100];
		strcpy (str, "Entered RECEIVER Task\n about to RECEIVE a number from the queue\n\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);

		if (xQueueReceive(SimpleQueue, &received, portMAX_DELAY) != pdTRUE)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *)"Error in Receiving from Queue\n\n", 31, 1000);
		}
		else
		{
			sprintf(str, " Successfully RECEIVED the number %d to the queue\nLeaving RECEIVER Task\n\n\n",received);
			HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen (str), HAL_MAX_DELAY);
		}

		vTaskDelay(TickDelay);
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(huart, &Rx_data, 1);
	int ToSend = 123456789;
	if (Rx_data == 'r')
	{
		 /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
		 it will get set to pdTRUE inside the interrupt safe API function if a
		 context switch is required. */
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if (xQueueSendToFrontFromISR(SimpleQueue, &ToSend, &xHigherPriorityTaskWoken) == pdPASS)
		{
			HAL_UART_Transmit(huart, (uint8_t *)"\n\nSent from ISR\n\n", 17, 500);
		}

		/* Pass the xHigherPriorityTaskWoken value into portEND_SWITCHING_ISR(). If
		 xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR()
		 then calling portEND_SWITCHING_ISR() will request a context switch. If
		 xHigherPriorityTaskWoken is still pdFALSE then calling
		 portEND_SWITCHING_ISR() will have no effect */

		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
//void StartDefaultTask(void const * argument)
//{
//  /* USER CODE BEGIN 5 */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END 5 */
//}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

