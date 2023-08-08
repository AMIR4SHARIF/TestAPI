/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include ""
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//------- user added libraries
#include "cJSON.h"
#include "AHT20.h"
#include "InfoDATA.h"
#include "RTC_CONF.h"
// ------ server
#include "w5500.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "W5500_SPI.h"
#include "sntp.h"
#include "HTTP.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_USER_START_ADDR   ((uint32_t)0x0800FF00)
#define FLASH_USER_END_ADDR     ((uint32_t)0x0800FFFF)
#define DATA_SIZE               ((uint32_t)0x000000FF)

#define NETINFO_MAC  	  0x00,0x08,0xdc,0x00,0xab,0xcd
#define NETINFO_IP   	  192,168,1,123
#define NETINFO_SUBMASK   255,255,255,0
#define NETINFO_GATEWAY   192,168,1,1
#define NETINFO_DNS   	  8,8,8,8
#define SNTP_IP   	      31,214,170,254

#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define PORT_TCPS		5000
#define PORT_UDPS       3000

#define REL_port GPIOB
#define REL1_pin GPIO_PIN_12
#define REL2_pin GPIO_PIN_13
#define REL3_pin GPIO_PIN_14
#define REL4_pin GPIO_PIN_15

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define DATABUFSIZE   2048
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
datetime DATE;
InfoDATA_craetor InfoDATA;
#define MAX_HTTPSOCK	6
uint8_t socknumlist[] = {1, 2, 3, 4, 5, 6};
char *JSONBUF; //JSON addres
uint8_t HTTPBUF[513];

st_http_request HTTPREQ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void network_init(void);
void RTC_NTP_sync(void);
void UPDATE_JSON(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

wiz_NetInfo gWIZNETINFO1 = {
		.mac = {NETINFO_MAC},
		.ip = {NETINFO_IP},
		.sn = {NETINFO_SUBMASK},
		.gw = {NETINFO_GATEWAY},
		.dns = {NETINFO_DNS},
		.dhcp = NETINFO_STATIC };
wiz_NetInfo gWIZNETINFO2;
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
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
    uint8_t tmp;
    uint8_t memsize[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

  //  -- WIZCHIP SPI INIT
    printf("WIZCHIP SPI Init ...\n\r");
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
    W5500_Restart();  // hardware restart through RESET pin

  //  -- WIZCHIP SOCKET Buffer initialize
    printf("WIZCHIP SOCKET Buffer Init ...\n\r");
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
      {
      	printf("WIZCHIP buffer Initialized fail.\r\n");
         while(1);
      }
  //  -- PHY link status check
    printf("PHY Checking ...\n\r");
    do{
         if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
      	   printf("Unknown PHY Link stauts.\r\n");
    }while(tmp == PHY_LINK_OFF);

  //  -- network initializing
    printf("Net initilize ...\n\r");
    network_init();

    printf("RTC NTP ...\n\r");
    RTC_NTP_sync();

    AHT20_Init(&hi2c2);
    HAL_ADC_Start(&hadc1);

    strncpy(InfoDATA.UinqID, "X132HT89CLQPS32S", sizeof(InfoDATA.UinqID));
    strncpy(InfoDATA.Version, "CLT.1.3", sizeof(InfoDATA.Version));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  AHT20_Measure();
	  InfoDATA.TEMP=(double)((int)(AHT20_Temp()*100)/100.0);
	  InfoDATA.HUMID=(double)((int)(AHT20_Humid()*100)/100.0);
	  InfoDATA.LPG=HAL_ADC_GetValue(&hadc1);
	  InfoDATA.SMK=HAL_ADC_GetValue(&hadc1)+200;
	  UPDATE_JSON();
	  calcdatetime(LL_RTC_TIME_Get(RTC)+ Twn_offset, &DATE);
      printf("\r\nDate: %d, %d/%02d/%02d %02d:%02d:%02d\r\n",DATE.wd,DATE.yy,DATE.mo,DATE.dd,DATE.hh,DATE.mm,DATE.ss);

	  //HTTP Request making
	  HTTPREQ.METHOD=METHOD_POST;
	  strcpy((char*)HTTPREQ.URI,"server");
	  HTTPREQ.API[0]=0;
	  HTTPREQ.DATE[0]=DATE.wd+1;  HTTPREQ.DATE[1]=(uint8_t)(DATE.yy%100);    HTTPREQ.DATE[2]=DATE.mo;    HTTPREQ.DATE[3]=DATE.dd;
	  HTTPREQ.DATE[4]=DATE.hh;	  HTTPREQ.DATE[5]=DATE.mm;        HTTPREQ.DATE[6]=DATE.ss;
	  strcpy((char*)HTTPREQ.HOST,"185.230.163.241");
	  HTTPREQ.PORT=8000;
	  HTTPREQ.TYPE=PTYPE_JSON;
	  HTTP_ERROR_HANDLER(HTTP_REQ_make(HTTPREQ,(uint8_t *) JSONBUF, HTTPBUF));
	  printf("This is Output:\r\n");
	  printf((char*)HTTPBUF);

	  HAL_Delay(1000);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};
  LL_RTC_TimeTypeDef RTC_TimeStruct = {0};

    LL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_BKP);
  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC and set the Time and Date
  */
  RTC_InitStruct.AsynchPrescaler = 0x8000;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  LL_RTC_SetAsynchPrescaler(RTC, 0x8000);

  /** Initialize RTC and set the Time and Date
  */
  RTC_TimeStruct.Hours = 0;
  RTC_TimeStruct.Minutes = 0;
  RTC_TimeStruct.Seconds = 0;
  LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_TimeStruct);
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RFID_SELECT_Pin|SPI_RST_Pin|SPI_SELECT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RELAY_1_Pin|RELAY_2_Pin|RELAY_3_Pin|RELAY_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RFID_SELECT_Pin SPI_RST_Pin SPI_SELECT_Pin */
  GPIO_InitStruct.Pin = RFID_SELECT_Pin|SPI_RST_Pin|SPI_SELECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RELAY_1_Pin RELAY_2_Pin RELAY_3_Pin RELAY_4_Pin */
  GPIO_InitStruct.Pin = RELAY_1_Pin|RELAY_2_Pin|RELAY_3_Pin|RELAY_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void UPDATE_JSON(void){
		cJSON *root = cJSON_CreateObject();
	    cJSON_AddNumberToObject(root, "temp", InfoDATA.TEMP);
	    cJSON_AddNumberToObject(root, "humid", InfoDATA.HUMID);
	    cJSON_AddNumberToObject(root, "lpg", InfoDATA.LPG);
	    cJSON_AddNumberToObject(root, "smk", InfoDATA.SMK);
	    cJSON_AddStringToObject(root, "uid", InfoDATA.UinqID);
	    cJSON_AddStringToObject(root, "ver", InfoDATA.Version);
	    JSONBUF = cJSON_Print(root);
	    // Clean up cJSON objects
	    cJSON_Delete(root);
}


/********************************** NTP and RTC ******************************************/
void RTC_NTP_sync(void){
	  LL_RTC_EnterInitMode(RTC);
	  LL_RTC_DisableWriteProtection(RTC);
	  uint8_t Timebuf[48]={0};
	  uint8_t ntpserverip[4] ={SNTP_IP};
	  datetime TIME_DST;
	  printf("SNTP initializing\r\n");
	  SNTP_init(0, ntpserverip, 21, Timebuf);
	  printf("SNTP Running\r\n");
	  tstamp S=0;
	  do{
		 HAL_Delay(100);
		 S = SNTP_run(&TIME_DST);
	  }while(S==0);
	  LL_RTC_TIME_Set(RTC,(uint32_t)(S - Twn_offset));
	  printf("SNTP success, configing RTC\r\n");
	  LL_RTC_ExitInitMode(RTC);
	  LL_RTC_EnableWriteProtection(RTC);
}
/********************************** END - NTP and RTC ******************************************/

/********************************** UART REDIRECT ******************************************/
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/********************************** END - UART REDIRECT ******************************************/

/********************************** Network Init ******************************************/
void network_init(void)
{
   uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO1);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO2);
	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO2.mac[0],gWIZNETINFO2.mac[1],gWIZNETINFO2.mac[2],
		  gWIZNETINFO2.mac[3],gWIZNETINFO2.mac[4],gWIZNETINFO2.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO2.ip[0],gWIZNETINFO2.ip[1],gWIZNETINFO2.ip[2],gWIZNETINFO2.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO2.gw[0],gWIZNETINFO2.gw[1],gWIZNETINFO2.gw[2],gWIZNETINFO2.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO2.sn[0],gWIZNETINFO2.sn[1],gWIZNETINFO2.sn[2],gWIZNETINFO2.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO2.dns[0],gWIZNETINFO2.dns[1],gWIZNETINFO2.dns[2],gWIZNETINFO2.dns[3]);
	printf("======================\r\n");
}
/********************************** END - Network Init ******************************************/

/* USER CODE END 4 */

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