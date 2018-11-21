/**
 * @file Protocol_F1F2.h
 * F1F2 protocol module header.
 *
 * This file defines the F1F2v2 protocol interface.
 *
 * $Rev: 19221 $
 * $Author: dalain $
 * $Date: 2016-09-22 14:04:24 -0400 (jeu., 22 sept. 2016) $
 * $Id: Protocol_F1F2.h 19221 2016-09-22 18:04:24Z dalain $
 * $URL: http://einstein/svn/firmware/FIR-00251-Common/branchs/2015-07-06%20-%20Comm%20Inter-FPGA/Software/Protocol_F1F2.h $
 *
 * (c) Copyright 2014 Telops Inc.
 */

#ifndef PROTOCOL_F1F2_H
#define PROTOCOL_F1F2_H

#include "IRC_Status.h"
#include "Utils.h"
#include <stdint.h>

#define GC_REG_MAX_WRITE_LENGTH		4	// From GenICam.h

#ifdef F1F2_VERBOSE
   #define F1F2_PRINTF(fmt, ...)    PRINTF("F1F2: " fmt, ##__VA_ARGS__)
#else
   #define F1F2_PRINTF(fmt, ...)    DUMMY_PRINTF("F1F2: " fmt, ##__VA_ARGS__)
#endif

#define F1F2_ERR(fmt, ...)          PRINTF("F1F2: Error: " fmt "\n", ##__VA_ARGS__)
#define F1F2_INF(fmt, ...)          F1F2_PRINTF("Info: " fmt "\n", ##__VA_ARGS__)

// Special F1F2 character
#define F1F2_SOP                    0xF1     /**< F1F2 Start Of Packet value */
#define F1F2_EOP                    0xF2     /**< F1F2 End Of Packet value */

// F1F2 command codes
#define F1F2_CMD_NONE               0x00     /**< F1F2 empty command code */
#define F1F2_CMD_ACK                0xA0     /**< F1F2 Acknowledge command code */
#define F1F2_CMD_NAK                0xA1     /**< F1F2 No Acknowledge command code */
#define F1F2_CMD_REG_READ_REQ       0x01     /**< F1F2 Register Read Request command code */
#define F1F2_CMD_REG_READ_RSP       0x02     /**< F1F2 Register Read Response command code */
#define F1F2_CMD_REG_WRITE          0x03     /**< F1F2 Register Write command code */
#define F1F2_CMD_FILE_COUNT_REQ     0x30     /**< F1F2 File Count Request command code */
#define F1F2_CMD_FILE_COUNT_RSP     0x31     /**< F1F2 File Count Response command code */
#define F1F2_CMD_FILE_INFO_REQ      0x32     /**< F1F2 File Information Request command code */
#define F1F2_CMD_FILE_INFO_RSP      0x33     /**< F1F2 File Information Response command code */
#define F1F2_CMD_FILE_CREATE_REQ    0x34     /**< F1F2 File Create Request command code */
#define F1F2_CMD_FILE_CREATE_RSP    0x35     /**< F1F2 File Create Response command code */
#define F1F2_CMD_FILE_READ_REQ      0x36     /**< F1F2 File Read Request command code */
#define F1F2_CMD_FILE_READ_RSP      0x37     /**< F1F2 File Read Response command code */
#define F1F2_CMD_FILE_WRITE         0x38     /**< F1F2 File Write Request command code */
#define F1F2_CMD_FILE_CLOSE         0x39     /**< F1F2 File Close Request command code */
#define F1F2_CMD_FILE_CHECK_REQ     0x3A     /**< F1F2 File Check Request command code */
#define F1F2_CMD_FILE_CHECK_RSP     0x3B     /**< F1F2 File Check Response command code */
#define F1F2_CMD_FILE_DELETE        0x3C     /**< F1F2 File Delete Request command code */
#define F1F2_CMD_FILE_FORMAT        0x3D     /**< F1F2 File Format Request command code */
#define F1F2_CMD_PROM_ERASE         0x40     /**< F1F2 PROM Erase Request command code */
#define F1F2_CMD_PROM_READ_REQ      0x41     /**< F1F2 PROM Read Request command code */
#define F1F2_CMD_PROM_READ_RSP      0x42     /**< F1F2 PROM Read Response command code */
#define F1F2_CMD_PROM_WRITE         0x43     /**< F1F2 PROM Write Request command code */
#define F1F2_CMD_PROM_CHECK_REQ     0x44     /**< F1F2 PROM Check Request command code */
#define F1F2_CMD_PROM_CHECK_RSP     0x45     /**< F1F2 PROM Check Response command code */
#define F1F2_CMD_PING               0x50     /**< F1F2 Ping command code */
#define F1F2_CMD_NETWORK            0x80     /**< F1F2 Network command code */
#define F1F2_CMD_ERROR              0xFF     /**< F1F2 command value returned on parsing error */

// F1F2 fields size
#define F1F2_CMD_CODE_SIZE          1        /**< F1F2 command code field size in bytes */
#define F1F2_PAYLOADDATACOUNT_SIZE  2        /**< F1F2 payload data count field size in bytes */
#define F1F2_NET_SRC_ADDR_SIZE      1        /**< F1F2 network command source address field size in bytes */
#define F1F2_NET_SRC_PORT_SIZE      1        /**< F1F2 network command source port field size in bytes */
#define F1F2_NET_DEST_ADDR_SIZE     1        /**< F1F2 network command destination address field size in bytes */
#define F1F2_NET_DEST_PORT_SIZE     1        /**< F1F2 network command destination port field size in bytes */
#define F1F2_REG_ADDR_SIZE          4        /**< F1F2 register address field size in bytes */
#define F1F2_FILE_COUNT_SIZE        4        /**< F1F2 file count field size in bytes */
#define F1F2_FILE_INDEX_SIZE        4        /**< F1F2 file index field size in bytes */
#define F1F2_FILE_ID_SIZE           8        /**< F1F2 file ID field size in bytes */
#define F1F2_FILE_NAME_SIZE         128      /**< F1F2 file name field size in bytes */
#define F1F2_FILE_TYPE_SIZE         4        /**< F1F2 file type field size in bytes */
#define F1F2_FILE_ATTR_SIZE         4        /**< F1F2 file attributes field size in bytes */
#define F1F2_FILE_SIZE_SIZE         4        /**< F1F2 file size field size in bytes */
#define F1F2_FILE_CRC16_SIZE        2        /**< F1F2 file CRC-16 field size in bytes */
#define F1F2_FILE_INFO_SIZE         (F1F2_FILE_INDEX_SIZE + F1F2_FILE_NAME_SIZE + F1F2_FILE_SIZE_SIZE + F1F2_FILE_ATTR_SIZE + F1F2_FILE_ID_SIZE + F1F2_FILE_TYPE_SIZE)  /**< F1F2 file info field size in bytes */
#define F1F2_FILE_OFFSET_SIZE       4        /**< F1F2 file data offset field size in bytes */
#define F1F2_FILE_LENGTH_SIZE       2        /**< F1F2 file data length field size in bytes */
#define F1F2_PROM_DEVICE_SIZE       1        /**< F1F2 PROM device number field size in bytes */
#define F1F2_PROM_OFFSET_SIZE       4        /**< F1F2 PROM data offset field size in bytes */
#define F1F2_PROM_LENGTH_SIZE       4        /**< F1F2 PROM data length field size in bytes */
#define F1F2_PROM_RW_LENGTH_SIZE    2        /**< F1F2 PROM data read/write length field size in bytes */
#define F1F2_PROM_CRC16_SIZE        2        /**< F1F2 PROM CRC-16 field size in bytes */
#define F1F2_CRC16_SIZE             2        /**< F1F2 CRC-16 field size in bytes */

// F1F2 packet header offset
#define F1F2_OFFSET_SOP             0                                                  /**< F1F2 Start Of Packet offset */
#define F1F2_OFFSET_CMD             1                                                  /**< F1F2 Command code offset */
#define F1F2_OFFSET_PDC             (F1F2_OFFSET_CMD + F1F2_CMD_CODE_SIZE)             /**< F1F2 Payload Data Count offset */
#define F1F2_OFFSET_PAYLOAD_DATA    (F1F2_OFFSET_PDC + F1F2_PAYLOADDATACOUNT_SIZE)     /**< F1F2 Payload Data offset */

// F1F2 payload data bytes offset
// Network payload data bytes offset
#define F1F2_PD_OFFSET_NET_SRC_ADDR    0                                                        /**< F1F2 Payload Data Network Source Address offset */
#define F1F2_PD_OFFSET_NET_SRC_PORT    (F1F2_PD_OFFSET_NET_SRC_ADDR + F1F2_NET_SRC_ADDR_SIZE)   /**< F1F2 Payload Data Network Source Port offset */
#define F1F2_PD_OFFSET_NET_DEST_ADDR   (F1F2_PD_OFFSET_NET_SRC_PORT + F1F2_NET_SRC_PORT_SIZE)   /**< F1F2 Payload Data Network Destination Address offset */
#define F1F2_PD_OFFSET_NET_DEST_PORT   (F1F2_PD_OFFSET_NET_DEST_ADDR + F1F2_NET_DEST_ADDR_SIZE) /**< F1F2 Payload Data Network Destination Port offset */
#define F1F2_PD_OFFSET_NET_CMD         (F1F2_PD_OFFSET_NET_DEST_PORT + F1F2_NET_DEST_PORT_SIZE) /**< F1F2 Payload Data Network Command offset */
#define F1F2_PD_OFFSET_NET_PAYLOAD_DATA (F1F2_PD_OFFSET_NET_CMD + F1F2_CMD_CODE_SIZE)           /**< F1F2 Payload Data Network Payload Data offset */
// ACK/NACK payload data bytes offset
#define F1F2_PD_OFFSET_CMD             0                                                        /**< F1F2 Payload Data Command offset */
// Register read/write payload data bytes offset
#define F1F2_PD_OFFSET_REG_ADDR        0                                                        /**< F1F2 Payload Data Register Address offset */
#define F1F2_PD_OFFSET_REG_VALUE       (F1F2_PD_OFFSET_REG_ADDR + F1F2_REG_ADDR_SIZE)           /**< F1F2 Payload Data Register Value offset */
// File management payload data bytes offset
#define F1F2_PD_OFFSET_FILE_COUNT      0                                                        /**< F1F2 Payload Data File Count offset */
#define F1F2_PD_OFFSET_FILE_INDEX      0                                                        /**< F1F2 Payload Data File Index offset */
#define F1F2_PD_OFFSET_FILE_INF_NAME   (F1F2_PD_OFFSET_FILE_INDEX + F1F2_FILE_INDEX_SIZE)       /**< F1F2 Payload Data File Info Name offset */
#define F1F2_PD_OFFSET_FILE_INF_SIZE   (F1F2_PD_OFFSET_FILE_INF_NAME + F1F2_FILE_NAME_SIZE)     /**< F1F2 Payload Data File Info Size offset */
#define F1F2_PD_OFFSET_FILE_INF_ATTR   (F1F2_PD_OFFSET_FILE_INF_SIZE + F1F2_FILE_SIZE_SIZE)     /**< F1F2 Payload Data File Info Attributes offset */
#define F1F2_PD_OFFSET_FILE_INF_ID     (F1F2_PD_OFFSET_FILE_INF_ATTR + F1F2_FILE_ATTR_SIZE)     /**< F1F2 Payload Data File Info ID offset */
#define F1F2_PD_OFFSET_FILE_INF_TYPE   (F1F2_PD_OFFSET_FILE_INF_ID + F1F2_FILE_ID_SIZE)         /**< F1F2 Payload Data File Info Type offset */
#define F1F2_PD_OFFSET_FILE_NAME       0                                                        /**< F1F2 Payload Data File Name offset */
#define F1F2_PD_OFFSET_FILE_OFFSET     (F1F2_PD_OFFSET_FILE_INDEX + F1F2_FILE_INDEX_SIZE)       /**< F1F2 Payload Data File Data Offset offset */
#define F1F2_PD_OFFSET_FILE_LENGTH     (F1F2_PD_OFFSET_FILE_OFFSET + F1F2_FILE_OFFSET_SIZE)     /**< F1F2 Payload Data File Data Length offset */
#define F1F2_PD_OFFSET_FILE_DATA       (F1F2_PD_OFFSET_FILE_LENGTH + F1F2_FILE_LENGTH_SIZE)     /**< F1F2 Payload Data File Data offset */
#define F1F2_PD_OFFSET_FILE_CRC16      (F1F2_PD_OFFSET_FILE_INDEX + F1F2_FILE_INDEX_SIZE)       /**< F1F2 Payload Data File Info CRC-16 offset */
// Firmware updater payload data bytes offset
#define F1F2_PD_OFFSET_PROM_DEVICE     0                                                        /**< F1F2 Payload Data PROM Device Number offset */
#define F1F2_PD_OFFSET_PROM_OFFSET     (F1F2_PD_OFFSET_PROM_DEVICE + F1F2_PROM_DEVICE_SIZE)     /**< F1F2 Payload Data PROM Data Offset offset */
#define F1F2_PD_OFFSET_PROM_RW_LENGTH  (F1F2_PD_OFFSET_PROM_OFFSET + F1F2_PROM_OFFSET_SIZE)     /**< F1F2 Payload Data PROM Data Read/Write Length offset */
#define F1F2_PD_OFFSET_PROM_DATA       (F1F2_PD_OFFSET_PROM_RW_LENGTH + F1F2_PROM_RW_LENGTH_SIZE) /**< F1F2 Payload Data PROM Data offset */
#define F1F2_PD_OFFSET_PROM_LENGTH     (F1F2_PD_OFFSET_PROM_OFFSET + F1F2_PROM_OFFSET_SIZE)     /**< F1F2 Payload Data PROM Data Check Length offset */
#define F1F2_PD_OFFSET_PROM_CRC16      (F1F2_PD_OFFSET_PROM_LENGTH + F1F2_PROM_LENGTH_SIZE)     /**< F1F2 Payload Data PROM CRC-16 offset */

// F1F2 size limits definition
#define F1F2_MAX_PAYLOADDATACOUNT   65535                                                       /**< Maximum F1F2 payload data count */
#define F1F2_MIN_PACKET_SIZE        (1 + F1F2_CMD_CODE_SIZE + F1F2_PAYLOADDATACOUNT_SIZE + F1F2_CRC16_SIZE + 1)   /**< Minimum F1F2 packet size in bytes */
#define F1F2_MAX_PACKET_SIZE        (F1F2_MIN_PACKET_SIZE + F1F2_MAX_PAYLOADDATACOUNT)          /**< Maximum F1F2 packet size in bytes */
#define F1F2_MIN_FILE_PACKET_SIZE   (F1F2_MIN_PACKET_SIZE + F1F2_FILE_INDEX_SIZE + F1F2_FILE_OFFSET_SIZE + F1F2_FILE_LENGTH_SIZE) /**< Minimum F1F2 file packet size in bytes */
#define F1F2_MAX_FILE_PACKET_SIZE   256                                                         /**< Maximum F1F2 file packet size in bytes */
#define F1F2_MAX_FILE_DATA_SIZE     (F1F2_MAX_FILE_PACKET_SIZE - F1F2_MIN_FILE_PACKET_SIZE)     /**< Maximum F1F2 file packet data size in bytes */
#define F1F2_MIN_PROM_PACKET_SIZE   (F1F2_MIN_PACKET_SIZE + F1F2_PROM_DEVICE_SIZE + F1F2_PROM_OFFSET_SIZE + F1F2_PROM_RW_LENGTH_SIZE) /**< Minimum F1F2 PROM packet size in bytes */
#define F1F2_MAX_PROM_PACKET_SIZE   256                                                         /**< Maximum F1F2 PROM packet size in bytes */
#define F1F2_MAX_PROM_DATA_SIZE     (F1F2_MAX_PROM_PACKET_SIZE - F1F2_MIN_PROM_PACKET_SIZE)     /**< Maximum F1F2 PROM packet data size in bytes */
#define F1F2_MIN_NET_PAYLOADDATACOUNT (F1F2_NET_SRC_ADDR_SIZE + F1F2_NET_SRC_PORT_SIZE + F1F2_NET_DEST_ADDR_SIZE + F1F2_NET_DEST_PORT_SIZE + F1F2_CMD_CODE_SIZE) /**< Minimum F1F2 network payload data count */
#define F1F2_MIN_NET_PACKET_SIZE    (F1F2_MIN_PACKET_SIZE + F1F2_MIN_NET_PAYLOADDATACOUNT)      /**< Minimum F1F2 network packet size in bytes */


/**
 * F1F2 file information data structure.
 */
struct F1F2FileInfoStruct {
   uint32_t index;                     /**< File index */
   uint32_t size;                      /**< File size in bytes */
   uint32_t attributes;                /**< File attributes */
   uint64_t id;                        /**< File ID */
   char name[F1F2_FILE_NAME_SIZE + 1]; /**< File name (F1F2_FILE_NAME_SIZE + NULL char) */
   char type[F1F2_FILE_TYPE_SIZE + 1]; /**< File type (4 char + NULL char) */
};

/**
 * F1F2 file information type definition.
 */
typedef struct F1F2FileInfoStruct F1F2FileInfo_t;


/**
 * F1F2 command data structure.
 */
struct F1F2CommandStruct {
   uint8_t cmd;               /**< F1F2 command code */
   uint8_t isNetwork;         /**< Indicates whether F1F2 command is a network command */
   uint8_t srcAddr;           /**< F1F2 network command source address */
   uint8_t srcPort;           /**< F1F2 network command source port */
   uint8_t destAddr;          /**< F1F2 network command destination address */
   uint8_t destPort;          /**< F1F2 network command destination port */
   union {
      struct {
         uint8_t cmd;         /**< Acknowledged command code */
      } ack;                  /**< F1F2 payload data for ACK and NAK command codes */

      struct {
         uint32_t address;    /**< Register address */
         uint8_t data[GC_REG_MAX_WRITE_LENGTH]; /**< Register data */
         uint16_t dataLength; /**< Register data length in bytes */
         uint16_t padLength;  /**< Register data padding length in bytes */
      } regRW;                /**< F1F2 payload data for REG_READ_REQ, REG_READ_RSP and REG_WRITE command codes */

      struct {
         uint32_t count;      /**< File count */
      } fileCount;            /**< F1F2 payload data for FILE_COUNT_RSP command code */

      struct {
         uint32_t index;      /**< File index */
      } fileIndex;            /**< F1F2 payload data for several command codes */

      F1F2FileInfo_t fileInfo;/**< F1F2 payload data for FILE_INFO_RSP command code */

      struct {
         char name[F1F2_FILE_NAME_SIZE + 1]; /**< File name */
      } fileName;             /**< F1F2 payload data for FILE_CREATE_REQ command code */

      struct {
         uint32_t index;      /**< File index */
         uint32_t offset;     /**< File data offset */
         uint8_t data[F1F2_MAX_FILE_DATA_SIZE]; /**< File data */
         uint16_t dataLength; /**< File data length in bytes */
      } fileRW;               /**< F1F2 payload data for FILE_READ_REQ, FILE_READ_RSP and FILE_WRITE command codes */

      struct {
         uint32_t index;      /**< File index */
         uint16_t crc16;      /**< File CRC-16 */
      } fileCheck;            /**< F1F2 payload data for FILE_CHECK_RSP command code */

      struct {
         uint8_t device;      /**< PROM device number */
         uint32_t offset;     /**< PROM data offset */
         uint32_t dataLength; /**< PROM data length in bytes */
      } promErase;            /**< F1F2 payload data for PROM_ERASE command code */

      struct {
         uint8_t device;      /**< PROM device number */
         uint32_t offset;     /**< PROM data offset */
         uint8_t data[F1F2_MAX_PROM_DATA_SIZE]; /**< PROM data */
         uint16_t dataLength; /**< PROM data length in bytes */
      } promRW;               /**< F1F2 payload data for PROM_READ_REQ, PROM_READ_RSP and PROM_WRITE command code */

      struct {
         uint8_t device;      /**< PROM device number */
         uint32_t offset;     /**< PROM data offset */
         uint32_t dataLength; /**< PROM data length in bytes */
         uint16_t crc16;      /**< PROM CRC-16 */
      } promCheck;            /**< F1F2 payload data for PROM_CHECK_REQ and PROM_CHECK_RSP command code */

   } payload;                 /**< F1F2 payload data */
};

/**
 * F1F2 command type definition.
 */
typedef struct F1F2CommandStruct F1F2Command_t;

/**
 * F1F2 payload data structure.
 */
struct F1F2PayloadDataStruct {
   void *p_data;        /**< F1F2 payload data bytes */
   uint16_t dataLength; /**< F1F2 payload data length (bytes) */
   uint16_t padLength;  /**< F1F2 payload pad length (bytes) */
};

/**
 * F1F2 payload data type definition.
 */
typedef struct F1F2PayloadDataStruct F1F2PayloadData_t;

void F1F2_FileInfoClear(F1F2FileInfo_t *f1f2FileInfo);
void F1F2_CommandClear(F1F2Command_t *f1f2Cmd);
IRC_Status_t F1F2_CommandParser(uint8_t *buffer, uint16_t buflen, F1F2Command_t *f1f2Cmd, uint32_t *crcErrorCount);
void F1F2_BuildResponse(F1F2Command_t *p_request, F1F2Command_t *p_response);
void F1F2_BuildACKResponse(F1F2Command_t *p_request, F1F2Command_t *p_response);
void F1F2_BuildNAKResponse(F1F2Command_t *p_request, F1F2Command_t *p_response);
uint32_t F1F2_CommandBuilder(F1F2Command_t *f1f2Cmd, uint8_t *buffer, uint16_t buflen);

#endif // PROTOCOL_F1F2_H
