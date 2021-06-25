#include "progress_bar.h"
#include "util.h"

progress_bar_class::progress_bar_class()
{
    set_color(255, 255, 255);
}

progress_bar_class::~progress_bar_class() {}

void progress_bar_class::set_value(int value_a, int value_b)
{
    if (value_a <= 0)
        value_a = 1;
    if (value_b < 0)
        value_b = 0;

    int total = value_a + value_b;
    this->setValue(total);

    float value = float(value_a) / float(total);

    if (isnan(value))
        return;

    value -= 0.00001;
    QString mid_value_a = QString::number(value);
    QString mid_value_b = QString::number(value + 0.00001);

    this->setStyleSheet("::chunk {"
                        "background-color: "
                        "qlineargradient(x0: 0, x2: 1, "
                        "stop: 0 " +
                        color_a + " , stop: " + mid_value_a + " " + color_a +
                        ", "
                        "stop: " +
                        mid_value_b + " " + color_b + ", stop: 1 " + color_b +
                        ")}");
}

void progress_bar_class::set_text(QString text)
{
    this->setFormat(text);
}

void progress_bar_class::set_color(int red, int green, int blue,
                                   float rgb_divide)
{
    color_a = "rgb(" + QString::number(red) + "," + QString::number(green) +
              "," + QString::number(blue) + ")";
    color_b = "rgb(" + QString::number(red / rgb_divide) + "," +
              QString::number(green / rgb_divide) + "," +
              QString::number(blue / rgb_divide) + ")";
}
