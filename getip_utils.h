// Переменные для хранения информации об использовании аргументов терминала
extern _Bool b_external_ip;

// Переменные для хранения данных из аргументов терминала
extern char* external_ip;

void print_help (int); // Вывод Help сообщения
void parameter_handler (int, char**); // Обработчик параметров терминала
_Bool check_toggle (); // Проверка на наличие включенных параметров
void exec_requests (); // Запрос и вывод информации об IP



