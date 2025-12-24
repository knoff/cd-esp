#include "Console.h"
#include "../hal/SystemIO.h"
#include "../core/NVSConfig.h"

std::vector<ConsoleCommand> Console::_commands;

void Console::begin() {
    LOG_INF("CD-ADB Shell Ready. Type 'help'.");

    SystemIO::onTextCommand([](String cmd) {
        // 1. Сбрасываем флаг перед запуском команды
        SystemIO::resetOutputFlag();
        // 2. Выполняем команду
        Console::processCommand(cmd);
        // 3. Проверяем: если команда промолчала, нужно восстановить промпт вручную
        if (!SystemIO::wasOutputProduced()) {
            SystemIO::showPrompt();
        }
    });

    // Регистрация системных команд (Bootstrap)
    registerCommand("help", printHelp, "Show help");
    registerCommand("reboot", cmd_reboot, "System restart");
    registerCommand("nvs", cmd_nvs, "Config tool: nvs <info|set_type|set_id>");

    SystemIO::showPrompt();
}

void Console::registerCommand(String name, CmdHandler handler, String help) {
    _commands.push_back({name, handler, help});
}

void Console::processCommand(String input) {
    if (input.length() == 0) return;

    // Разделяем "cmd" и "args"
    int spaceIdx = input.indexOf(' ');
    String cmdName = (spaceIdx == -1) ? input : input.substring(0, spaceIdx);
    String args = (spaceIdx == -1) ? "" : input.substring(spaceIdx + 1);

    cmdName.toLowerCase();

    // Поиск в реестре
    bool found = false;
    for (const auto& cmd : _commands) {
        if (cmd.name == cmdName) {
            cmd.handler(args);
            found = true;
            break;
        }
    }

    if (!found) {
        SystemIO::println("Unknown command: '%s'. Type 'help'.", cmdName.c_str());
    }
}

void Console::printHelp(String args) {
    SystemIO::println("--- Available Commands ---");
    // Выравнивание для красоты
    for (const auto& cmd : _commands) {
        // Простой форматированный вывод
        // Можно заморочиться с паддингом, но пока так
        char buf[64];
        snprintf(buf, sizeof(buf), "  %-10s : %s", cmd.name.c_str(), cmd.help.c_str());
        SystemIO::println(buf);
    }
    SystemIO::println("--------------------------");
}

void Console::cmd_reboot(String args) {
    LOG_WRN("Rebooting...");
    delay(500);
    ESP.restart();
}

void Console::cmd_nvs(String args) {
    // Логика NVS без изменений, только аргументы приходят готовые
    if (args == "info") {
        SystemIO::println("Type: 0x%02X, ID: 0x%02X, Rev: %d", NVSConfig::getDeviceType(), NVSConfig::getLogicalID(),
                          NVSConfig::getHardwareRevision());
    } else if (args.startsWith("set_type ")) {
        int val = args.substring(9).toInt();
        NVSConfig::setDeviceType((hu_device_type_t)val);
        LOG_INF("Type set to %d. Rebooting...", val);
        delay(1000);
        ESP.restart();
    } else if (args.startsWith("set_id ")) {
        int val = args.substring(7).toInt();
        NVSConfig::setLogicalID(val);
        LOG_INF("ID set to %d.", val);
    } else {
        SystemIO::println("Usage: nvs <info|set_type|set_id>");
    }
}

void Console::update() {}  // Заглушка
