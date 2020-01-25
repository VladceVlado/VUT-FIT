/*
 * VUT FIT IMP 2018/2019
 * Author: Vladimir Dusek, xdusek27
 * Date: 18/12/2018
 */

#include "MK60D10.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define RNGA_NUMBERS 1000000
#define BUFFER_SIZE 16
#define DELAY_LENGTH 1000

// Initialization of MCG (Multipurpose Clock Generator)
void MCG_init()
{
    MCG_C4 = 0x00;

    // DCO (Digitally Controlled Oscillator) is fine-tuned for maximum frequency with 32.768 kHz reference
    MCG_C4 |= MCG_C4_DMX32_MASK;

    // DCO Range Select - Encoding 1 � Mid range
    MCG_C4 |= MCG_C4_DRST_DRS(0x01);
}

// Initialization of pins
void pins_init()
{
    // Enable CLOCKs for UART5, RNGA and PORT-E
    SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
    SIM->SCGC3 = SIM_SCGC3_RNGA_MASK;
    SIM->SCGC5 = SIM_SCGC5_PORTE_MASK;

    // PORT E - Transmitter
    PORTE->PCR[8]  = PORT_PCR_MUX(0x03); // UART5_TX
}

// Initialization of UART5 module
void UART5_init()
{
    UART5->C1  = 0x00; // 8 data bits, Parity function disabled
    UART5->C2  = 0x00;
    UART5->C3  = 0x00;
    UART5->C4  = 0x0F; // Oversampling ratio 16, match address mode disabled
    UART5->BDH = 0x00;
    UART5->BDL = 0x1A; // Baud rate 115 200 Bd, 1 stop bit
    UART5->MA1 = 0x00; // no match address (mode disabled in C4)
    UART5->MA2 = 0x00; // no match address (mode disabled in C4)
    UART5->S2 |= 0xC0;
    UART5->C2 |= UART_C2_TE_MASK; // Transmitter on
}

// Initialization of RNGA module
void RNGA_init()
{
    RNG_CR = 0x00;
    RNG_CR |= RNG_CR_GO_MASK;
}

// Delay function
void delay(int bound)
{
  for (int i = 0; i < bound; i++) {
      ;
  }
}

// Store character to UART Data Register
void send_char(char ch)
{
    while (!(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK)) {
        ;
    }

    UART5->D = ch;
}

// Send string through UART module
void send_string(char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        send_char(str[i]);
    }
}

// Send number through UART module
void send_number(uint32_t num)
{
    char str[BUFFER_SIZE];
    sprintf(str, "%u", num);
    send_string(str);
    send_string("\r\n");
}

// Insert seed to Entropy Register for better results
void insert_seed(uint32_t seed)
{
    RNG_ER_EXT_ENT(seed);
}

// Return true if data are pushed to Output Register, false otherwise
bool output_ready()
{
    // Test if RNGA is in Sleep mode
    if (RNG->SR & RNG_SR_SLP_MASK) {
        return false;
    }

    // Test if random word is in RNGA Output Register
    if (!(RNG->SR & RNG_SR_OREG_LVL_MASK)) {
        return false;
    }

    return true;
}

// Return content of Output Register
uint32_t get_random_number()
{
    // Wait till random word is ready in Output Register
    while (!output_ready()) {
        ;
    }

    return RNG_OR;
}

// Main
int main(void)
{
    MCG_init();
    pins_init();
    UART5_init();
    RNGA_init();

    delay(DELAY_LENGTH);

    uint32_t number;

    for (int i = 0; i < RNGA_NUMBERS; i++) {

        // Insert seed using time()
        insert_seed((uint32_t) time(NULL));

        // Get random 32bit number
        number = get_random_number();

        // Send number through UART
        send_number(number);
    }

    // Embedded app should never leave main function
    while (1) {
        ;
    }

    return 0;
}
