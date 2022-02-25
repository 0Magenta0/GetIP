/* Переменная для хранения данных из аргументов терминала */
extern char* external_ip;

/* Переменная для хранения битовых флагов для API */
extern unsigned int api_bitset_word;

void print_help (int); /* Вывод Help сообщения */
void parameter_handler (int, char**); /* Обработчик параметров терминала */
char check_toggle (void); /* Проверка на наличие включенных параметров */
void exec_requests (void); /* Запрос и вывод информации об IP */

