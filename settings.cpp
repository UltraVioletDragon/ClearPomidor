
#include "settings.h"
#include <QDebug>

/*
 *
 */
Settings::Settings()
{
   // qDebug() <<  QDir::toNativeSeparators( QDir::currentPath() + "/config.ini");
    //settings = new QSettings(QDir::toNativeSeparators( qApp->applicationDirPath() + "/config.ini"), QSettings::IniFormat);
    settings = new QSettings(QDir::toNativeSeparators( QDir::currentPath() + "/config.ini"), QSettings::IniFormat);
    //
    timerWork = settings->value("Timer/work", timerWork).toInt();
    timerBreak = settings->value("Timer/break", timerBreak).toInt();
    //
    //alarmTime = settings->value("Alarm/time", alarmTime).toInt();
    //alarmWait = settings->value("Alarm/wait", alarmWait).toInt();
    alarmVolumeInit= settings->value("Alarm/volumeInit", alarmVolumeInit).toInt();
    alarmVolumeStep = settings->value("Alarm/volumeStep", alarmVolumeStep).toInt();
    alarmSound = settings->value("Alarm/sound", alarmSound).toString();
    alarmSound2 = settings->value("Alarm/sound2", alarmSound2).toString();

    messageWorkTitle = settings->value("Message/workTitle", messageWorkTitle).toString();
    messageWork= settings->value("Message/work", messageWork).toString();
    messageBreakTitle = settings->value("Message/breakTitle", messageBreakTitle).toString();
    messageBreak = settings->value("Message/break", messageBreak).toString();

    flushValues();
}
/*
 *
 */
void Settings::setValue(const QString &key, const QVariant &value)
{
    settings->setValue(key,value);
}
/*
 *
 */
QVariant Settings::value(const QString &key)
{
    return settings->value(key);
}
/*
 *
 */
void Settings::flushValues()
{
    settings->setValue("Timer/work", timerWork);
    settings->setValue("Timer/break", timerBreak);
    //
   // settings->setValue("Alarm/time", alarmTime);
    //settings->setValue("Alarm/wait", alarmWait);
    settings->setValue("Alarm/volumeInit", alarmVolumeInit);
    settings->setValue("Alarm/volumeStep", alarmVolumeStep);
    settings->setValue("Alarm/sound", alarmSound);
    settings->setValue("Alarm/soundLong", alarmSound2);
    //
    settings->setValue("Message/workTitle", messageWorkTitle);
    settings->setValue("Message/work", messageWork);
    settings->setValue("Message/breakTitle", messageBreakTitle);
    settings->setValue("Message/break", messageBreak);
    settings->sync();
}
/*
 * */
void Settings::save()
{
    settings->sync();
    emit callChangeSettings();
}


