/*
 * nvic.h --
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Maxim Integrated Products
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MAXIM INTEGRATED PRODUCTS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MAXIM INTEGRATED PRODUCTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* [INTERNAL] ------------------------------------------------------------------
 * Created on: June 6, 2013
 * Author: Jeremy B. <jeremy.brodt@maximintegrated.com>
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision: 4406 $:  Revision of last commit
 * $Author: jeremy.brodt $:  Author of last commit
 * $Date: 2012-07-06 13:09:03 -0500 (Fri, 06 Jul 2012) $:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */

#ifndef _NVIC_H_
#define _NVIC_H_

/** Global includes */
#include <config.h>
/** Other includes */
/** Local includes */


void handler_default_inital_(void);
void handler_default_reset_(void);
void handler_default_nmi_(void);
void handler_default_hard_fault_(void);
void handler_default_memory_manage_(void);
void handler_default_bus_fault_(void);
void handler_default_usage_fault_(void);
void handler_default_svcall_(void);
void handler_default_debug_monitor_(void);
void handler_default_pendsv_(void);
void handler_default_systick_(void);
void handler_default_irq_(void);
void handler_default_undefined_(void);

#endif /* _NVIC_H_ */

/******************************************************************************/
/* EOF */
