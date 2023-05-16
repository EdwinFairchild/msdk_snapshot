# BLE_dats_wdxs

This README describes how to use the Wireless Data Exchange (WDX) Server profile. The basic goal of this profile is to provide BLE Peripheral devices with Over The Air (OTA) firmware updates from a Central client.

## Table of Contents

<!-- MarkdownTOC autolink="true" bracket="round" markdown_preview="markdown" -->

- [Services](#services)
    - [WDX](#wdx)
- [Characteristics](#characteristics)
    - [WDX Device Configuration](#wdx-device-configuration)
    - [WDX File Transfer Control](#wdx-file-transfer-control)
        - [WDX FTC GET](#wdx-ftc-get)
        - [WDX FTC PUT](#wdx-ftc-put)
        - [WDX FTC VERIFY](#wdx-ftc-verify)
    - [WDX File Transfer Data](#wdx-file-transfer-data)
- [Usage](#usage)

<!-- /MarkdownTOC -->

## Services
####  WDX
All of the features of this profile are contained in the WDX Service
    
    UUID: 0xFEF6

## Characteristics

#### WDX Device Configuration
Provides the client with the slave's device configuration information. The client can either get or set the configuration values by writing to this characteristic.

    UIUD: 005f0002-2ff2-4ed5-b045-4C7463617865

The first byte is the operation.

    WDX_DC_OP_GET                  0x01
    WDX_DC_OP_SET                  0x02
    WDX_DC_OP_UPDATE               0x03

The second byte is the parameter.

    WDX_DC_ID_CONN_UPDATE_REQ      0x01
    WDX_DC_ID_CONN_PARAM           0x02
    WDX_DC_ID_DISCONNECT_REQ       0x03
    WDX_DC_ID_CONN_SEC_LEVEL       0x04
    WDX_DC_ID_SECURITY_REQ         0x05
    WDX_DC_ID_SERVICE_CHANGED      0x06
    WDX_DC_ID_DELETE_BONDS         0x07
    WDX_DC_ID_ATT_MTU              0x08
    WDX_DC_ID_PHY_UPDATE_REQ       0x09
    WDX_DC_ID_PHY                  0x0A
    WDX_DC_ID_BATTERY_LEVEL        0x20
    WDX_DC_ID_MODEL_NUMBER         0x21
    WDX_DC_ID_FIRMWARE_REV         0x22
    WDX_DC_ID_ENTER_DIAGNOSTICS    0x23
    WDX_DC_ID_DIAGNOSTICS_COMPLETE 0x24
    WDX_DC_ID_DISCONNECT_AND_RESET 0x25

#### WDX File Transfer Control
The File Transfer Control (FTC) characteristic controls the file transfer. Writes to the characteristic initiate an operation on a file handle.
    

    UIUD: 005f0003-2ff2-4ed5-b045-4C7463617865

The first byte is the operation paramter.

    WDX_FTC_OP_NONE                0x00
    WDX_FTC_OP_GET_REQ             0x01
    WDX_FTC_OP_GET_RSP             0x02
    WDX_FTC_OP_PUT_REQ             0x03
    WDX_FTC_OP_PUT_RSP             0x04
    WDX_FTC_OP_ERASE_REQ           0x05
    WDX_FTC_OP_ERASE_RSP           0x06
    WDX_FTC_OP_VERIFY_REQ          0x07
    WDX_FTC_OP_VERIFY_RSP          0x08
    WDX_FTC_OP_ABORT               0x09
    WDX_FTC_OP_EOF                 0x0a

The next two bytes are the file handle. The remaining bytes are used as parameters for GET and PUT operations.

##### WDX FTC GET
Returns data currently in the file.

    uint32_t offset;
    uint32_t length;
    uint8_t preferredXferType; /* Unused */

Execute a GET request on handle 0 at offset 0 to get a list of available files. The header for the list is 7 bytes with the following information. Following the header are all of the file records.

    uint8_t fileFormatVersion;
    uint16_t fileCount;
    uint32_t totalSize;

It is important to note that the total size is the total current size of all of the files.

Each file record is 40 bytes long and is organized as such. 

    uint16_t handle;
    uint8_t type;
    uint8_t permissions;
    uint32_t len;
    char name[16];
    char version[16];

##### WDX FTC PUT
Initiates a file update. Will send a PUT_RSP once prepared to receive data in the WDX File Transfer Data characteristic.

    uint32_t offset;
    uint32_t length;
    uint32_t totalLength; /* offset + length */
    uint8_t preferredXferType; /* Unused */

##### WDX FTC VERIFY
Starts the verificadtion of the file. Will use SHA256 to verify that the last 256 bits of the file match the SHA256 digest of the rest of the file.

TODO: Will commit the OTA on successful verification and reset the device with the new firmware. In future will commit on Device Configuration Disconnect and Reconnect command.

#### WDX File Transfer Data
End point for WDX File Transfer Data (FTD). Write to this characteristic after a PUT REQ to update the file. Data must be written in order.

    UIUD: 005f0004-2ff2-4ed5-b045-4C7463617865


## Usage

+ Perform file discovery by performing a WDX FTC GET on handle 0 to retrieve a listing of the available files. The read length should be a multiple of 40 bytes, typically at least two.
    * WriteReq: WDX FTC (005f0003)
        - Operation: 0x01   (GET) 
        - Handle       : 0x0000
        - Offset       : 0x00000000
        - Length       : 0x00000050 (80 bytes, two files)
        - PrefXferType : 0x00 (unused)
+ Send a put request to iniate the file update. Writes starting at offset 0x00 will automatically erase the file.
    * WriteReq: WDX FTC (005f0003)
        - Operation    : 0x03   (PUT)
        - Handle       : 0x01   (First file handle from discovery)
        - Offset       : 0x00
        - Length       : 0x30   (0x10 byte file, 0x20 byte SHA256 digest)
        - TotalLength  : 0x30   (Offset + Length) 
        - PrefXferType : 0x00 (unused)
+ Write to the FTD endpoint. Make sure writes are acknowledged before writing next block. Block size should be a multiple of 0x4, preferrably 0x10.
    * WriteReq: WDX FTD (005f0004)
        - Block 0
        - Wait for response
    * WriteReq: WDX FTD (005f0004)
        - Block 1
        - Wait for response
    * ... 
    * WriteReq: WDX FTD (005f0004)
        - Block n
        - Wait for response
+ Send a file verification request to verify the SHA256 digest
    * WriteReq: WDX FTC (005f0003)
        - Operation     : 0x07   (VERIFY)
        - Handle        : 0x01
+ Once the verificaiton is complete, send a Disconnect and reset command to the Device Configuration characteristic. This will cause the device to disconnect apply the firmware update, and reset.
    * WriteReq: WDX DC(005f0002)
        - Operation     : 0x02    (SET)
        - Parameter     : 0x25    (DISC_AND_RESET)