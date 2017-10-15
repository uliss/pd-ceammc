#ifndef VFORMATLIST_H
#define VFORMATLIST_H

#include "tinyformat.h"

#include <vector>

class VFormatList {
public:
    template <typename T>
    void add(const T& value)
    {
        m_argStore.push_back(new AnyT<T>(value));
        const T& storedValue = static_cast<AnyT<T>&>(*m_argStore.back()).value;
        m_argList.push_back(storedValue);
    }

    operator tfm::FormatList()
    {
        return tfm::FormatList(m_argList.data(), m_argList.size());
    }

    ~VFormatList()
    {
        for (size_t i = 0; i < m_argStore.size(); i++)
            delete m_argStore[i];
    }

private:
    struct Any {
    };

    template <typename T>
    struct AnyT : Any {
        T value;
        AnyT(const T& value)
            : value(value)
        {
        }
    };

    std::vector<tfm::detail::FormatArg> m_argList;
    std::vector<Any*> m_argStore;
};

#endif // VFORMATLIST_H
