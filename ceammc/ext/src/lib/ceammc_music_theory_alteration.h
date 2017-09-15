#ifndef CEAMMC_MUSIC_THEORY_ALTERATION_H
#define CEAMMC_MUSIC_THEORY_ALTERATION_H

#include <boost/functional/hash.hpp>
#include <iostream>

namespace ceammc {
namespace music {

    class Alteration {
    public:
        enum Type {
            DOUBLE_FLAT = -2,
            FLAT,
            NATURAL,
            SHARP,
            DOUBLE_SHARP
        };

    public:
        Alteration(Type t = NATURAL)
            : type_(t)
        {
        }

        Alteration::Type get() const { return type_; }
        void set(Type t) { type_ = t; }

        bool operator==(Type t) const { return type_ == t; }
        bool operator==(const Alteration& a) const { return type_ == a.type_; }
        bool operator!=(const Alteration& a) const { return type_ != a.type_; }
        bool operator<(const Alteration& a) const { return type_ < a.type_; }
        bool operator<=(const Alteration& a) const { return type_ <= a.type_; }
        bool operator>(const Alteration& a) const { return type_ > a.type_; }
        bool operator>=(const Alteration& a) const { return type_ >= a.type_; }

        bool operator++();
        bool operator--();

        bool alterate(int n);

        const char* fullName() const;
        const char* shortName() const;

        size_t hash_value() const;

    private:
        Type type_;
    };

    std::ostream& operator<<(std::ostream& os, const Alteration& a);
    size_t hash_value(const Alteration& a);
}
}

#endif // CEAMMC_MUSIC_THEORY_ALTERATION_H
