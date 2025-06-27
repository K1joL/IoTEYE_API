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

#ifndef IOTEYE_ENDPOINT_MANAGER_HPP
#define IOTEYE_ENDPOINT_MANAGER_HPP

#include <Arduino.h>
#include <stdarg.h>
#include <string.h>

#include "ioteye_types.hpp"

namespace ioteye {

struct TemplatesUnion {
    char cmd[MAX_TEMPLATE_CMD_LENGTH] = {0};
    char token[MAX_TEMPLATE_TOKEN_LENGTH] = {0};
    char pinNumber[MAX_TEMPLATE_PINNUMBER_LENGTH] = {0};  // from 1 to 999
    char value[MAX_TEMPLATE_VALUE_LENGTH] = {0};
    char dataType[MAX_TEMPLATE_DATATYPE_LENGTH] = {0};
    char defValue[MAX_TEMPLATE_DEFVALUE_LENGTH] = {0};
};

struct EndpointTemplate {
    char parts[MAX_PARTS][MAX_PART_LENGTH];
    uint8_t partsCount;
    uint8_t cmds[MAX_CMD_COUNT];
    uint8_t cmdsCount;
};

class EndpointManager {
public:
    EndpointManager() = default;
    EndpointTemplate *getEndpointTable();
    uint8_t getTemplatesCount();
    void setTemplatesCount(uint8_t count);

    void getDeviceStatusEndpoint(char *endpointDest, const char *token);
    void getUpdateDeviceStatusEndpoint(char *endpointDest, const char *token);
    void getCreateVirtPinEndpoint(char *endpointDest, const char *token,
                                  const char *pinNumber, const char *dataType,
                                  const char *defValue);
    void getUpdateVirtPinEndpoint(char *endpointDest, const char *token,
                                  const char *pinNumber, const char *value);
    void getGetVirtPinEndpoint(char *endpointDest, const char *token,
                               const char *pinNumber);
    void getDeleteVirtPinEndpoint(char *endpointDest, const char *token,
                                  const char *pinNumber);
    void getRegisterDeviceEndpoint(char *endpointDest);
    void getDeleteDeviceEndpoint(char *endpointDest, const char *token);

private:
    void getEndpoint(char *endpointDest, uint8_t commandCode, uint8_t argCount,
                     ...);

private:
    EndpointTemplate m_epTable[MAX_TEMPLATES] = {0};
    uint8_t m_templatesCount = 0;
};

enum TemplatesNumber{
    CMD,
    TOKEN,
    PINNUMBER,
    VALUE,
    DATATYPE,
    DEFVALUE,
    NUMBER_MAX
};

// Returns number of templates parsed
uint8_t parseTemplates(const char *jsonString, EndpointTemplate *templates,
                       uint8_t maxTemplates = MAX_TEMPLATES);
bool copyPart(const char *start, const char *end,
              char (*parts)[MAX_PART_LENGTH], uint8_t &partNumber);

// Fills the templUnion with certain templStr
bool addToUnion(TemplatesUnion *templUnion, uint8_t templNumber,
                const char *templValue);

const char *getFromUnion(TemplatesUnion *templUnion, uint8_t templNumber);
uint8_t getTemplateNumber(const char *templStr);
}  // namespace ioteye

#endif  // IOTEYE_ENDPOINT_MANAGER_HPP