/* Contains Target IP/Domain */
extern char *external_ip;

/* Contains API Bit Flags */
extern unsigned int api_bitset_word;

void print_help (int); /* Print Help Message */
void parameter_handler (int, char **); /* Terminal Parameters Handler */
char check_toggle (void); /* Check If Any API Terminal Parameter Is Enabled */
void exec_requests (void); /* HTTP Request To API And Print The Result */

