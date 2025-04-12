BGCOLORS = {
    1: 0xFF0000,
    2: 0xFF0000,
    4: 0xFF0000,
    6: 0x000000,
   17: 0xFF0000,
    9: 0,
    14: 0,
    20: 0,
    25: 0,
    30: 0,
    34: 0,
    39: 0,
    8: 0xFFAA00,
    10: 0xFFAA00,
    3: 0x00FFFF,
    5: 0x00FFFF,
   19: 0xFF00FF,
   21: 0xFF00FF,
   23: 0xFF00FF,
   27: 0x666666,
   28: 0x666666
}

NOMENU = set([
    1, 2, 4, 6, 17, 9, 14, 20, 25, 30, 34, 39, 27, 28
])

GPIO = {
    3: 2, 5: 3, 7: 4, 8: 14, 10: 15, 11: 17, 12: 18, 13: 27, 15: 22, 16: 23, 18: 24, 19: 10, 21: 9, 22: 25,
    23: 11, 24: 8, 26: 7, 29: 5, 31: 6, 32: 12, 33: 13, 35: 19, 36: 16, 37: 26, 38: 20, 40: 21
}

GND = "Ground"

PIN_LABEL = {
    1: "3.3V",
    2: "5V",
    4: "5V",
    6: GND,
    9: GND,
    14: GND,
    20: GND,
    25: GND,
    30: GND,
    34: GND,
    39: GND
}

def gpio(idx):
    if not idx in GPIO:
        return -1
    else:
        return GPIO[idx]

def pin_label(idx): 
    if idx in PIN_LABEL:
        txt = PIN_LABEL[idx]
        return f"@label {txt} @label_margins 2 0"
    else:
        x = gpio(idx)
        if x < 0:
            return f"@label \"Pin\\[{idx:02}\\]\" @label_margins 2 0"

        if idx & 1 == 0:
            return f"@label \"Pin\\[{idx:02}\\]\\ GPIO{x:02}\" @label_margins 10 0 @label_side right @label_valign center @label_align left @fontweight normal"
        else:
            return f"@label \"GPIO{x:02}\\ Pin\\[{idx:02}\\]\" @label_margins 10 0 @label_side left @label_valign center @label_align right @fontweight normal"


def bgcolor(idx):
    if idx not in BGCOLORS:
        return "@background_color 0 0.8 0 @active_color 0 0 0"
    else:
        h = BGCOLORS[idx]
        r = (h >>16) / 255.0
        g = ((h >> 8) & 0xFF) / 255.0
        b = (h & 0xFF) / 255.0
        color = f"@background_color {r} {g} {b}"
        if idx in NOMENU:
            color += f" @active_color {r} {g} {b}" 
        else:
            color += f" @active_color 0 0 0"
        
        return color


def make_menu(idx, h, x, y):
    if idx in NOMENU:
        return

    w = 140
    if idx & 1 == 1:
        x -= w + 100
    else:
        x += 120

    gp = gpio(idx)
    send = obj_send(f"mode{gp}")
    recv = "@receive \"#0-all-menu\""
    print(f"[ui.tab @items None In Out PWM {send} {recv} @size {w} {h} {{ax={x},ay={y}}}]")

    recv = "@receive \"#0-all-pwm\""
    send = obj_send(f"gpio{gp}")

    if idx & 1 == 1:
        x -= 100
    else:
        x += w

    print(f"[ui.slider @show_value 1 {send} {recv} @size 100 {h} @label_inner 1 @label_valign center @label duty: @fontsize 8 {{ax={x},ay={y}}}]")


def obj_send(name):
    return f"@send \"#0-s{name}\""

def obj_recv(name):
    return f"@receive \"#0-r{name}\""

def label_props(idx):
    if idx & 1 == 0:
        return f"@label_side right @label_align left @label_valign center"
    else:
        return f"@label_side left @label_align right @label_valign center"


def tgl_values(idx):
    if idx not in NOMENU:
        on = gpio(idx)
        off = -on
        return f"@on_value {on} @off_value {off}"
    else:
        return ""


def make_tgl(idx, w, x, y):
    lp = label_props(idx)
    bg = bgcolor(idx)
    gp = gpio(idx)
    send = obj_send(f"gpio{gp}")
    recv = obj_recv(f"gpio{gp}")

    if idx in NOMENU:
        send = '@send "#0_send"'
        recv = '@receive "#0_recv"'

    lbl = pin_label(idx)
    # vals = tgl_values(idx)
    print(f"[ui.t @size {w} {w} {bg} @fontsize 9 {send} {recv} {lbl} {lp} {{ax={x},ay={y}}}]")


def make_act():
    x = 20
    y = 500
    x1 = x + 20
    x2 = x1 + 20
    x3 = x2 + 20
    print("")
    print(f"[r \\$0-spin {{ax={x},ay={y}}}]")
    print("|")
    y += 25
    print(f"[route 0 1 2 3 #rt {{ax={x},ay={y}}}]")
    y += 25
    print(f"[reset $1 #rs {{i=2,o=1,ax={x},ay={y}}}(")
    print(f"[input $1, poll $1 1 #in {{i=2,o=1,ax={x1},ay={y+25}}}(")
    print(f"[output $1, clear_pwm $1 #out {{i=2,o=1,ax={x2},ay={y+50}}}(")
    print(f"[output $1, soft_pwm_freq $1 100 0 #pwm {{i=2,o=1,ax={x3},ay={y+75}}}(")
    y += 100
    print(f"[s \\$0-gpio #gpio {{ax={x},ay={y}}}]")
    print(f"[X rt:0->rs]")
    print(f"[X rt:1->in]")
    print(f"[X rt:2->out]")
    print(f"[X rt:3->pwm]")
    print(f"[X rs->gpio]")
    print(f"[X in->gpio]")
    print(f"[X out->gpio]")
    print(f"[X pwm->gpio]")
       
XOFF = 260
YOFF = 60

idx = 0

if __name__ == '__main__':
    # print(f"[cnv 16 100 400 _ _ _ 0 0 {{ax=100,ay={YOFF}}}]")
    print("[ui.label Raspberry Pi GPIO @size 350 50 {ax=200,ay=10}]")

    print("[ui.b @label \"Reset\\ All\" #reset0 {o=1,ax=600,ay=30} @background_color 1 0 0 @size 40 40]")
    print("[0 #reset1 {ax=600,ay=80}(")
    print("[s \\$0-all-menu #reset2 {ax=600,ay=105}]")
    print("[X reset0->reset1]")
    print("[X reset1->reset2]")

    sx = 350
    sy = 500
    print(f"[loadmsg 0 {{o=1,ax={sx},ay={sy}}}]")
    print("|")
    sy += 25
    print(f"[s \\$0-all-menu {{ax={sx},ay={sy}}}]")
   
    sy += 25
    print(f"[r \\$0-gpio {{ax={sx},ay={sy}}}]")
    print("|  |")
    sy += 25
    print(f"|  [print GPIO {{ax={sx},ay={sy}}}]")
    sy += 25
    print(f"[hw.rpi.gpio {{i=1,o=1,ax={sx},ay={sy}}}]")
    print("|")
    sy += 25
    print(f"[route pin {{i=1,o=2,ax={sx},ay={sy}}}]")
    print("|")
    sy += 25
    print(f"[list.prepend \\$0 {{i=1,o=1,ax={sx},ay={sy}}}]")
    print("|")
    sy += 25
    print(f"[\\\\; \\$1-rgpio\\$2 set \\$3 {{i=1,ax={sx},ay={sy}}}(")


    sx += 140
    sy = 500
    for i in range(1, 41):
        if i & 1 == 1:
            x = 100
            y = i * 10
        else:
            x = 150
            y = (i-1) * 10

        w = 20
        make_tgl(i, w, XOFF + x, YOFF + y)
        make_menu(i, w, XOFF + x, YOFF + y)

        if i not in NOMENU:
            gp = gpio(i)
            print(f"[global.float \\$0-smode{gp} #mode{gp} {{o=1,ax={sx},ay={sy}}}]")
            print("|")
            print(f"[list.prepend \\$0 {{o=1,i=1,ax={sx},ay={sy+25}}}]")
            print("|")
            print(f"[\\$2 {gp} \\\\; \\$1-rgpio{gp} set 0 {{ax={sx},ay={sy+50}}}(")
            print("|")
            print(f"[s \\$0-spin {{ax={sx},ay={sy+100}}}]")

            print(f"[global.float \\$0-sgpio{gp} {{o=1,ax={sx},ay={sy+125}}}]")
            print("|")
            print(f"[flow.demux 4 #mux{gp} {{i=2,o=4,ax={sx},ay={sy+150}}}]")
            print(f"[write {gp} \\$1 #wmux{gp} {{ax={sx},ay={sy+175}}}(")
            print(f"[soft_pwm_freq {gp} 100 \\$1 #pmux{gp} {{ax={sx+100},ay={sy+175}}}(")
            print(f"[s \\$0-gpio #smux{gp} {{ax={sx},ay={sy+200}}}]")
            
            print(f"[X mux{gp}:2->wmux{gp}:0]")
            print(f"[X mux{gp}:3->pmux{gp}:0]")
            print(f"[X pmux{gp}->smux{gp}]")
            print(f"[X wmux{gp}->smux{gp}]")
            print(f"[X mode{gp}->mux{gp}:1]")


    make_act()
