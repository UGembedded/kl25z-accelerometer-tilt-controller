/* UART polling configuration */
#include "MKL25Z4.h"
#include "UART0_TXRX2.h"

static int UART_FindDividers(uint32_t source_hz, uint32_t baud,
                      uint32_t *osr, uint32_t *sbr)
{
    uint32_t o, d, candidate, actual, difference;
    uint32_t best_error = 0xFFFFFFFFUL;
    /* Bounds cover KL25Z UART0 clock limits and this application's baud rate. */
    if (osr == 0 || sbr == 0 || source_hz == 0U || source_hz > 48000000UL ||
        baud < 300U || baud > 3000000UL) { return APP_ARGUMENT; }
    *osr = 0U;
    *sbr = 0U;
    for (o = 4U; o <= 32U; ++o) {
        d = source_hz / (baud * o);
        if (d == 0U) { d = 1U; }
        for (candidate = d; candidate <= d + 1U; ++candidate) {
            if (candidate > 8191U) { continue; }
            actual = source_hz / (o * candidate);
            difference = actual > baud ? actual - baud : baud - actual;
            if (difference <= best_error) {
                best_error = difference;
                *osr = o;
                *sbr = candidate;
            }
        }
    }
    if (*osr == 0U || best_error > baud / 50U) { return APP_CLOCK; }
    return APP_OK;
}

int UART0_init(void)
{
    uint32_t osr, sbr;
    int status = UART_FindDividers(Board_PeripheralClockHz(), 57600U, &osr, &sbr);
    if (status != APP_OK) { return status; }
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    UART0->C2 = 0U;
    SIM->SOPT2 = (SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(1);
    SIM->SOPT5 &= ~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK |
                    SIM_SOPT5_UART0ODE_MASK);
    PORTA->PCR[1] = PORT_PCR_MUX(2);
    PORTA->PCR[2] = PORT_PCR_MUX(2);
    UART0->C1 = 0U;
    UART0->C3 = 0U;
    UART0->C4 = UART0_C4_OSR(osr - 1U);
    UART0->C5 = (osr < 8U) ? UART0_C5_BOTHEDGE_MASK : 0U;
    UART0->BDH = (uint8_t)((sbr >> 8) & UART0_BDH_SBR_MASK);
    UART0->BDL = (uint8_t)sbr;
    UART0->C2 = UART0_C2_TE_MASK;
    return APP_OK;
}

int sendStr(const char *text, int length)
{
    int i;
    uint32_t start;
    if (text == 0 || length < 0) { return APP_ARGUMENT; }
    for (i = 0; i < length; ++i) {
        start = Board_Millis();
        while ((UART0->S1 & UART0_S1_TDRE_MASK) == 0U) {
            if ((uint32_t)(Board_Millis() - start) >= 20U) { return APP_TIMEOUT; }
        }
        UART0->D = (uint8_t)text[i];
    }
    return APP_OK;
}

int sendHelloWorld(void)
{
    static const char message[] = "KL25Z MMA8451Q RGB\r\n";
    return sendStr(message, (int)sizeof(message) - 1);
}
