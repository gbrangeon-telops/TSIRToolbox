/**
 * @file CRC.h
 * Cyclic redundancy check and checksum module header.
 *
 * This file defines the cyclic redundancy check and checksum module interface.
 *
 * $Rev: 19221 $
 * $Author: dalain $
 * $Date: 2016-09-22 14:04:24 -0400 (jeu., 22 sept. 2016) $
 * $Id: CRC.h 19221 2016-09-22 18:04:24Z dalain $
 * $URL: http://einstein/svn/firmware/FIR-00251-Common/trunk/Software/CRC.h $
 *
 * (c) Copyright 2014 Telops Inc.
 */

#ifndef CRC_H
#define CRC_H

#include <stdint.h>

uint8_t Checksum8(const uint8_t *buffer, uint32_t buflen);
uint8_t Checksum8Hex(const char *buffer, uint32_t buflen);
uint16_t CRC16(uint16_t crc, const uint8_t *buffer, uint32_t length);

#endif // CRC_H
