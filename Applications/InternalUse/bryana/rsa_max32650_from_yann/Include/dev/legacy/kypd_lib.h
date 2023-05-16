/*
 * Copyright 2006, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */

#ifndef _KYPD_LIB_H_
#define _KYPD_LIB_H_

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	PROTOTYPES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

void kypd_Pass ( void );
void kypd_Fail ( void );
void kypd_Start ( void );
void kypd_Step ( int stepno );
void kypd_PrntNum ( int intval );
void kypd_PrntBuf ( UINT8 *buf_p, UINT32 buflen );
void kypd_PrntStr ( char *buf_p );
void kypd_InitHW ( void );
void kypd_InitKypd (
	UINT16	debounce
	);

#endif
