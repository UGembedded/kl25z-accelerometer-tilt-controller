
#include "MKL25Z4.h"
#include "board.h"

static volatile uint32_t milliseconds;
static uint32_t bus_clock_hz;
static uint32_t peripheral_clock_hz;

int Board_Init(void)
{
    uint32_t divider_core, divider_bus, mcg_clock, selected_clock;

    SIM->COPC = 0U;
    SystemCoreClockUpdate();
    if ((MCG->C1 & MCG_C1_CLKS_MASK) != 0U || SystemCoreClock == 0U ||
        SystemCoreClock > 48000000UL) { return APP_CLOCK; }
    divider_core = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >>
                     SIM_CLKDIV1_OUTDIV1_SHIFT) + 1U;
    divider_bus = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >>
                    SIM_CLKDIV1_OUTDIV4_SHIFT) + 1U;
    mcg_clock = SystemCoreClock * divider_core;
    bus_clock_hz = mcg_clock / divider_bus;
    if (bus_clock_hz > 24000000UL || bus_clock_hz < 1000000UL) {
        return APP_CLOCK;
    }
    selected_clock = SIM->SOPT2 & ~SIM_SOPT2_PLLFLLSEL_MASK;
    if ((MCG->S & MCG_S_PLLST_MASK) != 0U) {
        if ((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(3)) {
            return APP_CLOCK;
        }
        selected_clock |= SIM_SOPT2_PLLFLLSEL_MASK;
        peripheral_clock_hz = mcg_clock / 2U;
    } else {
        if ((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(0)) {
            return APP_CLOCK;
        }
        peripheral_clock_hz = mcg_clock;
    }
    SIM->SOPT2 = selected_clock;
    milliseconds = 0U;
    if (SysTick_Config(SystemCoreClock / 1000U) != 0U) { return APP_CLOCK; }
    __enable_irq();
    return APP_OK;
}

void SysTick_Handler(void) { ++milliseconds; }
uint32_t Board_Millis(void) { return milliseconds; }
uint32_t Board_BusClockHz(void) { return bus_clock_hz; }
uint32_t Board_PeripheralClockHz(void) { return peripheral_clock_hz; }
void Board_DelayMs(uint32_t duration)
{
    uint32_t start = Board_Millis();
    while ((uint32_t)(Board_Millis() - start) < duration) { __WFI(); }
}
