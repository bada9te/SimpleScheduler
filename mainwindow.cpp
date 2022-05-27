#include "mainwindow.h"
#include "./ui_mainwindow.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, Qt::Window | Qt::FramelessWindowHint)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Simple Scheduler");
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    //hide and setup some elements
    ui->stop_playing_button->hide();
    ui->time_label->hide();
    ui->table_outdated->move(20, 130);
    ui->table_outdated->hide();
    ui->see_outdated->setDisabled(true);


    // setup shadows
    ui->SettingsButton->setGraphicsEffect(shadows.sh1);
    ui->helpButton->setGraphicsEffect(shadows.sh2);
    ui->infoButton->setGraphicsEffect(shadows.sh3);
    ui->openFileButton->setGraphicsEffect(shadows.sh4);
    ui->saveFileButton->setGraphicsEffect(shadows.sh5);
    ui->minimizeFormButton->setGraphicsEffect(shadows.sh6);
    ui->closeFormButton->setGraphicsEffect(shadows.sh7);
    ui->addtaskButton->setGraphicsEffect(shadows.sh8);
    ui->markAsDone->setGraphicsEffect(shadows.sh9);
    ui->returnButton->setGraphicsEffect(shadows.sh10);
    ui->clearDoneListButton->setGraphicsEffect(shadows.sh11);
    ui->label->setGraphicsEffect(shadows.sh12);
    ui->label_2->setGraphicsEffect(shadows.sh13);
    ui->table_done->setGraphicsEffect(shadows.sh14);
    ui->table_todo->setGraphicsEffect(shadows.sh15);
    ui->label_7->setGraphicsEffect(shadows.sh16);
    ui->time_label->setGraphicsEffect(shadows.sh17);
    ui->stop_playing_button->setGraphicsEffect(shadows.sh18);
    ui->high_peeker->setGraphicsEffect(shadows.sh19);
    ui->removeToDo->setGraphicsEffect(shadows.sh20);
    ui->clearInProgress->setGraphicsEffect(shadows.sh21);
    ui->see_outdated->setGraphicsEffect(shadows.sh22);
    //ui->table_outdated->setGraphicsEffect(shadows.sh23);   // cause visual bug with this list when enabled



    // tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP
    // tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP

    // system tray
    tray_icon = new QSystemTrayIcon(this);
    tray_icon->setIcon(QIcon(":/img/myAppIcon.png"));

    // tray menu
    QMenu * menu = new QMenu(this);
    openFromTrayAct = new QAction("Open Scheduler", this);
    muteNotificationsAct = new QAction("Temporarily Disable Sound Notifications", this); // after click -> Enable Notif...
    showNearestTaskAct = new QAction("Show Nearest Task");
    closeFromTrayAct = new QAction("Exit App", this);
    menu->addAction(openFromTrayAct);
    menu->addAction(showNearestTaskAct);
    menu->addAction(muteNotificationsAct);
    menu->addAction(closeFromTrayAct);
    tray_icon->setContextMenu(menu);
    tray_icon->show();


    // connect tray
    connect(openFromTrayAct, SIGNAL(triggered()), this, SLOT(openFromTray()));
    connect(closeFromTrayAct, SIGNAL(triggered()), this, SLOT(closeFromTray()));
    connect(muteNotificationsAct, SIGNAL(triggered()), this, SLOT(muteNotificationsTray()));
    connect(showNearestTaskAct, SIGNAL(triggered()), this, SLOT(showNearsetTaskTray()));

    // tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP
    // tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP tray SETUP


    // new task connects
    dialog = new Dialog();
    connect(dialog, &Dialog::signalOk, this, &MainWindow::slotOk);
    connect(dialog, &Dialog::signalCa, this, &MainWindow::slotCa);

    // info form
    info_form = new info();
    connect(info_form, &info::onInfoClose, this, &MainWindow::slotOnInfoClosed);

    // settings connects
    settings = new Settings();
    connect(settings, &Settings::onSettingsCloseWithChanges, this, &MainWindow::slotOnSettingsClosedWithChanges);
    connect(settings, &Settings::shadows_off_main, this, &MainWindow::turnOffShadows);
    connect(settings, &Settings::shadows_on_main, this, &MainWindow::turnOnShadows);
    connect(settings, &Settings::change_bg_main, this, &MainWindow::changeBG);

    // load settings
    read_settings("settings.json");
    std::cout<<"> FILE_READ_ON_APP_LOAD: [OK] (main_form)"<<std::endl;


    // connect time thread and autosave
    time_thread = new TimeThread();
    connect(time_thread, &TimeThread::onTimeChanged, this, &MainWindow::slotOnTimeChanged);
    connect(time_thread, &TimeThread::save_all, this, &MainWindow::saveAll);
    time_thread->start();


    // init audio player
    media_player = new QMediaPlayer;
    media_output = new QAudioOutput;

    if(std::filesystem::exists("settings.txt") && *custom_sound_location != "")
        media_player->setSource(QUrl::fromLocalFile(*custom_sound_location));
    else
        media_player->setSource(QUrl("qrc:/audio/new_message.mp3"));

    if(std::filesystem::exists("settings.txt") && *Volume >= 0)
    {
        media_output->setVolume(*Volume);
        cout<<"[MediaPlayer] Volume set to: " << *Volume << std::endl;
    }
    else
    {
        media_output->setVolume(50);
        cout<<"[MediaPlayer] Volume set to: 50" << std::endl;
    }
    media_player->setAudioOutput(media_output);

    // read autosave
    read_file("autosave.txt");
}


MainWindow::~MainWindow()
{
    delete ui;
    delete dialog;
    delete settings;
    delete info_form;
    delete media_player;
    delete media_output;
    delete custom_sound_location;

}


// write file
void MainWindow::write_file(const QString& location)
{
    ofstream writer;
    writer.open(location.toStdString());
    if(writer && (ui->table_todo->count() > 0 || ui->table_done->count() > 0))
    {
        for(int i = 0; i < ui->table_todo->count(); i++)
        {
            writer << ui->table_todo->item(i)->text().toStdString() + " td" << '\n';
        }

        for(int i = 0; i < ui->table_done->count(); i++)
        {
            writer << ui->table_done->item(i)->text().toStdString() + " ip" << '\n';
        }

        for(int i = 0; i < ui->table_outdated->count(); i++)
        {
            writer << ui->table_outdated->item(i)->text().toStdString() + " od" << '\n';
        }
    }
    writer.close();
}


void MainWindow::read_settings(const QString& location)
{
    std::ifstream reader;
    reader.open(location.toStdString());
    if(reader)
    {
        //js.clear();
        reader >> js;
        reader.close();


        if(js["Custom_Sound"] != "")
        {
            *custom_sound_location = QString::fromStdString(js["Custom_Sound"]);
            std::cout << "[Settings] Custom_Sound [+] " << custom_sound_location->toStdString() <<std::endl;
        }
        else
            std::cout<<"[Settings] CUSTOM_SOUND_DISABLED"<<std::endl;


        if(js["Run_With_Windows"] == true)
        {
            QSettings bootUpSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            QString app_path = QCoreApplication::applicationFilePath();
            bootUpSettings.setValue("Simple_Scheduler", app_path);
            std::cout<<"[Settings] Run_With_Windows [+]"<<std::endl;
        }
        else
        {
            QSettings bootUpSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            bootUpSettings.remove("Simple_Scheduler");
            std::cout<<"[Settings] WAKE_UP_WITH_WINDOWS_DISABLED"<<std::endl;
        }


        if(js["Play_Sound"] == true)
            std::cout<<"[Settings] Play_Sound [+]"<<std::endl;
        else
            std::cout<<"[Settings] SOUND_NOTIF_DISABLED"<<std::endl;


        if(js["Keep_Tray"] == true)
            std::cout<<"[Settings] Keep_Tray [+]"<<std::endl;
        else
            std::cout<<"[Settings] KEEP_TRAY_DISABLED"<<std::endl;


        if(js["Volume"] > 0)
        {
            Volume = new float(js["Volume"]);
            std::cout<<"[Settings] Volume [+] "<< *Volume << "%" << std::endl;
        }
        else
            std::cout<<"[Settings] VOLUME_SET_TO_DEF-VALUE"<<std::endl;


        if(js["Shadows"] == false)
        {
            shadows.turn_off_shadows();
            dialog->disable_shadows();
            info_form->disable_shadows();
            settings->disable_shadows();
            std::cout<<"[Settings] SHADOWS_DISABLED"<<std::endl;
        }
        else
        {
            shadows.turn_on_shadows();
            dialog->enable_shadows();
            info_form->enable_shadows();
            settings->enable_shadows();
            std::cout<<"[Settings] Shadows [+]"<<std::endl;
        }


        if(js["BG"][0] != -1)
        {
            std::cout << "[Settings] Background: " << js["BG"] << std::endl;
            QString *str = new QString("background-color: rgb(" + QString::fromStdString(to_string(js["BG"][0]) +
                                                                                   ',' + to_string(js["BG"][1]) +
                                                                                   ',' + to_string(js["BG"][2])) + ");\n"
                                       "border-style: outset;\n"
                                       "border-width: 3px;\n"
                                       "border-radius: 20px;\n"
                                       "border-color: rgb(46, 48, 66);\n"
                                       "font: bold 14px;\n"
                                       "padding: 1px;\n");
            ui->bg->setStyleSheet(*str);
            info_form->set_background(*str);
            dialog->set_background(*str);
            settings->set_background(*str);
            delete str;
        }
        else
        {
            std::cout << "[Settings] BG reset."<< std::endl;
            QString *str = new QString("background-color: rgb(88, 101, 242);\n"
                                       "border-style: outset;\n"
                                       "border-width: 3px;\n"
                                       "border-radius: 20px;\n"
                                       "border-color: rgb(46, 48, 66);\n"
                                       "font: bold 14px;\n"
                                       "padding: 1px;\n");
            ui->bg->setStyleSheet(*str);
            info_form->set_background(*str);
            dialog->set_background(*str);
            settings->set_background(*str);
            delete str;
        }
    }


}

// reader
void MainWindow::read_file(const QString &location)
{
    ifstream reader;
    reader.open(location.toStdString());
    if(reader)
    {
        string temp = "";
        while(getline(reader, temp))
        {
           if((int)temp.find(" td") != -1)
           {
               temp = temp.substr(0, temp.length()-3);
               ui->table_todo->addItem(QString::fromStdString(temp));
           }

           if((int)temp.find(" ip") != -1)
           {
               temp = temp.substr(0, temp.length()-3);
               ui->table_done->addItem(QString::fromStdString(temp));
           }

           if((int)temp.find(" od") != -1)
           {
               temp = temp.substr(0, temp.length()-3);
               ui->table_outdated->addItem(QString::fromStdString(temp));
           }
        }
        reader.close();
    }
}

//check for outdated tasks
void MainWindow::tick_outdated()
{
    cout << "[Info] Checked for outdated tasks." << endl;
    QDateTime* dt = new QDateTime(time_thread->get_time());
    string temp = "";
    for(int i = 0; i < ui->table_todo->count(); i++)
    {
        temp = ui->table_todo->item(i)->text().toStdString();
        temp = temp.substr(temp.length() - (sizeof("dd.MM.yyyy hh:mm") - 1));
        temp = QDateTime::fromString(QString::fromStdString(temp), "dd.MM.yyyy hh:mm").toString("yyyy.MM.dd hh:mm").toStdString();

        if (temp < dt->toString("yyyy.MM.dd hh:mm").toStdString())
        {
            QListWidgetItem* item = ui->table_todo->item(i);
            ui->table_todo->removeItemWidget(item);
            ui->table_outdated->addItem(item->text());
            delete item;

            if(!ui->see_outdated->isEnabled() || ui->table_outdated->count() > 0)
            {
                ui->see_outdated->setDisabled(false);
                ui->see_outdated->setStyleSheet("background-color: orange;\n"
                                                "border-style: outset;\n"
                                                "border-width: 2px;\n"
                                                "border-radius: 10px;\n"
                                                "border-color:rgb(44, 47, 51);\n"
                                                "font: bold 25px;\n"
                                                "padding: 1px;\n");
            }
        }
    }
    delete dt;
}


bool vec_compare(string v1, string v2)
{
    v1 = v1.substr(v1.length() - (sizeof("dd.MM.yyyy hh:mm") - 1));
    v1 = QDateTime::fromString(QString::fromStdString(v1), "dd.MM.yyyy hh:mm").toString("yyyy.MM.dd hh:mm").toStdString();

    v2 = v2.substr(v2.length() - (sizeof("dd.MM.yyyy hh:mm") - 1));
    v2 = QDateTime::fromString(QString::fromStdString(v2), "dd.MM.yyyy hh:mm").toString("yyyy.MM.dd hh:mm").toStdString();

    cout<< "[" << v1 << "]" <<"  COMPARE  "<< "[" << v2 << "]" << endl;
    return v1 < v2;
}



// add task
void MainWindow::on_addtaskButton_clicked()
{
    dialog->show();
}


// mark as done
void MainWindow::on_markAsDone_clicked()
{
    if(ui->table_todo->currentItem() != nullptr)
    {
        QListWidgetItem* item = ui->table_todo->currentItem();
        ui->table_todo->removeItemWidget(item);
        ui->table_done->addItem(item->text());
        delete item;
    }
}


//return task to tasks list
void MainWindow::on_returnButton_clicked()
{
    if(ui->table_done->currentItem() != nullptr)
    {
        QListWidgetItem* item = ui->table_done->currentItem();
        ui->table_done->removeItemWidget(item);
        ui->table_todo->addItem(item->text());
        delete item;

        vector <string> vec;

        for(int i = 0; i < ui->table_todo->count(); i++)
            vec.push_back(ui->table_todo->item(i)->text().toStdString());

        sort(vec.begin(), vec.end(), vec_compare);

        cout << "[Info] Sorted date vec:" << endl;
        for(auto item : vec)
            cout<< item <<endl;

        ui->table_todo->clear();
        cout<<"List [todo] updated."<<endl;

        for(auto item : vec)
            ui->table_todo->addItem(QString::fromStdString(item));

        vec.clear();
    }
}


//clear done list
void MainWindow::on_clearDoneListButton_clicked()
{
    QListWidgetItem* item = ui->table_done->currentItem();
    ui->table_done->removeItemWidget(item);
    delete item;
}


// open file to read via file dialog
void MainWindow::on_openFileButton_clicked()
{
    QString location = QFileDialog::getOpenFileName(this, "Pls, choose a file:", NULL);
    read_file(location);
}


// save file via file dialog
void MainWindow::on_saveFileButton_clicked()
{
    QString location = QFileDialog::getSaveFileName(this, "Save file to:", NULL);
    write_file(location);
}


// slot to get OK signal from dialog
void MainWindow::slotOk(QString info_text, QString info)
{
    ui->table_todo->addItem(info_text + ", at: " + info);

    vector <string> vec;

    for(int i = 0; i < ui->table_todo->count(); i++)
        vec.push_back(ui->table_todo->item(i)->text().toStdString());

    sort(vec.begin(), vec.end(), vec_compare);

    cout << "Sorted date vec:" << endl;
    for(auto x : vec)
        cout<< x <<endl;

    ui->table_todo->clear();
    cout<<"List [todo] updated."<<endl;

    for(unsigned long long i = 0; i < vec.size(); i++)
        ui->table_todo->addItem(QString::fromStdString(vec[i]));

    vec.clear();
}


// slot to get CANCEL signal from dialog
void MainWindow::slotCa()
{
    // this->setDisabled(false);
    // this->setWindowOpacity(1);
}



// settings closed slot
void MainWindow::slotOnSettingsClosedWithChanges()
{
    std::cout<<"> FILE_READ_ON_SETTINGS_CLOSED: [OK]"<<std::endl;
    read_settings("settings.json");
}


// slot close info form
void MainWindow::slotOnInfoClosed()
{
    // this->setDisabled(false);
    // this->setWindowOpacity(1);
}

// get time from timethread
void MainWindow::slotOnTimeChanged(QString s)
{
    std::string str;

    ui->time_label->setText(s);

    if(ui->table_todo->count() > 0)
    {
        for (int i = 0; i < ui->table_todo->count(); i++)
        {
            str = ui->table_todo->item(i)->text().toStdString();
            str = str.substr(str.length() - s.toStdString().length());
            //std::cout<<str<<std::endl;

            if(str == s.toStdString())
            {
                this->activateWindow();
                tray_icon->showMessage(tr("It's time for work:"), tr(ui->table_todo->item(i)->text().toStdString().c_str()));
                std::cout << "Alert: "<< ui->table_todo->item(i)->text().toStdString() <<" [Playing Sound...]" << std::endl;
                media_player->play();      
                ui->stop_playing_button->show();
                break;
            }
            else
                ui->stop_playing_button->hide();
        }
        tick_outdated();
    }
}


// open from tray (slot)
void MainWindow::openFromTray()
{
    if(!this->isHidden())
        tray_icon->showMessage(tr("Simple Scheduler"), tr("Application is not hidden."), QSystemTrayIcon::Warning);
    else
    {
        this->activateWindow();
        this->show();
    }
}


// close from tray (slot)
void MainWindow::closeFromTray()
{
    this->close();
}

bool dtcomp(QDateTime left, QDateTime right)
{
    return left < right;
}

// show nearest task if len todo_list > 1
void MainWindow::showNearsetTaskTray()
{
    if(ui->table_todo->count())
        tray_icon->showMessage(tr("Nearest task:"), tr(ui->table_todo->item(0)->text().toStdString().c_str()));
    else
        tray_icon->showMessage(tr("Nice news!"), tr("You have no tasks to do :)"));
}


// mute notifications from tray
void MainWindow::muteNotificationsTray()
{
    if(muteNotificationsAct->text().toStdString() == "Enable Sound Notifications")
    {
        js["Play_Sound"] = true;
        muteNotificationsAct->setText("Temporarily Disable Sound Notifications");
        tray_icon->showMessage(tr("Simple Scheduler"), tr("Sound notifications are enabled."));
    }
    else
    {
        js["Play_Sound"] = false;
        muteNotificationsAct->setText("Enable Sound Notifications");
        tray_icon->showMessage(tr("Simple Scheduler"), tr("Sound notifications are temporarily disabled."), QSystemTrayIcon::Warning);
    }
}


void MainWindow::turnOffShadows()
{
    shadows.turn_off_shadows();
    dialog->disable_shadows();
    info_form->disable_shadows();
    settings->disable_shadows();
}

void MainWindow::turnOnShadows()
{
    shadows.turn_on_shadows();
    dialog->enable_shadows();
    info_form->enable_shadows();
    settings->enable_shadows();
}

// change bg color from settings
void MainWindow::changeBG(QString &str)
{
    ui->bg->setStyleSheet(str);
    info_form->set_background(str);
    dialog->set_background(str);
}


// save all
void MainWindow::saveAll()
{
    write_file("autosave.txt");
}


// close Form Button
void MainWindow::on_closeFormButton_clicked()
{
    if(dialog->isHidden() && settings->isHidden() && info_form->isHidden())
    {
        if(js["Keep_Tray"] == true)
        {
            this->hide();
            tray_icon->showMessage(tr("Simple Scheduler"), tr("Application is running in tray.\nClick on 'S' icon -> 'Exit App' to close."));
        }
        else
            this->close();
    }
    else
    {
        settings->close();
        dialog->close();
        info_form->close();
        if(js["Keep_Tray"] == true)
        {
            this->hide();
            tray_icon->showMessage(tr("Simple Scheduler"), tr("Application is running in tray.\nClick on 'S' icon -> 'Exit App' to close."));
        }
        else
            this->close();
    }
}


// minimize From Button
void MainWindow::on_minimizeFormButton_clicked()
{
    MainWindow::showMinimized();
}


// open settings form
void MainWindow::on_SettingsButton_clicked()
{
    settings->show();
}


// open info form
void MainWindow::on_infoButton_clicked()
{
    info_form->show();
}


// help button click (open web)
void MainWindow::on_helpButton_clicked()
{
    QDesktopServices::openUrl(QUrl("www.google.com"));
}


void MainWindow::on_stop_playing_button_clicked()
{
    ui->stop_playing_button->hide();
    media_player->stop();
    std::cout << "[StopPB] Player stopped." << std::endl;

    QListWidgetItem* item;
    for(int i = 0; i < ui->table_todo->count(); i++)
        if((int)ui->table_todo->item(i)->text().toStdString().find("[Outdated]") < 0)
            item = ui->table_todo->item(i);

    ui->table_todo->removeItemWidget(item);
    ui->table_done->addItem(item->text());
    delete item;
}



// remove item from to_do
void MainWindow::on_removeToDo_clicked()
{
    if(!ui->table_outdated->isHidden() && ui->table_outdated->currentItem() != nullptr)
    {
        QListWidgetItem* item = ui->table_outdated->currentItem();
        ui->table_todo->removeItemWidget(item);
        delete item;

        if(ui->table_outdated->count() == 0)
        {
            ui->see_outdated->setDisabled(true);
            ui->see_outdated->setStyleSheet("background-color: gray;\n"
                                            "border-style: outset;\n"
                                            "border-width: 2px;\n"
                                            "border-radius: 10px;\n"
                                            "border-color:rgb(44, 47, 51);\n"
                                            "font: bold 25px;\n"
                                            "padding: 1px;\n");
            ui->label->setText("To do");
            ui->table_outdated->hide();
            ui->addtaskButton->show();
            ui->markAsDone->show();
            ui->table_todo->show();
        }
    }
    else if(ui->table_todo->currentItem() != nullptr)
    {
        QListWidgetItem* item = ui->table_todo->currentItem();
        ui->table_todo->removeItemWidget(item);
        delete item;
    }
}


// clear all inprogress
void MainWindow::on_clearInProgress_clicked()
{
    if(ui->table_done->count() > 0)
        ui->table_done->clear();
}


// show outdated tasks
void MainWindow::on_see_outdated_clicked()
{
    if(ui->label->text() == "To do")
    {
        ui->table_todo->hide();
        ui->label->setText("Outdated");
        ui->table_outdated->show();
        ui->addtaskButton->hide();
        ui->markAsDone->hide();
    }
    else
    {
        ui->table_outdated->hide();
        ui->label->setText("To do");
        ui->table_todo->show();
        ui->addtaskButton->show();
        ui->markAsDone->show();
    }
}

