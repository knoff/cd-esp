#pragma once
#include "../CD_Defs.h"
#include <vector>
#include <functional>

// Тип обработчика: получает строку аргументов (все, что после команды)
using CmdHandler = std::function<void(String args)>;

struct ConsoleCommand {
    String name;
    CmdHandler handler;
    String help;
};

class Console {
public:
    static void begin();
    static void update();  // Не нужен в loop, но пока оставим для совместимости API

    // API для модулей (аналог @click.command)
    static void registerCommand(String name, CmdHandler handler, String help);

private:
    static void processCommand(String cmd);
    static void printHelp(String args);

    // Обработчики команд
    static void cmd_nvs(String args);
    static void cmd_reboot(String args);

    static std::vector<ConsoleCommand> _commands;
};
