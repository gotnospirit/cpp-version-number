#ifndef __VERSION_H__
#define __VERSION_H__

#include <string>
#include <cstdint>

struct Version
{
    Version(uint32_t = 0, uint32_t = 0, uint32_t = 0, uint32_t = 0);

    bool operator < (Version const &);
    bool operator > (Version const &);
    bool operator == (Version const &);
    bool operator != (Version const &);
    bool operator <= (Version const &);
    bool operator >= (Version const &);

    operator std::string() const;

    bool in(std::string const &) const;

    static Version Parse(std::string const &);
    static bool ValidateInterval(std::string const &);

    friend bool operator == (Version const &, const char *);
    friend bool operator == (const char *, Version const &);
    friend bool operator != (Version const &, const char *);
    friend bool operator != (const char *, Version const &);

private:
    bool assign(std::string const &, uint8_t);

    uint32_t _major;
    uint32_t _minor;
    uint32_t revision;
    uint32_t build;
};

#endif // __VERSION_H__