#include "pystr.h"

#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <atomic>

#include <stb/stb_sprintf.h>

namespace py
{
    const string g_empty("");
    const string g_space(" ");

    struct refstr
    {
        std::atomic<int> refcount;
        char buffer[1];
    };

    static refstr *refstr_new(size_t length)
    {
        void *mem;
        size_t hdrsz = offsetof(refstr, buffer);
        if ((mem = malloc(hdrsz + length + 1)) != nullptr) {
            refstr *ref = static_cast<refstr *>(mem);
            ref->refcount = 1;
            return ref;
        } else {
            return nullptr;
        }
    }

    static void sstrcpy(char *buffer, const char *str, size_t length)
    {
        memcpy(buffer, str, length);
        buffer[length] = 0;
    }

    static refstr *refstr_from_str(const char *str, size_t length)
    {
        refstr *ref = refstr_new(length);
        sstrcpy(ref->buffer, str, length);
        return ref;
    }

    static refstr *refstr_from_char(char *m_str)
    {
        size_t offset = offsetof(refstr, buffer);
        return reinterpret_cast<refstr *>(m_str - offset);
    }

    class strb
    {
        static const size_t STRB_MINIMUM_CAPACITY = 8;

    public:
        strb();
        strb(size_t capacity);
        strb(const strb&) = delete;
        strb(strb&&);
        ~strb();

        string str();

    public:
        void resize(size_t newsize);
        void reserve(size_t capacity);

        void append(const string& s);
        void append(const char *str, size_t len);

    private:
        size_t m_len;
        size_t m_cap;
        refstr *m_ref;
    };

    strb::strb()
        : strb(STRB_MINIMUM_CAPACITY)
    {
    }

    strb::strb(size_t capacity)
        : m_len(0)
        , m_cap(0)
        , m_ref(nullptr)
    {
        capacity = std::max<size_t>(capacity, STRB_MINIMUM_CAPACITY);
        reserve(capacity);
    }

    strb::~strb()
    {
        // delete m_ref
    }

    string strb::str()
    {
        refstr *ref = m_ref;
        size_t len = m_len;

        m_ref = nullptr;
        m_len = 0;
        m_cap = 0;

        return string(ref, len);
    }

    void strb::resize(size_t newsize)
    {
        if (newsize <= m_len) {
            m_len = newsize;
            return;
        }

        if (newsize <= m_cap) {
            memset(m_ref->buffer, 0, m_cap - m_len);
            m_len = newsize;
            return;
        }

        reserve(newsize);

        memset(m_ref->buffer + m_len, 0, newsize - m_len);
        m_len = newsize;
    }

    void strb::reserve(size_t capacity)
    {
        if (capacity <= m_cap)
            return;

        size_t newcap = m_cap * 2;
        if (newcap < capacity)
            newcap = capacity;

        refstr *ref = refstr_new(newcap);
        memcpy(ref->buffer, m_ref->buffer, m_len + 1);
        free(m_ref);
    }

    void strb::append(const string& s)
    {
        append(s.begin(), s.len());
    }

    void strb::append(const char *str, size_t length)
    {
        reserve(m_len + length);
        for (size_t i = 0; i < length; ++i)
            m_ref->buffer[m_len + i] = str[i];
        m_len += length;
        assert(m_len <= m_cap);
    }

    string::string() noexcept
        : string(g_empty)
    {
    }

    string::string(const string& other)
        : m_str(other.ref())
        , m_len(other.m_len)
    {
    }

    string::string(string&& other)
        : m_str(other.m_str)
        , m_len(other.m_len)
    {
        other.m_str = nullptr;
        other = g_empty;
    }

    string::string(const char *ptr, size_t length)
        : m_len(length)
    {
        m_str = refstr_new(length)->buffer;
        sstrcpy(m_str, ptr, length);
    }

    string::string(const char *ptr)
        : string(ptr, strlen(ptr))
    {
    }

    string& string::operator=(const string& other)
    {
        if (this != &other) {
            unref();

            m_len = other.m_len;
            m_str = other.ref();
        }

        return *this;
    }

    string& string::operator=(string&& other)
    {
        unref();

        m_str = other.m_str;
        m_len = other.m_len;
        other.m_str = nullptr;

        other = g_empty;
        return *this;
    }

    string::string(refstr *ref, size_t length)
        : m_len(length)
        , m_str(ref->buffer)
    {
    }

    string::~string()
    {
        unref();
    }

    char *string::ref() const
    {
        ++(refstr_from_char(m_str)->refcount);
        return m_str;

    }

    void string::unref()
    {
        if (m_str != nullptr) {
            refstr *ref = refstr_from_char(m_str);
            if (--ref->refcount == 0)
                free(ref);
            m_str = nullptr;
        }
    }

    string string::substr(size_t pos, size_t length) const
    {
        assert(pos <= len());
        length = std::min<size_t>(length, len() - pos);
        // We don't need to do a copy if it's the current string.
        if (pos == 0 && length == m_len)
            return *this;

        refstr *ref = refstr_new(length);
        sstrcpy(ref->buffer, begin() + pos, length);
        return string(ref, length);
    }

    string string::strip(const string& chars) const
    {
        const char *str = begin();
        const char *sep = chars.begin();

        size_t lpos;
        for (lpos = 0; lpos < m_len; ++lpos) {
            if (!memchr(sep, str[lpos], chars.len()))
                break;
        }

        size_t rpos;
        for (rpos = m_len - 1; rpos < m_len; --rpos) {
            if (rpos < lpos)
                break;

            if (!memchr(sep, str[rpos], chars.len()))
                break;
        }

        return substr(lpos, (rpos + 1) - lpos);
    }

    string string::lstrip(const string& chars) const
    {
        const char *str = begin();
        const char *sep = chars.begin();

        size_t lpos;
        for (lpos = 0; lpos < m_len; ++lpos) {
            if (!memchr(sep, str[lpos], chars.len()))
                break;
        }

        return substr(lpos);
    }

    string string::rstrip(const string& chars) const
    {
        const char *str = begin();
        const char *sep = chars.begin();

        size_t rpos;
        for (rpos = m_len - 1; rpos < m_len; --rpos) {
            if (!memchr(sep, str[rpos], chars.len()))
                break;
        }

        return substr(0u, rpos + 1);
    }

    bool string::endswith(const string& suffix, size_t start, size_t end) const
    {
        end = std::min<size_t>(m_len, end);
        start = std::min<size_t>(m_len, start);
        if ((end - start) < suffix.len())
            return false;
        const char *ptr = begin() + (end - suffix.len());
        return !memcmp(ptr, suffix.begin(), suffix.len());
    }

    bool string::startswith(const string& suffix, size_t start, size_t end) const
    {
        end = std::min<size_t>(m_len, end);
        start = std::min<size_t>(m_len, start);
        if ((end - start) < suffix.len())
            return false;
        const char *ptr = begin() + start;
        return !memcmp(ptr, suffix.begin(), suffix.len());
    }

    string string::join(const std::vector<string>& seq) const
    {
        if (seq.empty())
            return g_empty;

        strb builder;
        for (size_t i = 0; i < (seq.size() - 1); ++i) {
            builder.append(seq[i]);
            builder.append(*this);
        }

        builder.append(seq[seq.size() - 1]);
        return builder.str();
    }

    bool string::equal(const string& other) const
    {
        if (len() != other.len())
            return false;
        return !memcmp(begin(), other.begin(), m_len);
    }

    string string::sprintf(const char *fmt, ...)
    {
        va_list va;
        va_start(va, fmt);
        string res = vsprintf(fmt, va);
        va_end(va);
        return res;
    }

    string string::vsprintf(const char *fmt, va_list va)
    {
        refstr *ref;
        int ilength = stbsp_vsnprintf(nullptr, 0, fmt, va);
        if (ilength < 0)
            return g_empty;
        size_t length = static_cast<size_t>(ilength);
        if (length == 0)
            return g_empty;
        if (!(ref = refstr_new(length)))
            return g_empty;
        stbsp_vsnprintf(ref->buffer, ilength + 1, fmt, va);
        return string(ref, length);
    }
}
