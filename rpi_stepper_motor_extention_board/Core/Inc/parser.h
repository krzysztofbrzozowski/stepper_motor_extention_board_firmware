/*
 * parser.h
 *
 *  Created on: Mar 31, 2020
 *      Author: krzysztofbrzozowski
 */

#ifndef INC_PARSER_H_
#define INC_PARSER_H_

#include "stm32f1xx_hal.h"
#include "main.h"

/* DEFINES */
#define UART_RX_BUF_SIZE 128
#define UART_TX_BUF_SIZE 128
#define UART_RX_BUF_MASK (UART_RX_BUF_SIZE - 1)
#define UART_TX_BUF_MASK (UART_TX_BUF_SIZE - 1)

/* NEW S_PARSER */
#define S_PARSER_SERVICES_CNT 18
#define TIM_MULTIPLIER 20

typedef struct {
	uint8_t cmd;
	int8_t (* parse_service)(uint8_t org_cmd, uint8_t * args);
} SPARSERCMD;

extern SPARSERCMD s_parser_cmd[];

/* VARIABLES */
uint8_t rx_arr[16];
uint8_t received[1];
uint8_t rx_buffer[16];
volatile uint8_t rec_data;
volatile uint8_t UART_RxHead;
volatile uint8_t UART_RxTail;
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TXTail;
volatile uint8_t UART_RxBuf[UART_RX_BUF_SIZE];
volatile uint8_t UART_TXBuf[UART_TX_BUF_SIZE];

volatile uint8_t prh_flag;
volatile uint8_t stp_flag;
volatile uint8_t tmp_counter;
volatile uint8_t run_m_0_flag;
volatile uint8_t run_m_1_flag;
volatile uint8_t run_m_2_flag;
volatile uint8_t take_photo_flag;
volatile uint8_t command_flag;
volatile uint8_t counter;
volatile uint8_t ph_counter;
//volatile uint32_t steps_amount;
volatile uint32_t stps_amt_m_0;
volatile uint32_t stps_amt_m_1;
volatile uint32_t stps_amt_m_2;

volatile uint32_t stps_tim_m_0;
volatile uint32_t stps_tim_m_1;
volatile uint32_t stps_tim_m_2;

volatile uint32_t tim_stp_m_0;
volatile uint32_t tim_stp_m_1;
volatile uint32_t tim_stp_m_2;

volatile uint8_t write_nvm_flag;
volatile uint8_t w_r_flag;

volatile uint8_t timeout_counter;
volatile uint8_t timeout;

uint8_t NVM_momory_map[40];
uint8_t drv;

/* DECLARATIONS */
/* -> CYCLIC BUFFER */
void register_uart_rx_event_callback(void (*callback)(uint8_t * p_buf));
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t * uart_get_data(uint8_t * buf);
void UART_RX_EVENT(uint8_t * rx_buf);
int uart_get_byte(void);

/* -> PARSERS */
void p_parser(uint8_t * p_buf);
void m_parser(uint8_t * p_buf);

int8_t enable_service(uint8_t in_msg, uint8_t * args);
int8_t direction_service(uint8_t in_msg, uint8_t * args);
int8_t mode_service(uint8_t in_msg, uint8_t * args);
int8_t sleep_service(uint8_t in_msg, uint8_t * args);

/* -> DECODERS */
void init_battery_ctrl(void);
void init_usbc_ctrl(void);

/* -> PERIPHERY MANAGERS */
void usb_controller_manage(uint8_t cmd, uint8_t rw);
void bat_controller_manage(uint8_t cmd, uint8_t rw);
void main_controller_manage(uint8_t cmd, uint8_t rw);


#endif /* INC_PARSER_H_ */
