#!/bin/bash

# ftp://ftp.adobe.com/pub/adobe/aftereffects/

# Instalar wine 32bit
# Wine tiene que ser de 32 bit por que el after effect es de 32 bit
# Solo funciona el aerende de After Efect CS4 de 32 bit
# Existe un .wine comprimido en el google drive que esta lista para usar, si es que no,
# para que funcione aerender y AfterFX es necesario los paquetes de playonlinux: Microsoft Core y vcrun2005
# pero antes de instalar los paquetes es importante que en las configuraciones del wine es que en "windows xp"
# para que pueda implementar vcrun2005 correctamente
# y en las configuraciones del wine en librerias borrar todas la librerias y agregar solo una "msvcp80"
yum -y install https://harbottle.gitlab.io/wine32/7/i386/wine32-release.rpm
yum -y install wine.i686
# -------------------------------

# Wine con after effect no funcion sin una pantalla asi que crea un display virtual
yum -y install xorg-x11-server-Xvfb.x86_64
#-------------------------------

rm -rf "/opt/AE9.0"
tar -xzvf AE9.0.tar.gz -C "/opt"

rm -rf "/root/.wine"
tar -xzvf wine.tar.gz -C "/root"

# crea 15 instancias para aprobechar los cpus
for ((i = 0; i <= 15; i++)); do
    echo Cloning .wine to wine$i ... 
    rm -rf "/root/wine$i"
    cp -rf "/root/.wine" "/root/wine$i"
done
# --------------------------