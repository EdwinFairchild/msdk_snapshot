
/**
 * @file hexdump.c
 * @brief Contains code for printing debugging information.
 */

/* ****************************************************************************
 * Copyright (C) 2015-2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date$
 * $Revision$
 *
 *************************************************************************** */

#define _HEXDUMP_C_

#include <assert.h>

#include <stdio.h>

#include "hexdump.h"

#define LINE_ENDING "\n"

/**
 * @brief Tabs a specified number of spaces into a line. */
static void displayIndent(unsigned int indentCount)
{
    unsigned int iter;

    for (iter = 0; iter < indentCount; iter++)
        printf("  ");
}

/**
 * @brief Displays a hex dump of the provided memory block.
 * @param pMemBlock A pointer to the memory block to dump.
 * @param memBlockSize The size of the memory block to dump.
 * @param indent The number of indents for each line. */
static void displayHexDump(
        const char * pMemBlock,
        size_t memBlockSize,
        unsigned int indent)
{
    unsigned int byteIter;
    unsigned int wordIter;
    unsigned int lineIter;
    unsigned int memIter;
    unsigned char nibble[2];
    unsigned int nibbleIter;

    byteIter = 0;
    wordIter = 0;
    lineIter = 0;
    for (memIter = 0; memIter < memBlockSize; memIter ++)
    {
        // If this is the begining of a line,
        if (byteIter == 0 && wordIter == 0)
        {
            // Print an indent.
            displayIndent(indent);

            // Print the memory offset.
            printf("%08X: ", (lineIter << 4));
        }
        // If the begining of a word block, add a large space.
        else if (byteIter == 0)
        {
            printf("  ");
        }
        // Otherwise, add a space.
        else
        {
            printf(" ");
        }

        // Convert current character into a hex representation.
        nibble[0] = (pMemBlock[memIter] >> 4) & 0x0F;
        nibble[1] = (pMemBlock[memIter] & 0xF);
        for (nibbleIter = 0; nibbleIter < 2; nibbleIter ++)
        {
            if (nibble[nibbleIter] < 0x0AU)
                nibble[nibbleIter] += '0';
            else
                nibble[nibbleIter] += ('A' - 0x0AU);
        }

        // Print current nibble.
        printf("%c%c", nibble[0], nibble[1]);

        // Increment counters.
        byteIter ++;
        if (byteIter == 4)
        {
            byteIter = 0;
            wordIter ++;
        }
        if (wordIter == 4)
        {
            wordIter = 0;
            lineIter ++;

            // Also need a line break.
            printf(LINE_ENDING);
        }
    }

    // Need a line break after an incomplete terminating line.
    if (byteIter != 0 || wordIter != 0)
        printf(LINE_ENDING);
}

/**
 * @brief Displays a hex dump of the provided memory block using the default indenting.
 * @param pMemBlock A pointer to the memory block to dump.
 * @param memBlockSize The size of the memory block to dump. */
void hexdump(
        const char * pMemBlock,
        size_t memBlockSize)
{
    displayHexDump(pMemBlock, memBlockSize, 1);
}

// vim:ts=4:sw=4:ft=c:expandtab
