#ifndef LL_DBG_PIN_H
#define LL_DBG_PIN_H

#ifdef LL_DBG_PIN_ENABLED

#define LL_DBG_PORT             MXC_GPIO1

#define LL_DBG_PIN_0            (1 << 6)
#define LL_DBG_PIN_1            (1 << 7)
#define LL_DBG_PIN_2            (1 << 10)
#define LL_DBG_PIN_3            (1 << 12)
#define LL_DBG_PIN_4            (1 << 13)
#define LL_DBG_PIN_5            (1 << 14)
#define LL_DBG_PIN_6            (1 << 15)

#endif /* LL_DBG_PIN_ENABLED */

#endif /* LL_DBG_PIN_H */
