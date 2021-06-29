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

void concat(QString folder, int task_size, float frame_rate = 24,
            QString format = "mov");

}; // namespace video

#endif // VIDEO_HPP
