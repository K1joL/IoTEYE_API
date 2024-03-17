#ifndef IOTEYE_PAYLOD_H
#define IOTEYE_PAYLOD_H

#include <Arduino.h>
#include <WString.h>

namespace ioteye
{

    class PairString
    {
    public:
        PairString();
        PairString(const char* key, const char* value);
        PairString(const char* key, const String &value);
        PairString(const String &value, const char* key);
        PairString(const PairString& pairString);

    public:
        String key;
        String value;
    };

    class Payload
    {
    public:
        Payload();
        Payload(const ioteye::PairString& pair);
        void Add(const ioteye::PairString& pair);
        ~Payload();
        void clear();
        
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

}

#endif //!IOTEYE_PAYLOD_H