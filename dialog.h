#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QMouseEvent>
#include <QtGui>
#include <qdatetime.h>
#include "shadows.h"
#include <fstream>
#include <iostream>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void disable_shadows(){ shadows.turn_off_shadows(); }
    void enable_shadows(){ shadows.turn_on_shadows(); }
    void set_background(QString &str);

private:
    Ui::Dialog *ui;
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
    void signalOk(QString, QString);
    void signalCa();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();


};

#endif // DIALOG_H
