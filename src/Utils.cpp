#include "Utils.h"
#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>

/**
 * Convert hex digit to its numerical value.
 *
 * @param x is the hex digit to convert.
 *
 * @return The value corresponding to the hex digit.
 * @return 0 if the specified char is not an hex digit.
 */
uint8_t Hex2Val(const char x)
{
   uint8_t val = 0;

   if ((x >= '0') && (x <= '9'))
   {
      val = (uint8_t)(x - '0');
   }
   else if ((x >= 'A') && (x <= 'F'))
   {
      val = ((uint8_t)(x - 'A')) + 10;
   }
   else if ((x >= 'a') && (x <= 'f'))
   {
      val = ((uint8_t)(x - 'a')) + 10;
   }

   return val;
}

/**
 * Convert two hex digits to its a byte value.
 *
 * @param buffer is char buffer containing the hex digits to convert.
 *
 * @return The byte value corresponding to the hex digits.
 */
uint8_t Hex2Byte(const char *buffer)
{
   return ((Hex2Val(buffer[0]) << 4) + Hex2Val(buffer[1]));
}

/**
 * Convert (2 * N) hex digits to their N bytes value.
 *
 * @param buffer is a char buffer containing the hex digits to convert.
 * @param buflen is the char buffer length.
 * @param bytebuffer is the byte buffer where the converted bytes will be written.
 * @param bytebuflen is the byte buffer length.
 *
 * @return The number of bytes written in byte buffer.
 */
uint32_t Hex2Bytes(const char *buffer, uint32_t buflen, uint8_t *bytebuffer, uint32_t bytebuflen)
{
   uint32_t i;
   uint32_t byteCount = 0;

   for (i = 0; ((i < buflen) && (byteCount < bytebuflen)); i += 2)
   {
      bytebuffer[byteCount++] = Hex2Byte(&buffer[i]);
   }

   return byteCount;
}

/**
 * Print memory data.
 *
 * @param buffer is the pointer to the memory buffer to print.
 * @param length is the amount of data to print in bytes.
 * @param baseAddr is the address corresponding to the first buffer byte.
 * @param lineSize is the number of bytes to print on a single line.
 */
void memdump(const void *buffer, const size_t length, const unsigned int baseAddr, const unsigned int lineSize)
{
	unsigned char *p_buffer = (unsigned char *)buffer;
	unsigned int i;

	for (i = 0; i < length; i++)
	{
		if ((length >= lineSize) && ( i % lineSize == 0))
		{
			PRINTF("0x%08X: 0x", baseAddr + i);
		}

		PRINTF("%02X", p_buffer[i]);

		if ( i % lineSize == lineSize - 1)
		{
			PRINTF("\n");
		}
		else if (i < (length - 1))
		{
			PRINTF(" ");

			if ((length >= lineSize) && (i % lineSize == (lineSize / 2) - 1))
			{
				PRINTF(" ");
			}
		}
	}

	if (length % lineSize != 0)
	{
		PRINTF("\n");
	}
}

bool IsDir(const char* path)
{
	size_t pathLength;
	WCHAR *pathw;
	DWORD fileAttributes;
	bool retval;

	// Convert to wide char
	pathLength = strlen(path) + 1;
	pathw = new WCHAR[pathLength];
	mbstowcs_s(&pathLength, pathw, pathLength, path, pathLength);

	fileAttributes = GetFileAttributes(pathw);

	delete[] pathw;

	retval = ((fileAttributes != INVALID_FILE_ATTRIBUTES) && ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY));

	// printf("%d: %s is%s a directory.\n", fileAttributes, path, retval? "": " not");

	return retval;
}

bool DirFileList(const char* path, std::vector<std::string> &fileList)
{
	size_t strLength;
	std::string strPath = std::string(path);
	WIN32_FIND_DATA ffd;

	fileList.clear();

	if (strPath.length() > (MAX_PATH - 4))
	{
		return false;
	}

	// Make sure that path ends with '\'
	if (strPath[strPath.length() - 1] != '\\')
		strPath += '\\';

	std::string strFilter = strPath + std::string("*.*");

	// Convert file filter to wide char
	strLength = strFilter.length() + 1;
	WCHAR *filterw = new WCHAR[strLength];
	mbstowcs_s(&strLength, filterw, strLength, strFilter.c_str(), strLength);

	HANDLE hFind = FindFirstFile(filterw, &ffd);

	delete[] filterw;

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	do
	{
		// Convert to char
		strLength = wcslen(ffd.cFileName) + 1;
		CHAR *filename = new CHAR[strLength];
		wcstombs_s(&strLength, filename, strLength, ffd.cFileName, strLength);
		
		if (!IsDir((strPath + filename).c_str()))
		{
			// Add file to list
			fileList.push_back(strPath + filename);
		}

		delete[] filename;
	}
	while (FindNextFile(hFind, &ffd));

	return true;
}

void FileParts(const char* file, std::string &filePath, std::string &filename)
{
	size_t lastSepPos = std::string(file).find_last_of("/\\");

	filePath = "";
	filename = std::string(file);

	if (lastSepPos == std::string::npos)
	{
		return;
	}

	filePath = std::string(file);
	filePath.erase(lastSepPos, filePath.length() - lastSepPos);

	filename.erase(0, lastSepPos + 1);
}

void FileName(const char* file, std::string &filename)
{
	std::string dummy;
	FileParts(file, dummy, filename);
}

void FilePath(const char* file, std::string &filePath)
{
	std::string dummy;
	FileParts(file, filePath, dummy);
}
