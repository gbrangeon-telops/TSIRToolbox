#include "FileManagerCmdLineParser.h"
#include <stdio.h>

char helpString[] = 
	"TSIR File Manager (TSIR Toolbox v%d.%d.%d.%d%s)\n"
	"\n"
	"tsirfmtb [command] [command_args]\n"
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
	"   -d filename             Deletes file specified by filename from device.\n"
	"\n"
	"   -p                      Transmits ping command.\n"
	"\n"
	"   -h                      Displays help.\n";

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

	if (cmd.numOfOpts > 0)
	{
		printf("Wrong command code.\n");
		return 1;
	}

	switch (cmd.command.code)
	{
		case FMCLP_CMD_FILE_LIST:
		case FMCLP_CMD_FILE_READ:
		case FMCLP_CMD_FILE_WRITE:
		case FMCLP_CMD_FILE_DELETE:
		case FMCLP_CMD_PING:
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

