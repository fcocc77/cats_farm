cd $(dirname "$0")


# IPs
manager_ip="192.168.10.77"

# Inicio de Servicios
manager_start=true
server_start=true
logger_start=true

function installer() {
    sh ./linux.sh $1 $manager_ip $manager_start $server_start $logger_start
}

if [ $1 == install ]; then
    installer install
fi
if [ $1 == uninstall ]; then
    installer uninstall
fi
if [ $1 == reinstall ]; then
    installer reinstall
fi
