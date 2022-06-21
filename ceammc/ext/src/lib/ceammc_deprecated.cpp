#include "ceammc_deprecated.h"
#include "ceammc_log.h"
#include "fmt/format.h"

extern "C" {
#include "m_imp.h"
}

#include <algorithm>

namespace ceammc {

DeprecatedImpl::DeprecatedImpl()
{
}

void DeprecatedImpl::addAlias(t_symbol* className, t_symbol* alias)
{
    aliases_[className].push_front(alias);
}

bool DeprecatedImpl::checkAlias(t_symbol* className, t_symbol* alias) const
{
    auto cls = aliases_.find(className);
    if (cls == aliases_.end())
        return false;

    auto& l = cls->second;

    auto it = std::find(l.begin(), l.end(), alias);
    if (it != l.end()) {
        LIB_ERR << fmt::format("[{}] alias '{}' is deprecated, in future versions it will be removed",
            className->s_name, alias->s_name);
        return false;
    }

    return true;
}

bool DeprecatedImpl::checkAlias(t_class* cls, t_symbol* alias) const
{
    return checkAlias(cls->c_name, alias);
}

}
