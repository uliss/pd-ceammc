%%{

machine color_rgb_hex;

action color_hex_set  { color.x = xchar2digit(fc); }
action color_hex_add  { (color.x *= 16) += xchar2digit(fc); }

color_xhex = xdigit{2} >color_hex_set $color_hex_add;

color_rgb = '#'
          (color_xhex %{color.r = color.x;}
           color_xhex %{color.g = color.x;}
           color_xhex %{color.b = color.x;})
           %{ cat_ = CAT_COLOR; type_ = TYPE_HEX; };

}%%

%%{

machine color_rgb_hex_short;

action color_hex_set  { color.x = xchar2digit(fc); }

color_xhex = xdigit >color_hex_set;

color_rgb = '#'
          (color_xhex %{color.r = color.x;}
           color_xhex %{color.g = color.x;}
           color_xhex %{color.b = color.x;})
           %{ cat_ = CAT_COLOR; type_ = TYPE_HEX; };

}%%

%%{

machine color_rgba_hex;

action color_rgba_hex_set  { color.x = xchar2digit(fc); }
action color_rgba_hex_add  { (color.x *= 16) += xchar2digit(fc); }

color_xhex = xdigit{2} >color_rgba_hex_set $color_rgba_hex_add;

color_rgba = '#'
          (
            color_xhex %{color.r = color.x;}
            color_xhex %{color.g = color.x;}
            color_xhex %{color.b = color.x;}
            (color_xhex %{color.a = color.x;})?
          )
           %{ cat_ = CAT_COLOR; type_ = TYPE_HEX; };

}%%

%%{

machine color_rgba_hex_short;

action color_hex_set  { color.x = xchar2digit(fc); }

color_xhex = xdigit >color_hex_set;

color_rgba = '#'
          (color_xhex  %{color.r = color.x;}
           color_xhex  %{color.g = color.x;}
           color_xhex  %{color.b = color.x;}
           (color_xhex %{color.a = color.x;})?)
           %{ cat_ = CAT_COLOR; type_ = TYPE_HEX; };

}%%

%%{

machine color_named;

black    = 'black'    %{ color.setHex(0x000000); };
blue     = 'blue'     %{ color.setHex(0x0000FF); };
cyan     = 'cyan'     %{ color.setHex(0x00FFFF); };
gold     = 'gold'     %{ color.setHex(0xFFD700); };
gray     = 'gray'     %{ color.setHex(0x808080); };
green    = 'green'    %{ color.setHex(0x008000); };
lime     = 'lime'     %{ color.setHex(0x00FF00); };
magenta  = 'magenta'  %{ color.setHex(0xFF00FF); };
navy     = 'navy'     %{ color.setHex(0x000080); };
olive    = 'olive'    %{ color.setHex(0x808000); };
orange   = 'orange'   %{ color.setHex(0xFFA500); };
pink     = 'pink'     %{ color.setHex(0xFFC0CB); };
purple   = 'purple'   %{ color.setHex(0x800080); };
red      = 'red'      %{ color.setHex(0xFF0000); };
white    = 'white'    %{ color.setHex(0xFFFFFF); };
yellow   = 'yellow'   %{ color.setHex(0xFFFF00); };

color_named =
    black |
    blue |
    cyan |
    gold |
    gray |
    green |
    lime |
    magenta |
    navy |
    olive |
    orange |
    pink
    purple |
    red |
    white |
    yellow
    ;

}%%
