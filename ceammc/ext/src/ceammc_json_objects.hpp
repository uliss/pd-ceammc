//
//  ceammc_json_objects.h
//  pd_ext
//
//  Created by Alex Nadzharov on 09/12/16.
//
//  Wrapper and some very basic functions for common JSON objects

/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.*/

#ifndef ceammc_json_objects_h
#define ceammc_json_objects_h

#include <stdlib.h>
#include <vector>
#include "json.hpp"

//todo: check Windows/MinGW C++11 compatibility. Change JSON library if needed.
using json = nlohmann::json;

typedef struct {
    t_symbol *symbol;
    json *object;
} json_object_ptr;   //todo refcounter?

std::vector<json_object_ptr> json_objects;


static inline json cm_json_from_atom(t_atom atom)
{
    json ret = json::object();
    
    int at1 = (int)atom.a_type;
    ret["type"] = at1;
    ret["symbol"] = atom.a_w.w_symbol->s_name;
    ret["float"] = atom.a_w.w_float;
    
    return ret;
}

static inline t_atom cm_atom_from_json(json in1)
{
    t_atom ret;
    
    ret.a_type = A_SYMBOL;
    ret.a_w.w_symbol = gensym("<>");
    
    if (in1.is_object())
    {
        int at1 = in1["type"];
        ret.a_type = (t_atomtype)at1;
        std::string sym = in1["symbol"];
        ret.a_w.w_symbol = gensym(sym.c_str());
        ret.a_w.w_float = in1["float"];
    }
    
    return ret;
}

static inline t_atom cm_atom_from_json_string(json in1)
{
    t_atom ret;
    
    ret.a_type = A_SYMBOL;
    ret.a_w.w_symbol = gensym("<>");
    
    if (in1.is_string())
    {
        std::string sym = in1;
        ret.a_w.w_symbol = gensym(sym.c_str());
    }
    
    return ret;
}

static inline std::string cm_jsonkey_from_atom(t_atom* atom)
{
    std::string key;
    if (atom->a_type == A_FLOAT)
    {
        key = std::to_string((int)atom->a_w.w_float);
    }
    if (atom->a_type == A_SYMBOL)
    {
        key = atom->a_w.w_symbol->s_name;
    }
    return key;
}


#pragma mark -

static inline void cm_setjson(std::string jsonkey)
{

}
static inline std::string cm_getjson(std::string jsonkey)
{
    std::string ret = "";
    
    return ret;
}


#endif /* ceammc_json_objects_h */
