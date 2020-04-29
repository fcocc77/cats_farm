renderer=$1
project=$2
node=$3
output=$4
first_frame=$5
last_frame=$6

# borra la extencion .ntp, para poder meterlo como argumento
# sin que lo reconosca el NatronRenderer
project="${project::-4}"
# ----------------------------

path=$(dirname "$0")

$renderer --clear-cache --no-settings $first_frame-$last_frame "$path/render.py" "$project" $node $output
