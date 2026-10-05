/* i2c configuration from:
https://community.nxp.com/t5/Sensors-Knowledge-Base/MMA8451Q-Bare-metal-example-project/ta-p/1127268
 */
#include "MKL25Z4.h"
#include "i2c.h"
#define I2C_TIMEOUT_MS 20U

static int WaitIdle(void)
{
    uint32_t start = Board_Millis();
    while ((I2C0->S & I2C_S_BUSY_MASK) != 0U) {
        if ((uint32_t)(Board_Millis() - start) >= I2C_TIMEOUT_MS) {
            return APP_TIMEOUT;
        }
    }
    return APP_OK;
}

static int WaitByte(void)
{
    uint32_t start = Board_Millis();
    uint8_t flags;
    for (;;) {
        flags = I2C0->S;
        if ((flags & I2C_S_ARBL_MASK) != 0U) {
            I2C0->S = I2C_S_ARBL_MASK | I2C_S_IICIF_MASK;
            return APP_ARBITRATION;
        }
        if ((flags & I2C_S_IICIF_MASK) != 0U) {
            I2C0->S = I2C_S_IICIF_MASK;
            return APP_OK;
        }
        if ((uint32_t)(Board_Millis() - start) >= I2C_TIMEOUT_MS) {
            return APP_TIMEOUT;
        }
    }
}

static int Transmit(uint8_t byte)
{
    int status;
    I2C0->D = byte;
    status = WaitByte();
    if (status != APP_OK) { return status; }
    return ((I2C0->S & I2C_S_RXAK_MASK) != 0U) ? APP_NACK : APP_OK;
}

static int EndTransfer(int status)
{
    int stop_status;
    /* Clearing MST generates STOP if this controller still owns the bus. */
    I2C0->C1 = I2C_C1_IICEN_MASK;
    stop_status = WaitIdle();
    if (status != APP_OK || stop_status != APP_OK) {
   
        I2C0->C1 = 0U;
        I2C0->S = I2C_S_IICIF_MASK | I2C_S_ARBL_MASK;
        I2C0->C1 = I2C_C1_IICEN_MASK;
    }
    return (status != APP_OK) ? status : stop_status;
}

static int BeginRegister(uint8_t address, uint8_t reg)
{
    int status = WaitIdle();
    if (status != APP_OK) { return status; }
    I2C0->S = I2C_S_IICIF_MASK | I2C_S_ARBL_MASK;
    I2C0->C1 = I2C_C1_IICEN_MASK | I2C_C1_TX_MASK | I2C_C1_MST_MASK;
    status = Transmit((uint8_t)(address << 1));
    if (status == APP_OK) { status = Transmit(reg); }
    return status;
}

int i2c_init(void)
{
    if (Board_BusClockHz() == 0U || Board_BusClockHz() > 24000000UL) {
        return APP_CLOCK;
    }
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
    I2C0->C1 = 0U;
    PORTE->PCR[24] = PORT_PCR_MUX(5);
    PORTE->PCR[25] = PORT_PCR_MUX(5);
    /* FRDM board supplies external I2C pull-ups.
     * ICR=0x1F: divider 240, MULT=0. <=100 kHz at bus <=24 MHz.
     */
    I2C0->F = 0x1FU;
    I2C0->C2 = 0U;
    I2C0->FLT = 0U;
    I2C0->SMB = 0U;
    I2C0->S = I2C_S_IICIF_MASK | I2C_S_ARBL_MASK;
    I2C0->C1 = I2C_C1_IICEN_MASK;
    return WaitIdle();
}

int i2c_WriteRegister(uint8_t address, uint8_t reg, uint8_t value)
{
    int status;
    if (address > 0x7FU) { return APP_ARGUMENT; }
    status = BeginRegister(address, reg);
    if (status == APP_OK) { status = Transmit(value); }
    return EndTransfer(status);
}

int I2C_ReadMultiRegisters(uint8_t address, uint8_t first_reg,
                           uint8_t count, uint8_t *values)
{
    uint8_t i, dummy;
    int status;
    if (address > 0x7FU || count == 0U || values == 0) { return APP_ARGUMENT; }
    status = BeginRegister(address, first_reg);
    if (status != APP_OK) { return EndTransfer(status); }
    I2C0->C1 |= I2C_C1_RSTA_MASK;
    status = Transmit((uint8_t)((address << 1) | 1U));
    if (status != APP_OK) { return EndTransfer(status); }

    /* Switch to RX; arrange NACK before starting a single-byte reception. */
    I2C0->C1 = I2C_C1_IICEN_MASK | I2C_C1_MST_MASK |
               ((count == 1U) ? I2C_C1_TXAK_MASK : 0U);
    dummy = I2C0->D; /* Dummy read initiates reception. */
    (void)dummy;
    for (i = 0U; i < count; ++i) {
        status = WaitByte();
        if (status != APP_OK) { return EndTransfer(status); }
        if (count > 1U && i == (uint8_t)(count - 2U)) {
            I2C0->C1 |= I2C_C1_TXAK_MASK;
        }
        if (i == (uint8_t)(count - 1U)) {
            /* STOP before reading the final byte, so no extra RX starts. */
            I2C0->C1 &= (uint8_t)~I2C_C1_MST_MASK;
        }
        values[i] = I2C0->D;
    }
    return EndTransfer(APP_OK);
}


volatile int I2C_LastError = APP_OK;
uint8_t I2C_ReadRegister(uint8_t address, uint8_t reg)
{
    uint8_t value = 0U;
    I2C_LastError = I2C_ReadMultiRegisters(address, reg, 1U, &value);
    return value;
}
