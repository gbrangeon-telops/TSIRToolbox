/**
 * @file IntelHex.h
 * Intel Hex file format parser module header.
 *
 * This file defines the module used to parse Intel HEX file format.
 * This code is inspired from the Xilinx srec bootloader.
 *
 * $Rev: 19221 $
 * $Author: dalain $
 * $Date: 2016-09-22 14:04:24 -0400 (jeu., 22 sept. 2016) $
 * $Id: IntelHexParser.h 19221 2016-09-22 18:04:24Z dalain $
 * $URL$
 *
 * (c) Copyright 2014 Telops Inc.
 */

#ifndef INTELHEX_H
#define INTELHEX_H

#include "IRC_Status.h"
#include "utils.h"
#include <stdint.h>

#define IHEX_VERBOSE

#ifdef IHEX_VERBOSE
   #define IHEX_PRINTF(fmt, ...)    PRINTF("IHEX: " fmt, ##__VA_ARGS__)
#else
   #define IHEX_PRINTF(fmt, ...)    DUMMY_PRINTF("IHEX: " fmt, ##__VA_ARGS__)
#endif

#define IHEX_ERR(fmt, ...)          IHEX_PRINTF("Error: " fmt "\n", ##__VA_ARGS__)
#define IHEX_INF(fmt, ...)          IHEX_PRINTF("Info: " fmt "\n", ##__VA_ARGS__)


#define IHEX_START_CODE          ':'   /** Intel HEX start code */

#define IHEX_MIN_DATA_COUNT      0     /** Minimum Intel HEX data count in bytes */
#define IHEX_MAX_DATA_COUNT      32    /** Maximum Intel HEX data count in bytes (Maximum data count supported by the parser) */

#define IHEX_START_CODE_SIZE     1     /** Intel HEX start code size in hex digits */
#define IHEX_BYTE_COUNT_SIZE     2     /** Intel HEX byte count size in hex digits */
#define IHEX_ADDRESS_SIZE        4     /** Intel HEX address size in hex digits */
#define IHEX_REC_TYPE_SIZE       2     /** Intel HEX record type size in hex digits */
#define IHEX_MAX_DATA_SIZE       (2 * IHEX_MAX_DATA_COUNT) /** Maximum Intel HEX data size in hex digits */
#define IHEX_CHECKSUM_SIZE       2     /** Intel HEX checksum size in hex digits */
#define IHEX_MAX_TERMINATOR_SIZE 2     /** Maximum Intel HEX terminator size in hex digits */


#define IHEX_MAX_LINE_SIZE       (IHEX_START_CODE_SIZE + IHEX_BYTE_COUNT_SIZE + IHEX_ADDRESS_SIZE + IHEX_REC_TYPE_SIZE + IHEX_MAX_DATA_SIZE + IHEX_CHECKSUM_SIZE + IHEX_MAX_TERMINATOR_SIZE)   /** Maximum line size in chars */


class intelHexParser
{
public:
   intelHexParser();
   ~intelHexParser();

   typedef enum ihexRecordTypeEnum {
      IHEXRT_00 = 0, /** Data */
      IHEXRT_01 = 1, /** End Of File */
      IHEXRT_02 = 2, /** Extended Segment Address */
      IHEXRT_03 = 3, /** Start Segment Address */
      IHEXRT_04 = 4, /** Extended Linear Address */
      IHEXRT_05 = 5  /** Start Linear Address */
   } ihexRecordType_t;

   typedef struct ihexRecordStruct {
      uint8_t byteCount;
      uint16_t address;
      ihexRecordType_t recordType;
      uint8_t data[IHEX_MAX_DATA_COUNT];
      uint8_t chksum;
      struct ihexRecordStruct *next;
   } ihexRecord_t;

   typedef struct promAreaStruct {
      uint32_t address;
      uint32_t size;
      uint8_t *data;
      uint16_t crc16;
      struct promAreaStruct *next;
   } promArea_t;

   IRC_Status_t parse(const char *filename);
   const promArea_t *getPromArea();

private:
   IRC_Status_t getNextLine(const char *buffer, uint32_t buflen, char *lbuffer, uint32_t *p_lbuflen, uint32_t *p_charRead, uint8_t *p_termCount);
   ihexRecord_t *parseLine(const char *lbuffer, uint32_t lbuflen);
   uint8_t computeChecksum(const char *buffer, uint32_t buflen);
   void deleteIntelHexRecord(ihexRecord_t *ihexRecord);
   void deletePROMArea(promArea_t *promArea);

   ihexRecord_t *m_ihexRecord;
   promArea_t *m_promArea;
};

#endif // INTELHEX_H
