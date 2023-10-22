i = 1
say = {}
pause = {}
msg = {}
---
say[1] = "Center of Electro Acoustic Music of Moscow Conservatory \
    is announcing the new release of distribution PureData CEAMMC, visual programming environment \
    and the library of externals CEAMMC-LIB."
say[2] = "New objects for working with colors, color spaces and their transformations to make dealing with light protocols like DMX more easy."
say[3] = "New object for Morse encoding. Hear how sounds sentence like: Eduard Artemiev"
say[4] = "..."
say[5] = "The metronome object that allows to create such complex patterns"
say[6] = "Objects for generating rhythmic sequences"
say[7] = "Object to work with LTC protocol"
say[8] = "PureData is a free software, distributed under GPL license and available for downloading at http://github.com. We wish you a new sound inventions!"
msg[4] = "Eduard Artemiev"
pause[1] = 300
pause[4] = 7000
pause[5] = 300
function on_bang(idx)
        i = 1
        any_to(0, "stop")
        any_to(0, "clear")
        any_to(0, "@voice", "Evgeniy-Eng")
        next()
end
function next()
        -- done
        if i > #say then
                return
        end
        -- say
        symbol_to(0, say[i])
        -- pause
        t = pause[i]
        if t ~= nil then
                send_float("pause", t)
        end
        -- msg
        m = msg[i]
        if m ~= nil then
                send_list("morse", msg[4])
        end
        i = i+1
end
