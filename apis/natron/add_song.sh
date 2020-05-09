video='/mnt/server_01/templates/wedding_01/renders/mov/slide_01_v05.mov'
audio='/home/pancho/Music/Mi Persona Faborita.mp3'

output='/mnt/server_01/templates/wedding_01/renders/mov/slide_01_v05_with_audio.mov'

_meta="$(ffprobe -show_streams $video 2>/dev/null)"
function meta() {
    echo "$_meta" | grep "$1=" | sed -e "s/$1=//g"
}

duration=$(meta duration)

fade_duration=4
fade_start=$(echo "$duration-$fade_duration" | bc)

ffmpeg -y -i "$video" -i "$audio" -c:v copy -filter:a "afade=t=out:st=$fade_start:d=$fade_duration" -t $duration $output

pkill -9 vlc
vlc $output
