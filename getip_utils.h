// Переменные для хранения информации об использовании аргументов терминала
extern _Bool only4, bExternalIP;

// Переменные для хранения данных из аргументов терминала
extern char* externalIP;

void printHelp (int); // Вывод Help сообщения
void parameterHandler (int, char**); // Обработчик параметров терминала
void execRequests (); // Запрос и вывод информации об IP

