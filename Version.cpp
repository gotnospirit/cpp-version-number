#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>

#include "Version.h"

namespace
{

bool eval(Version const &version, std::string const &interval, bool is_boundary)
{
    std::size_t len = interval.size();

    bool result = false;
    if (len > 0)
    {
        char first = interval[0];
        char last = interval[len - 1];

        bool lt = '[' == last;
        bool lte = ']' == last;
        bool gt = ']' == first;
        bool gte = '[' == first;
        bool eq = false;
        bool neq = false;

        if (!is_boundary)
        {
            eq = (lte && gte) || (::isdigit(first) && ::isdigit(last));
            neq = lt && gt;
        }

        Version other;

        if (lt || lte || gt || gte)
        {
            std::size_t from = 0;

            if (lt || lte)
            {
                --len;
            }

            if (gt || gte)
            {
                from = 1;
                --len;
            }

            other = Version::Parse(interval.substr(from, len));
        }
        else
        {
            other = Version::Parse(interval);
        }

        if (eq)
        {
            result = other == version;
        }
        else if (neq)
        {
            result = other != version;
        }
        else if (lt)
        {
            result = other > version;
        }
        else if (lte)
        {
            result = other >= version;
        }
        else if (gt)
        {
            result = other < version;
        }
        else if (gte)
        {
            result = other <= version;
        }
    }
    return result;
}

bool evalInterval(Version const &version, std::string const &interval)
{
    std::size_t pos = interval.find("-");
    if (pos == std::string::npos)
    {
        return eval(version, interval, false);
    }

    if (!Version::ValidateInterval(interval))
    {
        return false;
    }
    return eval(version, interval.substr(0, pos), true) && eval(version, interval.substr(pos + 1), true);
}

}

Version::Version(uint32_t major, uint32_t minor, uint32_t revision, uint32_t build) :
    _major(major),
    _minor(minor),
    revision(revision),
    build(build)
{
}

bool Version::operator < (Version const &other)
{
    if (_major < other._major)
    {
        return true;
    }
    else if (_major == other._major && _minor < other._minor)
    {
        return true;
    }
    else if (_major == other._major && _minor == other._minor && revision < other.revision)
    {
        return true;
    }
    else if (_major == other._major && _minor == other._minor && revision == other.revision && build < other.build)
    {
        return true;
    }
    return false;
}

bool Version::operator > (Version const &other)
{
    if (_major > other._major)
    {
        return true;
    }
    else if (_major == other._major && _minor > other._minor)
    {
        return true;
    }
    else if (_major == other._major && _minor == other._minor && revision > other.revision)
    {
        return true;
    }
    else if (_major == other._major && _minor == other._minor && revision == other.revision && build > other.build)
    {
        return true;
    }
    return false;
}

bool Version::operator == (Version const &other)
{
    return _major == other._major
        && _minor == other._minor
        && revision == other.revision
        && build == other.build;
}

bool Version::operator != (Version const &other)
{
    return !(*this == other);
}

bool Version::operator <= (Version const &other)
{
    return (*this < other) || (*this == other);
}

bool Version::operator >= (Version const &other)
{
    return (*this > other) || (*this == other);
}

Version::operator std::string() const
{
    std::ostringstream ss;
    ss << _major << "." << _minor << "." << revision << "." << build;
    return ss.str();
}

bool Version::in(std::string const &expr) const
{
    std::size_t i = 0,
                max = expr.size(),
                start = i,
                len = 0;

    bool result = false;
    for (; i < max; ++i)
    {
        if (',' == expr[i])
        {
            result = result || evalInterval(*this, expr.substr(start, i - start));
            start = i + 1;
        }
    }

    len = max - start;
    if (len > 0)
    {
        result = !start
            ? evalInterval(*this, expr.substr(start, len))
            : (result || evalInterval(*this, expr.substr(start, len)));
    }
    return result;
}

Version Version::Parse(std::string const &value)
{
    Version result;

    std::size_t i = 0,
                max = value.size(),
                start = i;
    uint8_t n = 0;
    char c = 0;

    for (; i < max; ++i)
    {
        c = value[i];
        if ('.' == c)
        {
            if (!result.assign(value.substr(start, i - start), n))
            {
                start = max;
                break;
            }

            start = i + 1;
            ++n;
        }
        else if (!::isdigit(c))
        {
            // -> invalid
            return { 0, 0, 0, 0 };
        }
    }

    if (start < max)
    {
        result.assign(value.substr(start, max - start), n);
    }
    return result;
}

bool Version::ValidateInterval(std::string const &expr)
{
    char c = 0;
    char last = 0;
    bool digit = false;

    std::size_t i = 0,
                max = expr.size();

    c = expr[0];
    if (',' == c || '-' == c || '.' == c)
    {
        return false;
    }

    c = expr[max - 1];
    if (',' == c || '-' == c || '.' == c)
    {
        return false;
    }

    for (; i < max; ++i)
    {
        c = expr[i];

        if (i > 0 && ('[' == c || ']' == c) && !digit && ',' != last)
        {
            return false;
        }

        digit = !!::isdigit(c);

        if ('[' != c && ']' != c && '-' != c && ',' != c && '.' != c && !digit)
        {
            return false;
        }
        else if (('[' == last || ']' == last) && !digit && ',' != c)
        {
            return false;
        }
        else if ('.' == last && '.' == c)
        {
            return false;
        }

        last = c;
    }
    return true;
}

bool operator == (Version const &a, const char * b)
{
    return Version::Parse(b) == a;
}

bool operator == (const char * a, Version const &b)
{
    return Version::Parse(a) == b;
}

bool operator != (Version const &a, const char * b)
{
    return Version::Parse(b) != a;
}

bool operator != (const char * a, Version const &b)
{
    return Version::Parse(a) != b;
}

bool Version::assign(std::string const &value, uint8_t index)
{
    if (index > 3)
    {
        return false;
    }
    else if (value.empty())
    {
        return true;
    }

    auto const number = std::stoi(value);
    if (0 == index)
    {
        _major = number;
    }
    else if (1 == index)
    {
        _minor = number;
    }
    else if (2 == index)
    {
        revision = number;
    }
    else if (3 == index)
    {
        build = number;
    }
    return true;
}
