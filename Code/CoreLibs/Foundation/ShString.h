#pragma once

SH_NAMESPACE_BEGIN

#define MAX_STRING_LENGTH   1000

template <typename _Type>
using BasicStringStream = std::basic_stringstream < _Type, std::char_traits<_Type>, StdAlloc<_Type> >;
using StringStream = BasicStringStream<char>;

class String
{
    char* data;    /*!< The ASCII characters that comprise the String */
    unsigned strlen;  /*!< The number of characters allocated in data */

public:

    ////////////////////////////////////////////////////////////////////////////
    /// Constructor And Destructor
    ////////////////////////////////////////////////////////////////////////////
    String();
    String(char c);
    String(const char* c);
    String(const String& s);

    ~String();

    ////////////////////////////////////////////////////////////////////////////
    /// Member Functions
    ////////////////////////////////////////////////////////////////////////////

    // return string length with string end
    unsigned len() const;

    // return string length
    unsigned length() const;
    unsigned size() const;

    // return index of c in String, if exists; -1 otherwise
    int index(char c) const;

    // return true if String is Empty, false otherwise
    bool empty() const;

    // convert String to c_str(LPSTR)
    char* c_str() const;

    // Converts lowercase alphabetic characters to uppercase
    void upcase(unsigned first, unsigned last);

    // Converts uppercase alphabetic characters to lowercase
    void downcase(unsigned first, unsigned last);

    // Toggles lowercase alphabetic characters to uppercase, and vice versa
    void togglecase(unsigned first, unsigned last);

    // Stream operators
    friend std::ostream& operator<< (std::ostream& so, const String& s);
    friend std::istream& operator>> (std::istream& so, String& s);

    ////////////////////////////////////////////////////////////////////////////
    /// Overrided Operators
    ////////////////////////////////////////////////////////////////////////////
    
    // Access string character
    char  operator[] (unsigned j) const;
    char& operator[] (unsigned j);
    
    // Set string value
    String& operator= (const char* c);
    String& operator= (const String& s);

    // Append string
    String& operator+= (const String& s);

    // lhs - The left hand operand string
    // rhs - The right hand operand string
    // 
    // String addtition
    friend String operator+ (const String& lhs, const String& rhs);
    friend String operator+ (const String& lhs, char          rhs);
    friend String operator+ (const String& lhs, const char* rhs);
    friend String operator+ (char          lhs, const String& rhs);
    friend String operator+ (const char*   lhs, const String& rhs);

    // String equality
    friend bool operator== (const String& lhs, const String& rhs);
    friend bool operator== (const String& lhs, char          rhs);
    friend bool operator== (const String& lhs, const char* rhs);
    friend bool operator== (char          lhs, const String& rhs);
    friend bool operator== (const char*   lhs, const String& rhs);

    // String non-equality
    friend bool operator!= (const String& lhs, const String& rhs);
    friend bool operator!= (const String& lhs, char          rhs);
    friend bool operator!= (const String& lhs, const char* rhs);
    friend bool operator!= (char          lhs, const String& rhs);
    friend bool operator!= (const char* lhs, const String& rhs);

    /// String comparison operators
    friend bool operator> (const String& lhs, const String& rhs);
    friend bool operator> (const String& lhs, char          rhs);
    friend bool operator> (const String& lhs, const char* rhs);
    friend bool operator> (char          lhs, const String& rhs);
    friend bool operator> (const char*   lhs, const String& rhs);

    friend bool operator< (const String& lhs, const String& rhs);
    friend bool operator< (const String& lhs, char          rhs);
    friend bool operator< (const String& lhs, const char* rhs);
    friend bool operator< (char          lhs, const String& rhs);
    friend bool operator< (const char*   lhs, const String& rhs);
   
    friend bool operator<= (const String& lhs, const String& rhs);
    friend bool operator<= (const String& lhs, char          rhs);
    friend bool operator<= (const String& lhs, const char* rhs);
    friend bool operator<= (char          lhs, const String& rhs);
    friend bool operator<= (const char*   lhs, const String& rhs);

    friend bool operator>= (const String& lhs, const String& rhs);
    friend bool operator>= (const String& lhs, char          rhs);
    friend bool operator>= (const String& lhs, const char* rhs);
    friend bool operator>= (char          lhs, const String& rhs);
    friend bool operator>= (const char*   lhs, const String& rhs);
};

SH_NAMESPACE_END