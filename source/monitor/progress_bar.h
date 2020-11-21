#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <includes.h>

class progress_bar_class : public QProgressBar
{

private:
    QString color_a;
    QString color_b;

public:
    progress_bar_class();
    ~progress_bar_class();

    void set_value(int value_a, int value_b);
    void set_text(QString text);
    void set_color(int red, int green, int blue, float rgb_divide = 1.5);
};

#endif // PROGRESS_BAR_HPP