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

#ifndef IOTEYE_PAYLOAD_HPP
#define IOTEYE_PAYLOAD_HPP

#include <Arduino.h>
#include <WString.h>

namespace ioteye {

class PairString {
public:
    PairString();
    PairString(const char* key, const char* value);
    PairString(const char* key, const String& value);
    PairString(const String& value, const char* key);
    PairString(const PairString& pairString);

public:
    String key;
    String value;
};

class Payload {
public:
    Payload();
    Payload(const ioteye::PairString& pair);
    void Add(const ioteye::PairString& pair);
    ~Payload();
    void clear();
    bool isEmpty() const;
    String GetJsonString() const;
    String GetArgsString() const;
    /*
     *   Returns:
     *   `if size > 0`:
     *       `if (0 <= index < size)`: elements value by index `else`: first elements value
     *   `else`: first elements value
     * */
    PairString& operator[](unsigned int index);
    const PairString& operator[](unsigned int index) const;

private:
    ioteye::PairString* m_pairs = nullptr;
    unsigned int m_size = 0;
    unsigned int m_capacity = 8;
    unsigned int m_overallLength = 0;
};

}  // namespace ioteye

#endif  //! IOTEYE_PAYLOAD_HPP