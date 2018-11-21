#include "FileManagerCmdLineParser.h"
#include <stdio.h>
#include <Utils.h> 

char helpString[] = 
	"TEL-2000 File Manager (TSIR Toolbox v%d.%d.%d.%d%s)\n"
	"\n"
	"t2kfmtb [command] [command_args] [options]\n"
	"\n"
	"Commands:\n"
	"   -l                      Lists file(s) contained in the device.\n"
	"\n"
	"   -r src_file dest_path   Reads file specified by src_file from device\n"
	"                           and writes it to dest_path. dest_path can either\n"
	"                           refer to a file or to a directory. If src_file\n"
	"                           is *, all of the files contained in the\n"
	"                           device are written to the the directory\n"
	"                           specified by dest_path.\n"
	"\n"
	"   -w src_path             Writes file(s) specified by src_path to device.\n"
	"                           src_path can either refer to a file or to a\n"
	"                           directory.\n"
	"\n"
	"   -c filename             Computes CRC-16 value of the file specified by\n"
	"                           filename.\n"
	"\n"
	"   -d filename             Deletes file specified by filename from device.\n"
	"\n"
	"   -f                      Formats device file system.\n"
	"\n"
	"   -p                      Transmits ping command.\n"
	"\n"
	"   -x msg                  Transmits hex message through USART.\n"
	"\n"
	"   -t bytesCount n         Transmits bytesCount bytes counter through USART\n"
	"                           and waits for loop back. The test will be\n"
	"                           repeated n times.\n"
	"\n"
	"   -h                      Displays help.\n"
	"\n"
	"Options:\n"
	"   -m                      Specifies maximum packet size for read and\n"
	"                           write commands. Ignored otherwise.\n"
	"\n"
	"   -v                      Enables verbose mode.\n";

int main(int argc, char *argv[])
{
	cmdlCommand_t cmd;
		
	if (ParseOptions(argc, argv, fmtbOptDesc, FMCLP_OPT_NUMOF, &cmd) != IRC_SUCCESS)
	{
		return 1;
	}

	if (cmd.command.code == CMDL_CMD_CODE_NONE)
	{
		cmd.command.code = FMCLP_CMD_HELP;
	}

	switch (cmd.command.code)
	{
		case FMCLP_CMD_FILE_LIST:
		case FMCLP_CMD_FILE_READ:
		case FMCLP_CMD_FILE_WRITE:
		case FMCLP_CMD_FILE_CHECK:
		case FMCLP_CMD_FILE_DELETE:
		case FMCLP_CMD_FILE_FORMAT:
		case FMCLP_CMD_PING:
		case FMCLP_CMD_TX_HEX:
		case FMCLP_CMD_TEST:
		case FMCLP_CMD_HELP:
			if (fmtbOptDesc[cmd.command.code].func(&cmd) != IRC_SUCCESS)
			{
				return 1;
			}
			break;

		default:
			printf("Wrong command code.\n");
			return 1;
			break;
	}

	return 0;
}
