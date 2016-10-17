#ifndef MAX11645_H_
#define MAX11645_H_

#include <avr/io.h>

// VREF
#define MAX11645_VREF_VDD				0x00U
#define MAX11645_VREF_EXTERNAL			0x20U
#define MAX11645_VREF_INTERNAL_OFF		0x40U
#define MAX11645_VREF_INTERNAL			0x50U
#define MAX11645_VREF_INTERNAL_OUT_OFF	0x60U
#define MAX11645_VREF_INTERNAL_OUT		0x70U
// Clock
#define MAX11645_CLK_EXTERNAL			0x08U
#define MAX11645_CLK_INTERNAL 			0x00U
// Input setup
#define MAX11645_BIPOLAR				0x40U
#define MAX11645_UNIPOLAR				0x00U
// Reset
#define MAX11645_NORESET				0x02U
#define MAX11645_RESET					0x00U

// Scanning configuration
#define MAX11645_SCAN_UP				0x00U
#define MAX11645_SCAN_CS0_EIGHT			0x20U
#define MAX11645_SCAN_CS0				0x60U
// Channel selection
#define MAX11645_CS0					0x02U
// Input selection
#define MAX11645_SINGLE					0x01U
#define MAX11645_DIFFERENTIAL			0x00U

void MAX11645_Init(TWI_t *twi);
uint8_t MAX11645_SetSetup(uint8_t setup);
uint8_t MAX11645_GetSetup();
uint8_t MAX11645_SetConfig(uint8_t config);
uint8_t MAX11645_GetConfig();
uint8_t MAX11645_Configure(uint8_t setup, uint8_t config);
uint8_t MAX11645_Read(uint16_t *value);
uint8_t MAX11645_ReadScan(uint16_t *buffer, uint8_t length);
uint8_t MAX11645_Reset();

#endif
