/****************************************************************************
 * Copyright (C) 2012 Maxim Integrated Products, All Rights Reserved.
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
 *             $Id: test_config.c 18243 2015-06-29 13:52:34Z kevin.gillespie $
 *
 ********************************************************************************
 */

#include "test_config.h"

 // Can only send bytes in slave mode
const spi_test_cfg_t spi_test_cfg[] = {
  /*   Master, Port, Map,    Freq,      Mode, Msg Len, Unit size, # of Msgs */
     {    0,         2,   1, 4000000,      0,      1, UNIT_BYTE,         8 },
     // {    0,         2,   1, 1000000, SPI_MODE_3,      1, UNIT_BYTE,         8 },
};

const unsigned int spi_test_cfg_size = sizeof(spi_test_cfg) / sizeof(spi_test_cfg_t);

