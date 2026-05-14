#include "hardware.h"
#include "config.h"
#include "tm4c123gh6pm.h"

static void Hardware_Init_PortF(void)
{
    SYSCTL_RCGCGPIO_R |= RCGCGPIO_F;
    while ((SYSCTL_PRGPIO_R & RCGCGPIO_F) == 0) {}

    GPIO_PORTF_LOCK_R = 0x4C4F434BU;
    GPIO_PORTF_CR_R  |= SW2;

    GPIO_PORTF_AMSEL_R &= ~(LED_MASK | SW1 | SW2);
    GPIO_PORTF_PCTL_R  &= ~0x000FFFFFU;
    GPIO_PORTF_AFSEL_R &= ~(LED_MASK | SW1 | SW2);

    GPIO_PORTF_DIR_R   |=  LED_MASK;
    GPIO_PORTF_DIR_R   &= ~(SW1 | SW2);
    GPIO_PORTF_PUR_R   |=  (SW1 | SW2);
    GPIO_PORTF_DEN_R   |=  (LED_MASK | SW1 | SW2);
    GPIO_PORTF_DATA_R  &= ~LED_MASK;
}

static void Hardware_Init_PortE(void)
{
    SYSCTL_RCGCGPIO_R |= RCGCGPIO_E;
    while ((SYSCTL_PRGPIO_R & RCGCGPIO_E) == 0) {}

    GPIO_PORTE_AMSEL_R &= ~PORTE_BTN_MASK;
    GPIO_PORTE_PCTL_R  &= ~0x0000FFFFU;
    GPIO_PORTE_AFSEL_R &= ~PORTE_BTN_MASK;
    GPIO_PORTE_DIR_R   &= ~PORTE_BTN_MASK;
    GPIO_PORTE_PDR_R   |=  PORTE_BTN_MASK;
    GPIO_PORTE_DEN_R   |=  PORTE_BTN_MASK;
}

static void Hardware_Init_PortD(void)
{
    SYSCTL_RCGCGPIO_R |= RCGCGPIO_D;
    while ((SYSCTL_PRGPIO_R & RCGCGPIO_D) == 0) {}

    GPIO_PORTD_AMSEL_R &= ~PORTD_BTN_MASK;
    GPIO_PORTD_PCTL_R  &= ~0x00000FFFU;
    GPIO_PORTD_AFSEL_R &= ~PORTD_BTN_MASK;
    GPIO_PORTD_DIR_R   &= ~PORTD_BTN_MASK;
    GPIO_PORTD_PDR_R   |=  PORTD_BTN_MASK;
    GPIO_PORTD_DEN_R   |=  PORTD_BTN_MASK;
}

void Hardware_Init(void)
{
    Hardware_Init_PortF();
    Hardware_Init_PortE();
    Hardware_Init_PortD();
}
