#include <iostream>

#include <tcinit/tcinit.h>
#include <tc/emh.h>
#include <mld/journal/journal.h>

const int WRONG_USAGE = 100001;

static inline void output_useage()
{
	std::cout << "\nUSEAGE: utility -u=user -p=password -g=group" << std::endl;

	return;
}

static inline void output_filename()
{
	char* journal_filename;
	char* system_log_filename;
	
	JOURNAL_ask_file_name(&journal_filename);
	EMH_ask_system_log_filename(&system_log_filename);
	std::cout << "journal file name: " << journal_filename << std::endl;
	std::cout << "system log: " << system_log_filename << std::endl;

	MEM_free(journal_filename);
	MEM_free(system_log_filename);
}

int ITK_user_main(int argc, char** argv)
{
	output_filename();

	char* usr = ITK_ask_cli_argument("-u=");
	char* upw = ITK_ask_cli_argument("-p=");
	char* ugp = ITK_ask_cli_argument("-g=");
	char* help = ITK_ask_cli_argument("-h=");

	if (!usr || !upw || !ugp || help)
	{
		output_useage();
		return WRONG_USAGE;
	}

	std::cout << "usr = " << usr << std::endl;
	std::cout << "upw = " << upw << std::endl;
	std::cout << "ugp = " << ugp << std::endl;

	ITK_initialize_text_services(0);
	int status = ITK_init_module(usr, upw, ugp);

	if (status != ITK_ok)
	{
		char* error;
		EMH_ask_error_text(status, &error);
		std::cout << "Error with ITK_init_module: " << error << std::endl;
		MEM_free(error);
		return status;
	}
	else
		std::cout << "Login to Teamcenter success as " << usr << std::endl;

	// Need env: TC_JOURNAL=FULL
	JOURNAL_comment("Preparing to list tool formats\n");
	TC_write_syslog("Preparing to list tool formats\n");
	/* Call your functions between here */

	ITK_exit_module(TRUE);

	std::cout << "Press Enter to continue...";
	std::cin.get();

	return status;
}
