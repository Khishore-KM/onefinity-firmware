#pragma once

#include <avr/io.h>

#include <stdint.h>
#include <stdbool.h>


// NOTE, RING_BUF_INDEX_TYPE must be be large enough to cover the buffer
#define USART_TX_RING_BUF_SIZE 1024
#define USART_RX_RING_BUF_SIZE 1024


typedef enum {
  USART_BAUD_9600,
  USART_BAUD_19200,
  USART_BAUD_38400,
  USART_BAUD_57600,
  USART_BAUD_115200,
  USART_BAUD_230400,
  USART_BAUD_460800,
  USART_BAUD_921600,
  USART_BAUD_500000,
  USART_BAUD_1000000
} baud_t;


typedef enum {
  USART_NONE,
  USART_EVEN,
  USART_ODD,
} parity_t;


typedef enum {
  USART_1STOP,
  USART_2STOP,
} stop_t;


typedef enum {
  USART_5BITS,
  USART_6BITS,
  USART_7BITS,
  USART_8BITS,
  USART_9BITS,
} bits_t;


void usart_set_baud(USART_t *port, baud_t baud);
void usart_set_parity(USART_t *port, parity_t parity);
void usart_set_stop(USART_t *port, stop_t stop);
void usart_set_bits(USART_t *port, bits_t bits);
void usart_init_port(USART_t *port, baud_t baud, parity_t parity, bits_t bits,
                     stop_t stop);

void usart_init();
void usart_putc(char c);
void usart_puts(const char *s);
int8_t usart_getc();
char *usart_readline();
void usart_flush();

void usart_rx_flush();
int16_t usart_rx_fill();
int16_t usart_rx_space();
inline bool usart_rx_empty() {return !usart_rx_fill();}
inline bool usart_rx_full() {return !usart_rx_space();}

int16_t usart_tx_fill();
int16_t usart_tx_space();
inline bool usart_tx_empty() {return !usart_tx_fill();}
inline bool usart_tx_full() {return !usart_tx_space();}
