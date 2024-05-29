#include <iostream> // Включение стандартной библиотеки ввода-вывода
#include <winsock2.h> // Включение библиотеки Windows Sockets 2

#pragma comment(lib, "Ws2_32.lib") // Автоматическая линковка с библиотекой Ws2_32.lib

#define DEFAULT_PORT 27015 // Определение порта по умолчанию
#define DEFAULT_IP "127.0.0.1" // Определение IP адреса по умолчанию

int main() {
    WSADATA wsaData; // Структура для хранения информации о реализации Windows Sockets
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Инициализация библиотеки Winsock
    if (result != 0) { // Проверка на успешную инициализацию
        std::cerr << "WSAStartup failed: " << result << std::endl; // Вывод сообщения об ошибке
        return 1; // Завершение программы при ошибке
    }

    SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Создание сокета
    if (connectSocket == INVALID_SOCKET) { // Проверка на успешное создание сокета
        std::cerr << "Socket failed with error: " << WSAGetLastError() << std::endl; // Вывод сообщения об ошибке
        WSACleanup(); // Завершение работы с Winsock
        return 1; // Завершение программы при ошибке
    }

    sockaddr_in serverAddr; // Структура для хранения информации о сервере
    serverAddr.sin_family = AF_INET; // Указание использования IPv4
    serverAddr.sin_port = htons(DEFAULT_PORT); // Указание порта сервера
    inet_pton(AF_INET, DEFAULT_IP, &serverAddr.sin_addr); // Преобразование IP-адреса из текстового в двоичный формат

    // Подключение к серверу
    if (connect(connectSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed with error: " << WSAGetLastError() << std::endl; // Вывод сообщения об ошибке
        closesocket(connectSocket); // Закрытие сокета
        WSACleanup(); // Завершение работы с Winsock
        return 1; // Завершение программы при ошибке
    }

    const char* sendbuf1 = "Hello, Server!"; // Первое сообщение для отправки серверу
    int sendResult = send(connectSocket, sendbuf1, (int)strlen(sendbuf1), 0); // Отправка первого сообщения
    if (sendResult == SOCKET_ERROR) { // Проверка на успешную отправку
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl; // Вывод сообщения об ошибке
        closesocket(connectSocket); // Закрытие сокета
        WSACleanup(); // Завершение работы с Winsock
        return 1; // Завершение программы при ошибке
    }

    const char* sendbuf2 = "Second message!"; // Второе сообщение для отправки серверу
    sendResult = send(connectSocket, sendbuf2, (int)strlen(sendbuf2), 0); // Отправка второго сообщения
    if (sendResult == SOCKET_ERROR) { // Проверка на успешную отправку
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl; // Вывод сообщения об ошибке
        closesocket(connectSocket); // Закрытие сокета
        WSACleanup(); // Завершение работы с Winsock
        return 1; // Завершение программы при ошибке
    }

    // Завершение соединения с сервером
    if (shutdown(connectSocket, SD_SEND) == SOCKET_ERROR) {
        std::cerr << "Shutdown failed with error: " << WSAGetLastError() << std::endl; // Вывод сообщения об ошибке
        closesocket(connectSocket); // Закрытие сокета
        WSACleanup(); // Завершение работы с Winsock
        return 1; // Завершение программы при ошибке
    }

    closesocket(connectSocket); // Закрытие сокета
    WSACleanup(); // Завершение работы с Winsock

    return 0; // Завершение программы с кодом успешного выполнения
}
