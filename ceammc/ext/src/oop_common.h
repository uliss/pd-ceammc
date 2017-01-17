
//
//  oop_common.h
//  pd_ext
//
//  Created by Alex Nadzharov on 17/01/17.
//
//

#include "ceammc.h"
#include <m_pd.h>
//#include <g_canvas.h>
#include <stdlib.h>
//
#include "ceammc_atomlist.h"

#include "m_imp.h"

#include <stdio.h>

#include "cicm_wrapper.h"

#include "ceammc_globaldata.h"
#include <algorithm>


typedef std::vector<t_outlet*> multiOutput;

typedef ceammc::GlobalData<t_canvas*> oPDClass;

class t_instance
{
private:
    std::map<t_symbol*,multiOutput> methodOutputs;
    multiOutput instanceOutputs;
    
public:
    std::string class_name;
    t_canvas *canvas;
    
    //t_outlet *inst_out;
    
    
    void addMethod(t_symbol* methodName, t_outlet *outlet)
    {
        this->methodOutputs[methodName].push_back(outlet);
    }
    void freeMethod(t_symbol* methodName)
    {
        this->methodOutputs.erase(methodName);
    }
    
    void addInstanceOut(t_outlet *outlet)
    {
        this->instanceOutputs.push_back(outlet);
    }
    void freeInstanceOut(t_outlet *outlet)
    {
        this->instanceOutputs.erase(std::remove(this->instanceOutputs.begin(), this->instanceOutputs.end(), outlet), this->instanceOutputs.end());
    }
    
    //
    void multipleOutput(ceammc::AtomList list)
    {
        
        for (multiOutput::iterator it =this->instanceOutputs.begin(); it!=this->instanceOutputs.end(); ++it)
        {
            list.output(*it);
        }
        
    }
    
    void callMethod(ceammc::AtomList list)
    {
        //post("call method %s", list[0].asString().c_str());
        t_symbol *method_name = list[0].asSymbol();
        
        ceammc::AtomList subList = list.subList(1, list.size());;
        
        multiOutput *out1 = &this->methodOutputs[method_name];
        
        if (out1)
        {
            for (multiOutput::iterator it =out1->begin(); it!=out1->end(); ++it)
            {
                subList.output(*it);
            }
        }
    }
    
} ;
typedef ceammc::GlobalData<t_instance> oPDInstance;


#include <sstream>

template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

#pragma mark -