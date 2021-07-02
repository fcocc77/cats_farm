// permite que funcionen los estilos cuando usamos el macro 'Q_OBJECT' de Qt,
// ese .h tenemos que incluirlo despues del Q_OBJECT
protected:
void paintEvent(QPaintEvent *pe)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
};
