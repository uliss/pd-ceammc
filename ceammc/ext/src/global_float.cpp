#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_globaldata.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class GlobalFloat : public BaseObject {
    GlobalData<t_float> data_;

public:
    GlobalFloat(const PdArgs& a)
        : BaseObject(a)
        , data_(a.args.empty() ? "default" : to_string(a.args[0]), "global.float")
    {
        if (args().empty())
            OBJ_ERR << "global object ID required! Using default: " << data_.name();

        createOutlet();
        createCbProperty("@id", &GlobalFloat::m_id);
        createCbProperty("@refs", &GlobalFloat::m_refs);
        createCbProperty("@keys", &GlobalFloat::m_keys);
    }

    void onBang()
    {
        floatTo(0, data_.ref());
    }

    void onFloat(float v)
    {
        data_.ref() = v;
        onBang();
    }

    void m_set(t_symbol* /*s*/, const AtomList& lst)
    {
        if (lst.size() > 0 && lst.at(0).isFloat()) {
            data_.ref() = lst.at(0).asFloat();
        } else {
            OBJ_ERR << "invalid arguments given: " << lst;
        }
    }

    AtomList m_id() const
    {
        return listFrom(data_.name());
    }

    AtomList m_keys() const
    {
        std::vector<std::string> keys;
        data_.keys(keys);
        return listFrom(keys);
    }

    AtomList m_refs() const
    {
        return listFrom(data_.refCount());
    }

    void dump() const
    {
        BaseObject::dump();
    }
};

extern "C" void setup_global0x2efloat()
{
    ObjectFactory<GlobalFloat> obj("global.float");
    obj.addMethod("set", &GlobalFloat::m_set);
}
