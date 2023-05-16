/****************************************************************************
 * Copyright (C) 2013 Maxim Integrated, All Rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to 
 * any party or parties not specified in the license agreement and/or 
 * nondisclosure agreement is expressly prohibited. 
 *
 *     Description: ART3 SPI Test Configuration
 *             $Id: test_config.h 17951 2015-05-19 16:05:45Z kevin.gillespie $
 *
 ********************************************************************************
 */

#ifndef _TEST_CONFIG_H_
#define _TEST_CONFIG_H_

#ifndef DUT_SPI
#define DUT_SPI		0
#endif

#define DUT_SLAVE     0
#define DUT_MASTER    1

#define UNIT_BIT 	  1
#define UNIT_BYTE 	  8
#define UNIT_PAGE	  16

//#define SYNC_BETWEEN_MESSAGES

typedef struct {
    unsigned int master;
    unsigned int port;
	unsigned int map;
	unsigned int freq;
	unsigned int mode;
	unsigned int datalen;
	unsigned int unit;
	unsigned int nummsgs;
} spi_test_cfg_t;

extern const spi_test_cfg_t spi_test_cfg[];
extern const unsigned int spi_test_cfg_size;

#endif /* _TEST_CONFIG_H_ */

