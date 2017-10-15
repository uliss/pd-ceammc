//
//  ceammc_autopreset.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 18/02/17.
//
//

#include "m_pd.h"

#include <map>
#include <stdio.h>
#include <string>
#include <utility>

namespace ceammc {

typedef std::pair<t_canvas, t_class> autoPresetPair;

std::map<autoPresetPair, int> autoPresetCounter;
std::map<t_class, std::string> autoPresetClassName;

class autoPreset {
public:
    static void addObject(t_canvas* canvas, t_class* pdclass);

    static std::string getName(t_canvas* canvas, t_class* pdclass);
};
}

#pragma mark -

void autopreset_addObject(t_canvas* canvas, t_class* pdclass)
{
    ceammc::autoPreset::addObject(canvas, pdclass);
}

t_symbol* autopreset_getname(t_canvas* canvas, t_class* pdclass)
{
    t_symbol* ret;

    std::string ret_str;
    ret_str = ceammc::autoPreset::getName(canvas, pdclass);
    ret = gensym(ret_str.c_str());

    return ret;
}