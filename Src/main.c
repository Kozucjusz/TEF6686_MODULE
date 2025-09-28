#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "TEF6686.h"
#include <stdio.h>
#include <string.h>

COM_InitTypeDef BspCOMInit;
__IO uint32_t BspButtonState = BUTTON_RELEASED;

void SystemClock_Config(void);
void send_string(char* s);

void I2C_Scan() {
    char buffer[64]; // Bufor do przechowywania sformatowanych danych
    send_string("Scanning I2C bus...\n");
    uint8_t i2c_address;
    HAL_StatusTypeDef result;
    int devices_found = 0;

    for (i2c_address = 1; i2c_address < 254; i2c_address++) {
        // Sprawdzanie dostępności urządzenia
        result = HAL_I2C_IsDeviceReady(&hi2c1, i2c_address, 1, 10);

        if (result == HAL_OK) { // Urządzenie odpowiedziało
            //sprintf(buffer, "Device found at address 0x%02X\n", i2c_address);
        	sprintf(buffer, "\n%d\n\n", i2c_address);
            send_string(buffer); // Wyślij przez UART
        	send_string("git\n");
            devices_found++;
        }
        else {
        	//sprintf(buffer, "%d\n", i2c_address);
        	//send_string(buffer); // Wyślij przez UART
        }
    }

    if (devices_found == 0) {
        send_string("No I2C devices found.\n");
    }
    send_string("Scanning completed.\n");
}

void send_string(char* s)
{
	//HAL_UART_Transmit(&huart1, (uint8_t*)s, strlen(s), 1000);
  printf(s);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  I2C_Scan();
  //uint8_t address;
  if(HAL_I2C_IsDeviceReady(&hi2c1, 0xC9, 1, 1000) == HAL_OK)
  {
	  TEF6686_Init(&hi2c1);
	  send_string("init_done\n");
	  TEF6686_Tune_To(32, 9840); // tune FM to 103,6
	  send_string("tune_done\n");
	  TEF6686_Audio_Set_Mute(0); // disable mute
	  send_string("git\n");
	  HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_0);
  } else {
    //HAL_GPIO_WritePin (LED2_GPIO_PORT, LED2_PIN, 1);
	  send_string("doopsko\n");
  }
  //I2C_Scan();



  while (1)
  {
	  HAL_Delay(500);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_USER)
  {
    BspButtonState = BUTTON_PRESSED;
  }
}

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
