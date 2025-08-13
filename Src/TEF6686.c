/*
 * TEF6686.c
 *
 *  Created on: Dec 12, 2024
 *      Author: kmurawski
 */


#include <TEF6686.h>
#include <i2c.h>

uint8_t DEVICE_ADDR = 0xC9;
uint8_t MODULE_FM = 32;
uint8_t MODULE_AM = 33;
uint8_t MODULE_AUDIO = 48;
uint8_t MODULE_APPL = 64;

void TEF6686_GetCommand(	I2C_HandleTypeDef *hi2c,
							uint8_t module,
							uint8_t cmd,
							uint16_t *response,
							uint8_t responseLength)
{
	uint8_t txData[3] = { module, cmd, 1 };

	// Wyślij komendę za pomocą HAL_I2C_Master_Transmit
	if (HAL_I2C_Master_Transmit(&hi2c1, DEVICE_ADDR, txData, 3, HAL_MAX_DELAY) != HAL_OK)	Error_Handler();

	// Odbierz dane za pomocą HAL_I2C_Master_Receive
	uint8_t rxData[responseLength * 2];
	if (HAL_I2C_Master_Receive(&hi2c1, DEVICE_ADDR, rxData, responseLength * 2, HAL_MAX_DELAY) != HAL_OK)	Error_Handler();

	// Przetwórz odebrane dane
	for (int i = 0; i < responseLength; i++) {
		response[i] = (rxData[2 * i] << 8) | rxData[2 * i + 1];
	}
}

void TEF6686_SetCommand(	I2C_HandleTypeDef *hi2c,
							uint8_t module,
							uint8_t cmd,
							uint16_t *params,
							uint8_t paramsCount)
{
	uint8_t txData[3 + paramsCount * 2];
	txData[0] = module;
	txData[1] = cmd;
	txData[2] = 1;

	// Dodaj parametry
	for (int i = 0; i < paramsCount; i++) {
		txData[3 + 2 * i] = params[i] >> 8; // MSB
		txData[3 + 2 * i + 1] = params[i] & 0xFF; // LSB
	}
	// Wyślij dane za pomocą HAL_I2C_Master_Transmit
	if (HAL_I2C_Master_Transmit(&hi2c1, DEVICE_ADDR, txData, 3 + paramsCount * 2, HAL_MAX_DELAY) != HAL_OK)	Error_Handler();
}

void TEF6686_WriteInitData(I2C_HandleTypeDef *hi2c1, const uint8_t *data)
{
    const uint8_t *pa = data;
    uint8_t len, first;

    while (1)
    {
        len = *pa++;
        first = *pa;
        if (!len)	break;
        if (len == 2 && first == 0xFF)
        {
            uint8_t delaytime = *(++pa);
            HAL_Delay(delaytime);
            pa++;
        }
        else
        {
            if (HAL_I2C_Master_Transmit(hi2c1, DEVICE_ADDR, (uint8_t *)pa, len, HAL_MAX_DELAY) != HAL_OK)	Error_Handler();
            pa += len;
        }
    }
}

void TEF6686_Appl_Set_OperationMode(uint8_t mode)
{
    uint16_t param[1] = {mode};
    TEF6686_SetCommand(&hi2c1, MODULE_APPL, 1, param, 1);
}

void TEF6686_Init(I2C_HandleTypeDef *hi2c1)
{
    HAL_Delay(40);

    uint16_t state[1];
    do
    {
    	TEF6686_GetCommand(hi2c1, 64, 128, state, 1);
        if (state[0] < 2)		TEF6686_WriteInitData(hi2c1, INIT_DATA); // Użyj funkcji WriteInitData skonwertowanej na STM32
        else if (state[0] >= 2)	TEF6686_Appl_Set_OperationMode(1);
    } while (state[0] < 2);
}

void TEF6686_Tune_To(uint8_t module, uint16_t freq)
{
    uint16_t params[2] = {1, freq};
    TEF6686_SetCommand(&hi2c1, module, 0x1, params, 2);
    //Currentfreq = freq;
    //memset(rdsData.rtText, 0, sizeof(rdsData.rtText));
    //memset(rdsData.psText, 0, sizeof(rdsData.psText));
}

void TEF6686_Audio_Set_Mute(uint8_t mute)
{
    uint16_t unmuteParams[1] = {mute};
    TEF6686_SetCommand(&hi2c1, MODULE_AUDIO, 11, unmuteParams, 1);
}


