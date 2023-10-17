#include "FileManagerCmdLineParser.h"
#include "FileManager.h"
#include "UsartPort.h"
#include "Protocol_F1F2.h"
#include "UsartPortF1F2.h"
#include "BuildInfo.h"
#include "TSIRToolbox.h"
#include <stdlib.h>
#include <sstream>
#include <string>

cmdlOptDesc_t fmtbOptDesc[FMCLP_OPT_NUMOF] =
{
	{FMCLP_CMD_FILE_LIST,		CMDL_TYP_COMMAND,	'l', 0, 0, FileManagerFileList},
	{FMCLP_CMD_FILE_READ,		CMDL_TYP_COMMAND,	'r', 2, 2, FileManagerFileRead},
	{FMCLP_CMD_FILE_WRITE,		CMDL_TYP_COMMAND,	'w', 1, 1, FileManagerFileWrite},
	{FMCLP_CMD_FILE_CHECK,		CMDL_TYP_COMMAND,	'c', 1, 1, FileManagerFileCheck},
	{FMCLP_CMD_FILE_DELETE,		CMDL_TYP_COMMAND,	'd', 1, 1, FileManagerFileDelete},
	{FMCLP_CMD_FILE_FORMAT,		CMDL_TYP_COMMAND,	'f', 0, 0, FileManagerFileFormat},
	{FMCLP_CMD_PING,			CMDL_TYP_COMMAND,	'p', 0, 0, FileManagerPing},
	{FMCLP_CMD_TX_HEX,			CMDL_TYP_COMMAND,	'x', 1, 1, FileManagerTxHex},
	{FMCLP_CMD_TEST,			CMDL_TYP_COMMAND,	't', 2, 2, FileManagerTest},
	{FMCLP_CMD_HELP,			CMDL_TYP_COMMAND,	'h', 0, 0, FileManagerHelp},
	{FMCLP_OPT_VERBOSE,			CMDL_TYP_OPTION,	'v', 0, 0, nullptr},
	{FMCLP_OPT_MAX_PACKET_SIZE, CMDL_TYP_OPTION,	'm', 1, 1, nullptr}
};

IRC_Status_t FileManagerFileList(cmdlCommand_t *cmd)
{
	FileManager fm;
	unsigned int i;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_FILE_LIST)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
			case FMCLP_OPT_VERBOSE:
				fm.SetVerbose(true);
				break;

			default:
				printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
				break;
		}
	}

	try
	{
		if (fm.Open() != IRC_SUCCESS)
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		if (fm.FileList() != IRC_SUCCESS)
		{
			printf("Failed to list file(s).\n");
			throw 0;
		}

		fm.Close();
	}
	catch (...)
	{
		fm.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerFileRead(cmdlCommand_t *cmd)
{
	FileManager fm;
	unsigned int i;
	unsigned int maxPacketSize = FM_DEFAULT_READ_MAX_PACKET_SIZE;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_FILE_READ)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
			case FMCLP_OPT_VERBOSE:
				fm.SetVerbose(true);
				break;

			case FMCLP_OPT_MAX_PACKET_SIZE:
				// TODO Check for conversion error.
				std::stringstream(cmd->options[i].args[0]) >> maxPacketSize;
				break;

			default:
				printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
				break;
		}
	}

	try
	{
		if (fm.Open() != IRC_SUCCESS)
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		if (fm.FileRead(cmd->command.args[0], cmd->command.args[1], maxPacketSize) != IRC_SUCCESS)
		{
			throw 0;
		}
		
		fm.Close();
	}
	catch (...)
	{
		fm.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerFileWrite(cmdlCommand_t *cmd)
{
	FileManager fm;
	unsigned int i;
	unsigned int maxPacketSize = FM_DEFAULT_WRITE_MAX_PACKET_SIZE;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_FILE_WRITE)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
		case FMCLP_OPT_VERBOSE:
			fm.SetVerbose(true);
			break;

		case FMCLP_OPT_MAX_PACKET_SIZE:
			// TODO Check for conversion error.
			std::stringstream(cmd->options[i].args[0]) >> maxPacketSize;
			break;

		default:
			printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
			break;
		}
	}

	try
	{
		if (fm.Open() != IRC_SUCCESS)
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		if (fm.FileWrite(cmd->command.args[0], maxPacketSize) != IRC_SUCCESS)
		{
			throw 0;
		}

		fm.Close();
	}
	catch (...)
	{
		fm.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerFileCheck(cmdlCommand_t *cmd)
{
	FileManager fm;
	unsigned int i;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_FILE_CHECK)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
		case FMCLP_OPT_VERBOSE:
			fm.SetVerbose(true);
			break;

		default:
			printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
			break;
		}
	}

	try
	{
		if (fm.Open() != IRC_SUCCESS)
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		if (fm.FileCheck(cmd->command.args[0]) != IRC_SUCCESS)
		{
			throw 0;
		}

		fm.Close();
	}
	catch (...)
	{
		fm.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerFileDelete(cmdlCommand_t *cmd)
{
	FileManager fm;
	unsigned int i;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_FILE_DELETE)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
		case FMCLP_OPT_VERBOSE:
			fm.SetVerbose(true);
			break;

		default:
			printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
			break;
		}
	}

	try
	{
		if (fm.Open() != IRC_SUCCESS)
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		if (fm.FileDelete(cmd->command.args[0]) != IRC_SUCCESS)
		{
			throw 0;
		}

		fm.Close();
	}
	catch (...)
	{
		fm.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerFileFormat(cmdlCommand_t *cmd)
{
	FileManager fm;
	unsigned int i;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_FILE_FORMAT)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
		case FMCLP_OPT_VERBOSE:
			fm.SetVerbose(true);
			break;

		default:
			printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
			break;
		}
	}

	try
	{
		if (fm.Open() != IRC_SUCCESS)
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		if (fm.FileFormat() != IRC_SUCCESS)
		{
			throw 0;
		}

		fm.Close();
	}
	catch (...)
	{
		fm.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerPing(cmdlCommand_t *cmd)
{
	UsartPort port;
	unsigned char buffer[16];
	unsigned int bytesCount;
	F1F2Command_t f1f2Cmd;
	IRC_Status_t status;
	unsigned int i;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_PING)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
			case FMCLP_OPT_VERBOSE:
				port.SetVerbose(true);
				break;

			default:
				printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
				break;
		}
	}

	try
	{
		if (!port.Open())
		{
			printf("Unable to open port.\n");
			throw 0;
		}

      F1F2_CommandClear(&f1f2Cmd);
		f1f2Cmd.cmd = F1F2_CMD_PING;

		bytesCount = F1F2_CommandBuilder(&f1f2Cmd, buffer, 16);
		if (port.Write(buffer, bytesCount) != bytesCount)
		{
			printf("Cannot send ping request.\n");
			throw 0;
		}

		printf("Ping request has been sent.\n");

		bytesCount = port.Read(buffer, 16, UPF1F2_DEFAULT_TIMEOUT);
		if (bytesCount == 0)
		{
			printf("No response received from the serial port.\n");
			throw 0;
		}

		status = F1F2_CommandParser(buffer, bytesCount, &f1f2Cmd, nullptr);
		if ((status != IRC_SUCCESS) || (f1f2Cmd.cmd != F1F2_CMD_ACK) || (f1f2Cmd.payload.ack.cmd != F1F2_CMD_PING))
		{
			printf("Invalid ping response has been received.\n");
			throw 0;
		}

		printf("Ping response has been received.\n");

		port.Close();
	}
	catch (...)
	{
		port.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerTxHex(cmdlCommand_t *cmd)
{
	UsartPort port;
	unsigned int msglen;
	unsigned int i;
	unsigned int buflen;
	unsigned char *outBuffer;
	unsigned char inBuffer[FMCLP_BUFFER_SIZE];
	char *strHexMsg;
	unsigned int bytesCount;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_TX_HEX)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
			case FMCLP_OPT_VERBOSE:
				port.SetVerbose(true);
				break;

			default:
				printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
				break;
		}
	}

	strHexMsg = cmd->command.args[0];
	msglen = strlen(strHexMsg);
	if (msglen % 2 != 0)
	{
		printf("Message string must have even number of nibbles.\n");
		return IRC_FAILURE;
	}

	try
	{
		if (!port.Open(false))
		{
			printf("Unable to open port.\n");
			throw 0;
		}

		buflen = msglen / 2;
		outBuffer = new unsigned char[buflen];

		for (i = 0; i < buflen; i++)
		{
			outBuffer[i] = Hex2Byte(&strHexMsg[i * 2]);
		}

		if (port.Write(outBuffer, buflen) != buflen)
		{
			printf("Failed to send hex message.\n");
			throw 0;
		}

		bytesCount = port.Read(inBuffer, FMCLP_BUFFER_SIZE, UPF1F2_DEFAULT_TIMEOUT);
		if (bytesCount > 0)
		{
			for (unsigned int i = 0; i < bytesCount; i++)
			{
				printf("%02X", inBuffer[i]);
			}
			printf("\n");
		}
		printf("%d byte(s) received.\n", bytesCount);

		delete []outBuffer;
		port.Close();
	}
	catch (...)
	{
		if (outBuffer != nullptr)
		{
			delete []outBuffer;
		}
		port.Close();
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerTest(cmdlCommand_t *cmd)
{
	UsartPort port;
	unsigned int bytesCount;
	unsigned int n;
	unsigned int i;
	unsigned int txBytesCount;
	unsigned int rxBytesCount;
	unsigned int errorCount = 0;
	IRC_Status_t result;
	unsigned char *outBuffer;
	unsigned char inBuffer[FMCLP_BUFFER_SIZE];
	unsigned int testNumber;

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_TEST)
	{
		return IRC_FAILURE;
	}

	// Check options
	for (i = 0; i < cmd->numOfOpts; i++)
	{
		switch (cmd->options[i].code)
		{
		case FMCLP_OPT_VERBOSE:
			port.SetVerbose(true);
			break;

		default:
			printf("-%c is no a supported option for this command.\n", fmtbOptDesc[cmd->options[i].code].c); 
			break;
		}
	}

	// TODO Check for conversion error.
	std::stringstream(cmd->command.args[0]) >> bytesCount;
	std::stringstream(cmd->command.args[1]) >> n;

	if (bytesCount == 0)
	{
		printf("Specified bytes count must be greater than 0.\n");
		return IRC_FAILURE;
	}

	if (n == 0)
	{
		printf("Specified n must be greater than 0.\n");
		return IRC_FAILURE;
	}

	if (!port.Open(false))
	{
		printf("Unable to open port.\n");
		port.Close();
		return IRC_FAILURE;
	}

	// Allocate test buffers
	outBuffer = new unsigned char[bytesCount];

	// Fill output buffer with counter data
	for (unsigned int i = 0; i < bytesCount; i++)
	{
		outBuffer[i] = i % 256;		
	}
		
	for (testNumber = 1; testNumber <= n; testNumber++)
	{
		printf("Performing test %d / %d\r", testNumber, n);

		result = IRC_SUCCESS;

		// Flush RX buffer
		rxBytesCount = port.Read(inBuffer, FMCLP_BUFFER_SIZE, 0);
		if (rxBytesCount > 0)
		{
			printf("Warning test %d: %d bytes have been flushed.\n", testNumber, rxBytesCount);
		}

		txBytesCount = port.Write(outBuffer, bytesCount);
		if (txBytesCount != bytesCount)
		{
			printf("Error test %d: Wrong number of transmitted bytes (tx = %d).\n", testNumber, txBytesCount);
			result = IRC_FAILURE;
		}
		else 
		{
			rxBytesCount = port.Read(inBuffer, FMCLP_BUFFER_SIZE, UPF1F2_DEFAULT_TIMEOUT);
			if (rxBytesCount != txBytesCount)
			{
				printf("Error test %d: Wrong number of received bytes (tx = %d, rx = %d).\n", testNumber, txBytesCount, rxBytesCount);
				memdump(inBuffer, rxBytesCount, 0, 16);
				result = IRC_FAILURE;
			}
			else if (memcmp(outBuffer, inBuffer, rxBytesCount) != 0)
			{
				printf("Error test %d: Received bytes are different from transmitted bytes.\n", testNumber);
				memdump(inBuffer, rxBytesCount, 0, 16);
				result = IRC_FAILURE;
			}
		}

		if (result == IRC_FAILURE)
		{
			errorCount++;
		}
	}

	printf("\nError count: %d\n", errorCount);

	delete[] outBuffer;
	port.Close();

	if (errorCount > 0)
	{
		return IRC_FAILURE;
	}

	return IRC_SUCCESS;
}

IRC_Status_t FileManagerHelp(cmdlCommand_t *cmd)
{
	extern char helpString[];

	// Validate command code
	if (cmd->command.code != FMCLP_CMD_HELP)
	{
		return IRC_FAILURE;
	}

	printf(helpString, TSIR_TOOLBOX_MAJOR_VERSION, TSIR_TOOLBOX_MINOR_VERSION, TSIR_TOOLBOX_SUBMINOR_VERSION, SVN_SOFTWARE_REV, SVN_SOFTWARE_MODIFIED);

	return IRC_SUCCESS;
}
