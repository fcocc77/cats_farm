
sudo dnf -y install \
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

sudo dnf -y group install "Development Tools"
