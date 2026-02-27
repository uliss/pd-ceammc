#!/bin/bash

./gen_gpio.py > gpio.txt
pd_ascii2pd -wd 800 -ht 500 gpio.txt
