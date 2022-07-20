#include "types.h"
#include "aux.h"
#include "irq.h"

// Interrupt Controller
void intr_contoller_init(void)
{
    IRQCLR_REGS->irq_disable_basic = INTR_MASK_ALL;
    IRQCLR_REGS->irq_disable_reg1 = INTR_MASK_ALL;
    IRQCLR_REGS->irq_disable_reg2 = INTR_MASK_ALL;

    IRQCLR_REGS->irq_enable_reg2 = 1 << 25;
}

u32 query_irq_number(void)
{
    //Check pending bit of pending register set
    return IRQCLR_REGS->irq_pending;
}