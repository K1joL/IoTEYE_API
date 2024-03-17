#include "payload.h"

ioteye::PairString::PairString()
    : key{},
      value{}
{
}

ioteye::PairString::PairString(const char* key, const char* value)
{
    this->key = key;
    this->value = value;
}

ioteye::PairString::PairString(const char *key, const String &value)
{
    this->key = key;
    this->value = value;
}

ioteye::PairString::PairString(const String &value, const char *key)
{
    this->key = key;
    this->value = value;
}

ioteye::PairString::PairString(const PairString &pairString)
{
  this->key = pairString.key;
  this->value = pairString.value;
}

ioteye::Payload::Payload()
{
    m_pairs = new PairString[m_capacity];
    m_pairs[0] = PairString("","");
}

ioteye::Payload::Payload(const ioteye::PairString &pair)
{
    m_pairs = new PairString[m_capacity];
    m_pairs[m_size] = pair;
    m_overallLength += pair.key.length() + pair.value.length();
    m_size++;
}

void ioteye::Payload::Add(const ioteye::PairString &pair)
{
    if (m_size == m_capacity)
    {
        m_capacity += 8;
        PairString *temp = m_pairs;
        m_pairs = new PairString[m_capacity];
        for (unsigned int i = 0; i < m_size; i++)
            m_pairs[i] = temp[i];
    }
    m_pairs[m_size] = pair;
    m_overallLength += pair.key.length() + pair.value.length();
    m_size++;
}

ioteye::Payload::~Payload()
{
    delete[] m_pairs;
}

void ioteye::Payload::clear()
{
    if(m_size == 0)
        return;
    if(m_capacity > 20)
    {
        delete[] m_pairs;
        m_capacity = 8;
        m_pairs = new PairString[m_capacity];
    }
    m_overallLength = 0;
    m_size = 0;
}

String ioteye::Payload::GetJsonString() const
{
    String jsonString('{');
    // Рассчитать емкость, чтобы избежать многократного перераспределения
    // m_overallLength - длина всех строк в парах
    // 6 - набор символов: "":"",
    // 2 - набор символов: {} 
    jsonString.reserve(this->m_overallLength + this->m_size*6+2);
    for(unsigned int i = 0; i < this->m_size; i++)
    {
        jsonString += "\"" + m_pairs[i].key + "\":\"" + m_pairs[i].value + "\",";
    }
    jsonString[jsonString.length()-1] = '}';
    return jsonString;
}

String ioteye::Payload::GetArgsString() const
{
    String args{};
    // Рассчитать емкость, чтобы избежать многократного перераспределения
    // 2 - набор символов между key и value: key=value&key2=value2&...
    args.reserve(this->m_overallLength + this->m_size*2);
    for(unsigned int i = 0; i < this->m_size; i++)
    {
        args += m_pairs[i].key + "=" + m_pairs[i].value;
        if(i != this->m_size-1)
            args += "&";
    }
    return args;
}

ioteye::PairString &ioteye::Payload::operator[](unsigned int index)
{
    if(!m_size)
        return this->m_pairs[0];

    if (index >= 0 && index < m_size)
        return this->m_pairs[index];

    return this->m_pairs[0];
}

const ioteye::PairString &ioteye::Payload::operator[](unsigned int index) const
{
    if(!m_size)
        return this->m_pairs[0];

    if (index >= 0 && index < m_size)
        return this->m_pairs[index];
    return this->m_pairs[0];
}
