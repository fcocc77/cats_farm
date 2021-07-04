cd $(dirname "$0")
cd ..

source utils/shell/svg2png.sh

normal_color="rgb(170,170,170)"
disable_color="rgb(60,60,60)"
checked_color="#0099ff"

images="resources/images"
svgs="resources/svg"

if [ ! -d "$images" ]; then
    mkdir "$images"
fi

for svg in $svgs/*; do
    svg_to_png "$images" $svg 'normal' $normal_color &>/dev/null
    svg_to_png "$images" $svg 'disable' $disable_color &>/dev/null
    svg_to_png "$images" $svg 'white' 'white' &>/dev/null
    svg_to_png "$images" $svg 'checked' $checked_color &>/dev/null
    echo $svg 'Exporting ...'
done

