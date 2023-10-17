#include "FirmwareUpdater.h"
#include "CmdLineParser.h"
#include "BuildInfo.h"
#include "TSIRToolbox.h"
#include <sstream>
#include <string>
#include <cstring>

enum {
   FU_CMD_PROGRAM_PROM,
   FU_CMD_READ_PROM,
   FU_CMD_VERIFY_PROM,
   FU_CMD_CHECK_PROM,
   FU_CMD_HELP,
   FU_OPT_VERBOSE,
   FU_OPT_MAX_PACKET_SIZE,
   FU_OPT_NUMOF
};

IRC_Status_t FirmwareUpdaterProgramPROM(cmdlCommand_t *cmd);
IRC_Status_t FirmwareUpdaterReadPROM(cmdlCommand_t *cmd);
IRC_Status_t FirmwareUpdaterVerifyPROM(cmdlCommand_t *cmd);
IRC_Status_t FirmwareUpdaterCheckPROM(cmdlCommand_t *cmd);
IRC_Status_t FirmwareUpdaterHelp(cmdlCommand_t *cmd);
IRC_Status_t FirmwareUpdaterValidateDeviceID(char *buffer, FirmwareUpdater::fuDevideID_t *id);

cmdlOptDesc_t fuOptDesc[FU_OPT_NUMOF] =
{
   {FU_CMD_PROGRAM_PROM,      CMDL_TYP_COMMAND, 'p',  2, FirmwareUpdaterProgramPROM},
   {FU_CMD_READ_PROM,         CMDL_TYP_COMMAND, 'r',  4, FirmwareUpdaterReadPROM},
   {FU_CMD_VERIFY_PROM,       CMDL_TYP_COMMAND, 'y',  2, FirmwareUpdaterVerifyPROM},
   {FU_CMD_CHECK_PROM,        CMDL_TYP_COMMAND, 'c',  3, FirmwareUpdaterCheckPROM},
   {FU_CMD_HELP,              CMDL_TYP_COMMAND, 'h',  0, FirmwareUpdaterHelp},
   {FU_OPT_VERBOSE,           CMDL_TYP_OPTION,  'v',  0, nullptr},
   {FU_OPT_MAX_PACKET_SIZE,   CMDL_TYP_OPTION,  'm',  1, nullptr}
};

char helpString[] = 
	"TS-IR Firmware Updater (TSIR Toolbox v%d.%d.%d.%d%s)\n"
   "\n"
	"tsirfu [command] [command_args] [options]\n"
   "\n"
   "Commands:\n"
	"  -p dev filename                  Programs PROM file into device PROM.\n"
   "\n"
   "   -r dev addr byteCount filename   Reads byteCount bytes in device PROM\n"
   "                                    at specified address and writes data\n"
   "                                    to filename on disk.\n"
   "\n"
   "   -y dev filename                  Verifies device PROM using specified\n"
   "                                    PROM file.\n"
   "\n"
   "   -c dev addr byteCount            Computes CRC-16 value of byteCount bytes\n"
   "                                    in device PROM at specified address.\n"
   "\n"
   "   -h                               Displays help.\n"
   "\n"
   "Devices:\n"
   "   p        Processing FPGA\n"
   "   o        Output FPGA\n"
   "   s        Storage FPGA\n"
   "\n"
   "Options:\n"
   "   -m       Specifies maximum packet size for read and\n"
   "            write commands. Ignored otherwise.\n"
   "\n"
   "   -v       Enables verbose mode.\n";


int main(int argc, char *argv[])
{
   cmdlCommand_t cmd;

   if (ParseOptions(argc, argv, fuOptDesc, FU_OPT_NUMOF, &cmd) != IRC_SUCCESS)
   {
      return 1;
   }

   if (cmd.command.code == CMDL_CMD_CODE_NONE)
   {
      cmd.command.code = FU_CMD_HELP;
   }

   switch (cmd.command.code)
   {
      case FU_CMD_PROGRAM_PROM:
      case FU_CMD_READ_PROM:
      case FU_CMD_VERIFY_PROM:
      case FU_CMD_CHECK_PROM:
      case FU_CMD_HELP:
         if (fuOptDesc[cmd.command.code].func(&cmd) != IRC_SUCCESS)
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

IRC_Status_t FirmwareUpdaterProgramPROM(cmdlCommand_t *cmd)
{
   FirmwareUpdater fu;
   unsigned int i;
   char *dev = cmd->command.args[0];
   char *filename = cmd->command.args[1];
   unsigned int maxPacketSize = FU_DEFAULT_WRITE_MAX_PACKET_SIZE;
   FirmwareUpdater::fuDevideID_t id;
   bool error = false;

   // Validate command code
   if (cmd->command.code != FU_CMD_PROGRAM_PROM)
   {
      return IRC_FAILURE;
   }

   // Check options
   for (i = 0; i < cmd->numOfOpts; i++)
   {
      switch (cmd->options[i].code)
      {
      case FU_OPT_VERBOSE:
         fu.SetVerbose(true);
         break;

      case FU_OPT_MAX_PACKET_SIZE:
         // TODO Check for conversion error.
         std::stringstream(cmd->options[i].args[0]) >> maxPacketSize;
         break;

      default:
         printf("-%c is no a supported option for this command.\n", fuOptDesc[cmd->options[i].code].c); 
         break;
      }
   }

   // Validate device ID
   if (FirmwareUpdaterValidateDeviceID(dev, &id) != IRC_SUCCESS)
   {
      return IRC_FAILURE;
   }

   // Verify coherence of FPGA device with MCS file
   if ((id == FirmwareUpdater::FUDID_PROCESSING_FPGA && !strstr(filename, "proc")) ||
	   (id == FirmwareUpdater::FUDID_OUTPUT_FPGA && !strstr(filename, "output")) ||
	   (id == FirmwareUpdater::FUDID_STORAGE_FPGA && !strstr(filename, "storage")))
   {
		fprintf(stderr, "MCS file does not match destination FPGA. Programming the wrong FPGA could damage a board!\n");
		return IRC_FAILURE;
   }

   if (fu.ProgramPROM(id, filename, false, maxPacketSize) != IRC_SUCCESS)
   {
      error = true;
   }

   fu.PrintReport();
   fu.Close();

   if (error)
   {
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdaterReadPROM(cmdlCommand_t *cmd)
{
   FirmwareUpdater fu;
   unsigned int i;
   unsigned int maxPacketSize = FU_DEFAULT_READ_MAX_PACKET_SIZE;
   FirmwareUpdater::fuDevideID_t id;
   unsigned int address;
   unsigned int byteCount;
   bool error = false;

   // Validate command code
   if (cmd->command.code != FU_CMD_READ_PROM)
   {
      return IRC_FAILURE;
   }

   // Check options
   for (i = 0; i < cmd->numOfOpts; i++)
   {
      switch (cmd->options[i].code)
      {
      case FU_OPT_VERBOSE:
         fu.SetVerbose(true);
         break;

      case FU_OPT_MAX_PACKET_SIZE:
         // TODO Check for conversion error.
         std::stringstream(cmd->options[i].args[0]) >> maxPacketSize;
         break;

      default:
         printf("-%c is no a supported option for this command.\n", fuOptDesc[cmd->options[i].code].c); 
         break;
      }
   }

   // Validate device ID
   if (FirmwareUpdaterValidateDeviceID(cmd->command.args[0], &id) != IRC_SUCCESS)
   {
      return IRC_FAILURE;
   }

   std::stringstream(cmd->command.args[1]) >> address;
   std::stringstream(cmd->command.args[2]) >> byteCount;

   if (fu.ReadPROM(id, address, byteCount, cmd->command.args[3], maxPacketSize) != IRC_SUCCESS)
   {
      error = true;
   }

   fu.PrintReport();
   fu.Close();

   if (error)
   {
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdaterVerifyPROM(cmdlCommand_t *cmd)
{
   FirmwareUpdater fu;
   unsigned int i;
   FirmwareUpdater::fuDevideID_t id;
   bool error = false;

   // Validate command code
   if (cmd->command.code != FU_CMD_VERIFY_PROM)
   {
      return IRC_FAILURE;
   }

   // Check options
   for (i = 0; i < cmd->numOfOpts; i++)
   {
      switch (cmd->options[i].code)
      {
      case FU_OPT_VERBOSE:
         fu.SetVerbose(true);
         break;

      default:
         printf("-%c is no a supported option for this command.\n", fuOptDesc[cmd->options[i].code].c); 
         break;
      }
   }

   // Validate device ID
   if (FirmwareUpdaterValidateDeviceID(cmd->command.args[0], &id) != IRC_SUCCESS)
   {
      return IRC_FAILURE;
   }

   if (fu.ProgramPROM(id, cmd->command.args[1], true) != IRC_SUCCESS)
   {
      error = true;
   }

   fu.PrintReport();
   fu.Close();

   if (error)
   {
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdaterCheckPROM(cmdlCommand_t *cmd)
{
   FirmwareUpdater fu;
   unsigned int i;
   FirmwareUpdater::fuDevideID_t id;
   unsigned int address;
   unsigned int byteCount;
   bool error = false;

   // Validate command code
   if (cmd->command.code != FU_CMD_CHECK_PROM)
   {
      return IRC_FAILURE;
   }

   // Check options
   for (i = 0; i < cmd->numOfOpts; i++)
   {
      switch (cmd->options[i].code)
      {
      case FU_OPT_VERBOSE:
         fu.SetVerbose(true);
         break;

      default:
         printf("-%c is no a supported option for this command.\n", fuOptDesc[cmd->options[i].code].c); 
         break;
      }
   }

   // Validate device ID
   if (FirmwareUpdaterValidateDeviceID(cmd->command.args[0], &id) != IRC_SUCCESS)
   {
      return IRC_FAILURE;
   }

   std::stringstream(cmd->command.args[1]) >> address;
   std::stringstream(cmd->command.args[2]) >> byteCount;

   if (fu.CheckPROM(id, address, byteCount) != IRC_SUCCESS)
   {
      error = true;
   }

   fu.PrintReport();
   fu.Close();

   if (error)
   {
      return IRC_FAILURE;
   }

   return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdaterHelp(cmdlCommand_t *cmd)
{
   // Validate command code
   if (cmd->command.code != FU_CMD_HELP)
   {
      return IRC_FAILURE;
   }

   printf(helpString, TSIR_TOOLBOX_MAJOR_VERSION, TSIR_TOOLBOX_MINOR_VERSION, TSIR_TOOLBOX_SUBMINOR_VERSION, SVN_SOFTWARE_REV, SVN_SOFTWARE_MODIFIED);

   return IRC_SUCCESS;
}

IRC_Status_t FirmwareUpdaterValidateDeviceID(char *buffer, FirmwareUpdater::fuDevideID_t *id)
{
   if (strlen(buffer) != 1)
   {
      printf("Wrong device ID length.\n"); 
      return IRC_FAILURE;
   }

   switch(buffer[0])
   {
   case 'p':
      *id = FirmwareUpdater::FUDID_PROCESSING_FPGA;
      break;

   case 'o':
      *id = FirmwareUpdater::FUDID_OUTPUT_FPGA;
      break;

   case 's':
      *id = FirmwareUpdater::FUDID_STORAGE_FPGA;
      break;

   default:
      printf("%s is no a supported device ID.\n", buffer); 
      return IRC_FAILURE;
      break;
   }

   return IRC_SUCCESS;
}
