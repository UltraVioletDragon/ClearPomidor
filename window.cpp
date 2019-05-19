/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "window.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
//#include <QCheckBox>
//#include <QComboBox>
//#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
//#include <QLabel>
//#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
//#include <QSpinBox>
//#include <QTextEdit>
//#include <QVBoxLayout>
//#include <QMessageBox>
#include <QGridLayout>
#include <QtGui>


/*
 *
 */
Window::Window()
{
    setWindowTitle("Clear Pomidor");

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    settings = new Settings;
    dialog = new Dialog(settings);


    setGeometry(width-300, height-350-30, 300, 350);
    setMaximumWidth(300);
    setMaximumHeight(350);
    setMinimumWidth(300);
    setMinimumHeight(350);

    createTrayIcon();

    // init clock
    clock = new FocusClock(settings);
    //clock->setSettings(settings);
    QWidget *clockWidget = QWidget::createWindowContainer(clock,this);
    clockWidget->setGeometry(0,0,300,300);


    //  connect(showMessageButton, &QAbstractButton::clicked, this, &Window::showMessage);
    // connect(showIconCheckBox, &QAbstractButton::toggled, trayIcon, &QSystemTrayIcon::setVisible);
    // connect(iconComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //         this, &Window::setIconI);

    //QVBoxLayout *mainLayout = new QVBoxLayout(this);


    //mainLayout->addWidget(clockWidget); ??

    // init buttons
    QGroupBox *messageGroupBox = new QGroupBox(this);
    QGridLayout *messageLayout = new QGridLayout();

    buttonStart = new QPushButton(tr("Start"));
    buttonPause = new QPushButton(tr("Pause"));
    buttonStop = new QPushButton(tr("Stop"));
    buttonSettings = new QPushButton(tr("Settings"));

    messageLayout->addWidget(buttonStart, 0, 0);
    messageLayout->addWidget(buttonPause, 0, 0);
    buttonPause->hide();
    messageLayout->addWidget(buttonStop, 0, 1);
    messageLayout->addWidget(buttonSettings, 0, 2);
    messageGroupBox->setLayout(messageLayout);
    messageGroupBox->setGeometry(0,300,300,50);
    //mainLayout->addWidget(messageGroupBox);

    //setLayout(mainLayout);

    //    QGroupBox *messageGroupBox2 = new QGroupBox(this);
    //    QGridLayout *messageLayout2 = new QGridLayout();
    //    buttonStart2 = new QPushButton(tr("Start"));
    //    buttonStop2 = new QPushButton(tr("Stop"));

    //    messageLayout2->addWidget(buttonStart2, 0, 0);
    //    messageLayout2->addWidget(buttonStop2, 1, 0);
    //    messageGroupBox2->setLayout(messageLayout2);
    //    messageGroupBox2->setGeometry(0,100,300,100);

    //    clockWidget->hide();

    connect(buttonStart, &QAbstractButton::clicked, clock, &FocusClock::start);
    connect(buttonPause, &QAbstractButton::clicked, clock, &FocusClock::pause);
    connect(buttonStop, &QAbstractButton::clicked, clock, &FocusClock::stop);
    connect(buttonSettings, &QAbstractButton::clicked, dialog, &Dialog::show);

    //connect(buttonStart2, &QAbstractButton::clicked, clock, &FocusClock::start);
    //connect(buttonStop2, &QAbstractButton::clicked, clock, &FocusClock::stop);

    connect(clock,&FocusClock::callStart,this,&Window::setStart);
    connect(clock,&FocusClock::callPause,this,&Window::setPause);
    connect(clock,&FocusClock::callStop,this,&Window::setStop);
    connect(clock,&FocusClock::callTimerEnd,this,&Window::manageTimerEnd);

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Window::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);

    connect(&trayTimer, SIGNAL(timeout()), this, SLOT(updateTrayTimer()));

    connect(actionStop, &QAction::triggered, clock, &FocusClock::stop);
    connect(actionStart, &QAction::triggered, clock, &FocusClock::start);
    connect(actionPause, &QAction::triggered, clock, &FocusClock::pause);

  //  connect(settings, &Settings::callChangeSettings, this, &Window::updateSettings);

    soundPlayer = new QMediaPlayer;
    soundPlayer->setMedia(QUrl(QFileInfo(settings->value("Alarm/sound").toString()).filePath()));
    soundPlayer->setVolume(settings->value("Alarm/volumeInit").toInt());
    volumeLevel=settings->value("Alarm/volumeInit").toInt();

}
/*
 *
 */
void Window::setVisible(bool visible)
{
    qDebug() << "visible";
    //minimizeAction->setEnabled(visible);
    //maximizeAction->setEnabled(!isMaximized());
    //   restoreAction->setEnabled(isMaximized() || !visible);☺
    QMainWindow::setVisible(visible);
}

/*
 *
 */
void Window::setStart() {
    buttonStart->hide();
    buttonPause->show();

    if(clock->isBreak()) {
        setIcon("break");

    } else {
       setIcon("active");
    }

    trayTimer.start(200);

    reinit();

}
/*
 *
 */
void Window::setPause() {
    buttonPause->hide();
    buttonStart->show();
    setIcon("pause");
    trayTimer.stop();

    reinit();
}
/*
 *
 */
void Window::setStop() {
    buttonPause->hide();
    buttonStart->show();
    setIcon("off");
    trayTimer.stop();

    reinit();
}
/*
*/
void Window::stopSimpleTimer(int *id) {
    if(*id > 0) {
        killTimer(*id);
        *id=0;
    }

    if(id == &_trayIconTimer) {
        updateTrayTimer();
    }
}
/**/
void Window::reinit()
{
    stopSimpleTimer(&_trayIconTimer);

    soundTik=0;
    soundTry=0;
    volumeLevel=settings->value("Alarm/volumeInit").toInt();

    soundPlayer->setMedia(QUrl(QFileInfo(settings->value("Alarm/sound").toString()).filePath()));
    soundPlayer->setVolume(settings->value("Alarm/volumeInit").toInt());
    soundPlayer->stop();
}
/*
 *
 */
void Window::manageTimerEnd() {
    buttonPause->hide();
    buttonStart->show();

    _trayIconTimer = startTimer(1000);

    //setIcon("off");
    updateTrayTimer();
    trayTimer.stop();
    soundPlayer->play();

    if(clock->isBreak()) {
        trayIcon->showMessage(settings->value("Message/breakTitle").toString(), settings->value("Message/break").toString(), icons["break"], 2000);
    } else {
        trayIcon->showMessage(settings->value("Message/workTitle").toString(),settings->value("Message/work").toString(), icons["active"], 2000);
    }
}
/**/
void Window::manageSoundTik()
{
/*    3 раза полностью повторялся,
    15 сек перерыв, 3 раза повторился,
    20 сек перерыв, 4 раза повторился,
    минута перерыв, 4 раз повторился и
    минута перерыв, 4 раз повторился,
*/
    if( soundTry < 2 ) {
        if(soundTry==0
            || (soundTry==1 && soundTik>=15)
                ) {
            runSound();
        }
    } else {
        if((soundTry==2 && soundTik>=20)
            || (soundTry>=3 && soundTik>=60)
                                ) {
            soundPlayer->setMedia(QUrl(QFileInfo(settings->value("Alarm/soundLong").toString()).filePath()));
            runSound();
        }
    }
//    if(soundTik>=(settings->value("Alarm/time").toInt()+settings->value("Alarm/wait").toInt())) {
//        qDebug() << "play Sound";
//        soundPlayer->play();
//        soundTik=0;
//        if(volumeLevel < 100) {
//            volumeLevel+=settings->value("Alarm/volumeStep").toInt();
//            soundPlayer->setVolume(volumeLevel);
//        }

//        soundTry++;

//    } else if(soundTik==settings->value("Alarm/time").toInt()) {
//        soundPlayer->stop();
//        qDebug() << "stop Sound";
//    }

    soundTik++;
}

void Window::runSound()
{
    soundPlayer->play();
    soundTik=0;
    soundTry++;
    if(volumeLevel < 100) {
        volumeLevel+=settings->value("Alarm/volumeStep").toInt();
        soundPlayer->setVolume(volumeLevel);
    }
}

/*
 *  обновляем время в трее
*/
void Window::updateTrayTimer()
{
    //qDebug() << clock->timeLeftString() ;
    trayIcon->setToolTip( clock->timeLeftString() );
}

/*
 *
 */
void Window::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _trayIconTimer) {
        if(icoCurrent=="off") {
            if(clock->isTimer()) {
                setIcon("break");
            } else if(clock->isBreak()) {
                setIcon("active");
            } else if(clock->isBreak()) {
                setIcon("pause");
            }
        } else {
            setIcon("off");
        }
        manageSoundTik();

    }
}
/*
 *
 */
void Window::setIcon(QString icon)
{
    if(icon=="active") {
        ico = icons["active"];
    } else if(icon=="break") {
        ico = icons["break"];
    } else if(icon=="pause") {
        ico = icons["pause"];
    } else {
        ico = icons["off"];
    }

    icoCurrent=icon;

    setWindowIcon(ico);
    dialog->setWindowIcon(ico);
    Sleep(100);
    trayIcon->setIcon(ico);

}
/*
 *
 */
void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //showWindow();

    switch (reason) {
    case QSystemTrayIcon::Trigger:
        showWindow();
        //trayIcon->showMessage("Таймер",  clock->timeLeftString(),  ico,
        //                      1 * 1000);
        break;
    case QSystemTrayIcon::DoubleClick:
        showWindow();
        //qDebug() << "::DoubleClick";
        // iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
        //    case QSystemTrayIcon::MiddleClick:
        //        qDebug() << "::MiddleClick";

        //        break;
    default:
        ;
    }
}

/*
 *
 */
void Window::messageClicked()
{
    showWindow();

    //    qDebug() << "messageClicked";
    //    QMessageBox::information(0, tr("Clear Pomidor"),
    //                             tr("Sorry, I already gave what help I could.\n"
    //                                "Maybe you should try asking a human?"));
}
/*
 *
 */
void Window::fillIcons() {
    icons.insert("active",QIcon(":/images/active.png"));
    icons.insert("break",QIcon(":/images/break.png"));
    icons.insert("pause",QIcon(":/images/pause.png"));
    icons.insert("off",QIcon(":/images/off.png"));
}

/*
 *
 */
void Window::createActions()
{
    //  minimizeAction = new QAction(tr("Mi&nimize"), this);
    //   connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    //   maximizeAction = new QAction(tr("Ma&ximize"), this);
    //    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    actionStart = new QAction(tr("&Старт"), this);

    actionStop = new QAction(tr("&Стоп"), this);

    actionPause = new QAction(tr("&Пауза"), this);


    showClockAction = new QAction(QObject::tr("&Таймер"), this);
    connect(showClockAction, &QAction::triggered, this, &Window::showWindow);

    actionRestore = new QAction(tr("&Настройки"), this);
    connect(actionRestore, &QAction::triggered, dialog, &QWidget::showNormal );

    actionQuit = new QAction(tr("&Выход"), this);
    connect(actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
}
/*
 *
 */
void Window::createTrayIcon()
{
    fillIcons();
    createActions();
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(actionStart);
    trayIconMenu->addAction(actionPause);
    trayIconMenu->addAction(actionStop);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(showClockAction);
    trayIconMenu->addAction(actionRestore);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionQuit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    setIcon("off");

    trayIcon->show();
    //trayIcon->setToolTip("test text");

}
/*
*
*/
void Window::showWindow()
{
    //&QWidget::showNormal
    //setWindowState(Qt::WindowActive);
    raise();
    activateWindow();
    showNormal();

}


/**/
void Window::updateSettings()
{

}


/*
 *
 */
void Window::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif

    hide();
    event->ignore();

}

#endif
