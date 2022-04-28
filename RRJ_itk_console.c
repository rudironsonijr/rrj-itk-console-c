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
	char* user_role = ITK_ask_cli_argument("-r=");
	schedule_key_prop = ITK_ask_cli_argument("-schKeyProp=");
	schedule_key_value = ITK_ask_cli_argument("-schKeyValue=");
	schedule_new_name = ITK_ask_cli_argument("-schNewName=");

	printf("\n User ID:                  [%s]", user_id);
	printf("\n User Group:               [%s]", user_group);
	printf("\n User Role:                [%s]", user_role);
	printf("\n Schedule Key Property:    [%s]", schedule_key_prop);
	printf("\n Schedule Key Value:       [%s]", schedule_key_value);
	printf("\n Schedule New Name:        [%s]", schedule_new_name);

	printf("\nInitializing RRJ_itk_console.");

	ERROR_CHECK(ITK_initialize_text_services(0));
	ERROR_CHECK(ITK_init_module(user_id, user_password, user_group));
	ERROR_CHECK(ITK_set_journalling(TRUE));


	do_it();

	ERROR_CHECK(ITK_exit_module(TRUE));
	printf("\n");
	return ITK_ok;
}


extern void do_it(
	void
	)
{
	// ERROR_CHECK(ITK_set_bypass(true));

	// logical bypass = false;
	// ERROR_CHECK(ITK_ask_bypass(&bypass));
	// printf("\nBypass status: %s", bypass ? "true" : "false");

	// ERROR_CHECK(POM_set_env_info( POM_bypass_access_check, TRUE, 0, 0.0, NULLTAG, "" ));

	tag_t* schedules = NULL;
	int num_schedules = 0;

	getSchedules(schedule_key_prop, schedule_key_value, &num_schedules, &schedules);

	printf("\nSchedules: %i", num_schedules);

	for (int i = 0; i < num_schedules; i++)
	{
		char* p7mSCHcodigo = NULL;
		char* schObjectName = NULL;

		ERROR_CHECK(AOM_ask_value_string(schedules[i], "p7mSCHcodigo", &p7mSCHcodigo));
		ERROR_CHECK(AOM_ask_value_string(schedules[i], "object_name", &schObjectName));
		printf("\n Schedule [%i], ID: [%s], Name: [%s]", i, p7mSCHcodigo, schObjectName);


		const char* POM_user_name = "infodba";
		const char* POM_password = "infodba";
		const char* POM_group_name = "dba";
		char* dummy_name = NULL;
		tag_t user_tag = NULLTAG;
		tag_t topmost_class_id = NULLTAG;
		int pom_version = 0;

		ERROR_CHECK(POM_init_module());
		ERROR_CHECK(POM_login(POM_user_name, POM_password, POM_group_name));
        ERROR_CHECK(POM_get_user(&dummy_name, &user_tag));
        // ERROR_CHECK(POM_start(POM_user_name, POM_password, POM_group_name, &user_tag, &topmost_class_id, &pom_version));
		printf("\n pom_version: [%d]", pom_version);

		ERROR_CHECK(ITK_set_bypass(true));
		ERROR_CHECK(AOM_lock(schedules[i]));
		ERROR_CHECK(AOM_set_value_string(schedules[i], "object_name", schedule_new_name));
		ERROR_CHECK(AOM_save(schedules[i]));
		ERROR_CHECK(AOM_unlock(schedules[i]));
		ERROR_CHECK(AOM_refresh(schedules[i], 0));
		ERROR_CHECK(ITK_set_bypass(false));

		ERROR_CHECK(AOM_ask_value_string(schedules[i], "object_name", &schObjectName));
		printf("\n Schedule, updated object_name: [%s]", schObjectName);
	}
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

	tag_t* instances = NULL;
	instances = MEM_alloc( n_rows * sizeof(tag_t*) );

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