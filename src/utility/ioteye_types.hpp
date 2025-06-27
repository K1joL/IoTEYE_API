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

#ifndef IOTEYE_TYPES_HPP
#define IOTEYE_TYPES_HPP

#include <WString.h>
#include <stdint.h>
#include <string.h>

// IoTeye
#define MAX_DOUBLE_STR_WIDTH 20
#define MAX_INT_STR_WIDTH 11
#define MAX_TOKEN_LENGTH 256
#define MAX_VALUE_LENGTH MAX_TOKEN_LENGTH
#define MAX_PASSWORD_LENGTH 64
#define MAX_SSID_LENGTH 64

// Endpoints
#define MAX_TEMPLATES 10
#define MAX_PARTS 10
#define MAX_CMD_COUNT 10
#define MAX_PART_LENGTH 16
#define NONE_ENDPOINT "/none"
#define MAX_ENDPOINT_SIZE (MAX_PART_LENGTH * MAX_PARTS) + MAX_TOKEN_LENGTH
#define MAX_URL_LENGTH MAX_ENDPOINT_SIZE + 8 // + http://

// Templates
#define TEMPLATE_CMD "{cmd}"
#define TEMPLATE_PINNUMBER "{pinNumber}"
#define TEMPLATE_TOKEN "{token}"
#define TEMPLATE_DEFVALUE "{defValue}"
#define TEMPLATE_DATATYPE "{dataType}"
#define TEMPLATE_VALUE "{value}"
#define MAX_TEMPLATE_CMD_LENGTH 3
#define MAX_TEMPLATE_PINNUMBER_LENGTH 4
#define MAX_TEMPLATE_TOKEN_LENGTH MAX_TOKEN_LENGTH
#define MAX_TEMPLATE_DEFVALUE_LENGTH MAX_VALUE_LENGTH
#define MAX_TEMPLATE_DATATYPE_LENGTH 10
#define MAX_TEMPLATE_VALUE_LENGTH MAX_VALUE_LENGTH

// Commands
#define CMD_REGISTER_DEVICE_STR "rd"       // registerDevice
#define CMD_DELETE_DEVICE_STR "dd"         // deleteDevice
#define CMD_DEVICE_STATUS_STR "ds"         // getDeviceStatus
#define CMD_DEVICE_STATUS_UPDATE_STR "us"  // updateDeviceStatus
#define CMD_CREATE_PIN_STR "cp"            // createVirtualPin
#define CMD_UPDATE_PIN_STR "up"            // writeVirtualPin
#define CMD_DELETE_PIN_STR "dp"            // deleteVirtualPin
#define CMD_GET_PIN_STR "pv"               // getVirtualPin

#define NON_COMMAND 0x00               // 0
#define CMD_REGISTER_DEVICE 0xD6       // 214 (r)egister (d)evice
#define CMD_DELETE_DEVICE 0xC8         // 200 (d)elete (d)evice
#define CMD_DEVICE_STATUS 0xD7         // 215 get (d)evice (s)tatus
#define CMD_DEVICE_STATUS_UPDATE 0xE8  // 232 (u)pdate device (s)tatus
#define CMD_CREATE_PIN 0xD3            // 211 (c)reate virtual (p)in
#define CMD_UPDATE_PIN 0xE5            // 229 (u)pdate virtual (p)in value
#define CMD_DELETE_PIN 0xD4            // 212 (d)elete virtual (p)in
#define CMD_GET_PIN 0xE6               // 230 get virtual (p)in (v)alue

#define MAX_COMMANDS 16
#define MAX_COMMAND_LENGTH 3

namespace ioteye {

class HttpCode {
public:
    explicit HttpCode(int code) : m_code(code) {
    }
    HttpCode& operator=(int code);
    operator int() const;

    // Comparison operators
    bool operator==(const HttpCode& other) const;
    bool operator!=(const HttpCode& other) const;
    bool isSuccess() const;
    bool isValid();
    // Predefined HTTP status codes
    static const HttpCode NULL_CODE;
    static const HttpCode OK;
    static const HttpCode CREATED;
    static const HttpCode BAD_REQUEST;
    static const HttpCode UNAUTHORIZED;
    static const HttpCode NOT_FOUND;
    static const HttpCode INTERNAL_SERVER_ERROR;

private:
    int m_code;
};

struct Response {
    String body;
    HttpCode statusCode = HttpCode::NULL_CODE;
};

enum class HttpMethod { GET, POST, PUT, DELETE };

class CommandMap {
public:
    struct Command {
        char cmd[MAX_COMMAND_LENGTH];
        uint8_t code;
    };

public:
    CommandMap() = default;
    void switchEdited();
    bool isEdited();
    Command& operator[](uint8_t index);
    // Adds command to commandMap
    // Returns command code if command added or NON_COMMAND otherwise
    uint8_t registerCommand(const char* commandStr);

private:
    Command m_map[MAX_COMMANDS] = {
        {CMD_REGISTER_DEVICE_STR, CMD_REGISTER_DEVICE},
        {CMD_DELETE_DEVICE_STR, CMD_DELETE_DEVICE},
        {CMD_DEVICE_STATUS_STR, CMD_DEVICE_STATUS},
        {CMD_DEVICE_STATUS_UPDATE_STR, CMD_DEVICE_STATUS_UPDATE},
        {CMD_CREATE_PIN_STR, CMD_CREATE_PIN},
        {CMD_UPDATE_PIN_STR, CMD_UPDATE_PIN},
        {CMD_DELETE_PIN_STR, CMD_DELETE_PIN},
        {CMD_GET_PIN_STR, CMD_GET_PIN},
        {0}};
    bool m_isEdited = false;
};

// Returns number of commands in commandMap
uint8_t getNumCommands();
// Returns command code if found or NON_COMMAND otherwise
uint8_t commandToCode(const char* commandStr);
// Returns command string if found or empty string otherwise
const char* commandToString(uint8_t commandCode);
// Compares two commands by chars
// Returns true if commands equal and false otherwise
bool compareCommands(const char* cmd1, const char* cmd2);

namespace dev {
// Adds the fisrt and second chars of a string
// This does not guarantee that the command was found in the commandMap!
// Returns command code or NON_COMMAND if error
uint8_t getCommandCode(const char* cmd);
// Checks for any collisions with commandMap
// Returns true if command found and false otherwise
bool isCommandValid(const char* cmd);
}  // namespace dev

extern CommandMap commandMap;

}  // namespace ioteye

#endif  // IOTEYE_TYPES_HPP