#!/bin/bash

# Datas del proyecto after effect
project="/home/vm/Desktop/cs2/cs2.aep"
comp="Comp 1"
# --------------------------------

#------------------------------
aerender="/opt/AE7.0/aerender.exe"
#------------------------------
# para que funcione en init 3 es nececsarion un Display virtual y se crea con Xvfb
Xvfb :7 &
export DISPLAY=:7
# ------------------------------

wine "$aerender" -project "$project" -comp "$comp"
