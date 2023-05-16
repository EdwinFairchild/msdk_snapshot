/*
 * dcc.h --
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Maxim Integrated
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
 * THIS SOFTWARE IS PROVIDED BY MAXIM INTEGRATED ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* [INTERNAL] ------------------------------------------------------------------
 * Created on: Jul 15, 2010
 * Author:
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision:: 220      $:  Revision of last commit
 * $Author:: gregory.ro#$:  Author of last commit
 * $Date:: 2010-07-15 2#$:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */
#ifndef DCC_H_
#define DCC_H_

#define DCC_OUTPUT_BUSY 2
#define DCC_INPUT_READY 1

static inline unsigned int read_dcc(void) {
	unsigned int c;
	__asm__ volatile(
			"mrc p14,0, %0, c1, c0\n"
			: "=r" (c));
	return c;
}

static inline void write_dcc(unsigned int c) {
	__asm__ volatile(
			"mcr p14,0, %0, c1, c0\n"
			:
			: "r" (c));
}
static inline unsigned int poll_dcc(void) {
	unsigned int ret;
	__asm__ volatile(
			"mrc p14,0, %0, c0, c0\n"
			: "=r" (ret));
	return ret;
}

void write_dcc_char(unsigned int c);

unsigned int read_dcc_char(void);

void write_dcc_string(const char* s);

#endif /* DCC_H_ */
