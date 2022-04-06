#include <RRJ_itk_console.h>

extern int ITK_user_main(
	int argc,
	char* argv[]
	)
{
	char* message;
	char* user_id = ITK_ask_cli_argument("-u=");
    char* user_password = ITK_ask_cli_argument("-p=");
    char* user_group = ITK_ask_cli_argument("-g=");
	char* schedule_key_prop = ITK_ask_cli_argument("-schKeyProp=");

	printf("\nInitializing RRJ_itk_console.");

	ERROR_CHECK(ITK_initialize_text_services(0));
	ERROR_CHECK(ITK_init_module(user_id, user_password, user_group));
	ERROR_CHECK(ITK_set_journalling(TRUE));

	do_it();

	ERROR_CHECK(ITK_exit_module(TRUE));

	return ITK_ok;
}

extern void do_it(
	void
	)
{
	ERROR_CHECK(ITK_set_bypass(true));

	logical bypass = false;
	ERROR_CHECK(ITK_ask_bypass(&bypass));
	printf("\nBypass status: %s", bypass ? "true" : "false");

	tag_t* schedules = NULL;
	int num_schedules = 0;

	getSchedules("p7mSCHcodigo", "*", &num_schedules, &schedules);

	printf("\nSchedules: %i", num_schedules);

	for (int i = 0; i < num_schedules; i++)
	{
		char* p7mSCHcodigo = NULL;
		char* schObjectName = NULL;

		ERROR_CHECK(AOM_ask_value_string(schedules[i], "p7mSCHcodigo", &p7mSCHcodigo));
		ERROR_CHECK(AOM_ask_value_string(schedules[i], "object_name", &schObjectName));
		printf("\n Schedule [%i], ID: [%s], Name: [%s]", i, p7mSCHcodigo, schObjectName);

	}

	// ifail = AOM_lock(schedules[schIndex]);
	// if (ifail != ITK_ok)
	// 	printf("\t\n =====> Erro: AOM_lock");

	// ifail = AOM_set_value_string(schedules[schIndex], "object_name", prodRequestRevision_p7mPROnomeProduto);
	// if (ifail != ITK_ok)
	// 	printf("\t\n =====> Erro: AOM_set_value_string");
}

int getSchedules(
	char* key_attribute,
	char* key_value,
	int* num_schedules,
	tag_t** schedules
	)
{
	int ifail = ITK_ok;
	// TC_write_syslog("getSchedules");
	printf("\n getSchedules");

	const char* objectType = "Schedule";
	const char* value = NULL;
	const char* enquiry = "Provedor de Schedules";
	const char* selectedAttrs[] = { "puid" };

	logical queryExists = false;

	int n_rows = 0;
	int n_cols = 0;

	tag_t tagObj;
	void*** reports;

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

	// *schedules = 0;
	// *schedules = MEM_alloc((n_rows) * sizeof(tag_t*));

	tag_t*  instances = NULL;
	instances = (tag_t *)MEM_alloc( n_rows * sizeof(tag_t) );

	*num_schedules = n_rows;
	for (int i = 0; i < n_rows; i++)
	{
        for (int i = 0; i < n_rows; i++)
        {
			instances[i] = *(( tag_t* )( reports[i][0] ));
        }
	}
	*schedules = instances;

	// Delete the enquiry
	ERROR_CHECK(POM_enquiry_delete(enquiry));

	return ifail;
}