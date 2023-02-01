pd_post("Hi!")

function on_float(i,f) 
   float_to(0, f*2)
end

function on_bang(i)
   bang_to(0)
end

function on_symbol(o,s)
   symbol_to(0, s .. "+++")
end

function on_list(o,l)
   for i,v in ipairs(l) do l[i]=v*2 end
   list_to(0, l)
end

function on_any(sel,l)
   any_to(0,sel .. "+",l)
end
