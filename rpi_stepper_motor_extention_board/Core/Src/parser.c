/*
 * parser.c
 *
 *  Created on: Mar 31, 2020
 *      Author: krzysztofbrzozowski
 */

#include "parser.h"
#include "parser_functions.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "i2c.h"



static void (*uart_rx_event_callback)(uint8_t * pBuf);

void register_uart_rx_event_callback(void (*callback)(uint8_t * p_buf)) {
	uart_rx_event_callback = callback;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	register uint8_t tmp_head;
	tmp_head = (UART_RxHead + 1) & UART_RX_BUF_MASK;

	if(tmp_head == UART_RxTail) {
		UART_RxHead = UART_RxTail;
	} else {
		switch(received[0]) {
			case 0: rec_data++;
			default: UART_RxHead = tmp_head; UART_RxBuf[tmp_head] = received[0];
		}
	}
	HAL_UART_Receive_DMA(&huart1, received, 1);
	command_flag = 1;
}


int uart_get_byte(void) {
	if(UART_RxHead == UART_RxTail) { return -1; }
	UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
	return UART_RxBuf[UART_RxTail];
}

uint8_t * uart_get_data(uint8_t * buf) {
	int data;
	uint8_t * wsk = buf;
	if(rec_data) {
		while( (data = uart_get_byte()) ) {
			*buf++ = data;
			if(data == 0) break;
		}
		*buf = 0;
		rec_data--;
	}
	return wsk;
}

void UART_RX_EVENT(uint8_t * rx_buf) {
	if(rec_data) {
		if(uart_rx_event_callback) {
			uart_get_data(rx_buf);
			(*uart_rx_event_callback)(rx_buf);
		}
		else { UART_RxHead = UART_RxTail; }
	}
}


SPARSERCMD s_parser_cmd[S_PARSER_SERVICES_CNT] = {
		{ 0x00, enable_service },
		{ 0x01, direction_service },
		{ 0x02, mode_service },
		{ 0x03, sleep_service },
};

int8_t enable_service(uint8_t in_msg, uint8_t * args) {
	uint8_t tmp = 1;

	if(val_mask(in_msg) == TIM) {
		uint32_t tmp_buf = 0;
		while(args[tmp]) {
			tmp_buf = tmp != 1 ? tmp_buf << 8 : 0;
			tmp_buf |= args[tmp];
			tmp++;
		}

		switch(mtr_mask(in_msg)) {
			case 0:
				stps_tim_m_0 = tmp_buf * TIM_MULTIPLIER;
				if(stps_amt_m_0) { tim_stp_m_0 = stps_tim_m_0 / stps_amt_m_0; }
				break;
			case 1:
				stps_tim_m_1 = tmp_buf * TIM_MULTIPLIER;
				if(stps_amt_m_1) { tim_stp_m_1 = stps_tim_m_1 / stps_amt_m_1; }
				break;
			case 2:
				stps_tim_m_2 = tmp_buf * TIM_MULTIPLIER;
				if(stps_amt_m_2) { tim_stp_m_2 = stps_tim_m_2 / stps_amt_m_2; }
				break;
		}
		return 0;
	}

	if(val_mask(in_msg) == STP) {
		uint32_t tmp_buf = 0;
		while(args[tmp]) {
			tmp_buf = tmp != 1 ? tmp_buf << 8 : 0;
			tmp_buf |= args[tmp];
			tmp++;
		}

		switch(mtr_mask(in_msg)) {
			case 0:
				stps_amt_m_0 = tmp_buf + 1;
				if(stps_tim_m_0) { tim_stp_m_0 = stps_tim_m_0 / stps_amt_m_0; }
				break;
			case 1:
				stps_amt_m_1 = tmp_buf + 1;
				if(stps_tim_m_1) { tim_stp_m_1 = stps_tim_m_1 / stps_amt_m_1; }
				break;
			case 2:
				stps_amt_m_2 = tmp_buf + 1;
				if(stps_tim_m_2) { tim_stp_m_2 = stps_tim_m_2 / stps_amt_m_0; }
				break;
		}

		return 0;
	}

	if(val_mask(in_msg) == PRH) {
//		memset(cut, 0, sizeof(cut));
		uint8_t service = rx_arr[1];
		int8_t (*_srv)(uint8_t in_msg, uint8_t * args);
		for(uint8_t i = 0; i < P_PARSER_SERVICES_CNT + 1; i++) {
			if(service == p_parser_cmd[i].cmd) {
				_srv = p_parser_cmd[i].parse_service;
				if(_srv) { _srv(rx_arr[1], rx_arr); }
				break;
			}
		}
	}

	else {
		switch(mtr_mask(in_msg)) {
			case 0: val_mask(in_msg) == 0x01 ? disable_ax(0) : enable_ax(0); break;
			case 1: val_mask(in_msg) == 0x01 ? disable_ax(1) : enable_ax(1); break;
			case 2: val_mask(in_msg) == 0x01 ? disable_ax(2) : enable_ax(2); break;
		}
		return 0;
	}

	return 0;
}

int8_t direction_service(uint8_t in_msg, uint8_t * args) {
	switch(mtr_mask(in_msg)) {
		case 0: val_mask(in_msg) == 0x01 ? cw_ax(0) : ccw_ax(0); break;
		case 1: val_mask(in_msg) == 0x01 ? cw_ax(1) : ccw_ax(1); break;
		case 2: val_mask(in_msg) == 0x01 ? cw_ax(2) : ccw_ax(2); break;
	}
	return 0;
}

int8_t mode_service(uint8_t in_msg, uint8_t * args) {
	reset_all_modes();
	switch(mtr_mask(in_msg)) {
		case 0:
			if(val_mask(in_msg) & 4) setPinMode(2, 0);
			if(val_mask(in_msg) & 2) setPinMode(1, 0);
			if(val_mask(in_msg) & 1) setPinMode(0, 0);
			break;
		case 1:
			if(val_mask(in_msg) & 4) setPinMode(2, 1);
			if(val_mask(in_msg) & 2) setPinMode(1, 1);
			if(val_mask(in_msg) & 1) setPinMode(0, 1);
			break;
		case 2:
			if(val_mask(in_msg) & 4) setPinMode(2, 2);
			if(val_mask(in_msg) & 2) setPinMode(1, 2);
			if(val_mask(in_msg) & 1) setPinMode(0, 2);
			break;
	}
	return 0;
}

int8_t sleep_service(uint8_t in_msg, uint8_t * args) {
	return 0;
}

//	MAIN PARSER.
void m_parser(uint8_t * p_buf) {
	uint8_t size = 0;
//	uint8_t rx_arr[16] = {0};

	while(p_buf[size]) { size++; }

	for(uint8_t i = 1; i < size; i++) {
		rx_arr[i - 1] = p_buf[i] ^ p_buf[0];
	}

	uint8_t lead_msg = rx_arr[0];

	if(lead_msg & 0x80) {
		uint8_t service = srv_mask(rx_arr[0]);
		int8_t (*_srv)(uint8_t in_msg, uint8_t * args);
		for(uint8_t i = 0; i < S_PARSER_SERVICES_CNT + 1; i++) {
			if(service == s_parser_cmd[i].cmd) {
				_srv = s_parser_cmd[i].parse_service;
				if(_srv) { _srv(rx_arr[0], rx_arr); }
				break;
			}
		}
	}

	else {
//		run_m_0_flag = 0;
//		run_m_1_flag = 0;
//		run_m_2_flag = 0;

		if(motor_run(lead_msg) & 1)		{ if(!run_m_0_flag) { run_m_0_flag = 1; } }
		if(motor_run(lead_msg) & 2) 	{ if(!run_m_1_flag) { run_m_1_flag = 1; } }
		if(motor_run(lead_msg) & 4) 	{ if(!run_m_2_flag) { run_m_2_flag = 1; } }
	}

	memset(rx_arr, 0, sizeof(rx_arr));
//	HAL_UART_Transmit_IT(&huart1, &rx_arr[0], 3);
}

