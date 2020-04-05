/*
 * parser_functions.h
 *
 *  Created on: Mar 31, 2020
 *      Author: krzysztofbrzozowski
 */

#ifndef INC_PARSER_FUNCTIONS_H_
#define INC_PARSER_FUNCTIONS_H_

#include "stm32f1xx_hal.h"
#include "main.h"

/* DEFINES */
#define SINGLE_BIT_MASK	 	0b00000001
#define STEP_MOTOR_MASK 	0b00001111
#define TAKE_PHOTO_MASK		0b00110000
#define PRH_KEY 			0b11100111
#define TIM					0b101
#define STP					0b110
#define PRH					0b111
#define END_KEY 			0b11111111

#define enable_ax(x) 		(HAL_GPIO_WritePin(AX##x##_ENBL_GPIO_Port, AX##x##_ENBL_Pin, GPIO_PIN_RESET))
#define disable_ax(x) 		(HAL_GPIO_WritePin(AX##x##_ENBL_GPIO_Port, AX##x##_ENBL_Pin, GPIO_PIN_SET))
#define cw_ax(x) 			(HAL_GPIO_WritePin(AX##x##_DIR_GPIO_Port, AX##x##_DIR_Pin, GPIO_PIN_RESET))
#define ccw_ax(x) 			(HAL_GPIO_WritePin(AX##x##_DIR_GPIO_Port, AX##x##_DIR_Pin, GPIO_PIN_SET))

#define setPinMode(m, ax) 	(HAL_GPIO_WritePin(AX##ax##_MODE##m##_GPIO_Port, AX##ax##_MODE##m##_Pin, GPIO_PIN_SET))
#define resetPinMode(m, ax) (HAL_GPIO_WritePin(AX##ax##_MODE##m##_GPIO_Port, AX##ax##_MODE##m##_Pin, GPIO_PIN_RESET))

#define P_PARSER_SERVICES_CNT 11


/* VARIABLES */
uint8_t stp_motor_selected;
uint8_t cut[16];
uint8_t read_status_flag;

struct {
	uint8_t cfg_mask : 1;
	uint8_t srv_mask : 2;
	uint8_t mtr_mask : 2;
	uint8_t val_mask : 3;
} t_m_data_decode;

struct {
	uint8_t cfg_mask : 1;
	uint8_t prh_maskor : 3;
	uint8_t cmd_maskor : 4;
} t_p_data_decode;

struct {
	uint8_t usbc_ctrl : 2;
	uint8_t batt_ctrl : 2;
	uint8_t main_ctrl : 2;
} prh_num;

/* Control union for battery controller */
typedef union {
	struct {
		uint8_t cmd_r_actual_a[2];			/* Actual current */
		uint8_t cmd_r_actual_avg_a[2];
		uint8_t cmd_r_actual_u[2];
		uint8_t cmd_r_actual_b_temp[2];		/* Actual battery temperature */
		uint8_t cmd_r_actual_f_temp[2];		/* Actual FET temperature */
		uint8_t cmd_r_rel_state_of_charge[2];
	} cmds;

	uint8_t iterator[12];
} t_battery_ctrl_commands;
t_battery_ctrl_commands battery_ctrl;

typedef union {
	struct {
		uint8_t cmd_r_resp[2];
		uint8_t cmd_r_max_a[2];			/* Actual current */
		uint8_t cmd_r_max_v[2];
	} cmds;

	uint8_t iterator[4];
} t_usbc_ctrl_commands;
t_usbc_ctrl_commands usbc_ctrl;


typedef struct {
	uint8_t cmd;
	int8_t (* parse_service)(uint8_t org_cmd, uint8_t * args);
} PRHCMD;

extern PRHCMD p_parser_cmd[];


/* DECLARATIONS */
/* PARSER FUNCTIONS */
uint8_t cfg_mask(uint8_t msg);
uint8_t mtr_mask(uint8_t msg);
uint8_t srv_mask(uint8_t msg);
uint8_t val_mask(uint8_t msg);
uint8_t prh_mask(uint8_t msg);
uint8_t cmd_mask(uint8_t msg);
uint8_t dir_mask(uint8_t msg);
uint8_t motor_run(uint8_t msg);
uint8_t take_photo(uint8_t msg);

/* MOTOR FUNCTIONS */
void reset_all_modes(void);

/* PERIPHERY FUNCTIONS */
int8_t act_current(uint8_t in_msg, uint8_t * args);
int8_t avg_current(uint8_t in_msg, uint8_t * args);
int8_t act_voltage(uint8_t in_msg, uint8_t * args);
int8_t act_temperatures(uint8_t in_msg, uint8_t * args);
int8_t bat_rel_SOC(uint8_t in_msg, uint8_t * args);
int8_t bat_abs_SOC(uint8_t in_msg, uint8_t * args);
int8_t bat_run_time_to_empty(uint8_t in_msg, uint8_t * args);
int8_t bat_avg_time_to_empty(uint8_t in_msg, uint8_t * args);
int8_t bat_time_to_full(uint8_t in_msg, uint8_t * args);
int8_t bat_state_of_health(uint8_t in_msg, uint8_t * args);
int8_t bat_cycle_count(uint8_t in_msg, uint8_t * args);

/* OTHER */
void init_masks(void);
uint8_t value_in_array(uint8_t val, uint8_t * buf);
uint8_t * encode(uint8_t * buf, uint8_t size);
void toggle_indicator(char *color);
void display_confirmation(uint8_t blinks);
void CUST_NVMFlash(uint8_t * NVMMap);

#endif /* INC_PARSER_FUNCTIONS_H_ */
