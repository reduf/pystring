#ifndef PYSTR_H
#define PYSTR_H

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>

#include <vector>

namespace py
{
    struct refstr;

    class string;
    extern const string g_empty;
    extern const string g_space;

    class string
    {
    public:
        string() noexcept;
        string(const string&);
        string(string&& other);

        string(const char *ptr, size_t length);
        string(const char *ptr);

        string& operator=(const string& other);
        string& operator=(string&& other);

        string(refstr *ref, size_t length);

        ~string();

    private:
        char *ref() const;
        void  unref();

    public:
        size_t len() const
        {
            return m_len;
        }

        bool empty() const
        {
            return len() == 0;
        }

        char operator[](size_t index) const
        {
            assert(index < len());
            return m_str[index];
        }

        const char *begin() const
        {
            return m_str;
        }

        const char *end() const
        {
            return begin() + len();
        }

    public:
        string upper() const;
        string lower() const;

        string substr(size_t pos = 0, size_t length = SIZE_MAX) const;

        string strip(const string& chars = g_space) const;
        string lstrip(const string& chars = g_space) const;
        string rstrip(const string& chars = g_space) const;

        bool endswith(const string& suffix, size_t start = 0,
                      size_t end = SIZE_MAX) const;
        bool startswith(const string& suffix, size_t start = 0,
                        size_t end = SIZE_MAX) const;

        int find(const string& sub, int start, int end = INT_MAX) const;
        int rfind(const string& sub, int start, int end = INT_MAX) const;

        string join(const std::vector<string>& seq) const;
        string replace(const string& oldstr, const string& newstr,
                       size_t count = SIZE_MAX) const;

        std::vector<string> split(const string& sep, int maxsplit = -1) const;
        std::vector<string> splitlines(bool keepends = false) const;

        bool isalnum() const;
        bool isalpha() const;
        // bool isascii() const;
        bool isdecimal() const;
        bool isdigit() const;
        bool islower() const;
        bool isnumeric() const;
        bool isprintable() const;
        bool isspace() const;
        bool istitle() const;
        bool isupper() const;

        bool equal(const string& other) const;

    public:
        static string sprintf(const char *fmt, ...);
        static string vsprintf(const char *fmt, va_list va);

    private:
        size_t        m_len;
        mutable char *m_str;
    };

    inline bool operator==(const string& left, const string& right)
    {
        return left.equal(right);
    }

    inline bool operator!=(const string& left, const string& right)
    {
        return !left.equal(right);
    }
}

#endif // PYSTR_H
