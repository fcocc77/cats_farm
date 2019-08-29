

project="C:/Users/Pancho/Google Drive/develop/video_vina/templates/wedding/wedding_v01.aep"
comp="wedding"
firstFrame=1
lastFrame=30
taskSize=5
name="Wedding"
group="after"





submit=C:/cats_farm/bin/win/submit.exe

$submit -jobName $name -serverGroup $group -firstFrame $firstFrame -lastFrame $lastFrame -taskSize $taskSize -render "$comp" -project $project -software AE -instances 1

