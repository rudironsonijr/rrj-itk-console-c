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