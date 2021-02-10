# instala las dependencias para vinarender

sudo yum -y install epel-release http://li.nux.ro/download/nux/dextop/el7/x86_64/nux-dextop-release-0-5.el7.nux.noarch.rpm

sudo yum -y install \
    qt5-qtbase \
    qt5-qtbase-devel \
    qt5-qtmultimedia.x86_64 \
    qt5-qtmultimedia-devel.x86_64 \
    qt5-qtsvg.x86_64 \
    qt5-qtsvg-devel.x86_64 \
    mesa-libGL-devel \
    mesa-libGLU-devel \
    pulseaudio-libs-glib2 \
    ffmpeg \
    lm_sensors \
    gcc-c++ \
    sshpass \
    net-tools \
    nmap \
    psmisc #fuser

sudo yum -y group install "Development Tools"
# ----------------------
