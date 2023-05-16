/*
 * mq55.h --
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Maxim Integrated
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
 * Created on: June 1, 2012
 * Author: Jeremy B. <jeremy.brodt maximintegrated.com>
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision: 8826 $:  Revision of last commit
 * $Author: jeremy.brodt $:  Author of last commit
 * $Date: 2013-06-19 11:12:06 -0500 (Wed, 19 Jun 2013) $:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */

#ifndef _BITBAND_H_
#define _BITBAND_H_
/** Global includes */
#include <config.h>
/** Other includes */
/** Local includes */

// bit_word_offset = (byte_offset x 32) + (bit_number × 4)
// bit_word_addr = bit_band_base + bit_word_offset
#define SET_IO_BIT(register_address, bit_number)   *((volatile unsigned int *)((((unsigned int)(register_address) - PFM(IOBASE)) << 5) + ((bit_number) << 2) + PFM(IO_BITBAND_BASE))) = 1
#define CLR_IO_BIT(register_address, bit_number)   *((volatile unsigned int *)((((unsigned int)(register_address) - PFM(IOBASE)) << 5) + ((bit_number) << 2) + PFM(IO_BITBAND_BASE))) = 0
#define GET_IO_BIT(register_address, bit_number)   *((volatile unsigned int *)((((unsigned int)(register_address) - PFM(IOBASE)) << 5) + ((bit_number) << 2) + PFM(IO_BITBAND_BASE)))

#endif /* _BITBAND_H_ */

/******************************************************************************/
/* EOF */
