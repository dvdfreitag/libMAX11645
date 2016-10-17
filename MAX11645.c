#include <avr/io.h>

#include "MAX11645.h"
#include "TWI.h"

#define MAX11645_ADDRESS		0x6CU
// Setup byte
#define MAX11645_SETUP			0x80U
// Configuration byte
#define MAX11645_CONFIG			0x00U
// Channels
#define MAX11645_MAX_CHANNELS	0x02U

TWI_t *TWI;
uint8_t Setup;
uint8_t Config;

void MAX11645_Init(TWI_t *twi)
{
	TWI = twi;
	Setup = MAX11645_SETUP | MAX11645_NORESET;  // Default value
	Config = MAX11645_CONFIG;					// Default value
}

uint8_t MAX11645_SetSetup(uint8_t setup)
{
	Setup = setup | MAX11645_SETUP | MAX11645_NORESET;

	TWI_Start(TWI);

	if (TWI_WriteByte(MAX11645_ADDRESS) != TWI_ACK) goto error;
	if (TWI_WriteByte(Setup) != TWI_ACK) goto error;

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}

uint8_t MAX11645_GetSetup()
{
	return Setup;
}

uint8_t MAX11645_SetConfig(uint8_t config)
{
	Config = config & ~MAX11645_SETUP;

	TWI_Start(TWI);

	if (TWI_WriteByte(MAX11645_ADDRESS) != TWI_ACK) goto error;
	if (TWI_WriteByte(Config) != TWI_ACK) goto error;

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}

uint8_t MAX11645_GetConfig()
{
	return Config;
}

uint8_t MAX11645_Configure(uint8_t setup, uint8_t config)
{
	Setup = setup | MAX11645_SETUP | MAX11645_NORESET;
	Config = config & ~MAX11645_SETUP;

	TWI_Start(TWI);

	if (TWI_WriteByte(MAX11645_ADDRESS) != TWI_ACK) goto error;
	if (TWI_WriteByte(Setup) != TWI_ACK) goto error;
	if (TWI_WriteByte(Config) != TWI_ACK) goto error;

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}

uint8_t MAX11645_Read(uint16_t *value)
{
	TWI_Start(TWI);

	if (TWI_WriteByte(MAX11645_ADDRESS | 0x01) != TWI_ACK) goto error;
	*value = ((uint16_t)TWI_ReadByte(TWI_ACK)) << 8;
	*value |= (uint16_t)TWI_ReadByte(TWI_ACK);

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}

uint8_t MAX11645_ReadScan(uint16_t *buffer, uint8_t length)
{
	uint8_t count = 0;
	uint8_t cs0 = (Config & MAX11645_CS0) != 0;
	uint8_t *lower = (uint8_t *)buffer;
	uint8_t *upper = &((uint8_t *)buffer)[1];
	// Determine how many contiguous bytes can be read
	if (Config & MAX11645_SCAN_CS0_EIGHT)
	{
		count = 8;
	}
	else if (Config & MAX11645_SCAN_UP)
	{
		count = MAX11645_MAX_CHANNELS - cs0;
	}
	else
	{
		count = 1;
	}

	TWI_Start(TWI);

	if (TWI_WriteByte(MAX11645_ADDRESS | 0x01) != TWI_ACK) goto error;

	for (uint8_t i = 0; i < length; i++)
	{
		*upper = TWI_ReadByte(TWI_ACK);
		*lower = TWI_ReadByte(TWI_ACK);

		upper += 2;
		lower += 2;
		// Send repeated start if necessary
		if (((i % count) == 0) && (i != (length - 1))) TWI_Restart();
	}

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}

uint8_t MAX11645_Reset()
{
	TWI_Start(TWI);

	if (TWI_WriteByte(MAX11645_ADDRESS) != TWI_ACK) goto error;
	if (TWI_WriteByte(Setup & ~MAX11645_RESET) != TWI_ACK) goto error;

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}