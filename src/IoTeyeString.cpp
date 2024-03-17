#include "IoTeyeString.h"

ioteye::String::String()
{
    m_length = 0;
    m_rawString = new char[m_capacity + 1];
    m_rawString[0] = '\0';
}

ioteye::String::String(const char c, unsigned int length)
    : m_length{length},
      m_capacity{length + 8}
{
    m_rawString = new char[m_capacity + 1];
    for (unsigned int i = 0; i < length; i++)
        m_rawString[i] = c;
    m_rawString[length] = '\0';
}

ioteye::String::String(const char *cString)
{
    if (cString == nullptr)
    {
        m_length = 0;
        m_rawString = new char[m_capacity + 1];
        m_rawString[0] = '\0';
    }
    else
    {
        m_length = ioteye::GetRawStringLength(cString);
        m_capacity = m_length + 8;
        m_rawString = new char[m_capacity + 1];
        ioteye::StringCopy(m_rawString, cString);
    }
}

ioteye::String::String(const String &string)
    : m_length{string.GetLength()},
      m_capacity{string.GetLength() + 8}
{
    m_rawString = new char[m_capacity + 1];
    ioteye::StringCopy(m_rawString, string.GetRawString());
}

ioteye::String::String(String &&other)
    : m_length{0},
      m_capacity{0},
      m_rawString{nullptr}
{
    this->m_length = other.m_length;
    this->m_capacity = other.m_capacity;
    this->m_rawString = other.m_rawString;
    other.m_length = 0;
    other.m_capacity = 0;
    other.m_rawString = nullptr;
}

ioteye::String::~String()
{
    if (m_rawString)
        delete[] m_rawString;
}

ioteye::String &ioteye::String::operator=(const String &string)
{
    if (this == &string || !string.m_length)
        return *this;
    if (string.m_length >= this->m_capacity)
        this->Reallocate(string.m_length);

    ioteye::StringCopy(m_rawString, string.m_rawString);
    this->m_length = string.m_length;
    return *this;
}

ioteye::String &ioteye::String::operator=(String &&other)
{
    if(this == &other)
        return *this;
    delete[] this->m_rawString;
    this->m_length = other.m_length;
    this->m_capacity = other.m_capacity;
    this->m_rawString = other.m_rawString;
    other.m_length = 0;
    other.m_capacity = 0;
    other.m_rawString = nullptr;

    return *this;
}

ioteye::String &ioteye::String::operator=(const char *cString)
{
    if (cString == nullptr)
        return *this;
    unsigned int cStringLength = GetRawStringLength(cString);
    if (cStringLength >= this->m_capacity)
        this->Reallocate(cStringLength);
    ioteye::StringCopy(m_rawString, cString);
    this->m_length = cStringLength;
    return *this;
}

ioteye::String &ioteye::String::operator=(const char c)
{
    if(m_capacity < 1)
        Reallocate(1);
    this->m_length = 1;
    this->m_rawString[0] = c;
    this->m_rawString[1] = '\0';
    return *this;
}

ioteye::String &ioteye::String::operator+=(const String &string)
{
    if (!string.GetLength())
        return *this;
    unsigned int newLength = this->m_length + string.m_length;
    if (newLength >= m_capacity)
        Reallocate(newLength, true);

    ioteye::StringCopy(this->m_rawString + this->m_length, string.GetRawString());
    this->m_length = newLength;

    // String result;
    // result.m_length = this->m_length + string.GetLength();
    // delete[] result.m_rawString;
    // result.m_rawString = new char[result.m_length + 1];

    // ioteye::StringCopy(result.m_rawString, this->m_rawString);
    // ioteye::StringCopy(result.m_rawString + this->m_length, string.GetRawString());

    // delete[] this->m_rawString;
    // this->m_length = result.m_length;
    // this->m_rawString = result.m_rawString;

    // result.m_rawString = nullptr;

    return *this;
}

ioteye::String &ioteye::String::operator+=(const char *cString)
{
    if (!cString)
        return *this;

    unsigned int newLength = this->m_length + ioteye::GetRawStringLength(cString);
    if (newLength >= m_capacity)
        Reallocate(newLength, true);

    ioteye::StringCopy(this->m_rawString + this->m_length, cString);
    this->m_length = newLength;
    // String result;
    // result.m_length = this->m_length + ioteye::GetRawStringLength(cString);
    // delete[] result.m_rawString;
    // result.m_rawString = new char[result.m_length + 1];

    // ioteye::StringCopy(result.m_rawString, this->m_rawString);
    // ioteye::StringCopy(result.m_rawString + this->m_length, cString);

    // delete[] this->m_rawString;
    // this->m_length = result.m_length;
    // this->m_rawString = result.m_rawString;

    // result.m_rawString = nullptr;

    return *this;
}

ioteye::String &ioteye::String::operator+=(const char c)
{
    unsigned int newLength = this->m_length + 1;
    if (newLength >= m_capacity)
        Reallocate(newLength, true);

    this->m_rawString[m_length] = c;
    this->m_rawString[m_length+1] = '\0';


    // String result;
    // result.m_length = this->m_length + 1;
    // delete[] result.m_rawString;
    // result.m_rawString = new char[result.m_length + 1];

    // ioteye::StringCopy(result.m_rawString, this->m_rawString);
    // result.m_rawString[this->m_length] = c;
    // result.m_rawString[result.m_length] = '\0';

    // delete[] this->m_rawString;
    // this->m_length = result.m_length;
    // this->m_rawString = result.m_rawString;

    // result.m_rawString = nullptr;

    return *this;
}

char &ioteye::String::operator[](unsigned int index)
{
    if(!m_length)
        return m_rawString[0];
    if(index >= 0 && index < m_length)
        return m_rawString[index];
    return m_rawString[0];
}

const char &ioteye::String::operator[](unsigned int index) const
{
    if(!m_length)
        return m_rawString[0];
    if(index >= 0 && index < m_length)
        return m_rawString[index];
    return m_rawString[0];
}

void ioteye::String::Reserve(unsigned int size)
{
    Reallocate(size-8, true);
}

void ioteye::String::Reallocate(unsigned int length, bool saveData)
{
    m_capacity = length + 8;
    const char *temp = this->m_rawString;
    this->m_rawString = new char[m_capacity + 1];
    if (saveData)
        ioteye::StringCopy(this->m_rawString, temp, length);
    this->m_rawString[length + 1] = '\0';
    delete[] temp;
}

unsigned int ioteye::GetRawStringLength(const char *cString)
{
    if (cString == nullptr)
        return 0;
    const char *end = cString;
    while (*end != '\0')
        end++;
    return end - cString;
}

ioteye::String ioteye::operator+(const String &fstString, const String &scndString)
{
    String result;
    result.m_length = fstString.GetLength() + scndString.GetLength();
    delete[] result.m_rawString;
    result.m_rawString = new char[result.GetLength() + 1];

    ioteye::StringCopy(result.m_rawString, fstString.GetRawString());
    ioteye::StringCopy(result.m_rawString + fstString.GetLength(), scndString.GetRawString());

    return result;
}

char *ioteye::StringCopy(char *dest, const char *src, unsigned int destLength)
{
    if (dest == nullptr &&
        src == nullptr)
        return dest;
    char *d = dest;
    if (!destLength)
        while ((*d++ = *src++))
            ;
    else
    {
        unsigned int i = 0;
        while ((i < destLength) &&
               (*d++ = *src++))
            i++;
        *d = '\0';
    }
    return dest;
}
