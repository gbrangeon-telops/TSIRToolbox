#ifndef __FILEMANAGERCMDLINEPARSER_H__
#define __FILEMANAGERCMDLINEPARSER_H__

#include "CmdLineParser.h"
#include "IRC_status.h"

#define FMCLP_BUFFER_SIZE			65536

typedef enum {
	FMCLP_CMD_FILE_LIST,
	FMCLP_CMD_FILE_READ,
	FMCLP_CMD_FILE_WRITE,
	FMCLP_CMD_FILE_CHECK,
	FMCLP_CMD_FILE_DELETE,
	FMCLP_CMD_FILE_FORMAT,
	FMCLP_CMD_PING,
	FMCLP_CMD_TX_HEX,
	FMCLP_CMD_TEST,
	FMCLP_CMD_HELP,
	FMCLP_OPT_VERBOSE,
	FMCLP_OPT_MAX_PACKET_SIZE,
	FMCLP_OPT_NUMOF
} fmtbOptCode_t;

extern cmdlOptDesc_t fmtbOptDesc[FMCLP_OPT_NUMOF];

IRC_Status_t FileManagerFileList(cmdlCommand_t *cmd);
IRC_Status_t FileManagerFileRead(cmdlCommand_t *cmd);
IRC_Status_t FileManagerFileWrite(cmdlCommand_t *cmd);
IRC_Status_t FileManagerFileCheck(cmdlCommand_t *cmd);
IRC_Status_t FileManagerFileDelete(cmdlCommand_t *cmd);
IRC_Status_t FileManagerFileFormat(cmdlCommand_t *cmd);
IRC_Status_t FileManagerPing(cmdlCommand_t *cmd);
IRC_Status_t FileManagerTxHex(cmdlCommand_t *cmd);
IRC_Status_t FileManagerTest(cmdlCommand_t *cmd);
IRC_Status_t FileManagerHelp(cmdlCommand_t *cmd);

#endif // __FILEMANAGERCMDLINEPARSER_H__