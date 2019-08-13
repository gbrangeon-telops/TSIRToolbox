#include "FileManager.h"
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <string.h>
#include "CRC.h"

FileManager::FileManager():
	m_verbose(true)
{

}

FileManager::~FileManager()
{

}

IRC_Status_t FileManager::Open()
{
	return m_port.Open();
}

bool FileManager::IsOpened()
{
	return m_port.IsOpened();
}

IRC_Status_t FileManager::Close()
{
	return m_port.Close();
}

IRC_Status_t FileManager::GetFileList(std::vector<F1F2FileInfo_t> &fileList)
{
	// Clear file list
	fileList.clear();

	// Read file count
	uint32_t fileCount;
	if (m_port.FileCountRequest(&fileCount) != IRC_SUCCESS)
	{
		return IRC_FAILURE;
	}

	fileList.reserve(fileCount);
	
	for (uint32_t i = 0; i < fileCount; i++)
	{
		// Read file info
		F1F2FileInfo_t f1f2FileInfo;
		if (m_port.FileInfoRequest(i, &f1f2FileInfo) != IRC_SUCCESS)
		{
			return IRC_FAILURE;
		}

		// Fill file list
		fileList.push_back(f1f2FileInfo);
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManager::FileList()
{
	// Prepare file info list
	std::vector<F1F2FileInfo_t> fileInfoList;
	if (this->GetFileList(fileInfoList) != IRC_SUCCESS)
	{
		return IRC_FAILURE;
	}

	std::vector<F1F2FileInfo_t>::iterator fileInfoListIterator;
	for (fileInfoListIterator = fileInfoList.begin(); fileInfoListIterator != fileInfoList.end(); fileInfoListIterator++)
	{
		PRINTF("%3d ", fileInfoListIterator->index);
		PRINTF("%s ", fileInfoListIterator->name);
		PRINTF("(%d) ", fileInfoListIterator->size);
		PRINTF("[%08X] ", fileInfoListIterator->attributes);
		PRINTF("0x%X%08X ", (uint32_t)((fileInfoListIterator->id >> 32) & 0xFFFFFFFF), (uint32_t)(fileInfoListIterator->id & 0xFFFFFFFF));
		PRINTF("%s", fileInfoListIterator->type);
		PRINTF("\n");
	}
	PRINTF("%d file(s)\n", fileInfoList.size());

	bool spaceInfoAvail = true;
	uint64_t spaceTotal, spaceUsed, spaceFree;
	if (m_port.FileUsedSpaceRequest(&spaceUsed) != IRC_SUCCESS) spaceInfoAvail = false;
	if (m_port.FileFreeSpaceRequest(&spaceFree) != IRC_SUCCESS) spaceInfoAvail = false;
	if (m_port.FileTotalSpaceRequest(&spaceTotal) != IRC_SUCCESS) spaceInfoAvail = false;
	if (spaceInfoAvail)
		PRINTF("Space in bytes: used = %I64u, free = %I64u, total = %I64u\n", spaceUsed, spaceFree, spaceTotal);
	else
		PRINTF("Space in bytes: not available\n");

	return IRC_SUCCESS;
}


IRC_Status_t FileManager::FileFind(const char *filename, F1F2FileInfo_t *p_f1f2FileInfo)
{
	// Read file count
	uint32_t fileCount;
	if (m_port.FileCountRequest(&fileCount) != IRC_SUCCESS)
	{
		return IRC_FAILURE;
	}

	// Search file
	for (uint32_t i = 0; i < fileCount; i++)
	{
		F1F2FileInfo_t f1f2FileInfo;
		if (m_port.FileInfoRequest(i, &f1f2FileInfo) != IRC_SUCCESS)
		{
			return IRC_FAILURE;
		}

		if (strcmp(f1f2FileInfo.name, filename) == 0)
		{
			if (p_f1f2FileInfo != nullptr)
			{
				*p_f1f2FileInfo = f1f2FileInfo;
			}
			return IRC_SUCCESS;
		}
	}

	return IRC_FAILURE;
}

IRC_Status_t FileManager::FileRead(const char *src_filename, const char *dest_filename, const unsigned int maxPacketSize)
{
	char *fileData = nullptr;
	uint32_t errorCount = 0;
	uint32_t fileTransferred = 0;

	if (maxPacketSize > USART_MAX_BUFFER_SIZE)
	{
		FM_ERR("Maximum packet size cannot be greater than %d.", USART_MAX_BUFFER_SIZE);
		return IRC_FAILURE;
	}

	// Prepare file lists
	std::vector<std::string> fileList;
	fileList.clear();
	if (strcmp(src_filename, "*") == 0)
	{
		if (!IsDir(dest_filename))
		{
			FM_ERR("%s is not a valid directory.", dest_filename);
			return IRC_FAILURE;
		}

		std::vector<F1F2FileInfo_t> fileInfoList;
		if (this->GetFileList(fileInfoList) != IRC_SUCCESS)
		{
			FM_ERR("Cannot build device file list.");
			return IRC_FAILURE;
		}

		std::vector<F1F2FileInfo_t>::iterator fileInfoListIterator;
		for (fileInfoListIterator = fileInfoList.begin(); fileInfoListIterator != fileInfoList.end(); fileInfoListIterator++)
		{
			fileList.push_back(std::string(fileInfoListIterator->name));
		}
	}
	else
	{
		fileList.push_back(std::string(src_filename));
	}

	if (fileList.size() == 0)
	{
		FM_INF("No file to transfer.");
		return IRC_SUCCESS;
	}

	clock_t tStart = clock();

	std::vector<std::string>::iterator fileListIterator;
	for (fileListIterator = fileList.begin(); fileListIterator != fileList.end(); fileListIterator++)
	{
		try
		{
			// Check if file exists
			F1F2FileInfo_t f1f2FileInfo;
			if (FileFind(fileListIterator->c_str(), &f1f2FileInfo) != IRC_SUCCESS)
			{
				FM_ERR("%s file not found.", fileListIterator->c_str());
				throw 0;
			}

			// Build destination filename
			std::string strDest_Filename(dest_filename);
			if (IsDir(strDest_Filename.c_str()))
			{
				// Make sure that path ends with '\'
				if (strDest_Filename[strDest_Filename.length() - 1] != '\\')
					strDest_Filename += '\\';

				strDest_Filename += *fileListIterator;
			}

			// Open destination file
			std::ofstream file;
			file.open(strDest_Filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
			if (!file.is_open())
			{
				FM_ERR("Unable to open %s.", strDest_Filename.c_str());
				throw 0;
			}

			// Read data from file
			fileData = new char[f1f2FileInfo.size];
			uint32_t byteCount = 0;

			PRINTF("Reading %s... 0%%", fileListIterator->c_str());
			while (byteCount < f1f2FileInfo.size)
			{
				uint16_t length = MIN(f1f2FileInfo.size - byteCount, maxPacketSize - F1F2_MIN_FILE_PACKET_SIZE);
				if (m_port.FileReadRequest(f1f2FileInfo.index, byteCount, length, (uint8_t *)&fileData[byteCount]) != IRC_SUCCESS)
				{
					PRINTF("\n");
					FM_ERR("Failed to read %d bytes @ %d in file %s.", length, byteCount, src_filename);
					throw 0;;
				}

				byteCount += length;

				PRINTF("\rReading %s... %d%%", fileListIterator->c_str(), (byteCount * 100) / f1f2FileInfo.size);
			}
			PRINTF("\n");

			// Write data to destination file
			file.write(fileData, f1f2FileInfo.size);
			file.close();

			delete[] fileData;

			fileTransferred++;
		}
		catch (...)
		{
			if (fileData != nullptr)
			{
				delete[] fileData;
			}

			errorCount++;
		}
	}

	if (fileTransferred != fileList.size())
	{
		PRINTF("%d file(s) out of ", fileTransferred);
	}

	PRINTF("%d file(s) transferred in %.3fs\n", fileList.size(), (double)(clock() - tStart)/CLOCKS_PER_SEC);

	if (errorCount > 0)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManager::FileWrite(const char *src_filename, const unsigned int maxPacketSize)
{
	char *fileData = nullptr;
	uint32_t errorCount = 0;
	uint32_t fileTransferred = 0;

	if (maxPacketSize > USART_MAX_BUFFER_SIZE)
	{
		FM_ERR("Maximum packet size cannot be greater than %d.", USART_MAX_BUFFER_SIZE);
		return IRC_FAILURE;
	}
	
	// Prepare file list
	std::vector<std::string> fileList;
	if (IsDir(src_filename))
	{
		if (!DirFileList(src_filename, fileList))
		{
			FM_ERR("Cannot build file list from %s.", src_filename);
			return IRC_FAILURE;
		}
	}
	else
	{
		fileList.clear();
		fileList.push_back(std::string(src_filename));
	}

	if (fileList.size() == 0)
	{
		FM_INF("No file to transfer.");
		return IRC_SUCCESS;
	}

	clock_t tStart = clock();

	std::vector<std::string>::iterator fileListIterator;
	for (fileListIterator = fileList.begin(); fileListIterator != fileList.end(); fileListIterator++)
	{
		try
		{
			// Open source file
			std::ifstream file;
			file.open(fileListIterator->c_str(), std::ios::in | std::ios::binary | std::ios::ate);
			if (!file.is_open())
			{
				FM_ERR("Unable to open %s.", fileListIterator->c_str());
				throw 0;
			}

			// Read source file data
			uint32_t fileSize = (uint32_t) file.tellg();
			fileData = new char[fileSize];
			file.seekg(0, std::ios::beg);
			file.read((char *) fileData, fileSize);
			file.close();

			// Get filename
			std::string dest_filename;
			FileName(fileListIterator->c_str(), dest_filename);

			// Check if file exists
			if (FileFind(dest_filename.c_str(), nullptr) == IRC_SUCCESS)
			{
				FM_ERR("%s file already exists in device.", dest_filename.c_str());
				throw 0;
			}

			// Create file
			uint32_t fileIndex;
			if (m_port.FileCreateRequest(dest_filename.c_str(), &fileIndex) != IRC_SUCCESS)
			{
				FM_ERR("Cannot create file %s.", dest_filename.c_str());
				throw 0;
			}

			// Write data to file
			uint32_t byteCount = 0;

			PRINTF("Writing %s... 0%%", dest_filename.c_str());
			while (byteCount < fileSize)
			{
				uint16_t length = MIN(fileSize - byteCount, maxPacketSize - F1F2_MIN_FILE_PACKET_SIZE);
				if (m_port.FileWriteRequest(fileIndex, byteCount, length, (uint8_t *)&fileData[byteCount]) != IRC_SUCCESS)
				{
					PRINTF("\n");
					FM_ERR("Failed to write %d bytes @ %d in file %s.", length, byteCount, dest_filename.c_str());
					throw 0;
				}

				byteCount += length;

				PRINTF("\rWriting %s... %d%%", dest_filename.c_str(), (byteCount * 100) / fileSize);
			}
			PRINTF("\n");

			// Check file data
			uint16_t crc16;
			if (m_port.FileCheckRequest(fileIndex, &crc16) != IRC_SUCCESS)
			{
				FM_ERR("Cannot check file %s.", dest_filename.c_str());
				throw 0;
			}

			// Compute source file CRC-16
			uint16_t fileCRC16 = CRC16(0xFFFF, (uint8_t *)fileData, fileSize);
			delete[] fileData;

			if (crc16 != fileCRC16)
			{
				FM_ERR("File CRC-16 mismatch (local file CRC-16 = 0x%04X, device file CRC-16 = 0x%04X).", 
					fileCRC16, crc16);

				// Delete file
				if (m_port.FileDeleteRequest(fileIndex) != IRC_SUCCESS)
				{
					FM_ERR("Cannot delete file %s.", dest_filename.c_str());
					throw 0;
				}
			}
			else
			{
				// Close file
				if (m_port.FileCloseRequest(fileIndex) != IRC_SUCCESS)
				{
					FM_ERR("Cannot close file %s.", dest_filename.c_str());
					throw 0;
				}
			}

			fileTransferred++;
		}
		catch (...)
		{
			if (fileData != nullptr)
			{
				delete[] fileData;
			}

			errorCount++;
		}
	}

	if (fileTransferred != fileList.size())
	{
		PRINTF("%d file(s) out of ", fileTransferred);
	}

	PRINTF("%d file(s) transferred in %.3fs\n", fileList.size(), (double)(clock() - tStart)/CLOCKS_PER_SEC);

	if (errorCount > 0)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManager::FileCheck(const char *filename)
{
	try
	{
		// Check if file exists
		F1F2FileInfo_t f1f2FileInfo;
		if (FileFind(filename, &f1f2FileInfo) != IRC_SUCCESS)
		{
			FM_ERR("%s file not found.", filename);
			throw 0;
		}

		// Check file
		uint16_t crc16;
		if (m_port.FileCheckRequest(f1f2FileInfo.index, &crc16) != IRC_SUCCESS)
		{
			FM_ERR("Cannot check file %s.", filename);
			throw 0;
		}

		FM_INF("%s CRC-16 is 0x%04X.", filename, crc16);
	}
	catch (...)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManager::FileDelete(const char *filename)
{
	try
	{
		// Check if file exists
		F1F2FileInfo_t f1f2FileInfo;
		if (FileFind(filename, &f1f2FileInfo) != IRC_SUCCESS)
		{
			FM_ERR("%s file not found.", filename);
			throw 0;
		}

		// Delete file
		if (m_port.FileDeleteRequest(f1f2FileInfo.index) != IRC_SUCCESS)
		{
			FM_ERR("Cannot delete file %s.", filename);
			throw 0;
		}
	}
	catch (...)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManager::FileFormat()
{
	try
	{
		// Format file system
		if (m_port.FileFormatRequest() != IRC_SUCCESS)
		{
			FM_ERR("Cannot format file system.");
			throw 0;
		}
	}
	catch (...)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

void FileManager::SetVerbose(bool verbose)
{
	// m_verbose = verbose;
	m_port.SetVerbose(verbose);
}

void FileManager::PrintReport()
{
	PRINTF("Error count: %d\n", m_port.GetErrorCount());
	PRINTF("Timeout error count: %d\n", m_port.GetTimeoutErrorCount());
	PRINTF("CRC-16 error count: %d\n", m_port.GetCRCErrorCount());
	PRINTF("Max retry count: %d / %d\n", m_port.GetMaxRetryCount(), m_port.GetMaxRetry());
}


