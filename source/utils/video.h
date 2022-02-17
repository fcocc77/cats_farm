// Author: Francisco Jose Contreras Cuevas
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <QString>

namespace video
{

struct meta
{
    float frame_rate;
    int frames;
};

meta get_meta_data(QString file);
float frame_to_seconds(int frame, float frame_rate);

void concat(QString folder, int task_size, QString format = "mov");

}; // namespace video

#endif // VIDEO_HPP
