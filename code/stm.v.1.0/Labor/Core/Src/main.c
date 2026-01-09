/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
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

/* USER CODE BEGIN PV */


#define ADC_RESOLUTION      4095.0f
#define VREF                3.3f

#define SCALE_CH0_LOW    1.0f    // CH0 → 1V Bereich
#define SCALE_CH0_HIGH   10.0f   // CH0 → 10V Bereich

#define SCALE_CH1_LOW    10.0f   // CH1 → 10V Bereich
#define SCALE_CH1_HIGH   36.0f   // CH1 → 36V Bereich

// DMA/ADC Buffer
volatile uint16_t adcValues[4]; // [PA4, PA5, PA6, PA7]
volatile uint8_t adcReady = 0;  // gesetzt im Callback
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_ADC1_Init();

  /* USER CODE BEGIN 2 */

  // Start ADC in DMA mode (4 channels)
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValues, 4) != HAL_OK)
  {
      Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  if (adcReady)
	    {
	        adcReady = 0;

	        // Rohwerte in ADC-Spannung umrechnen (0..VREF)
	        float va = (float)adcValues[0] * VREF / ADC_RESOLUTION; // PA4 (CH0 low)
	        float vb = (float)adcValues[1] * VREF / ADC_RESOLUTION; // PA5 (CH0 high)
	        float vc = (float)adcValues[2] * VREF / ADC_RESOLUTION; // PA6 (CH1 low)
	        float vd = (float)adcValues[3] * VREF / ADC_RESOLUTION; // PA7 (CH1 high)

	        // Threshold zur Bereichserkennung
	        const float THRESH = 0.05f;
	        char buffer[128];
	        int len = 0; // vorab deklarieren

	        // CH0: wähle aktiven Bereich
	        float CH0;
	        if (va > THRESH)
	        {
	            CH0 = va * SCALE_CH0_LOW;
	            len = snprintf(buffer, sizeof(buffer), "%.6f\n", CH0);
	        }
	        if (vb > THRESH)
	        {
	            CH0 = vb * SCALE_CH0_HIGH;
	            len = snprintf(buffer, sizeof(buffer), "%.6f\n", CH0);
	        }

	        // CH1: wähle aktiven Bereich
	        float CH1;
	        if (vc > THRESH)
	        {
	            CH1 = vc * SCALE_CH1_LOW;
	            len += snprintf(buffer + len, sizeof(buffer) - len, "%.6f\n", CH1);
	        }
	        if (vd > THRESH)
	        {
	            CH1 = vd * SCALE_CH1_HIGH;
	            len += snprintf(buffer + len, sizeof(buffer) - len, "%.6f\n", CH1);
	        }

	        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, 20);
	    }



  }


    /* USER CODE END WHILE */
<<<<<<< HEAD

    /* USER CODE BEGIN 3 */
=======
  }
    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}
>>>>>>> 52c20d3237fc1ca76fba56bffd58218fe4cd2d71

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        // Eine komplette Gruppe von 4 Messwerten ist verfügbar
        adcReady = 1;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler()
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add your own implementation to report the HAL error return state */
  __disable_irq();

  while (1)
  {
    // Optional: LED blinken lassen oder Debug-Meldung senden
  }
}
  /* USER CODE END Error_Handler_Debug */





#ifdef USE_FULL_ASSERT
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
