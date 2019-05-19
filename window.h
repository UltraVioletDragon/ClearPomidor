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

#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QWidget>
#include <QMainWindow>
#include <QMediaPlayer>
//#include <QSound>

#include "focusclock.h"
#include "settings.h"
#include "dialog.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMainWindow;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();
    QAction *showClockAction;
    void setVisible(bool visible) override;

public slots:
    void setIcon(QString icon);
    void setStart();
    void setPause();
    void setStop();
    void manageTimerEnd();
    void updateTrayTimer();
    void updateSettings();

protected:
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

    void showWindow();

private:
    // void createIconGroupBox();
    // void createMessageGroupBox();
    void createActions();
    void createTrayIcon();
    void fillIcons();
    void stopSimpleTimer(int *id);
    void manageSoundTik();
    void runSound();
    void reinit();

    QTimer trayTimer;
    int _trayIconTimer;

    QPushButton *buttonStart;
    QPushButton *buttonPause;
    QPushButton *buttonStop;
    QPushButton *buttonSettings;

    QPushButton *buttonStart2;
    QPushButton *buttonStop2;

    QAction *actionMinimize;
    QAction *actionMaximize;
    QAction *actionRestore;
    QAction *actionQuit;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionPause;
    // QAction *showClockAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    FocusClock *clock;
    QIcon ico;
    QMap<QString, QIcon> icons;
    QString icoCurrent;
    //
    QMediaPlayer *soundPlayer;
    int volumeLevel=20;
    int soundTik=0;
    int soundTry=0;

    Settings *settings;
    Dialog *dialog;
};
//! [0]

#endif // QT_NO_SYSTEMTRAYICON

#endif
