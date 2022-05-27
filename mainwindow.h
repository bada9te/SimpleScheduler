#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qfiledialog.h>
#include <QLabel>
#include <QLineEdit>
#include <QDateTime>
#include <QtGui>
#include <QStyle>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsDropShadowEffect>
#include <QDesktopServices>
#include <QUrl>
#include <thread>
#include <chrono>


#include <windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <fstream>
#include <conio.h>


#include "dialog.h"
#include "settings.h"
#include "info.h"
#include "timethread.h"
#include "shadows.h"
#include "json.hpp"


using json = nlohmann::json;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    QPoint position;
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


private:
    Ui::MainWindow  *ui;
    Dialog          *dialog;
    Settings        *settings;
    info            *info_form;
    TimeThread      *time_thread;
    QSystemTrayIcon *tray_icon;

    float* Volume;
    QString* custom_sound_location = new QString("");
    QAction *openFromTrayAct;
    QAction *muteNotificationsAct;
    QAction *showNearestTaskAct;
    QAction *closeFromTrayAct;
    QMediaPlayer *media_player;
    QAudioOutput *media_output;
    shadows shadows;
    json js;
    void write_file(const QString& location);
    void read_settings(const QString& location);
    void read_file(const QString& location);
    void tick_outdated();


private slots:
    void on_addtaskButton_clicked();

    void on_markAsDone_clicked();

    void on_returnButton_clicked();

    void on_clearDoneListButton_clicked();

    void on_openFileButton_clicked();

    void on_saveFileButton_clicked();

    void on_closeFormButton_clicked();

    void on_minimizeFormButton_clicked();

    void on_SettingsButton_clicked();

    void on_infoButton_clicked();

    void on_helpButton_clicked();

    void on_stop_playing_button_clicked();


    void on_removeToDo_clicked();

    void on_clearInProgress_clicked();

    void on_see_outdated_clicked();

public slots:
    // new task slots
    void slotOk(QString info_text, QString time);
    void slotCa();

    // settings slots
    void slotOnSettingsClosedWithChanges();

    // info slots
    void slotOnInfoClosed();

    // time slot
    void slotOnTimeChanged(QString s);

    // open from tray
    void openFromTray();

    // close from tray
    void closeFromTray();

    // show nearest task from tray
    void showNearsetTaskTray();

    // mute notificatons tray
    void muteNotificationsTray();

    // turn off/on shadows
    void turnOffShadows();
    void turnOnShadows();

    // change bg
    void changeBG(QString &str);

    // save_all from timethread
    void saveAll();
};

#endif // MAINWINDOW_H
