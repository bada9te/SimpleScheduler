#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMouseEvent>
#include <QtGui>
#include <QColorDialog>
#include <QColor>
#include <string>
#include <vector>
#include <fstream>
#include "shadows.h"
#include "json.hpp"


using json = nlohmann::json;


namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    void disable_shadows(){ shadows.turn_off_shadows(); }
    void enable_shadows(){ shadows.turn_on_shadows(); }
    void set_background(QString &str);

private:
    Ui::Settings *ui;
    QPoint position;
    shadows shadows;
    bool NoShadows = false;
    bool BgChanged = false;
    int red = 88;
    int green = 101;
    int blue = 242;
    json js;

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
    void onSettingsCloseWithChanges();
    void shadows_off_main();
    void shadows_on_main();
    void change_bg_main(QString &str);

private slots:
    void on_closeForm_clicked();
    void on_openFile_clicked();
    void on_custom_sound_stateChanged(int arg1);
    void on_horizontalSlider_valueChanged(int value);
    void on_Disable_Shadows_Button_stateChanged(int arg1);
    void on_ColorPeekButton_clicked();
    void on_ReserBackgroundButton_clicked();

};

#endif // SETTINGS_H
