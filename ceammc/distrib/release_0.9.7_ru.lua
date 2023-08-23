i = 1
say = {}
sleep = {}
msg = {}
---
say[1] = "Центр электроакустической музыки Московской консерватории \
    анонсирует новый выпуск сборки программы для визуального программирования звука \
    и мультимедиа --- PureData и библиотеку расширений CEAMMC-LIB."
say[2] = "Добавлены новые объекты для работы с цветами, преобразованиями цеветовых пространств, что позволяет удобнее работать с таикми протоколами, как DMX."
say[3] = "Добавлены объекты для кодировки азбуки Морзе. Вот так, например, звучит фраза: "
say[4] = "Объект метронома, который позволяет создавать сложные ритмы"
say[5] = "Объекты для генерации ритмических последовательностей"
say[6] = "Объекты для работы с протоколом LTC"
say[7] = "Программное обеспечение PureData - распространяется под свободной лицензией GPL и доступно для скачивания на сайте http://github.com. Желаем новых звуковых открытий!"

msg[3] = "В нердрах тундры выдры в гетрах"

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
