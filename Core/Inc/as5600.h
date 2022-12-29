//Header include
#ifndef AS5600_INCLUDED
#define AS5600_INCLUDED

/// includes
#include <stdint.h>
#include "stm32f1xx_hal.h" //board lib
#include "stdlib.h"

/// AS5600 address
#define AS5600_SLAVE_ADDRESS			0x36
#define AS5600_SHIFTED_SLAVE_ADDRESS	0x6c//(for create spacing for R/W bit)
#define AS5600_I2C_TIMEOUT_DEFAULT		10	// can be choosen from (1 - 30) ms
#define I2C_MEMADD_SIZE_8BIT            0x00000001U //from HAL library


//datasheet : https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf

/* AS5600 configuration registers */
#define AS5600_REGISTER_ZMCO			0x00
#define AS5600_REGISTER_ZPOS_HIGH		0x01
#define AS5600_REGISTER_ZPOS_LOW		0x02
#define AS5600_REGISTER_MPOS_HIGH		0x03
#define AS5600_REGISTER_MPOS_LOW		0x04
#define AS5600_REGISTER_MANG_HIGH		0x05
#define AS5600_REGISTER_MANG_LOW		0x06
#define AS5600_REGISTER_CONF_HIGH		0x07
#define AS5600_REGISTER_CONF_LOW		0x08
/* AS5600 output registers */
#define AS5600_REGISTER_RAW_ANGLE_HIGH	0x0C
#define AS5600_REGISTER_RAW_ANGLE_LOW	0x0D
#define AS5600_REGISTER_ANGLE_HIGH		0x0E
#define AS5600_REGISTER_ANGLE_LOW		0x0F
/* AS5600 status registers */
#define AS5600_REGISTER_STATUS			0x0B
#define AS5600_REGISTER_AGC				0x1A
#define AS5600_REGISTER_MAGNITUDE_HIGH	0x1B
#define AS5600_REGISTER_MAGNITUDE_LOW	0x1C
#define AS5600_REGISTER_BURN			0xFF

//Ignoring power setting register , hysteresis register , pwm , slow filte , fast filter ,watchdog setting

/* AS5600 direction definitions */
#define AS5600_DIR_CW					1
#define AS5600_DIR_CCW					2

/* AS5600 bit mask */
#define AS5600_12_BIT_MASK				(uint16_t)4095 // 4096 resolution
/* AS5600 angle conversions */
#define AS5600_DEG_CONV 8.7890625e-2    /* 360/4096 */
#define AS5600_RAD_CONV 1.5339808e-3    /* 2pi/4096 */

typedef struct {	// variables are placed from largest to smallest to keep the struct as small as possible
    I2C_HandleTypeDef*	i2c_handle;
    GPIO_TypeDef*		dir_port;
    uint32_t			i2c_timeout;
    uint16_t			dir_pin;
    uint8_t				positive_rotation_direction;
     uint8_t	config_register[2];
} AS5600_TypeDef;

#endif

AS5600_TypeDef*		AS5600_new								(void);
HAL_StatusTypeDef	AS5600_init								(AS5600_TypeDef* handle);
HAL_StatusTypeDef 	AS5600_set_start_position				(AS5600_TypeDef* const handle, const uint16_t position);

HAL_StatusTypeDef	AS5600_get_rawAngle						(AS5600_TypeDef* const handle, float* const angle);
HAL_StatusTypeDef	AS5600_get_angle						(AS5600_TypeDef* const handle, float* const angle);

AS5600_TypeDef* AS5600_new(void) { return (AS5600_TypeDef*)calloc(1, sizeof(AS5600_TypeDef)); }
HAL_StatusTypeDef AS5600_init(AS5600_TypeDef* handle) {

	if (!(handle->i2c_timeout))					{ handle->i2c_timeout = AS5600_I2C_TIMEOUT_DEFAULT; }
	if (!(handle->positive_rotation_direction))	{ handle->positive_rotation_direction = AS5600_DIR_CW; }

	return HAL_OK;
}

HAL_StatusTypeDef AS5600_write_config_register(AS5600_TypeDef* const handle) //writing bit on specified register
{
    return HAL_I2C_Mem_Write(handle->i2c_handle, AS5600_SHIFTED_SLAVE_ADDRESS, AS5600_REGISTER_CONF_HIGH, I2C_MEMADD_SIZE_8BIT, handle->config_register, 2, handle->i2c_timeout);
}


//getting raw angle data from reading
HAL_StatusTypeDef AS5600_get_rawAngle(AS5600_TypeDef* const handle, float* const angle)
{
	uint8_t data[2] = {0};
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(handle->i2c_handle, AS5600_SHIFTED_SLAVE_ADDRESS, AS5600_REGISTER_RAW_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, data, 2, handle->i2c_timeout);
	*angle = ((data[0] << 8) | data[1]);
	return status;
}
HAL_StatusTypeDef AS5600_get_angle(AS5600_TypeDef* const handle, float* const angle)
{
    uint8_t data[2] = {0};
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(handle->i2c_handle, AS5600_SHIFTED_SLAVE_ADDRESS, AS5600_REGISTER_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, data, 2, handle->i2c_timeout);
	*angle = ((data[0] << 8) | data[1]);
	return status;
}
