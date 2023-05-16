/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * Description: Communications Device Class ACM (Serial Port) over USB
 * $Id: descriptors.h 26835 2017-03-02 16:54:52Z zach.metzinger $
 *
 *******************************************************************************
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <stdint.h>
#include "usb.h"
#include "audio.h"

usb_device_descriptor_t __attribute__((aligned(4))) device_descriptor = {
    0x12,         /* bLength = 18                                        */
    0x01,         /* bDescriptorType = Device                            */
    0x0110,       /* bcdUSB USB spec rev (BCD)                           */
    0x00,         /* bDeviceClass = defined in interface descriptor (0)  */
    0x00,         /* bDeviceSubClass                                     */
    0x00,         /* bDeviceProtocol                                     */
    0x40,         /* bMaxPacketSize0 is 64 bytes                         */
    0x0B6A,       /* idVendor (Maxim Integrated)                         */
    0x003D,       /* idProduct                                           */
    0x0100,       /* bcdDevice                                           */
    0x01,         /* iManufacturer Descriptor ID                         */
    0x02,         /* iProduct Descriptor ID                              */
    0x00,         /* iSerialNumber = (0) No string                       */
    0x01          /* bNumConfigurations                                  */
};

__attribute__((aligned(4)))
struct __attribute__((packed))
{
    usb_configuration_descriptor_t  config_descriptor;
    usb_interface_descriptor_t      audio_interface_descriptor;
    uint8_t                         header_functional_descriptor[10];
    uint8_t                         input_mic_terminal_descriptor[12];
    uint8_t                         output_usb_terminal_descriptor[9];
    uint8_t                         input_usb_terminal_descriptor[12];
    uint8_t                         output_speak_terminal_descriptor[9];
    uint8_t                         output_feature_unit_descriptor[8];
    usb_interface_descriptor_t      streaming_a_0_interface_descriptor;
    usb_interface_descriptor_t      streaming_a_1_interface_descriptor;
    uint8_t                         audio_general_a_interface_descriptor[7];
    uint8_t                         audio_type_I_a_descriptor[11];
    uint8_t                         endpoint_descriptor_1[9];
    uint8_t                         audio_endpoint_a_descriptor[7];
    usb_interface_descriptor_t      streaming_b_0_interface_descriptor;
    usb_interface_descriptor_t      streaming_b_1_interface_descriptor;
    uint8_t                         audio_general_b_interface_descriptor[7];
    uint8_t                         audio_type_I_b_descriptor[11];
    uint8_t                         endpoint_descriptor_2[9];
    uint8_t                         audio_endpoint_b_descriptor[7];
}
config_descriptor = {
    {   /*General Configuration Descriptor*/
        0x09,       /*  bLength = 9                         */
        0x02,       /*  bDescriptorType = Config (2)        */
        0x00B6,     /*  wTotalLength(L/H)                   */
        0x03,       /*  bNumInterfaces                      */
        0x01,       /*  bConfigValue                        */
        0x00,       /*  iConfiguration                      */
        0xC0,       /*  bmAttributes (self-powered)         */
        0x05        /*  MaxPower is 2ma (units are 2ma/bit) */
    },
    {   /*  Audio Control Interface Descriptor */
        0x09,       /*  bLength = 9                     */
        0x04,       /*  bDescriptorType = Interface (4) */
        0x00,       /*  bInterfaceNumber                */
        0x00,       /*  bAlternateSetting               */
        0x00,       /*  bNumEndpoints                   */
        0x01,       /*  bInterfaceClass = Audio Interface Class (1)         */
        0x01,       /*  bInterfaceSubClass = Audio Interface Class (1)      */
        0x00,       /*  bInterfaceProtocol =  no class specific protocol (0)*/
        0x00        /*  iInterface                      */
    },
    {   /*  Class Specific Audio Control Descriptor */
        0x0A,         /*  bLength = 9 (lenth of this header)    */
        0x24,         /*  bDescriptorType                       */
        0x01,         /*  bDescriptorSubtype                    */
        0x00, 0x01,   /*  bcdADC                                */
        0x45, 0x00,   /*  wTotalLength (of all AC headers)      */
        0x02,         /*  bInCollection (# of Audio/MIDI Streaming Interfaces) */
        0x01,         /*  baInterfaceNr(1)                      */
        0x02          /*  baInterfaceNr(2)                      */
    },
    {   /*  First Unit Terminal Descriptor (Input - Microphone)   */
        0x0C,         /*  bLength = 12                          */
        0x24,         /*  bDescriptorType                       */
        0x02,         /*  bDescriptorSubtype (INPUT TERMINAL)   */
        0x01,         /*  bTerminalID                           */
        0x01, 0x02,   /*  wTerminalType (0x0201 - Microphone)   */
        0x00,         /*  bAssocTerminal (Associated Input)     */
        0x01,         /*  bNrChannels                           */
        0x00, 0x00,   /*  wChannelConfig                        */
        0x00,         /*  iChannelNames                         */
        0x03          /*  iTerminal                             */
    },
    {   /*  Second Unit Terminal Descriptor (Output - USB Iso In) */
        0x09,         /*  bLength = 9                           */
        0x24,         /*  bDescriptorType                       */
        0x03,         /*  bDescriptorSubtype (OUTPUT TERMINAL)  */
        0x02,         /*  bTerminalID                           */
        0x01, 0x01,   /*  wTerminalType (0x0101 - USB Streaming)*/
        0x00,         /*  bAssocTerminal (Associated Input)     */
        0x01,         /*  bSourceID (Input Terminal)            */
        0x03          /*  iTerminal                             */
    },
    {   /*  Third Unit Terminal Descriptor (Input - USB Iso Out)  */
        0x0C,         /*  bLength = 12                          */
        0x24,         /*  bDescriptorType                       */
        0x02,         /*  bDescriptorSubtype (INPUT TERMINAL)   */
        0x03,         /*  bTerminalID                           */
        0x01, 0x01,   /*  wTerminalType (0x0101 - USB Streaming)*/
        0x00,         /*  bAssocTerminal (Associated Input)     */
        0x01,         /*  bNrChannels                           */
        0x00, 0x00,   /*  wChannelConfig                        */
        0x00,         /*  iChannelNames                         */
        0x04          /*  iTerminal                             */
    },
    {   /*  Fourth Unit Terminal Descriptor (Output - Speaker)    */
        0x09,         /*  bLength = 9                           */
        0x24,         /*  bDescriptorType                       */
        0x03,         /*  bDescriptorSubtype (OUTPUT TERMINAL)  */
        0x04,         /*  bTerminalID                           */
        0x01, 0x03,   /*  wTerminalType (0x0301 - Speaker)      */
        0x00,         /*  bAssocTerminal (Associated Input)     */
        0x05,         /*  bSourceID (Input Terminal)            */
        0x04          /*  iTerminal                             */
    },
    {   /* Fifth Unit Terminal Descriptor (Feature Unit - Speaker Mute/Volume) */
        0x08,         /*  bLength                               */
        0x24,         /*  bDescriptorType                       */
        0x06,         /*  bDescriptorSubtype                    */
        0x05,         /*  bUnitId                               */
        0x03,         /*  bSourceID                             */
        0x01,         /*  bControlSize                          */
        0x03,         /*  bmaControls                           */
        0x04          /*  iFeature                              */
    },
    {   /*  Audio Streaming Generic Interface Descriptor - Alt 0  */
        0x09,         /*  bLength                               */
        0x04,         /*  bDescriptorType (Interface)           */
        0x01,         /*  bInterfaceNumber                      */
        0x00,         /*  bAlternateSetting                     */
        0x00,         /*  bNumEndpoints                         */
        0x01,         /*  bInterfaceClass = Audio Interface (1) */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)              */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0)   */
        0x00,         /*  iInterface = No Text String (0)       */
    },
    {   /*  Audio Streaming Generic Interface Descriptor - Alt 1  */
        0x09,         /*  bLength                               */
        0x04,         /*  bDescriptorType (Interface)           */
        0x01,         /*  bInterfaceNumber                      */
        0x01,         /*  bAlternateSetting                     */
        0x01,         /*  bNumEndpoints                         */
        0x01,         /*  bInterfaceClass = Audio Interface (1) */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)              */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0)   */
        0x00,         /*  biInterface = No Text String (0)      */
    },
    {   /*  Audio Streaming Class Specific Interface Descriptor   */
        0x07,         /*  bLength                               */
        0x24,         /*  bDescriptorType (ClassSpecific)       */
        0x01,         /*  bDescriptorSubtype                    */
        0x02,         /*  bTerminalLink                         */
        0x01,         /*  bDelay                                */
        0x01, 0x00,   /*  wFormatTag (PCM Format)               */
    },
    {   /*  Audio Streaming Type I Format Descriptor */
        0x0B,         /*  bLength                          */
        0x24,         /*  bDescriptorType (ClassSpecific)  */
        0x02,         /*  bDescriptorSubtype               */
        0x01,         /*  bFormatType                      */
        0x01,         /*  bNrChannels                      */
        0x02,         /*  bSubFrameSize                    */
        0x10,         /*  bBitResolution (16 Bit)          */
        0x01,         /*  bSamFreqType (1 Samp. Freq)      */
        0x80, 0xBB, 0x00   /*  tSamFreq (48000 Hz)         */
    },
    {   /*  Generic Endpoint Descriptor */
        0x09,         /*  bLength                          */
        0x05,         /*  bDescriptorType (Endpoint)       */
        0x8A,         /*  bEndpointAddress (EP10-IN)        */
        0x05,         /*  bmAttributes (Isochronous, Asynchronus Synchronization) */
        0x60, 0x00,   /*  wMaxPacketSize                   */
        0x01,         /*  bInterval (N/A)                  */
        0x00,         /*  bRefresh                         */
        0x00          /*  bSynchAddress (no synch endpoint)*/
    },
    {   /*  Audio Streaming Class Specific Endpoint Descriptor    */
        0x07,         /*  bLength                               */
        0x25,         /*  bDescriptorType (CS_ENDPOINT)         */
        0x01,         /*  bDescriptorSubtype (General)          */
        0x80,         /*  bmAttributes (No Sam. Freq Control, no pitch control, no padding) */
        0x02,         /*  bLockDelayUnits                       */
        0x00, 0x00    /*  wLockDelay                        */
    },
    {   /*  Audio Streaming Generic Interface 2 Descriptor - Alt 0 */
        0x09,         /*  bLength                          */
        0x04,         /*  bDescriptorType (Interface)      */
        0x02,         /*  bInterfaceNumber                 */
        0x00,         /*  bAlternateSetting                */
        0x00,         /*  bNumEndpoints                    */
        0x01,         /*  bInterfaceClass = Audio Interface (1)                 */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)              */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0)   */
        0x00,         /*  iInterface = No Text String (0)  */
    },
    {   /*  Audio Streaming Generic Interface 2 Descriptor - Alt 1 */
        0x09,         /*  bLength                          */
        0x04,         /*  bDescriptorType (Interface)      */
        0x02,         /*  bInterfaceNumber                 */
        0x01,         /*  bAlternateSetting                */
        0x01,         /*  bNumEndpoints                    */
        0x01,         /*  bInterfaceClass = Audio Interface (1)                 */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)              */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0)   */
        0x00,         /*  biInterface = No Text String (0) */
    },
    {   /*  Audio Streaming Class Specific Interface Descriptor   */
        0x07,         /*  bLength                               */
        0x24,         /*  bDescriptorType (ClassSpecific)       */
        0x01,         /*  bDescriptorSubtype                    */
        0x03,         /*  bTerminalLink                         */
        0x01,         /*  bDelay                                */
        0x01, 0x00,   /*  wFormatTag (PCM Format)               */
    },
    {   /*  Audio Streaming Type I Format Descriptor */
        0x0B,         /*  bLength                          */
        0x24,         /*  bDescriptorType (ClassSpecific)  */
        0x02,         /*  bDescriptorSubtype               */
        0x01,         /*  bFormatType                      */
        0x01,         /*  bNrChannels                      */
        0x02,         /*  bSubFrameSize                    */
        0x10,         /*  bBitResolution (16 Bit)          */
        0x01,         /*  bSamFreqType (1 Samp. Freq)      */
        0x80, 0xBB, 0x00   /*  tSamFreq (48000 Hz)         */
    },
    {   /*  Generic Endpoint Descriptor */
        0x09,         /*  bLength                           */
        0x05,         /*  bDescriptorType (Endpoint)        */
        0x0B,         /*  bEndpointAddress (EP11-OUT)        */
        0x05,         /*  bmAttributes (Isochronous, Asynchronus Synchronization) */
        0x60, 0x00,   /*  wMaxPacketSize                    */
        0x01,         /*  bInterval (N/A)                   */
        0x00,         /*  bRefresh                          */
        0x00          /*  bSynchAddress (no synch endpoint) */
    },
    {   /*  Audio Streaming Class Specific Endpoint Descriptor    */
        0x07,         /*  bLength                               */
        0x25,         /*  bDescriptorType (CS_ENDPOINT)         */
        0x01,         /*  bDescriptorSubtype (General)          */
        0x80,         /*  bmAttributes (No Sam. Freq Control, no pitch control, no padding) */
        0x02,         /*  bLockDelayUnits                       */
        0x00, 0x00    /*  wLockDelay                            */
    }
};

__attribute__((aligned(4)))
uint8_t lang_id_desc[] = {
    0x04,         /* bLength */
    0x03,         /* bDescriptorType */
    0x09, 0x04    /* bString = wLANGID (see usb_20.pdf 9.6.7 String) */
};

__attribute__((aligned(4)))
uint8_t mfg_id_desc[] = {
    0x22,         /* bLength */
    0x03,         /* bDescriptorType */
    'M', 0,
    'a', 0,
    'x', 0,
    'i', 0,
    'm', 0,
    ' ', 0,
    'I', 0,
    'n', 0,
    't', 0,
    'e', 0,
    'g', 0,
    'r', 0,
    'a', 0,
    't', 0,
    'e', 0,
    'd', 0,
};

__attribute__((aligned(4)))
uint8_t prod_id_desc[] = {
    0x26,         /* bLength */
    0x03,         /* bDescriptorType */
    'M', 0,
    'A', 0,
    'X', 0,
    '3', 0,
    '2', 0,
    '6', 0,
    '5', 0,
    '0', 0,
    ' ', 0,
    'I', 0,
    'S', 0,
    'O', 0,
    '-', 0,
    'A', 0,
    'U', 0,
    'D', 0,
    'I', 0,
    'O', 0
};

__attribute__((aligned(4)))
uint8_t terminal_in_id_desc[] = {
    0x21,         /* bLength */
    0x03,         /* bDescriptorType */
    'H', 0,
    'E', 0,
    'A', 0,
    'R', 0,
    ' ', 0,
    'b', 0,
    'y', 0,
    ' ', 0,
    'I', 0,
    'N', 0,
    'T', 0,
    'E', 0,
    'R', 0,
    'N', 0,
    'S', 0
};

__attribute__((aligned(4)))
uint8_t terminal_out_id_desc[] = {
    0x22,         /* bLength */
    0x03,         /* bDescriptorType */
    'S', 0,
    'P', 0,
    'E', 0,
    'A', 0,
    'K', 0,
    ' ', 0,
    'b', 0,
    'y', 0,
    ' ', 0,
    'I', 0,
    'N', 0,
    'T', 0,
    'E', 0,
    'R', 0,
    'N', 0,
    'S', 0
};
/* Not currently used (see device descriptor), but could be enabled if desired */
__attribute__((aligned(4)))
uint8_t serial_id_desc[] = {
    0x14,         /* bLength */
    0x03,         /* bDescriptorType */
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '0', 0,
    '1', 0
};

/* Device qualifier needed for high-speed operation */
usb_device_qualifier_descriptor_t __attribute__((aligned(4))) device_qualifier_descriptor = {
    0x0A,         /* bLength = 10                       */
    0x06,         /* bDescriptorType = Device Qualifier */
    0x0200,       /* bcdUSB USB spec rev (BCD)          */
    0x00,         /* bDeviceClass define in interface   */
    0x00,         /* bDeviceSubClass                    */
    0x00,         /* bDeviceProtocol                    */
    0x40,         /* bMaxPacketSize0 is 64 bytes        */
    0x01,         /* bNumConfigurations                 */
    0x00          /* Reserved, must be 0                */
};

/* Other speed configuration needed for high-speed operation */
__attribute__((aligned(4)))
struct __attribute__((packed))
{
    usb_configuration_descriptor_t  config_descriptor;
    usb_interface_descriptor_t      audio_interface_descriptor;
    uint8_t                         header_functional_descriptor[10];
    uint8_t                         input_mic_terminal_descriptor[12];
    uint8_t                         output_usb_terminal_descriptor[9];
    uint8_t                         input_usb_terminal_descriptor[12];
    uint8_t                         output_speak_terminal_descriptor[9];
    uint8_t                         output_feature_unit_descriptor[9];
    usb_interface_descriptor_t      streaming_a_0_interface_descriptor;
    usb_interface_descriptor_t      streaming_a_1_interface_descriptor;
    uint8_t                         audio_general_a_interface_descriptor[7];
    uint8_t                         audio_type_I_a_descriptor[11];
    uint8_t                         endpoint_descriptor_1[9];
    uint8_t                         audio_endpoint_a_descriptor[7];
    usb_interface_descriptor_t      streaming_b_0_interface_descriptor;
    usb_interface_descriptor_t      streaming_b_1_interface_descriptor;
    uint8_t                         audio_general_b_interface_descriptor[7];
    uint8_t                         audio_type_I_b_descriptor[11];
    uint8_t                         endpoint_descriptor_2[9];
    uint8_t                         audio_endpoint_b_descriptor[7];
}
other_speed_config_descriptor = {
    /*General Configuration Descriptor*/
    {
        0x09,       /*  bLength = 9                     */
        0x02,       /*  bDescriptorType = Config (2)    */
        0x00B7,     /*  wTotalLength(L/H)               */
        0x03,       /*  bNumInterfaces                  */
        0x01,       /*  bConfigValue                    */
        0x00,       /*  iConfiguration                  */
        0xC0,       /*  bmAttributes (self-powered)     */
        0x05        /*  MaxPower is 2ma (units are 2ma/bit) */
    },
    {   /*  Audio Control Interface Descriptor */
        0x09,       /*  bLength = 9                     */
        0x04,       /*  bDescriptorType = Interface (4) */
        0x00,       /*  bInterfaceNumber                */
        0x00,       /*  bAlternateSetting               */
        0x00,       /*  bNumEndpoints                   */
        0x01,       /*  bInterfaceClass = Audio Interface Class (1) */
        0x01,       /*  bInterfaceSubClass = Audio Interface Class (1) */
        0x00,       /*  bInterfaceProtocol =  no class specific protocol (0) */
        0x00        /*  iInterface                      */
    },
    {   /*  Class Specific Audio Control Descriptor */
        0x0A,         /*  bLength = 9 (length of this header)  */
        0x24,         /*  bDescriptorType                 */
        0x01,         /*  bDescriptorSubtype              */
        0x00, 0x01,       /*  bcdADC                          */
        0x3C, 0x00,       /*  wTotalLength (of all AC headers)*/
        0x02,         /*  bInCollection (# of Audio/MIDI Streaming Interfaces) */
        0x01,          /*  baInterfaceNr(1)                */
        0x02
    },
    {   /*  First Unit Terminal Descriptor (Input - Microphone) */
        0x0C,         /*  bLength = 12           */
        0x24,         /*  bDescriptorType                 */
        0x02,         /*  bDescriptorSubtype (INPUT TERMINAL) */
        0x01,         /*  bTerminalID                     */
        0x01, 0x02,       /*  wTerminalType (0x0201 - Microphone) */
        0x00,         /*  bAssocTerminal (Associated Input)*/
        0x01,         /*  bNrChannels                     */
        0x00, 0x00,       /*  wChannelConfig                  */
        0x00,         /*  iChannelNames                   */
        0x03          /*  iTerminal                       */
    },
    {   /*  Second Unit Terminal Descriptor (Output - USB Iso In) */
        0x09,         /*  bLength = 9           */
        0x24,         /*  bDescriptorType                 */
        0x03,         /*  bDescriptorSubtype (OUTPUT TERMINAL) */
        0x02,         /*  bTerminalID                     */
        0x01, 0x01,   /*  wTerminalType (0x0101 - USB Streaming) */
        0x00,         /*  bAssocTerminal (Associated Input)*/
        0x01,         /*  bSourceID (Input Terminal)      */
        0x03          /*  iTerminal                       */
    },
    {   /*  Third Unit Terminal Descriptor (Input - USB Iso Out) */
        0x0C,         /*  bLength = 12           */
        0x24,         /*  bDescriptorType                 */
        0x02,         /*  bDescriptorSubtype (INPUT TERMINAL) */
        0x03,         /*  bTerminalID                     */
        0x01, 0x01,       /*  wTerminalType (0x0101 - USB Streaming) */
        0x00,         /*  bAssocTerminal (Associated Input)*/
        0x01,         /*  bNrChannels                     */
        0x00, 0x00,       /*  wChannelConfig                  */
        0x00,         /*  iChannelNames                   */
        0x03          /*  iTerminal                       */
    },
    {   /*  Fourth Unit Terminal Descriptor (Output - Speaker) */
        0x09,         /*  bLength = 9           */
        0x24,         /*  bDescriptorType                 */
        0x03,         /*  bDescriptorSubtype (OUTPUT TERMINAL) */
        0x04,         /*  bTerminalID                     */
        0x01, 0x03,   /*  wTerminalType (0x0301 - Speaker) */
        0x00,         /*  bAssocTerminal (Associated Input)*/
        0x05,         /*  bSourceID (Input Terminal)      */
        0x03          /*  iTerminal                       */
    },
    {   /* Fifth Unit Terminal Descriptor (Feature Unit - Speaker Mute/Volume) */
        0x09,           /*  bLength = 9             */
        0x24,           /*  bDescriptorType         */
        0x06,           /*  bDescriptorSubtype      */
        0x05,           /*  bUnitID                 */
        0x03,           /*  bSourceID               */
        0x01,           /*  bControlSize            */
        0x03, 0x00,     /*  bmaControls(0)          */
        0x00            /*  iFeature                */
    },
    {   /*  Audio Streaming Generic Interface Descriptor - Alt 0 */
        0x09,         /*  bLength                          */
        0x04,         /*  bDescriptorType (Interface)      */
        0x01,         /*  bInterfaceNumber                 */
        0x00,         /*  bAlternateSetting                */
        0x00,         /*  bNumEndpoints                    */
        0x01,         /*  bInterfaceClass = Audio Interface (1) */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)    */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0) */
        0x00,         /*  iInterface = No Text String (0)  */
    },
    {   /*  Audio Streaming Generic Interface Descriptor - Alt 1 */
        0x09,         /*  bLength                          */
        0x04,         /*  bDescriptorType (Interface)      */
        0x01,         /*  bInterfaceNumber                 */
        0x01,         /*  bAlternateSetting                */
        0x01,         /*  bNumEndpoints                    */
        0x01,         /*  bInterfaceClass = Audio Interface (1) */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)    */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0) */
        0x00,         /*  biInterface = No Text String (0) */
    },
    {   /*  Audio Streaming Class Specific Interface Descriptor */
        0x07,         /*  bLength                          */
        0x24,         /*  bDescriptorType (ClassSpecific)  */
        0x01,         /*  bDescriptorSubtype               */
        0x02,         /*  bTerminalLink                    */
        0x01,         /*  bDelay                           */
        0x01, 0x00,       /*  wFormatTag (PCM Format)          */
    },
    {   /*  Audio Streaming Type I Format Descriptor */
        0x0B,         /*  bLength                          */
        0x24,         /*  bDescriptorType (ClassSpecific)  */
        0x02,         /*  bDescriptorSubtype               */
        0x01,         /*  bFormatType                      */
        0x01,         /*  bNrChannels                      */
        0x02,         /*  bSubFrameSize                    */
        0x10,         /*  bBitResolution (16 Bit)          */
        0x01,         /*  bSamFreqType (1 Samp. Freq)      */
        0x80, 0x3E, 0x00   /*  tSamFreq (48000 Hz)         */
    },
    {   /*  Generic Endpoint Descriptor */
        0x09,         /*  bLength                          */
        0x05,         /*  bDescriptorType (Endpoint)       */
        0x81,         /*  bEndpointAddress (EP1-IN)        */
        0x05,         /*  bmAttributes (Isochronous, Asynchronus Synchronization) */
        0x20, 0x00,   /*  wMaxPacketSize                   */
        0x01,         /*  bInterval (N/A)                  */
        0x00,         /*  bRefresh                         */
        0x00          /*  bSynchAddress (no synch endpoint)*/
    },
    {   /*  Audio Streaming Class Specific Endpoint Descriptor */
        0x07,         /*  bLength                           */
        0x25,         /*  bDescriptorType (CS_ENDPOINT)     */
        0x01,         /*  bDescriptorSubtype (General)      */
        0x80,         /*  bmAttributes (No Sam. Freq Control, no pitch control, no padding) */
        0x02,         /*  bLockDelayUnits                   */
        0x00, 0x00        /*  wLockDelay                        */
    },
    {   /*  Audio Streaming Generic Interface 2 Descriptor - Alt 0 */
        0x09,         /*  bLength                          */
        0x04,         /*  bDescriptorType (Interface)      */
        0x02,         /*  bInterfaceNumber                 */
        0x00,         /*  bAlternateSetting                */
        0x00,         /*  bNumEndpoints                    */
        0x01,         /*  bInterfaceClass = Audio Interface (1) */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)    */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0) */
        0x00,         /*  iInterface = No Text String (0)  */
    },
    {   /*  Audio Streaming Generic Interface 2 Descriptor - Alt 1 */
        0x09,         /*  bLength                          */
        0x04,         /*  bDescriptorType (Interface)      */
        0x02,         /*  bInterfaceNumber                 */
        0x01,         /*  bAlternateSetting                */
        0x01,         /*  bNumEndpoints                    */
        0x01,         /*  bInterfaceClass = Audio Interface (1) */
        0x02,         /*  bInterfaceSubClass = Audio Streaming (2)    */
        0x00,         /*  bInterfaceProtocol = No class specific protocol (0) */
        0x00,         /*  biInterface = No Text String (0) */
    },
    {   /*  Audio Streaming Class Specific Interface Descriptor */
        0x07,         /*  bLength                          */
        0x24,         /*  bDescriptorType (ClassSpecific)  */
        0x01,         /*  bDescriptorSubtype               */
        0x04,         /*  bTerminalLink                    */
        0x01,         /*  bDelay                           */
        0x01, 0x00,       /*  wFormatTag (PCM Format)          */
    },
    {   /*  Audio Streaming Type I Format Descriptor */
        0x0B,         /*  bLength                          */
        0x24,         /*  bDescriptorType (ClassSpecific)  */
        0x02,         /*  bDescriptorSubtype               */
        0x01,         /*  bFormatType                      */
        0x01,         /*  bNrChannels                      */
        0x02,         /*  bSubFrameSize                    */
        0x10,         /*  bBitResolution (16 Bit)          */
        0x01,         /*  bSamFreqType (1 Samp. Freq)      */
        0x80, 0x3E, 0x00   /*  tSamFreq (48000 Hz)         */
    },
    {   /*  Generic Endpoint Descriptor */
        0x09,         /*  bLength                          */
        0x05,         /*  bDescriptorType (Endpoint)       */
        0x02,         /*  bEndpointAddress (EP2-OUT)        */
        0x05,         /*  bmAttributes (Isochronous, Asynchronus Synchronization) */
        0x20, 0x00,   /*  wMaxPacketSize                   */
        0x01,         /*  bInterval (N/A)                  */
        0x00,         /*  bRefresh                         */
        0x00          /*  bSynchAddress (no synch endpoint)*/
    },
    {   /*  Audio Streaming Class Specific Endpoint Descriptor */
        0x07,         /*  bLength                           */
        0x25,         /*  bDescriptorType (CS_ENDPOINT)     */
        0x01,         /*  bDescriptorSubtype (General)      */
        0x80,         /*  bmAttributes (No Sam. Freq Control, no pitch control, no padding) */
        0x02,         /*  bLockDelayUnits                   */
        0x00, 0x00        /*  wLockDelay                        */
    }
};


#endif /* _DESCRIPTORS_H_ */
