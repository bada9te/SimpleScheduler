#include "info.h"
#include "ui_info.h"

info::info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info)
{
    ui->setupUi(this);
    this->setWindowTitle("Information");

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    setWindowFlags(Qt::FramelessWindowHint);

    ui->closeForm->setGraphicsEffect(shadows.sh1);
    ui->info_label->setGraphicsEffect(shadows.sh2);
    ui->high_peeker->setGraphicsEffect(shadows.sh3);
}

info::~info()
{
    delete ui;
}

// set bg color
void info::set_background(QString &str)
{
    ui->bg->setStyleSheet(str);
}

void info::on_closeForm_clicked()
{
    this->hide();
    emit onInfoClose();
}









