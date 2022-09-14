#include "hardware.h"
#include "rtc.h"
#include "usart.h"
#include "config.h"
#include "pgmspace.h"

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include <stdbool.h>
#include <stddef.h>


typedef struct {
  char id[26];
  bool hard_reset;         // flag to perform a hard reset
} hw_t;

static hw_t hw = {{0}};


#define PROD_SIGS (*(NVM_PROD_SIGNATURES_t *)0x0000)
#define HEXNIB(x) "0123456789abcdef"[(x) & 0xf]


static void _init_clock()  {
#if 0 // 32Mhz Int RC
  OSC.CTRL |= OSC_RC32MEN_bm | OSC_RC32KEN_bm;  // Enable 32MHz & 32KHz osc
  while (!(OSC.STATUS & OSC_RC32KRDY_bm));      // Wait for 32Khz oscillator
  while (!(OSC.STATUS & OSC_RC32MRDY_bm));      // Wait for 32MHz oscillator

  // Defaults to calibrate against internal 32Khz clock
  DFLLRC32M.CTRL = DFLL_ENABLE_bm;              // Enable DFLL
  CCP = CCP_IOREG_gc;                           // Disable register security
  CLK.CTRL = CLK_SCLKSEL_RC32M_gc;              // Switch to 32MHz clock

#else
  // 12-16 MHz crystal; 0.4-16 MHz XTAL w/ 16K CLK startup
  OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
  OSC.CTRL = OSC_XOSCEN_bm;                // enable external crystal oscillator
  while (!(OSC.STATUS & OSC_XOSCRDY_bm));  // wait for oscillator ready

  OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 2;    // PLL source, 2x (32 MHz sys clock)
  OSC.CTRL = OSC_PLLEN_bm | OSC_XOSCEN_bm; // Enable PLL & External Oscillator
  while (!(OSC.STATUS & OSC_PLLRDY_bm));   // wait for PLL ready

  CCP = CCP_IOREG_gc;
  CLK.CTRL = CLK_SCLKSEL_PLL_gc;           // switch to PLL clock
#endif

  OSC.CTRL &= ~OSC_RC2MEN_bm;              // disable internal 2 MHz clock
}


#ifdef __AVR__
static void _load_hw_id_byte(int i, register8_t *reg) {
  NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
  uint8_t byte = pgm_read_byte(reg);
  NVM.CMD = NVM_CMD_NO_OPERATION_gc;

  hw.id[i] = HEXNIB(byte >> 4);
  hw.id[i + 1] = HEXNIB(byte);
}
#endif // __AVR__


static void _read_hw_id() {
#ifdef __AVR__
  int i = 0;
  _load_hw_id_byte(i, &PROD_SIGS.LOTNUM5); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.LOTNUM4); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.LOTNUM3); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.LOTNUM2); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.LOTNUM1); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.LOTNUM0); i += 2;
  hw.id[i++] = '-';
  _load_hw_id_byte(i, &PROD_SIGS.WAFNUM);  i += 2;
  hw.id[i++] = '-';
  _load_hw_id_byte(i, &PROD_SIGS.COORDX1); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.COORDX0); i += 2;
  hw.id[i++] = '-';
  _load_hw_id_byte(i, &PROD_SIGS.COORDY1); i += 2;
  _load_hw_id_byte(i, &PROD_SIGS.COORDY0); i += 2;
  hw.id[i] = 0;

#else // __AVR__
  for (int i = 0; i < 25; i++)
    hw.id[i] = '0';
  hw.id[25] = 0;
#endif // __AVR__
}


/// Lowest level hardware init
void hw_init() {
  _init_clock();                           // set system clock
  rtc_init();                              // real time counter
  _read_hw_id();

  // Round-robin, interrupts in application section, all interupts levels
  CCP = CCP_IOREG_gc;
  PMIC.CTRL =
    PMIC_RREN_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
}


void hw_request_hard_reset() {hw.hard_reset = true;}


static void _hard_reset() {
  usart_flush();
  cli();
  CCP = CCP_IOREG_gc;
  RST.CTRL = RST_SWRST_bm;
}


/// Controller's rest handler
void hw_reset_handler() {
  if (!hw.hard_reset) return;

  // Flush serial port and wait for EEPROM writes to finish
  while (!usart_tx_empty() || !eeprom_is_ready())
    continue;

  _hard_reset();
}


const char *get_hw_id() {return hw.id;}
