#include "param_float.h"
#include "ceammc_factory.h"
#include "param_storage.h"

#include <fstream>

extern "C" {
#include "g_canvas.h"
}

static t_symbol* SYM_EMPTY = gensym("");

ParamFloat::ParamFloat(const PdArgs& args)
    : BaseObject(args)
    , max_(0)
    , init_(0)
    , cnv_(0)
    , bind_addr_(0)
{
    max_ = new SizeTProperty("@max", 10, true);
    createProperty(max_);

    init_ = new FloatProperty("@init", 0, true);
    current_value_ = init_->value();
    createProperty(init_);

    name_ = new SymbolProperty("@name", positionalSymbolArgument(0, SYM_EMPTY), true);
    createProperty(name_);

    cnv_ = canvas_getcurrent();

    createOutlet();

    bind_addr_ = Preset::makeBindAddress(name_->value());
    pd_bind(&owner()->te_g.g_pd, bind_addr_);
}

ParamFloat::~ParamFloat()
{
    pd_unbind(&owner()->te_g.g_pd, bind_addr_);
}

void ParamFloat::onFloat(float f)
{
    current_value_ = f;
}

void ParamFloat::m_clear(t_symbol*, const AtomList&)
{
    PresetStorage::instance().remove(name_->value());
}

void ParamFloat::m_store(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_NATURAL)) {
        OBJ_ERR << "single float value expected: " << l;
        return;
    }

    size_t idx = l[0].asSizeT(0);

    PresetStorage& storage = PresetStorage::instance();
    if (idx >= storage.maxPresetCount()) {
        OBJ_ERR << "preset index is too big: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    if (!storage.setFloatValueAt(name_->value(), idx, current_value_))
        OBJ_DBG << "can't save preset: " << name_->str();
}

void ParamFloat::m_load(t_symbol*, const AtomList& l)
{
    if (!checkArgs(l, ARG_FLOAT)) {
        OBJ_ERR << "single float value expected: " << l;
        return;
    }

    size_t idx = l[0].asSizeT(0);

    PresetStorage& storage = PresetStorage::instance();
    if (idx >= storage.maxPresetCount()) {
        OBJ_ERR << "preset index is too big: " << idx << ". Max " << storage.maxPresetCount() << " allowed";
        return;
    }

    current_value_ = storage.floatValueAt(name_->value(), idx, init_->value());
    floatTo(0, current_value_);
}

std::string ParamFloat::makeParamFilePath() const
{
    std::string res;

    if (cnv_) {
        res += canvas_getdir(cnv_)->s_name;
        res += '/';
    }

    res += name_->value()->s_name;
    return res;
}

void setup_param_float()
{
    ObjectFactory<ParamFloat> obj("param.float");
    obj.addMethod("clear", &ParamFloat::m_clear);
    obj.addMethod("store", &ParamFloat::m_store);
    obj.addMethod("load", &ParamFloat::m_load);
}
