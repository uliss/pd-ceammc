//
//  ceammc_namedatom.h
//  pd_ext
//
//  Created by Alex on 17/12/16.
//
//

#ifndef ceammc_namedatom_h
#define ceammc_namedatom_h

#pragma mark t_namedatom

class t_namedatom
{
    std::string name;
    std::string type;
    std::string value;
    
    std::map<t_atomtype,std::string> _c_at_to_s =
    {
        {A_SYMBOL , "symbol"},
        {A_FLOAT , "float"},
        {A_POINTER , "pointer"},
        {A_NULL , "null"},
        {A_DOLLAR , "dollar"},
        {A_DOLLSYM , "dollsym"},
        {A_DEFFLOAT , "deffloat"},
        {A_SEMI , "semi"},
        {A_COMMA , "comma"},
        {A_GIMME , "gimme"},
        {A_CANT , "cant"}
    };
    
    std::map<std::string, t_atomtype> _c_s_to_at =
    {
        { "symbol", A_SYMBOL },
        { "float", A_FLOAT },
        { "pointer", A_POINTER },
        { "null", A_NULL },
        { "dollar", A_DOLLAR },
        { "dollsym", A_DOLLSYM },
        { "deffloat", A_DEFFLOAT },
        { "semi", A_SEMI },
        { "comma", A_COMMA },
        { "gimme", A_GIMME },
        { "cant", A_CANT }
    };
    
    std::string cnv_atype_to_string(t_atom atom)
    {
        return _c_at_to_s[atom.a_type];
    }
    
    
    t_atomtype cnv_string_to_atype(std::string str)
    {
        return _c_s_to_at[str];
    }
    
    t_atom atom_from_type_value(std::string type, std::string value)
    {
        t_atom ret;
        
        ret.a_type = cnv_string_to_atype(type);
        
        switch (ret.a_type) {
            case A_SYMBOL:
                ret.a_w.w_symbol = gensym(value.c_str());
                break;
            case A_FLOAT:
                ret.a_w.w_float = atof(value.c_str());
                break;
                
            default:
                break;
        }
        
        return ret;
    }
    
    std::vector<std::string> type_value_from_atom(t_atom atom)
    {
        std::vector<std::string> ret;
        
        ret.push_back(cnv_atype_to_string(atom));
        
        switch (atom.a_type) {
            case A_SYMBOL:
                ret.push_back(atom.a_w.w_symbol->s_name);
                break;
            case A_FLOAT:
                ret.push_back(std::to_string(atom.a_w.w_float));
                break;
                
            default:
                break;
        }
        
        return ret;
    }
    
#pragma mark -
    
    t_namedatom from_str(std::string str)
    {
        std::vector<std::string> str_list;
        std::stringstream sstr;
        sstr.str(str);
        std::string item;
        while (std::getline(sstr, item, ':')) {
            str_list.push_back(item);
        }
        
        t_namedatom ret;
        
        ret.name = str_list[0];
        ret.type = str_list[1];
        ret.value = str_list[2];
        
        return ret;
    }
    
    std::string to_str()
    {
        std::string ret;
        
        ret = this->name + ":" + this->type + ":" + this->value;
        
        return ret;
    }
    
    t_atom get_atom()
    {
        return this->atom_from_type_value(this->type, this->value);
    }
    
    
    
};


#endif /* ceammc_namedatom_h */
