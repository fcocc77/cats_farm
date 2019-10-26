#!/bin/bash

# Datas del proyecto after effect
project=$1
vina2ae=$2
# --------------------------------

#------------------------------
afterfx="/opt/AE9.0/AfterFX.exe"
#------------------------------
# para que funcione en init 3 es nececsarion un Display virtual y se crea con Xvfb
Xvfb :7 &
export DISPLAY=:7
# ------------------------------

wine "$afterfx" -noui -s "var aep = '$project'; //@include '$vina2ae';"
