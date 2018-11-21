#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include "UsartPortF1F2.h"
#include "IRC_status.h"
#include "utils.h"
#include <vector>

#define FM_PRINTF(fmt, ...)    if (m_verbose) PRINTF("FM: " fmt, ##__VA_ARGS__)
#define FM_ERR(fmt, ...)       FM_PRINTF("Error: " fmt "\n", ##__VA_ARGS__)
#define FM_INF(fmt, ...)       FM_PRINTF("Info: " fmt "\n", ##__VA_ARGS__)

#define FM_DEFAULT_READ_MAX_PACKET_SIZE	248
#define FM_DEFAULT_WRITE_MAX_PACKET_SIZE	USART_MAX_BUFFER_SIZE

class FileManager
{
public:
	FileManager();
	~FileManager();
	
	IRC_Status_t Open();
	bool IsOpened();
	IRC_Status_t Close();

	IRC_Status_t GetFileList(std::vector<F1F2FileInfo_t> &fileList);
	IRC_Status_t FileList();
	IRC_Status_t FileFind(const char *filename, F1F2FileInfo_t *p_f1f2FileInfo);
	IRC_Status_t FileRead(const char *src_filename, const char *dest_filename, const unsigned int maxPacketSize = FM_DEFAULT_READ_MAX_PACKET_SIZE);
	IRC_Status_t FileWrite(const char *src_filename, const unsigned int maxPacketSize = FM_DEFAULT_WRITE_MAX_PACKET_SIZE);
	IRC_Status_t FileCheck(const char *filename);
	IRC_Status_t FileDelete(const char *filename);
	IRC_Status_t FileFormat();

	void SetVerbose(bool verbose);

	void PrintReport();

private:
	UsartPortF1F2 m_port;
	bool m_verbose;
};

#endif // __FILEMANAGER_H__