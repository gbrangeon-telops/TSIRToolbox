#ifndef __CMDLINEPARSER_H__
#define __CMDLINEPARSER_H__

#include "IRC_status.h"


#define CMDL_MAX_NUMOF_ARGS		4
#define CMDL_MAX_NUMOF_OPT			2

#define CMDL_CMD_CODE_NONE       0xFF

typedef enum {
	CMDL_TYP_COMMAND,
	CMDL_TYP_OPTION
} cmdlOptType_t;

typedef struct {
	unsigned int code;
	unsigned int numOfArgs;
	char *args[CMDL_MAX_NUMOF_ARGS];
} cmdlOption_t;

typedef struct {
	cmdlOption_t command;
	unsigned int numOfOpts;
	cmdlOption_t options[CMDL_MAX_NUMOF_OPT];
} cmdlCommand_t;

typedef IRC_Status_t (*cmdlCmdFunc_t)(cmdlCommand_t *cmd);

typedef struct {
	unsigned int code;
	cmdlOptType_t type;
	char c;
	unsigned int numOfArgs;
	cmdlCmdFunc_t func;
} cmdlOptDesc_t;

IRC_Status_t ParseOptions(unsigned int argc, char *argv[], const cmdlOptDesc_t cmdlOptDesc[], unsigned int cmdlOptDescSize, cmdlCommand_t *cmd);

#endif // __CMDLINEPARSER_H__