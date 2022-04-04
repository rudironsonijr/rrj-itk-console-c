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
static void report_error( char *file, int line, char *function, int return_code)
{
    if (return_code != ITK_ok)
    {
		char *error_msg_string;
		EMH_ask_error_text(return_code, &error_msg_string);
		printf("ERROR: %d ERROR MSG: %s.\n", return_code, error_msg_string);
        TC_write_syslog("ERROR: %d ERROR MSG: %s.\n", return_code, error_msg_string);
		printf ("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
        TC_write_syslog("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
		if(error_msg_string) MEM_free(error_msg_string);
	}
}

extern void do_it(void);

extern int ITK_user_main(int argc, char* argv[]);

int getSchedules(char *key_attribute, char *key_value, int *num_schedules, tag_t *schedules);


extern int ITK_user_main(int argc, char* argv[])
{
	char
        *message,
        *user_id = ITK_ask_cli_argument("-u="),
        *user_password = ITK_ask_cli_argument("-p="),
        *user_group = ITK_ask_cli_argument("-g=");

	printf("\nInitializing RRJ_itk_console.");

	ERROR_CHECK(ITK_initialize_text_services(0));
	ERROR_CHECK(ITK_init_module(user_id, user_password, user_group));

	tag_t schedules = NULLTAG;
	int num_schedules = 0;
	getSchedules("p7mSCHcodigo", "*", &num_schedules, &schedules);

	printf("\nSchedules: %i", num_schedules);

	// for (int i = 0; i < num_schedules; i++)
	// {
	// 	char
	// 		*p7mSCHcodigo = NULL,
	// 		*schObjectName = NULL;

	// 	tag_t theSchedule = schedules[i];
	// 	ERROR_CHECK(AOM_ask_value_string(theSchedule, "p7mSCHcodigo", &p7mSCHcodigo));
	// }


	ERROR_CHECK(ITK_set_journalling(TRUE));

	do_it();

	ERROR_CHECK(ITK_exit_module(TRUE));

	return ITK_ok;
}

extern void do_it(void)
{
	ERROR_CHECK(ITK_set_bypass(true));

	// ifail = AOM_lock(schedules[schIndex]);
	// if (ifail != ITK_ok)
	// 	printf("\t\n =====> Erro: AOM_lock");

	// ifail = AOM_set_value_string(schedules[schIndex], "object_name", prodRequestRevision_p7mPROnomeProduto);
	// if (ifail != ITK_ok)
	// 	printf("\t\n =====> Erro: AOM_set_value_string");

	logical bypass = false;
	ERROR_CHECK(ITK_ask_bypass(&bypass));
	printf("\nBypass status: %s", bypass ? "true" : "false");
}

int getSchedules(char *key_attribute, char *key_value, int *num_schedules, tag_t *schedules)
{
	int ifail = ITK_ok;
	TC_write_syslog("getSchedules");

	const char
		*objectType = "Schedule",
		*value = NULL,
		*enquiry = "Provedor de Schedules",
		*selectedAttrs[] = { "puid" };

	logical queryExists = false;
	int
		n_rows = 0,
		n_cols = 0;

	tag_t tagObj;
	void ***reports;

	ERROR_CHECK(POM_enquiry_does_query_exists(enquiry, &queryExists));
	if (queryExists)
	{
		ERROR_CHECK(POM_enquiry_delete(enquiry));
	}

	// Criação da enquiry (inquisição)
	ERROR_CHECK(POM_enquiry_create(enquiry));
	// Resultados devem ser distintos
	ERROR_CHECK(POM_enquiry_set_distinct(enquiry, true));

	// Setar campos de retorno da enquiry (inquisição)
	ERROR_CHECK(POM_enquiry_add_select_attrs(enquiry, objectType, 1, selectedAttrs));

	// Expressão 01
	ERROR_CHECK(POM_enquiry_set_string_expr(enquiry, "expr1", objectType, key_attribute, POM_enquiry_like, key_value));
	ERROR_CHECK(POM_enquiry_set_expr_proprety(enquiry, "expr1", POM_case_insensitive));

	// Expressão 03, vinculando a expressão 01 e 02
	//ifail = POM_enquiry_set_expr(enquiry, "expr3", "expr1", POM_enquiry_and, "expr2");
	ERROR_CHECK(POM_enquiry_set_where_expr(enquiry, "expr1"));

	// Execute the enquiry
	ERROR_CHECK(POM_enquiry_execute(enquiry, &n_rows, &n_cols, &reports));

	*schedules = 0;
	*num_schedules = n_rows;
	for (int i = 0; i < n_rows; i++)
	{
		// tagObj = *(tag_t*)(reports[i][0]);
		schedules = (tag_t*)MEM_alloc((n_rows) * sizeof(tag_t));
        for (int i = 0; i < n_rows; ++i)
        {
            schedules[i] = *(tag_t*)(reports[i][0]);
        }

		char
			*p7mSCHcodigo = NULL,
			*schObjectName = NULL;

		// tag_t theSchedule = schedules[i];
		ERROR_CHECK(AOM_ask_value_string(schedules[i], "p7mSCHcodigo", &p7mSCHcodigo));
		ERROR_CHECK(AOM_ask_value_string(schedules[i], "object_name", &schObjectName));
		printf("\n Schedule [%i], ID: [%s], Name: [%s]", i, p7mSCHcodigo, schObjectName);
	}

	// Delete the enquiry
	ERROR_CHECK(POM_enquiry_delete(enquiry));

	return ifail;
}