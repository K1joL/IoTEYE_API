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