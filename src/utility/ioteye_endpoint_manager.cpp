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

#include "ioteye_endpoint_manager.hpp"

namespace ioteye {

void EndpointManager::getEndpoint(char* endpointDest, uint8_t commandCode,
                                  uint8_t argCount, ...) {
    if (endpointDest == nullptr)
        return;

    EndpointTemplate* foundTempl = nullptr;
    uint8_t partsCount = 0;
    for (uint8_t i = 0; i < m_templatesCount; ++i) {
        for (uint8_t j = 0; j < m_epTable[i].cmdsCount; ++j) {
            if (m_epTable[i].cmds[j] == commandCode) {
                foundTempl = &m_epTable[i];
                break;
            }
        }
        if (partsCount != 0)
            break;
    }
    if (foundTempl->parts == nullptr) {
        strlcpy(endpointDest, NONE_ENDPOINT, MAX_ENDPOINT_SIZE);
        return;
    }
    TemplatesUnion templUnion;
    addToUnion(&templUnion, TemplatesNumber::CMD, commandToString(commandCode));

    va_list args;
    va_start(args, argCount);
    for (uint8_t argI = 1; argI <= argCount; ++argI) {
        const char* arg = va_arg(args, const char*);
        addToUnion(&templUnion, argI, arg);
    }
    va_end(args);

    // build the endpoint
    endpointDest[0] = '\0';
    size_t currentLength = 0;
    for (uint8_t partI = 0; partI < foundTempl->partsCount; ++partI) {
        const char* currentPart = foundTempl->parts[partI];
        const char* epPart = nullptr;
        if (currentPart != nullptr && currentPart[1] == '{') {
            epPart =
                getFromUnion(&templUnion, getTemplateNumber(currentPart + 1));
            if (MAX_ENDPOINT_SIZE > currentLength + 1) {
                strcat(endpointDest, "/");
                ++currentLength;
                endpointDest[currentLength] = '\0';
            } else {
                strlcpy(endpointDest, NONE_ENDPOINT, MAX_ENDPOINT_SIZE);
                break;
            }
        } else
            epPart = currentPart;
        if (epPart != nullptr) {
            size_t partLength = strlen(epPart);
            if (MAX_ENDPOINT_SIZE - currentLength >= partLength) {
                strcat(endpointDest, epPart);
                currentLength += partLength;
                endpointDest[currentLength] = '\0';
            } else {
                strlcpy(endpointDest, NONE_ENDPOINT, MAX_ENDPOINT_SIZE);
                break;
            }
        }
    }
}

void EndpointManager::getRegisterDeviceEndpoint(char* endpointDest) {
    getEndpoint(endpointDest, CMD_REGISTER_DEVICE, 0);
}

void EndpointManager::getDeleteDeviceEndpoint(char* endpointDest,
                                              const char* token) {
    getEndpoint(endpointDest, CMD_DELETE_DEVICE, 1, token);
}

void EndpointManager::getDeviceStatusEndpoint(char* endpointDest,
                                              const char* token) {
    getEndpoint(endpointDest, CMD_DEVICE_STATUS, 1, token);
}

void EndpointManager::getUpdateDeviceStatusEndpoint(char* endpointDest,
                                                    const char* token) {
    getEndpoint(endpointDest, CMD_DEVICE_STATUS_UPDATE, 1, token);
}

void EndpointManager::getDeleteVirtPinEndpoint(char* endpointDest,
                                               const char* token,
                                               const char* pinNumber) {
    getEndpoint(endpointDest, CMD_DELETE_PIN, 2, token, pinNumber);
}

void EndpointManager::getGetVirtPinEndpoint(char* endpointDest,
                                            const char* token,
                                            const char* pinNumber) {
    getEndpoint(endpointDest, CMD_GET_PIN, 2, token, pinNumber);
}

void EndpointManager::getCreateVirtPinEndpoint(char* endpointDest,
                                               const char* token,
                                               const char* pinNumber,
                                               const char* dataType,
                                               const char* defValue) {
    getEndpoint(endpointDest, CMD_CREATE_PIN, 4, token, pinNumber, defValue,
                dataType);
}

void EndpointManager::getUpdateVirtPinEndpoint(char* endpointDest,
                                               const char* token,
                                               const char* pinNumber,
                                               const char* value) {
    getEndpoint(endpointDest, CMD_UPDATE_PIN, 3, token, pinNumber, value);
}

EndpointTemplate* EndpointManager::getEndpointTable() {
    return m_epTable;
}

void EndpointManager::setTemplatesCount(uint8_t count) {
    m_templatesCount = count;
}

uint8_t EndpointManager::getTemplatesCount() {
    return m_templatesCount;
}

uint8_t parseTemplates(const char* jsonString, EndpointTemplate* templates,
                       uint8_t maxTemplates) {
    if (jsonString == nullptr || templates == nullptr)
        return 0;
    const char* currChar = jsonString;
    const char* templateStart = nullptr;
    const char* commandStart = nullptr;
    const char* commandEnd = nullptr;
    const char* prevSeparator = nullptr;
    bool inTemplate = false;
    bool inCommand = false;
    uint8_t templateCount = 0;

    char (*parts)[MAX_PART_LENGTH] = nullptr;
    uint8_t partNumber = 0;
    uint8_t* cmds = nullptr;
    char cmdStr[MAX_COMMAND_LENGTH];
    uint8_t cmdNumber = 0;
    uint8_t cmdIndex = 0;
    bool skipTemplate = false;
    if (*currChar == '{') {
        ++currChar;
    }
    for (; *currChar != '\0'; ++currChar) {
        if (skipTemplate) {
            if (*currChar == ',' && *(currChar - 1) == '"') {
                skipTemplate = false;
                inCommand = false;
                inTemplate = false;
                commandStart = nullptr;
            }
        } else if (*currChar == '"') {
            if (!inTemplate && !inCommand) {
                templateStart = currChar + 1;
                inTemplate = true;
                partNumber = 0;
                prevSeparator = templateStart;
                parts = templates[templateCount].parts;
                cmds = templates[templateCount].cmds;
                partNumber = 0;
                cmdNumber = 0;
            } else if (inTemplate) {
                inTemplate = false;
                inCommand = true;
                if (!copyPart(prevSeparator, currChar, parts, partNumber)) {
                    skipTemplate = true;
                    continue;
                }
            } else if (inCommand) {
                if (commandStart == nullptr)
                    commandStart = currChar + 1;
                else {
                    commandEnd = currChar;
                    inCommand = false;
                    // Out of bounds for cmds
                    if (cmdNumber >= MAX_CMD_COUNT) {
                        skipTemplate = true;
                        continue;
                    }
                    cmdStr[MAX_COMMAND_LENGTH - 1] = '\0';
                    uint8_t cmdCode = commandToCode(cmdStr);
                    if (cmdCode != NON_COMMAND)
                        cmds[cmdNumber++] = cmdCode;
                    cmdIndex = 0;
                }
            }
        } else if (!inCommand && !inTemplate &&
                   (*currChar == ',' || *currChar == '}')) {
            templates[templateCount].partsCount = partNumber;
            templates[templateCount].cmdsCount = cmdNumber;

            templateCount++;
            if (templateCount >= maxTemplates)
                return templateCount;

            commandStart = nullptr;
            partNumber = 0;
        } else if (inTemplate) {
            if (*currChar == '/') {
                if (currChar != templateStart) {
                    if (!copyPart(prevSeparator, currChar, parts, partNumber)) {
                        skipTemplate = true;
                        continue;
                    }
                    prevSeparator = currChar;
                }
            }
        } else if (inCommand && *currChar != ' ' && *currChar != ':') {
            if (*currChar == ',') {
                // Out of bounds for cmds
                if (cmdNumber >= MAX_CMD_COUNT) {
                    skipTemplate = true;
                    continue;
                }
                cmdStr[MAX_COMMAND_LENGTH - 1] = '\0';
                uint8_t cmdCode = commandToCode(cmdStr);
                if (cmdCode != NON_COMMAND)
                    cmds[cmdNumber++] = cmdCode;
                cmdIndex = 0;
            } else {
                // Wrong cmd format
                if (cmdIndex >= 2) {
                    skipTemplate = true;
                    cmdIndex = 0;
                    continue;
                }
                cmdStr[cmdIndex++] = *currChar;
            }
        }
    }

    return templateCount;
}

bool copyPart(const char* start, const char* end,
              char (*parts)[MAX_PART_LENGTH], uint8_t& partNumber) {
    uint8_t partLength = end - start + 1;
    // Out of bounds for parts or wrong part format
    if (partNumber >= MAX_PARTS || partLength >= (MAX_PART_LENGTH)) {
        return false;
    }
    char* partCopy = parts[partNumber++];
    strlcpy(partCopy, start, partLength);
    return true;
}

// TODO:
// Rewrite with using Defines somehow :>
// Of course we can use strcmp buuuut... you know... it would be so slow
// It would be great if we got templates too

bool addToUnion(TemplatesUnion* templUnion, uint8_t templNumber,
                const char* templValue) {
    if (templNumber >= TemplatesNumber::NUMBER_MAX)
        return false;
    switch (templNumber) {
        case TemplatesNumber::CMD:
            strlcpy(templUnion->cmd, templValue, MAX_TEMPLATE_CMD_LENGTH);
            return true;
        case TemplatesNumber::TOKEN:
            strlcpy(templUnion->token, templValue, MAX_TEMPLATE_TOKEN_LENGTH);
            return true;
        case TemplatesNumber::PINNUMBER:
            strlcpy(templUnion->pinNumber, templValue,
                    MAX_TEMPLATE_PINNUMBER_LENGTH);
            return true;
        case TemplatesNumber::VALUE:
            strlcpy(templUnion->value, templValue, MAX_TEMPLATE_VALUE_LENGTH);
            return true;
        case TemplatesNumber::DATATYPE:
            strlcpy(templUnion->dataType, templValue,
                    MAX_TEMPLATE_DATATYPE_LENGTH);
            return true;
        default:
            return false;
    }
    return false;
}

const char* getFromUnion(TemplatesUnion* templUnion, uint8_t templNumber) {
    if (templNumber >= NUMBER_MAX)
        return nullptr;
    switch (templNumber) {
        case TemplatesNumber::CMD:
            return templUnion->cmd;
        case TemplatesNumber::TOKEN:
            return templUnion->token;
        case TemplatesNumber::PINNUMBER:
            return templUnion->pinNumber;
        case TemplatesNumber::DATATYPE:
            return templUnion->dataType;
        case TemplatesNumber::VALUE:
            return templUnion->value;
        default:
            return nullptr;
    }
    return nullptr;
}

uint8_t getTemplateNumber(const char* templStr) {
    if (templStr == nullptr)
        return TemplatesNumber::NUMBER_MAX;
    switch (templStr[1]) {
        case 'c':
            return TemplatesNumber::CMD;
        case 't':
            return TemplatesNumber::TOKEN;
        case 'p':
            return TemplatesNumber::PINNUMBER;
        case 'v':
            return TemplatesNumber::VALUE;
        case 'd':
            return TemplatesNumber::DATATYPE;
        default:
            return TemplatesNumber::NUMBER_MAX;
    }
    return TemplatesNumber::NUMBER_MAX;
}

}  // namespace ioteye
