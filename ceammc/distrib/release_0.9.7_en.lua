i = 1
say = {}
sleep = {}
msg = {}
---
say[1] = "Center of Electro Acoustic Music of Moscow Conservatory \
    is announcing the new release of distribution PureData CEAMMC, visual programming environment \
    and the library of externals CEAMMC-LIB."
say[2] = "New objects for working with colors, color spaces and their transformations to make dealing with light protocols like DMX more easy."
say[3] = "New object for Morse encoding. Hear how sounds sentence like: "
say[4] = "The metronome object that allows to create such complex patterns:"
say[5] = "Objects for generating rhythmic sequences:"
say[6] = "Object to work with LTC protocol:"
say[7] = "PureData is a free software, distributed under GPL license and available for downloading at http://github.com. We wish you a new sound inventions!"

msg[3] = "She sells see shells by the shore"

ctl = {}

sleep[3] = 5000

--ctl[1] = ["lang" "Anna"];
--ctl[2] = ["lang" "Mikhail"];
--ctl[3] = ["lang" "Timofey"];
--ctl[4] = ["lang" "Arina"];
--ctl[5] = ["lang" "Clb"];
--ctl[6] = ["lang" "Letícia-F123"];
--ctl[7] = ["lang" "Talgat"];
--ctl[8] = ["lang" "Tatiana"];
--ctl[9] = ["lang" "Pavel"];
--ctl[10] = ["lang", "Irina"];
--ctl[11] = ["lang", "Elena"];
--ctl[12] = ["lang", "Aleksandr-hq"];
--ctl[13] = ["drums", 1];

function on_bang(idx)
    i = 1
    next()
end

function next()
    -- done
    if i > #say then
        return
    end

    ct = ctl[i]
    if ct ~= nil then
        any_to(0, "@voice", ctl[i])
    end

    -- say
    symbol_to(0, say[i])

    -- sleep
    t = sleep[i]
    if t ~= nil then
        sleep(t)
    end

    -- msg
    m = msg[i]
    if m ~= nil then
        send_list("morse", msg[3])
    end


    i = i+1
end
