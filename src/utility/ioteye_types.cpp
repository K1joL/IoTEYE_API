/*# MIT License

# Copyright (c) 2025 Shults Bogdan aka K1joL

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/

#include "ioteye_types.hpp"


namespace ioteye {
CommandMap commandMap;

HttpCode& HttpCode::operator=(int code) {
    m_code = code;
    return *this;
}

HttpCode::operator int() const {
    return m_code;
}

// Comparison operators
bool HttpCode::operator==(const HttpCode& other) const {
    return m_code == other.m_code;
}

bool HttpCode::operator!=(const HttpCode& other) const {
    return m_code != other.m_code;
}

bool HttpCode::isSuccess() const {
    return m_code >= 200 && m_code < 300;
}

bool HttpCode::isValid() {
    return m_code >= 100 && m_code < 600;
}

// Define the static constants
const HttpCode HttpCode::NULL_CODE = HttpCode(0);
const HttpCode HttpCode::OK = HttpCode(200);
const HttpCode HttpCode::CREATED = HttpCode(201);
const HttpCode HttpCode::BAD_REQUEST = HttpCode(400);
const HttpCode HttpCode::UNAUTHORIZED = HttpCode(401);
const HttpCode HttpCode::NOT_FOUND = HttpCode(404);
const HttpCode HttpCode::INTERNAL_SERVER_ERROR = HttpCode(500);

uint8_t getNumCommands() {
    static uint8_t numCommands = 0;
    if (numCommands == 0 || commandMap.isEdited())
        for (uint8_t cmdIndex = 0; cmdIndex < MAX_COMMANDS; ++cmdIndex) {
            if (commandMap[cmdIndex].code == 0)
                return cmdIndex;
        }
    return numCommands;
}

uint8_t commandToCode(const char* commandStr) {
    if (strlen(commandStr) != 2)
        return NON_COMMAND;
    for (int i = 0; i < getNumCommands(); i++) {
        if (compareCommands(commandStr, commandMap[i].cmd)) {
            return commandMap[i].code;
        }
    }
    return NON_COMMAND;
}

const char* commandToString(uint8_t commandCode) {
    for (int i = 0; i < getNumCommands(); i++) {
        if (commandCode == commandMap[i].code) {
            return commandMap[i].cmd;
        }
    }
    return "";
}

uint8_t getCommandCode(const char* cmd) {
    if (cmd == nullptr)
        return NON_COMMAND;
    if (strlen(cmd) == 2)
        return cmd[0] + cmd[1];
    else
        return NON_COMMAND;
}

bool isCommandValid(const char* cmd) {
    uint8_t cmdCode = getCommandCode(cmd);
    if (cmdCode == NON_COMMAND)
        return false;
    for (uint8_t cmdIndex = 0; cmdIndex < MAX_COMMANDS; ++cmdIndex) {
        if (compareCommands(commandMap[cmdIndex].cmd, cmd) ||
            cmdCode == commandMap[cmdIndex].code)
            return false;
    }
    return true;
}

bool compareCommands(const char* cmd1, const char* cmd2) {
    return (cmd1[0] == cmd2[0]) && (cmd1[1] == cmd2[1]);
}

void CommandMap::switchEdited() {
    m_isEdited = !m_isEdited;
}

bool CommandMap::isEdited() {
    return m_isEdited;
}

CommandMap::Command& CommandMap::operator[](uint8_t index) {
    if (index < MAX_COMMANDS)
        return m_map[index];
    else
        return m_map[MAX_COMMANDS - 1];
}

uint8_t CommandMap::registerCommand(const char* commandStr) {
    uint8_t numCommands = getNumCommands();

    if (!isCommandValid(commandStr))
        return NON_COMMAND;

    if (numCommands < MAX_COMMANDS) {
        strcpy(commandMap[numCommands].cmd, commandStr);
        commandMap[numCommands].cmd[MAX_COMMAND_LENGTH - 1] = '\0';
        commandMap[numCommands].code = getCommandCode(commandStr);
        return commandMap[numCommands++].code;
    }
    return NON_COMMAND;
}
}  // namespace ioteye
