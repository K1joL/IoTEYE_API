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
    String body = String();
    HttpCode statusCode = HttpCode::NULL_CODE;
};

enum class HttpMethod { GET, POST, PUT, DELETE };
}  // namespace ioteye

#endif  // IOTEYE_TYPES_HPP