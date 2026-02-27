#!/bin/bash

python gen_rpi_gpio.py > gpio.txt
pd_ascii2pd -wd 800 -ht 500 --format pd gpio.txt hw.rpi.gpio-example.pd
rm gpio.txt
