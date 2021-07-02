#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include <QPushButton>

class check_box : public QPushButton
{
private:
    bool checked;

    void mousePressEvent(QMouseEvent *event) override;
public:
    check_box(QString label, bool checked = true);

    inline bool is_checked() const;
    void set_checked(bool _checked);
};

inline bool check_box::is_checked() const
{
    return checked;
}

#endif // CHECK_BOX_H
