// #include <iostream>
// #include <string>
// #include <sstream>
#include <stdlib.h>
#include <stdio.h>

#include <unidefs.h>
#include <base_utils/Mem.h>
#include <fclasses/tc_string.h>
#include <pom/enq/enq.h>
// #include <schmgt/schmgt_itk.h>
// #include <schmgt/schmgt_bridge_itk.h>
#include <tc/emh.h>
#include <tc/tc_errors.h>
#include <tc/tc_startup.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/tctype.h>
#include <tcinit/tcinit.h>

#define ERROR_CHECK(X) (report_error( __FILE__, __LINE__, #X, (X)))
static void report_error(
	char* file,
	int line,
	char* function,
	int return_code
	)
{
    if (return_code != ITK_ok)
    {
		char* error_msg_string;
		EMH_ask_error_text(return_code, &error_msg_string);
		printf("ERROR: %d ERROR MSG: %s.\n", return_code, error_msg_string);
        TC_write_syslog("ERROR: %d ERROR MSG: %s.\n", return_code, error_msg_string);
		printf ("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
        TC_write_syslog("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
		if(error_msg_string) MEM_free(error_msg_string);
	}
}

extern void do_it(
	void
	);

extern int ITK_user_main(
	int argc,
	char* argv[]
	);

int getSchedules(
	char* key_attribute,
	char* key_value,
	int* num_schedules,
	tag_t** schedules
	);