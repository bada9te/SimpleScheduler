#ifndef INFO_H
#define INFO_H

#include <QDialog>
#include <QMouseEvent>
#include <QtGui>
#include "shadows.h"
#include <fstream>
#include <iostream>

namespace Ui {
class info;
}

class info : public QDialog
{
    Q_OBJECT


public:
    explicit info(QWidget *parent = nullptr);
    ~info();
    void disable_shadows(){ shadows.turn_off_shadows(); }
    void enable_shadows(){ shadows.turn_on_shadows(); }
    void set_background(QString &str);

private:
    Ui::info *ui;
    QPoint position;
    bool NoShadows = false;
    shadows shadows;

    void mousePressEvent(QMouseEvent *m_event)
    {
        position = m_event->globalPos();
    }

    void mouseMoveEvent(QMouseEvent *m_event)
    {
        QPoint delta = m_event->globalPos() - position;
        move(x()+delta.x(), y() + delta.y());
        position = m_event->globalPos();
    }


signals:
    void onInfoClose();

private slots:
    void on_closeForm_clicked();

};

#endif // INFO_H
