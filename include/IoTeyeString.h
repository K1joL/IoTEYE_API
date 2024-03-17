#ifndef IOTEYE_STRING_H
#define IOTEYE_STRING_H

namespace ioteye
{
    class String
    {
    public:
        String();
        String(const char c, unsigned int length = 1);
        String(const char *cString);
        String(const String &string);
        String(String &&other);
        ~String();
        inline unsigned int GetLength() const { return m_length; }

        String &operator=(const String &string);
        String &operator=(String &&other);
        String &operator=(const char *cString);
        String &operator=(const char c);
        String &operator+=(const String &string);
        String &operator+=(const char *cString);
        String &operator+=(const char c);
        char &operator[](unsigned int index);
        const char &operator[](unsigned int index) const;
        inline const char *GetRawString() const { return m_rawString; }
        void Reserve(unsigned int size);

        friend String operator+(const String &fstString, const String &scndString);
        friend char *StringCopy(char *dest, const char *src, unsigned int destLength);
        friend unsigned int GetRawStringLength(const char *cString);

    private:
        void Reallocate(unsigned int length, bool saveData = false);
    private:
        char *m_rawString = nullptr;
        unsigned int m_length = 0;
        unsigned int m_capacity = 8;
    };

    String operator+(const String &fstString, const String &scndString);
    char *StringCopy(char *dest, const char *src, unsigned int destLength = 0);
    unsigned int GetRawStringLength(const char *cString);
}

#endif //! IOTEYE_STRING_H