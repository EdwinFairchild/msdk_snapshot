#include "common.h"
#include "gpio_regs.h"
#include "ll_debug.h"
#include "ll_dbg_pin.h"

#ifdef LL_DBG_PIN_ENABLED

void ll_dbg_pin_init(void)
{
    LL_DBG_PORT->out_set   =  LL_DBG_PIN_ALL;
    LL_DBG_PORT->out_en   |=  LL_DBG_PIN_ALL;
    LL_DBG_PORT->pad_cfg1 &= ~LL_DBG_PIN_ALL;
    LL_DBG_PORT->pad_cfg2 &= ~LL_DBG_PIN_ALL;
    LL_DBG_PORT->en       |=  LL_DBG_PIN_ALL;
}

void ll_dbg_pin_assert(uint32_t mask)
{
    LL_DBG_PORT->out_clr = mask;
}

void ll_dbg_pin_deassert(uint32_t mask)
{
    LL_DBG_PORT->out_set = mask;
}

#endif /* LL_DBG_PIN_ENABLED */
