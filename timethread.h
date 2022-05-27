#ifndef TIMETHREAD_H
#define TIMETHREAD_H
#include <QtCore>
#include <qdatetime.h>
#include <string>

class TimeThread : public QThread
{
    Q_OBJECT

public:
    TimeThread();
    void run();
    QDateTime get_time();
    QDateTime time_now;
    QString time_str;
    std::string check_str;


signals:
    void onTimeChanged(QString s);
    void save_all();
};

#endif // TIMETHREAD_H
