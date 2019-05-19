#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDir>
//#include <QApplication>

class Settings : public QObject
{
    Q_OBJECT

public:
    //  Settings(const QString &fileName = QDir::toNativeSeparators( qApp->applicationDirPath() + "/config.ini"), Format format = QSettings::IniFormat, QObject *parent = nullptr);
    Settings();
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key);
    void save();

signals:
    void callChangeSettings();

private:
    void flushValues();
    QSettings *settings;
    int timerWork = 1500;
    int timerBreak = 300;
    //
    //int alarmTime=6;
    //int alarmWait=14;
    int alarmVolumeInit=20;
    int alarmVolumeStep=20;
    QString alarmSound="qrc:/sound/miui-3.wav";
    QString alarmSound2="qrc:/sound/miui-4.wav";
    //
    QString messageBreakTitle="Работа окончена!";
    QString messageBreak="Начало перерыва!";
    QString messageWorkTitle="Перерыв окончен!";
    QString messageWork="Начало работы!";

};

#endif // SETTINGS_H
