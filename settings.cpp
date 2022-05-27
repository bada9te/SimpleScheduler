#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    this->setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);  // hide butons



    // shadows
    ui->closeForm->setGraphicsEffect(shadows.sh1);
    ui->custom_sound->setGraphicsEffect(shadows.sh2);
    ui->file_inp->setGraphicsEffect(shadows.sh3);
    ui->horizontalSlider->setGraphicsEffect(shadows.sh4);
    ui->runWithWindos->setGraphicsEffect(shadows.sh5);
    ui->keepInTray->setGraphicsEffect(shadows.sh6);
    ui->notifSound->setGraphicsEffect(shadows.sh7);
    ui->volume_label->setGraphicsEffect(shadows.sh8);
    ui->high_peeker->setGraphicsEffect(shadows.sh9);
    ui->openFile->setGraphicsEffect(shadows.sh10);
    ui->Disable_Shadows_Button->setGraphicsEffect(shadows.sh11);
    ui->background_label->setGraphicsEffect(shadows.sh12);
    ui->ReserBackgroundButton->setGraphicsEffect(shadows.sh13);
    ui->ColorPeekButton->setGraphicsEffect(shadows.sh14);


    std::ifstream reader("settings.json");
    if(reader)
    {
        NoShadows = true;
        reader >> js;
        if(js["Custom_Sound"] != "")
        {
            ui->custom_sound->setCheckState(Qt::CheckState::Checked);
            ui->file_inp->setText(QString::fromStdString(js["Custom_Sound"]));
            ui->file_inp->setEnabled(true);
            ui->openFile->setEnabled(true);
        }
        if(js["Run_With_Windows"] == true)
        {
            ui->runWithWindos->setCheckState(Qt::CheckState::Checked);
        }
        if(js["Play_Sound"] == true)
        {
            ui->notifSound->setCheckState(Qt::CheckState::Checked);
        }
        if(js["Keep_Tray"] == true)
        {
            ui->keepInTray->setCheckState(Qt::CheckState::Checked);
        }
        if(js["Volume"] > 0)
        {
            ui->horizontalSlider->setValue(js["Volume"]);
        }


        if(js["Shadows"] == true)
            NoShadows = false;
        else
        {
            NoShadows = true;
            ui->Disable_Shadows_Button->setCheckState(Qt::CheckState::Checked);
        }


        if(js["BG"][0] != -1)
        {
            BgChanged = true;
        }
        std::cout<<"> FILE_READ_ON_APP_LOAD: [OK] (settings_form)"<<std::endl;
    }
    reader.close();
}


Settings::~Settings()
{
    delete ui;
}

// set bg function
void Settings::set_background(QString &str)
{
    ui->bg->setStyleSheet(str);
}


void Settings::on_closeForm_clicked()
{
    std::ofstream writer;
    writer.open("settings.json");

    if(ui->custom_sound->isChecked() && ui->file_inp->text() != "")
        js["Custom_Sound"] = ui->file_inp->text().toStdString();
    else
        js["Custom_Sound"] = "";

    if(ui->notifSound->isChecked())
        js["Play_Sound"] = true;
    else
        js["Play_Sound"] = false;

    if(ui->runWithWindos->isChecked())
        js["Run_With_Windows"] = true;
    else
        js["Run_With_Windows"] = false;

    if(ui->keepInTray->isChecked())
        js["Keep_Tray"] = true;
    else
        js["Keep_Tray"] = false;

    if(ui->Disable_Shadows_Button->isChecked())
        js["Shadows"] = false;
    else
        js["Shadows"] = true;

    if(BgChanged)
        js["BG"] = {red, green, blue};
    else
        js["BG"] = {-1, -1, -1};

    js["Volume"] = ui->horizontalSlider->value();

    writer << std::setw(4) << js << std::endl;
    emit onSettingsCloseWithChanges();
    this->close();
}


// custom sound from file
void Settings::on_custom_sound_stateChanged(int arg1)
{
    if(ui->custom_sound->isChecked())
    {
        ui->file_inp->setEnabled(true);
        ui->openFile->setEnabled(true);
    }
    if(!ui->custom_sound->isChecked())
    {
        ui->file_inp->setEnabled(false);
        ui->openFile->setEnabled(false);
    }
}

// run with windows
void Settings::on_runWithWindos_stateChanged(int arg1)
{

}


// play sound on notification
void Settings::on_notifSound_stateChanged(int arg1)
{

}


// keep in tray
void Settings::on_keepInTray_stateChanged(int arg1)
{

}


// open custom audio file
void Settings::on_openFile_clicked()
{
    QString location = QFileDialog::getOpenFileName(this, "Pls, choose a file:", NULL);
    ui->file_inp->setText(location);
}

// volume slider
void Settings::on_horizontalSlider_valueChanged(int value)
{
    ui->volume_label->setText("Volume: (" + QString::fromStdString(std::to_string(value)) + "%)");
}


// on / off shadows
void Settings::on_Disable_Shadows_Button_stateChanged(int arg1)
{
    if(ui->Disable_Shadows_Button->isChecked())
    {
        emit shadows_off_main();
    }
    else
    {
        emit shadows_on_main();
    }
}


// pick and set a new color to all forms
void Settings::on_ColorPeekButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Background color:");
    if(color.isValid())
    {
        red = color.red();
        green = color.green();
        blue = color.blue();

        QString str = "background-color: rgb(" + QString::fromStdString(std::to_string(color.red())) + ","
                                               + QString::fromStdString(std::to_string(color.green())) + ","
                                               + QString::fromStdString(std::to_string(color.blue())) + ");\n"
                      "border-style: outset;\n"
                      "border-width: 3px;\n"
                      "border-radius: 20px;\n"
                      "border-color: rgb(46, 48, 66);\n"
                      "font: bold 14px;\n"
                      "padding: 1px;\n";

        ui->bg->setStyleSheet(str);
        emit change_bg_main(str);
        BgChanged = true;
    }
}


// reset background color
void Settings::on_ReserBackgroundButton_clicked()
{
    red = 88;
    green = 101;
    blue = 242;

    QString str = "background-color: rgb(88, 101, 242);\n"
                  "border-style: outset;\n"
                  "border-width: 3px;\n"
                  "border-radius: 20px;\n"
                  "border-color: rgb(46, 48, 66);\n"
                  "font: bold 14px;\n"
                  "padding: 1px;\n";

    ui->bg->setStyleSheet(str);
    emit change_bg_main(str);
    BgChanged = true;
}

