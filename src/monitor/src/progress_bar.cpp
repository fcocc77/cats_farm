#include <progress_bar.hpp>

progress_bar_class::progress_bar_class()
{
    set_color(255, 255, 255);
}

progress_bar_class::~progress_bar_class()
{
}

void progress_bar_class::set_value(int value_a, int value_b)
{

    int total = value_a + value_b;

    float percent_a = float(value_a) / float(total);
    float percent_b = float(value_b) / float(total);

    QString mid_value_a = QString::number(percent_a);
    QString mid_value_b = QString::number(percent_a + 0.00001);

    this->setValue(total);

    this->setStyleSheet("::chunk {"
                        "background-color: "
                        "qlineargradient(x0: 0, x2: 1, "
                        "stop: 0 " +
                        color_a + " , stop: " + mid_value_a + " " + color_a + ", "
                                                                              "stop: " +
                        mid_value_b + " " + color_b + ", stop: 1 " + color_b +
                        ")}");
}

void progress_bar_class::set_text(QString text)
{
    this->setFormat(text);
}

void progress_bar_class::set_color(int red, int green, int blue)
{
    float rgb_divide = 1.5;

    color_a = "rgb(" + QString::number(red) + "," + QString::number(green) + "," + QString::number(blue) + ")";
    color_b = "rgb(" + QString::number(red / rgb_divide) + "," + QString::number(green / rgb_divide) + "," + QString::number(blue / rgb_divide) + ")";
}
