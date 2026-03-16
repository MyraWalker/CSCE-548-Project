#include "src/services/HttpServer.h"
#include "src/client/ConsoleUI.h"
#include "src/utils/Logger.h"
#include <thread>
#include <chrono>

int main()
{
    Logger::Info("Application starting");

    HttpServer server;

    std::thread serverThread([&server]()
        {
            server.Start();
        });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    ConsoleUI ui;
    ui.Run();

    Logger::Info("Application shutting down");

    serverThread.detach();
    return 0;
}