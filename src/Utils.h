/**
 * @file utils.h
 * Utility module header.
 *
 * This file defines the utility module interface.
 *
 * $Rev: 19237 $
 * $Author: dalain $
 * $Date: 2016-10-03 07:59:14 -0400 (lun., 03 oct. 2016) $
 * $Id: Utils.h 19237 2016-10-03 11:59:14Z dalain $
 * $URL: http://einstein/svn/firmware/Sandbox/DAL/TEL2000FileManagerToolbox/trunk/t2kfmtb/utils.h $
 *
 * (c) Copyright 2014 Telops Inc.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdio>
#include <stdint.h>
#include <vector>
#include <string>

#define PRINTF			std::printf
#define DUMMY_PRINTF	// std::printf
#define MAX(a,b) (a>b? a : b) /**< Find maximum between to number */
#define MIN(a,b) (a>b? b : a) /**< Find minimum between to number */

#define BitMaskSet(arg, mask) ((arg) |= (mask))             /**< Set masked bits in arg */
#define BitMaskClr(arg, mask) ((arg) &= ~(mask))            /**< Clear masked bits in arg */
#define BitMaskTst(arg, mask) (((arg) & (mask)) == (mask))  /**< Test if masked bits in arg are all set */
#define BitMaskTstAny(arg, mask) (((arg) & (mask)) != 0)    /**< Test if at least of of the masked bits in arg is set */
#define BitMaskFlp(arg, mask) ((arg) ^= (mask))              /**< Flip masked bits in arg */

#define BitSet(arg, pos) BitMaskSet(arg, (1L << (pos)))   	/**< Set bit(pos) in arg */
#define BitClr(arg, pos) BitMaskClr(arg, (1L << (pos)))   	/**< Clear bit(pos) in arg */
#define BitTst(arg, pos) BitMaskTst(arg, (1L << (pos)))   	/**< Test if bit(pos) in arg is true else false */
#define BitFlp(arg, pos) BitMaskFlp(arg, (1L << (pos)))   	/**< Flip bit(pos) in arg */

#define bitIsOn(bitField, bitNumber) (((uint32_t)bitField & (1 << bitNumber)) == (1 << bitNumber))

#define numof(x) (sizeof(x) / sizeof(x[0]))


uint8_t Hex2Val(const char x);
uint8_t Hex2Byte(const char *buffer);
uint32_t Hex2Bytes(const char *buffer, uint32_t buflen, uint8_t *bytebuffer, uint32_t bytebuflen);

void memdump(const void *buffer, const size_t length, const unsigned int baseAddr, const unsigned int lineSize);

bool IsDir(const char* path);
bool DirFileList(const char* path, std::vector<std::string> &fileList);
void FileParts(const char* filePath, std::string &path, std::string &filename);
void FileName(const char* file, std::string &filename);
void FilePath(const char* file, std::string &filePath);

#endif // __UTILS_H__
