#include "appsignal.h"

AppSignal::AppSignal(QObject *parent) : QObject(parent)
{

}

AppSignal *AppSignal::getInstance()
{
    static AppSignal appsignal;
    return &appsignal;
}
