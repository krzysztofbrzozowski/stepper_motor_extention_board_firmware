/*
 * parser_functions.c
 *
 *  Created on: Mar 31, 2020
 *      Author: krzysztofbrzozowski
 */


#include "parser_functions.h"
#include "usart.h"
#include "tim.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <i2c.h>

/*M_PARSER FUNCTIONS */
uint8_t cfg_mask(uint8_t msg)	{ return (msg >> 7) & t_m_data_decode.cfg_mask; }		/* X xx xx xxx -> packet selection  	*/
uint8_t mtr_mask(uint8_t msg) 	{ return (msg >> 5) & t_m_data_decode.mtr_mask; }		/* x XX xx xxx -> motor selection 		*/
uint8_t srv_mask(uint8_t msg) 	{ return (msg >> 3) & t_m_data_decode.srv_mask; }		/* x xx XX xxx -> action selection		*/
uint8_t val_mask(uint8_t msg) 	{ return msg & t_m_data_decode.val_mask; }				/* x xx xx XXX -> value selection 		*/

/* P_PARSER FUNCTIONS */
uint8_t prh_mask(uint8_t msg)	{ return (msg >> 4) & t_p_data_decode.prh_maskor; }
uint8_t cmd_mask(uint8_t msg)	{ return msg & t_p_data_decode.cmd_maskor; }
uint8_t dir_mask(uint8_t msg) 	{ return msg & t_m_data_decode.cfg_mask; }
uint8_t is_true(uint8_t msg) 	{ return msg & t_m_data_decode.cfg_mask; }
uint8_t motor_run(uint8_t msg) 	{ return msg & STEP_MOTOR_MASK; }
uint8_t take_photo(uint8_t msg) { return msg & TAKE_PHOTO_MASK; }


/* MOTOR FUNCTION */
void reset_all_modes(void) {
	HAL_GPIO_WritePin(AX0_MODE0_GPIO_Port, AX0_MODE0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AX0_MODE1_GPIO_Port, AX0_MODE1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AX0_MODE2_GPIO_Port, AX0_MODE2_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(AX1_MODE0_GPIO_Port, AX1_MODE0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AX1_MODE1_GPIO_Port, AX1_MODE1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AX1_MODE2_GPIO_Port, AX1_MODE2_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(AX2_MODE0_GPIO_Port, AX2_MODE0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AX2_MODE1_GPIO_Port, AX2_MODE1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AX2_MODE2_GPIO_Port, AX2_MODE2_Pin, GPIO_PIN_RESET);
}


/* OTHER */
void display_confirmation(uint8_t blinks) {
	return;
}


PRHCMD p_parser_cmd[P_PARSER_SERVICES_CNT] = {
		{ 0x90, act_current },
		{ 0x91, avg_current },
		{ 0x92, act_voltage },
		{ 0x93, act_temperatures },
		{ 0x94, bat_rel_SOC },
		{ 0x95, bat_abs_SOC },
		{ 0x96, bat_run_time_to_empty },
		{ 0x97, bat_avg_time_to_empty },
		{ 0x98, bat_time_to_full },
		{ 0x99, bat_state_of_health },
		{ 0x9A, bat_cycle_count }

};

void init_masks(void) {
	t_m_data_decode.val_mask = 0x07;
	t_m_data_decode.mtr_mask = 0x03;
	t_m_data_decode.srv_mask = 0x03;
	t_m_data_decode.cfg_mask = 0x01;

	t_p_data_decode.cmd_maskor = 0x0F;
	t_p_data_decode.prh_maskor = 0x07;
	t_p_data_decode.cfg_mask = 0x01;

	prh_num.main_ctrl = 0x02;
	prh_num.batt_ctrl = 0x01;
	prh_num.usbc_ctrl = 0x00;
}

int8_t act_current(uint8_t in_msg, uint8_t * args) {
	uint8_t reg_addr = 0x0A;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, &cut, 2);
	if(cut[0]) {
		HAL_UART_Transmit_DMA(&huart1, cut, 2);
//		display_confirmation(3);
	}
//	else { HAL_GPIO_WritePin(LED_INDICATOR_G_GPIO_Port, LED_INDICATOR_G_Pin, GPIO_PIN_RESET); }
	return 0;
}

int8_t avg_current(uint8_t in_msg, uint8_t * args) {
	uint8_t reg_addr = 0x0B;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 2);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_IT(&huart1, cut, 2); }
	return 0;
}

int8_t act_voltage(uint8_t in_msg, uint8_t * args) {
	uint8_t reg_addr = 0x09;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 2);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_DMA(&huart1, cut, 2); }
	return 0;
}

int8_t act_temperatures(uint8_t in_msg, uint8_t * args) {
	uint8_t reg_addr = 0x72;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 15);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_IT(&huart1, cut, 15); }
	return 0;
}

int8_t bat_rel_SOC(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x0D;
	read_status_flag = 0;
	if(I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 1) == HAL_OK) { read_status_flag = 4; HAL_UART_Transmit_DMA(&huart1, cut, 1); }
	return 0;
}


int8_t bat_abs_SOC(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x0E;
	read_status_flag = 0;
	if(I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 1) == HAL_OK) { read_status_flag = 4; HAL_UART_Transmit_DMA(&huart1, cut, 1); }
	return 0;
}


int8_t bat_run_time_to_empty(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x11;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 2);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_DMA(&huart1, cut, 2); }
	return 0;
}

int8_t bat_avg_time_to_empty(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x12;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 2);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_DMA(&huart1, cut, 2); }
	return 0;
}

int8_t bat_time_to_full(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x13;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 2);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_DMA(&huart1, cut, 2); }
	return 0;
}

int8_t bat_state_of_health(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x4F;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 1);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_DMA(&huart1, cut, 1); }
	return 0;
}

int8_t bat_cycle_count(uint8_t in_msg, uint8_t * args){
	uint8_t reg_addr = 0x17;
	I2C_Read_BAT_CTRL(BQ40Z60_ADDR, reg_addr, cut, 1);
	read_status_flag = cut[0] ? 4 : 0;
	if(read_status_flag) { HAL_UART_Transmit_DMA(&huart1, cut, 1); }
	return 0;
}


uint8_t value_in_array(uint8_t val, uint8_t * buf) {
    for(uint8_t i = 0; i < sizeof(buf) / sizeof(buf[0]); i++) { if(buf[i] == val) { return 1; } }
    return 0;
}

uint8_t * encode(uint8_t * buf, uint8_t size) {
	uint8_t key = 1;
	uint8_t * wsk = buf;
//	uint8_t size = 0;
//	while(buf[size]) { size++; }
//	size = size;
	uint8_t stop_t = 0;
	for(uint8_t i = 1; (i < 255 + 1) && !stop_t ; i++) { if(!value_in_array(i, buf)) { key = i; break; } }
	for(uint8_t i = 0; i < size; i++) { buf[i] = key ^ buf[i]; }
//
	for(int c = (sizeof(buf) / sizeof(buf[0])) - 1; c >= 0; c--) { buf[c + 1] = buf[c]; }
	buf[0] = key;
	buf[size + 1] = 0;
	return wsk;
}

void toggle_indicator(char *color) {
	if(!strcmp(color, "red"))	{ HAL_GPIO_TogglePin(LED_INDICATOR_R_GPIO_Port, LED_INDICATOR_R_Pin); }
	if(!strcmp(color, "green"))	{ HAL_GPIO_TogglePin(LED_INDICATOR_G_GPIO_Port, LED_INDICATOR_G_Pin); }
	if(!strcmp(color, "blue"))	{ HAL_GPIO_TogglePin(LED_INDICATOR_B_GPIO_Port, LED_INDICATOR_B_Pin); }
}
