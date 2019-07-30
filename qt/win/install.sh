# Instalador de libreria de QT5 version 5.11.3 para windows

path=$(dirname "$0")

# Descomprime la libreria en C:
zip="$path/Qt5.11.3.zip"
# unzip $zip -d "C:/"
# --------------------

# Crea variable PATH en el systema
paths=$(cmd /C "cmd /C echo %PATH%") # Variables antiguas
setx PATH "C:\Qt\Qt5.11.3\Tools\mingw530_32\bin;C:\Qt\Qt5.11.3\5.11.3\mingw53_32\bin;$paths"
# ---------------------------------------