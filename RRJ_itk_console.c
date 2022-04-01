#include <RRJ_itk_console.h>

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

extern int ITK_user_main(int argc, char* argv[])
{
	char
        *message,
        *users_id = ITK_ask_cli_argument("-u="),
        *users_password = ITK_ask_cli_argument("-p="),
        *users_group = ITK_ask_cli_argument("-g=");

	printf("\nInitializing RRJ_itk_console.");

	ERROR_CHECK(ITK_initialize_text_services(0));
	ERROR_CHECK(ITK_init_module(users_id, users_password, users_group));
	ERROR_CHECK(ITK_set_journalling(TRUE));

	do_it();

	ERROR_CHECK(ITK_exit_module(TRUE));

	return ITK_ok;
}