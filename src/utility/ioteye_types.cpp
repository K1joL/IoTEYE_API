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

}  // namespace ioteye
