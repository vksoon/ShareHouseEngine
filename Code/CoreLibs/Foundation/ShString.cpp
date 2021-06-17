#pragma once

#include "ShPrerequisites.h"

SH_NAMESPACE_BEGIN

String::String()
{
    strlen = 1;
    data = SH_NewA<char>(strlen);
    data[0] = '\0';
}

String::String(char c)
{
    strlen = 2;
    data = SH_NewA<char>(strlen);
    data[0] = c;
    data[1] = '\0';
}

String::String(const char* c)
{
    if (c)
    {
        unsigned n = 0;
        while (c[n] != '\0') n++;

        n++;
        strlen = n;
        data = SH_NewA<char>(strlen);

        for (unsigned j = 0; j < n; j++)
            data[j] = c[j];

        data[strlen - 1] = '\0';
    }
    else
    {
        strlen = 1;
        data = SH_NewA<char>(strlen);
        data[0] = '\0';
    }
}

String::String(const String& s)
{
    strlen = s.len();
    data = SH_NewA<char>(strlen);
    for (unsigned j = 0; j < strlen; j++)
        data[j] = s[j];

    data[strlen - 1] = '\0';
}

String::~String()
{
    SH_DeleteA<char>(data, strlen);
}



unsigned String::len() const
{
    return strlen;
}

unsigned String::length() const
{
    return strlen - 1;
}

unsigned String::size() const
{
    return strlen - 1;
}

int String::index(char c) const
{
    for (unsigned j = 0; j < strlen; j++)
        if (data[j] == c) return (int)j;
    return -1;
}

bool String::empty() const
{
    if (length() == 0)
        return true;

    return false;
}

char* String::c_str() const
{
    return data;
}



void String::upcase(unsigned first, unsigned last)
{
    if ((first >= last) || (last > strlen)) throw 1;

    for (unsigned j = first; j < last; j++)
        if ('a' <= data[j] && data[j] <= 'z')
            data[j] -= ('a' - 'A');
}

void String::downcase(unsigned first, unsigned last)
{
    if ((first >= last) || (last > strlen)) throw 1;

    for (unsigned j = first; j < last; j++)
        if ('A' <= data[j] && data[j] <= 'Z')
            data[j] += ('a' - 'A');
}

void String::togglecase(unsigned first, unsigned last)
{
    if ((first >= last) || (last > strlen)) throw 1;

    for (unsigned j = first; j < last; j++)
        if ('A' <= data[j] && data[j] <= 'Z')
            data[j] += ('a' - 'A');
        else if ('a' <= data[j] && data[j] <= 'z')
            data[j] -= ('a' - 'A');
}



std::ostream& operator<< (std::ostream& os, const String& s)
{
    if (s.length() > 0)
    {
        for (unsigned j = 0; j < s.length(); j++)
            os << s[j];
    }
    else os << "";

    return os;
}

std::istream& operator>> (std::istream& is, String& s)
{
    char* c = SH_NewA<char>(MAX_STRING_LENGTH);
    is >> c;
    s = String(c);
    SH_DeleteA<char>(c, MAX_STRING_LENGTH);

    return is;
}



char String::operator[] (unsigned j) const
{
    if (j >= strlen) throw 1;
    return data[j];
}

char& String::operator[] (unsigned j)
{
    if (j >= strlen) throw 1;
    return data[j];
}



String& String::operator=(const char* c)
{
    SH_DeleteA<char>(data, strlen);

    if (c != nullptr)
    {
        unsigned n = 0;
        while (c[n] != '\0') n++;

        n++;
        strlen = n;
        data = SH_NewA<char>(strlen);

        for (unsigned j = 0; j < n; j++)
            data[j] = c[j];

        data[strlen - 1] = '\0';
    }
    else
    {
        strlen = 1;
        data = SH_NewA<char>(strlen);
        data[0] = '\0';
    }

    return *this;
}

String& String::operator= (const String& s)
{
    if (this == &s) return *this;

    SH_DeleteA<char>(data, strlen);
    strlen = s.len();

    data = SH_NewA<char>(strlen);
    for (unsigned j = 0; j < strlen; j++)
        data[j] = s[j];

    data[strlen - 1] = '\0';

    return *this;
}



String& String::operator+= (const String& s)
{
    unsigned lhsLength = length();
    unsigned rhsLength = s.length();

    unsigned len = lhsLength + rhsLength + 1;
    char* str = SH_NewA<char>(len);

    for (unsigned j = 0; j < lhsLength; j++)
        str[j] = data[j];

    for (unsigned i = 0; i < rhsLength; i++)
        str[lhsLength + i] = s[i];

    str[len - 1] = '\0';

    SH_DeleteA<char>(data, strlen);
    strlen = len;
    data = str;
    return *this;
}



String operator+ (const String& lhs, const String& rhs)
{
    return String(lhs) += rhs;
}

String operator+ (const String& lhs, char rhs)
{
    return String(lhs) += String(rhs);
}

String operator+ (const String& lhs, const char* rhs)
{
    return String(lhs) += String(rhs);
}

String operator+ (char lhs, const String& rhs)
{
    return String(lhs) += rhs;
}
String operator+ (const char* lhs, const String& rhs)
{
    return String(lhs) += rhs;
}



bool operator== (const String& lhs, const String& rhs)
{
    if (lhs.length() != rhs.length()) return false;

    unsigned cap = lhs.length();
    unsigned   n = 0;
    while ((n < cap) && (lhs[n] == rhs[n])) n++;
    return (n == cap);
}

bool operator== (const String& lhs, char rhs)
{
    return (lhs == String(rhs));
}

bool operator== (const String& lhs, const char* rhs)
{
    return (lhs == String(rhs));
}

bool operator== (char lhs, const String& rhs)
{
    return (String(lhs) == rhs);
}

bool operator== (const char* lhs, const String& rhs)
{
    return (String(lhs) == rhs);
}

bool operator!= (const String& lhs, const String& rhs)
{
    return !(lhs == rhs);
}

bool operator!= (const String& lhs, char rhs)
{
    return !(lhs == rhs);
}

bool operator!= (const String& lhs, const char* rhs)
{
    return !(lhs == rhs);
}

bool operator!= (char lhs, const String& rhs)
{
    return !(lhs == rhs);
}

bool operator!= (const char* lhs, const String& rhs)
{
    return !(lhs == rhs);
}



bool operator> (const String& lhs, const String& rhs)
{
    unsigned cap = (lhs.length() < rhs.length()) ? lhs.length() : rhs.length();
    unsigned n = 0;
    while ((n < cap) && (lhs[n] == rhs[n])) n++;
    if (n == cap) return (lhs.length() > rhs.length());

    if ((('A' <= lhs[n] && lhs[n] <= 'Z') || ('a' <= lhs[n] && lhs[n] <= 'z')) &&
        (('A' <= rhs[n] && rhs[n] <= 'Z') || ('a' <= rhs[n] && rhs[n] <= 'z')))
    {
        char A = (lhs[n] & ~32);
        char B = (rhs[n] & ~32);
        if (A != B) return (A > B);
    }
    return lhs[n] > rhs[n];
}

bool operator> (const String& lhs, char rhs)
{
    return (lhs > String(rhs));
}

bool operator> (const String& lhs, const char* rhs)
{
    return (lhs > String(rhs));
}

bool operator> (char lhs, const String& rhs)
{
    return (String(lhs) > rhs);
}

bool operator> (const char* lhs, const String& rhs)
{
    return (String(lhs) > rhs);
}

bool operator< (const String& lhs, const String& rhs)
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const String& lhs, char rhs)
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const String& lhs, const char* rhs)
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (char lhs, const String& rhs)
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const char* lhs, const String& rhs)
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator<= (const String& lhs, const String& rhs)
{
    return !(lhs > rhs);
}

bool operator<= (const String& lhs, char rhs)
{
    return !(lhs > rhs);
}

bool operator<= (const String& lhs, const char* rhs)
{
    return !(lhs > rhs);
}

bool operator<= (char lhs, const String& rhs)
{
    return !(lhs > rhs);
}

bool operator<= (const char* lhs, const String& rhs)
{
    return !(lhs > rhs);
}

bool operator>= (const String& lhs, const String& rhs)
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const String& lhs, char rhs)
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const String& lhs, const char* rhs)
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (char lhs, const String& rhs)
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const char* lhs, const String& rhs)
{
    return (lhs == rhs) || (lhs > rhs);
}

SH_NAMESPACE_END