#include "CmdLineParser.h"
#include <string>

IRC_Status_t ParseOptions(unsigned int argc, char *argv[], const cmdlOptDesc_t cmdlOptDesc[], unsigned int cmdlOptDescSize, cmdlCommand_t *cmd)
{
	unsigned int argi = 1;
	bool commandFound = false;
	bool optionIsValid;
	unsigned int i, j;
	cmdlOption_t *p_option;

	memset(cmd, 0, sizeof(cmdlCommand_t));

	if (argc < 2)
	{
		cmd->command.code = CMDL_CMD_CODE_NONE;
		return IRC_SUCCESS;
	}

	while (argi < argc)
	{
		if ((strlen(argv[argi]) != 2) || (argv[argi][0] != '-'))
		{
			printf("Wrong tag format.\n");
			return IRC_FAILURE;
		}

		optionIsValid = false;

		for (i = 0; i < cmdlOptDescSize; i++)
		{
			if (argv[argi][1] == cmdlOptDesc[i].c)
			{
				optionIsValid = true;

				if (argc - (argi + 1) < cmdlOptDesc[i].numOfArgs)
				{
					printf("Not enough arguments for -%c.\n", cmdlOptDesc[i].c);
					return IRC_FAILURE;
				}
				
				switch (cmdlOptDesc[i].type)
				{
					case CMDL_TYP_COMMAND:
						if (commandFound)
						{
							printf("Cannot execute more than one command.\n");
							return IRC_FAILURE;
						}
						commandFound = true;
						p_option = &cmd->command;
						break;

					case CMDL_TYP_OPTION:
						p_option = &cmd->options[cmd->numOfOpts++];
						break;
				}

            if (cmdlOptDesc[i].numOfArgs > CMDL_MAX_NUMOF_ARGS)
            {
               printf("Number of arguments (%d) exceeds maximum number of arguments (%d)\n", cmdlOptDesc[i].numOfArgs, CMDL_MAX_NUMOF_ARGS);
               return IRC_FAILURE;
            }

            p_option->code = cmdlOptDesc[i].code;
				p_option->numOfArgs = cmdlOptDesc[i].numOfArgs;
            for (j = 0; j < cmdlOptDesc[i].numOfArgs; j++)
				{
					p_option->args[j] = argv[argi + 1 + j];
				}

				argi += (1 + cmdlOptDesc[i].numOfArgs);
				break;
			}
		}

		if (!optionIsValid)
		{
			printf("Wrong command code.\n");
			return IRC_FAILURE;
		}
	}

	if (!commandFound)
	{
		printf("Command is missing.\n");
	}

	return IRC_SUCCESS;
}
