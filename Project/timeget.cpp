#include "timeget.h"
#include <iostream>

TimeThread::TimeThread()
{

}

void TimeThread::run()
{
    while(true)
    {
        time_str = time_now.currentDateTime().toString("dd.MM.yyyy hh:mm");


        std::cout<< "Local time: " << time_now.currentDateTime().toString("dd.MM.yyyy hh:mm").toStdString() << std::endl;


        if(time_now.currentDateTime().toString("hh:mm").toStdString()[0] == '0')
        {

            check_str = time_now.currentDateTime().toString("dd.MM.yyyy").toStdString() +' '+ time_now.currentDateTime().toString("hh:mm").toStdString().substr(1, 4);
            std::cout<< "Local time updated: " << check_str << std::endl;
            time_str = QString::fromStdString(check_str);
        }

        emit onTimeChanged(time_str);
        emit save_all();
        this->sleep(5);

    }
}

QDateTime TimeThread::get_time()
{
    return time_now.currentDateTime();
}
