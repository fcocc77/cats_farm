renderer='/opt/Natron2/NatronRenderer'
project='./project/test.ntp'

write='Write2'

api='./vina_to_natron.py'

first_frame=1
last_frame=10

# python3 $api

$renderer $api $first_frame-$last_frame

# $renderer -w $write $first_frame-$last_frame $project

