#ifndef CEAMMC_DEPRECATED_H
#define CEAMMC_DEPRECATED_H

#include "ceammc_containers.h"

#include <forward_list>
#include <unordered_map>

namespace ceammc {

class DeprecatedImpl {
    using SymbolList = std::forward_list<t_symbol*>;
    std::unordered_map<t_symbol*, SymbolList> aliases_;

public:
    DeprecatedImpl();
    void addAlias(t_symbol* className, t_symbol* alias);
    bool checkAlias(t_symbol* className, t_symbol* alias) const;
    bool checkAlias(t_class* cls, t_symbol* alias) const;
};

using Deprecated = SingletonMeyers<DeprecatedImpl>;

#define DEPRECATED_ALIAS(obj, name) Deprecated::instance().addAlias(obj.className(), gensym(name))

}

#endif // CEAMMC_DEPRECATED_H
