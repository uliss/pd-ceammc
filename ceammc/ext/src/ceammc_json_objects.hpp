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
#include <map>
#include <string>
//#include "json.hpp"

//todo: check Windows/MinGW C++11 compatibility. Change JSON library if needed.
//using json = nlohmann::json;

//moving
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"

#include "ceammc_globaldata.h"

#include "ceammc_atomlist.h"

using namespace ceammc;
using namespace rapidjson;

//enum json_object_type
//{
//    oAtomList,
//    oObject
//
//} t_jsonObjectType;
//
//typedef class {
//    t_symbol *symbol;
//    //json *object;
//    void* object;
//    t_jsonObjectType type;
//
//
//} json_object_ptr;
//
//std::vector<json_object_ptr> json_objects;


//static inline Document* cm_json_from_atom(t_atom atom)
//{
//    Document *ret = new Document;
//    Document::AllocatorType& allocator = ret->GetAllocator();
//
//    ret->AddMember("type", (int)atom.a_type, allocator);
//    ret->AddMember("symbol", *atom.a_w.w_symbol->s_name, allocator);
//    ret->AddMember("float", atom.a_w.w_float, allocator);
//
//    return ret;
//}

static inline Document* cmJSONFromAtom(Atom atom, Document::AllocatorType& allocator)
{
    Document *ret = new Document;

    if (atom.isSymbol())
    {
        ret->SetString(atom.asString().c_str(), (int)atom.asString().size(), allocator);
    }
    else if (atom.isInteger())
    {
        ret->SetInt(atom.asInt());
    }
    else if (atom.isFloat())
    {
        ret->SetFloat(atom.asFloat());
    }

    return ret;

}

static inline void cmJSONFromAtomList(Document *doc, AtomList list)
{
    Document::AllocatorType& allocator = doc->GetAllocator();

    Value arr;
    arr.SetArray();

    for (int i=1; i<list.size();i++)    //iterator!
    {
        Atom a = list.at(i);
        arr.PushBack(*cmJSONFromAtom(a, allocator), allocator);

    }

    Value *vKey = new Value;

    vKey->SetString(list.at(0).asString().c_str() , (int)(list.at(0).asString().size()), allocator );

    if (doc->HasMember(*vKey))
    {
        doc->RemoveMember(*vKey);
    }

    doc->AddMember( *vKey , arr, allocator);

}


static inline AtomList cmAtomListFromJSON(Document *doc, std::string key)
{
    AtomList list;

    Document::AllocatorType& allocator = doc->GetAllocator();

    Value *vKey = new Value;

    vKey->SetString(key.c_str() , (int)(key.size()), allocator );


    Value::ConstMemberIterator itr = doc->FindMember(*vKey);
    if (itr != doc->MemberEnd())
    {
        const Value& arr  = itr->value;

        if (arr.IsArray())
        {
            for (int i=0; i<arr.Size(); i++)
            {
                Atom a;

                if (arr[i].IsString())
                {
                    a = Atom( gensym(arr[i].GetString()) );

                    list.append(a);

                }

                if (arr[i].IsFloat())
                {
                    a= Atom( (arr[i].GetFloat()) );

                    list.append(a);
                }

                if (arr[i].IsInt())
                {
                    a = Atom( (arr[i].GetInt()) );

                    list.append(a);
                }
            }
        }
        else
        {

            error("bad array");
            error("type: %d",arr.GetType());
        }
    }
    else
    {
        error("key not found");
    }

    return list;
}

//static inline t_atom cm_atom_from_json(Document in1)
//{
//    t_atom ret;
//
//    ret.a_type = A_SYMBOL;
//    ret.a_w.w_symbol = gensym("<>");
//
//    if (in1["type"].IsNumber() && in1["symbol"].IsString() && in1["float"].IsNumber())
//    {
//        int at1 = in1["type"].GetInt();
//        ret.a_type = (t_atomtype)at1;
//        std::string sym = in1["symbol"].GetString();
//        ret.a_w.w_symbol = gensym(sym.c_str());
//        ret.a_w.w_float = in1["float"].GetFloat();      //check PD_FLOATSIZE
//    }
//
//    return ret;
//
//}
//
//
//static inline t_atom cm_atom_from_json_string(Document in1)
//{
//    t_atom ret;
//
//    ret.a_type = A_SYMBOL;
//    ret.a_w.w_symbol = gensym("<>");
//
//    if (in1.IsString())
//    {
//        std::string sym = in1.GetString();
//        ret.a_w.w_symbol = gensym(sym.c_str());
//    }
//
//    return ret;
//}
//
//static inline std::string cm_jsonkey_from_atom(t_atom* atom)
//{
//    std::string key;
//    if (atom->a_type == A_FLOAT)
//    {
//        key = std::to_string((int)atom->a_w.w_float);
//    }
//    if (atom->a_type == A_SYMBOL)
//    {
//        key = atom->a_w.w_symbol->s_name;
//    }
//    return key;
//}
//
//
//#pragma mark -
//
//static inline void cm_setjson(std::string jsonkey)
//{
//
//}
//static inline std::string cm_getjson(std::string jsonkey)
//{
//    std::string ret = "";
//
//    return ret;
//}


#endif /* ceammc_json_objects_h */
