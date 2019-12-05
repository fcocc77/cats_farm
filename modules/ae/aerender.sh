#!/bin/bash

# Datas del proyecto after effect
comp=$1
project=$2
output=$3
log=$4
firstFrame=$5
lastFrame=$6
instance=$7
# --------------------------------

#------------------------------
aerender="/opt/AE9.0/aerender.exe"
#------------------------------
# para que funcione en init 3 es nececsarion un Display virtual y se crea con Xvfb
Xvfb :7 &
export DISPLAY=:7
# ------------------------------

# establece el wine dependiendo de la instancia, esto es para aprobechar los cpus
export WINEPREFIX="/root/wine$instance"
# -------------------------------

wine "$aerender" -project "$project" -comp "$comp" -output "$output" -log "$log" -s $firstFrame -e $lastFrame
