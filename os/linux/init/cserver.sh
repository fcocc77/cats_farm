pid_file='/opt/cats_farm/os/linux/init/serverPID'
log_file='/opt/cats_farm/log/server crash - '$(date)'.log'
debug=$(cat '/opt/cats_farm/etc/debug')

run() {
	# mata el pid a partir del puerto de catsfarm
	fuser -k 7001/tcp &>/dev/null
	#----------------------------------
	if $debug; then
		rm $log_file &> /dev/null
		echo "Started debugging."
		gdb -ex "set logging file $log_file" -ex "set logging redirect on" -ex "set confirm off" -ex "set pagination off" -ex r -ex "set logging on" -ex bt -ex q "./cServer" &> /dev/null
	else
		"./cServer" &> /dev/null
	fi
}

start() {
 	pid=$(cat $pid_file)

	if ! kill -0 $pid > /dev/null 2>&1; then
		echo "CatsFarm Server has started."
		export DISPLAY=:1 && export QT_QPA_PLATFORM=offscreen && cd '/opt/cats_farm/bin/linux' && run &>/dev/null & echo $! > $pid_file
	else
		echo "CatsFarm Server is running now."
	fi
}

stop() {
	cpid=$(cat $pid_file)
	if kill -0 $cpid > /dev/null 2>&1; then
		fuser -k 7001/tcp
		echo "CatsFarm Server has stopped."
	else
		echo "Is not running."
	fi
}


if [ "$1" = "start" ]
then
    start
fi

if [ "$1" = "stop" ]
then
    stop
fi

exit 0
