renderer='/opt/Natron2/NatronRenderer'
natron='/opt/Natron2/Natron'

api='./render.py'

first_frame=1
last_frame=120

# crea el proyecto
$natron -b './convert.py'

# renderea el proyecto creado
$renderer --clear-cache --no-settings $api $first_frame-$last_frame
