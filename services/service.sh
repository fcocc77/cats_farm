path=$(cat /etc/vinarender)
name=$2
port=$3

log_file=$path'/log/$name crash - '$(date)'.log'
debug=false

run() {
	# mata el pid a partir del puerto de vinarender
	fuser -k $port/tcp &>/dev/null
	#----------------------------------
	if $debug; then
		rm $log_file &>/dev/null
		echo "Started debugging."
		gdb -ex "set logging file $log_file" -ex "set logging redirect on" -ex "set confirm off" -ex "set pagination off" -ex r -ex "set logging on" -ex bt -ex q "./$name" &>/dev/null
	else
		"./$name" &>/dev/null
	fi
}

start() {
	pscax=$(ps cax | grep $name)

	if [[ $pscax != *"$name"* ]]; then
		echo "VinaRender $name has started."
		export DISPLAY=:1 && export QT_QPA_PLATFORM=offscreen && cd $path'/bin' && run &>/dev/null &
	else
		echo "VinaRender $name is running now."
	fi
}

stop() {
	pscax=$(ps cax | grep $name)

	if [[ $pscax == *"$name"* ]]; then
		pkill -9 $name
		echo "VinaRender $name has stopped."
	else
		echo "Is not running."
	fi
}

if [ "$1" = "start" ]; then
	start
fi

if [ "$1" = "stop" ]; then
	stop
fi

exit 0
