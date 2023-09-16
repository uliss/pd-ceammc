-- called on bang
function on_bang(idx)
    pd_post("Bang at [" .. idx .."]")
    float_to(1, idx)
    bang_to(0)
end
-- called on float
function on_float(idx, f)
    pd_post("Float at [" .. idx .. "] " .. f)
    float_to(1, idx)
    float_to(0, f)
end
-- called on symbol
function on_symbol(idx, s)
    pd_post("Symbol at [" .. idx .. "] " .. s)
    float_to(1, idx)
    symbol_to(0, s)
end
-- called on list
function on_list(idx, l)
    pd_post("List at [" .. idx .. "] " .. str(l))
    float_to(1, idx)
    list_to(0, l)
end
-- function fn
function fn(arg)
    pd_post("Called fn with arg: " .. str(arg))
    any_to(0, "fn", arg)
end
