/*
 * ttb.h --
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
 * Created on: Jun 22, 2010
 * Author:
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision:: 222      $:  Revision of last commit
 * $Author:: gregory.ro#$:  Author of last commit
 * $Date:: 2010-07-15 2#$:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */
#ifndef TTB_H_
#define TTB_H_

/** @defgroup ARM_TTB Translations Tables Management.
 *
 * @li TTB: First level Translation Table
 *
 * @{
 *
 */

/* -------------------------------------------------------------------------- */

/** No Access Permissions. */
#define PAGE_AP_NONE		0x0
/** Client Access Permissions. */
#define PAGE_AP_CLIENT		0x1
/** Manager Access Permissions. */
#define PAGE_AP_MANAGER		0x3

/* -------------------------------------------------------------------------- */

/** TTB Entry: Fault.
 * Section accesses generate fault.
 */
#define TTB_ENTRY_FAULT		0x0
/** TTB Entry: Coarse pages.
 * Section is mapped using a coarse pages table.
 */
#define TTB_ENTRY_COARSE	0x1
/** TTB Entry: Section.
 * The entry define section page (1MB)
 */
#define TTB_ENTRY_SECTION	0x2
#define TTB_ENTRY_1MBPAGE	(TTB_ENTRY_SECTION)
/** TTB Entry: Fine.
 * Section is mapped using a fine pages table.
 */
#define TTB_ENTRY_FINE		0x3

/* -------------------------------------------------------------------------- */

/** Set Section Domain. */
#define SECTION_SETDOMAIN(id)	((id)<<5)
/** Set Section Cache Configuration. */
#define SECTION_SETCACHE(id)	((id)<<2)
/** Set Section RFU */
#define SECTION_SETRFU() 		0x010
/** Set Section Access Permissions. */
#define SECTION_SETAP(id)		((id)<<10)
/** Set Section Id to section. */
#define SECTION_SETSECID()		TTB_ENTRY_SECTION
/** Set Section Id to Coarse. */
#define SECTION_SETCRSID()		TTB_ENTRY_COARSE

/* -------------------------------------------------------------------------- */

/** Coarse pages table Entry: Fault.
 * Coarse page accesses generate fault.
 */
#define COARSE_ENTRY_FAULT		0x0
/** Coarse pages table Entry: 64KB page.
 * The entry define a 64KB page.
 */
#define COARSE_ENTRY_64KBPAGE	0x1
/** Coarse pages table Entry: 4KB page.
 * The entry define a 64KB page.
 */
#define COARSE_ENTRY_4KBPAGE	0x2
/** Coarse pages table Entry: 1KB page.
 * The entry define a 64KB page.
 */
#define COARSE_ENTRY_1KBPAGE	0x2

/** Set 4KB Access Permissions: 1st 1KB. */
#define COARSE_4KBPAGE_SETAP0(id)	((id)<<4)
/** Set 4KB Access Permissions: 1st 1KB. */
#define COARSE_4KBPAGE_SETAP1(id)	((id)<<6)
/** Set 4KB Access Permissions: 1st 1KB. */
#define COARSE_4KBPAGE_SETAP2(id)	((id)<<8)
/** Set 4KB Access Permissions: 1st 1KB. */
#define COARSE_4KBPAGE_SETAP3(id)	((id)<<10)
/** Set 4KB Id. */
#define COARSE_4KBPAGE_SETID() (COARSE_ENTRY_4KBPAGE)
/** Set 4KB Cache Configuration. */
#define COARSE_4KBPAGE_SETCACHE(id)	((id)<<2)

/* -------------------------------------------------------------------------- */

#define SECTION_VA_MASK		0xFFF00000
#define SECTION_AP_MASK		0x00000C00
#define SECTION_DOM_MASK	0x000001E0
#define SECTION_CB_MASK		0x0000000C
#define SECTION_ID_MASK		0x00000003

#define COARSE_4KBPAGE_VA_MASK	0xFFFFF000
#define COARSE_4KBPAGE_AP3_MASK	0x00000C00
#define COARSE_4KBPAGE_AP2_MASK	0x00000300
#define COARSE_4KBPAGE_AP1_MASK	0x000000C0
#define COARSE_4KBPAGE_AP0_MASK	0x00000030
#define COARSE_4KBPAGE_CB_MASK	0x0000000C
#define COARSE_4KBPAGE_ID_MASK	0x00000003
#define COARSE_4KBPAGE_ID		COARSE_ENTRY_4KBPAGE

/* -------------------------------------------------------------------------- */

enum cache_config {
	CACHE_DISABLED_NOTBUFFERABLE = 0x0,	/**< Cache disabled, not Bufferable. *///!< CACHE_DISABLED_NOTBUFFERABLE
	CACHE_DISABLED_BUFFERABLE = 0x1, 	/**< Cache disabled, Bufferable */       //!< CACHE_DISABLED_BUFFERABLE
	CACHE_ENABLED_WRITETHROUGH = 0x2, 	/**< Cache enabled, Write Through */    //!< CACHE_ENABLED_WRITETHROUGH
	CACHE_ENABLED_WRITEBACK = 0x3		/**< Cache enabled, Write Back */           //!< CACHE_ENABLED_WRITEBACK
};

/* -------------------------------------------------------------------------- */

void ttb_init(enum cache_config config);

/** Enable Cache for section */
void ttb_section_setcache(unsigned addr, enum cache_config config);

/** @} */

#endif /* TTB_H_ */
