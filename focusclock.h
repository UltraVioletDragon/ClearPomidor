
#ifndef FOCUSCLOCK_H
#define FOCUSCLOCK_H

#include <QtGui>
#include <QPushButton>
#include "settings.h"

class FocusClock : public QWindow
{
    Q_OBJECT

public:
    //FocusClock();
    //explicit
        FocusClock(Settings *setting);
    virtual void render(QPainter *painter);
    QString timeLeftString();
    bool isTimer();
    bool isBreak();

public slots:
    void renderLater();
    void renderNow();

    void stop();
    void start();
    void pause();

    void timerEndBreak();
    void timerEnd();

    void updateSettings();

signals:
    void changeIcon(QString icon);
    void callStart();
    void callPause();
    void callStop();
    void callTimerEnd();


protected:
    bool event(QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
    int timeLeft;
    int timeMax;
    void timerEvent(QTimerEvent *) override;
    //void render(QPainter *p) override;
    void calcLeftTime();
    QString prepeareNumber(int);
    int getRemainingTime();

private:
    QBackingStore *m_backingStore;

    QPushButton *m_button;

    int m_timerId;
    QTimer mainTimer;
    int mainTimerLeft=0;
    QTimer mainTimerBreak;
    int mainTimerBreakLeft=0;
    QColor colorTimer;


    int leftMinutes;
    int leftSeconds;
    bool _isTimer=false;
    bool _isBreak=false;

    bool _isBlur=false;

        Settings *settings;
};

#endif // FOCUSCLOCK_H
