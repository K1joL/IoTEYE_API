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
