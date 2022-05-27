#include "dialog.h"
#include "ui_dialog.h"



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);  // hide butons
    this->setWindowTitle("New Task");

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    //setup shadows
    ui->cancelButton->setGraphicsEffect(shadows.sh1);
    ui->okButton->setGraphicsEffect(shadows.sh2);
    ui->timePeeker->setGraphicsEffect(shadows.sh3);
    ui->info_inp->setGraphicsEffect(shadows.sh4);
    ui->high_peeker->setGraphicsEffect(shadows.sh5);
    ui->timelabel->setGraphicsEffect(shadows.sh6);
    ui->taskname->setGraphicsEffect(shadows.sh7);


    QDateTime t;
    ui->timePeeker->setDateTime(t.currentDateTime());


}


Dialog::~Dialog()
{
    delete ui;
}


// set bg color
void Dialog::set_background(QString &str)
{
    ui->bg->setStyleSheet(str);
}


void Dialog::on_okButton_clicked()
{
    QString time = ui->timePeeker->text();
    QString hours;
    QString minutes;



    if(ui->info_inp->text() == "")
    {
        ui->info_inp->setText("Pls, name your new task.");
    }
    else
    {
        emit signalOk(ui->info_inp->text(), ui->timePeeker->text());
        ui->info_inp->setText("");
        ui->timePeeker->clear();
        this->hide();
    }
}


void Dialog::on_cancelButton_clicked()
{
    emit signalCa();
    this->hide();
}




