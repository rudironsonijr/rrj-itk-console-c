#include <stdlib.h>
#include <stdio.h>


#include <base_utils/Mem.h>
#include <fclasses/tc_string.h>
#include <pom/enq/enq.h>
#include <pom/pom/pom.h>
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
		printf("\n==================================================");
		TC_write_syslog("\n==================================================");
		printf("\n ERROR: %d ERROR MSG: %s.", return_code, error_msg_string);
        TC_write_syslog("\nERROR: %d ERROR MSG: %s.", return_code, error_msg_string);
		printf ("\nFUNCTION: %s\nFILE: %s LINE: %d", function, file, line);
        TC_write_syslog("\nFUNCTION: %s\nFILE: %s LINE: %d", function, file, line);
		printf("\n==================================================");
		TC_write_syslog("\n==================================================");
		if(error_msg_string) MEM_free(error_msg_string);
	}
}


char* schedule_key_prop;
char* schedule_key_value;
char* schedule_new_name;

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