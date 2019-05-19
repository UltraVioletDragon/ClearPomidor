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

#include "dialog.h"

#ifndef QT_NO_SYSTEMTRAYICON

//#include <QAction>
//#include <QCheckBox>
//#include <QComboBox>
//#include <QCoreApplication>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
//#include <QMenu>
#include <QPushButton>
//#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtGui>

//! [0]
Dialog::Dialog(Settings *setting)
{
    settings = setting;

    createGBTimer();
    createGBMessage();

    QPushButton *submit = new QPushButton(tr("Сохранить"));
    connect(submit, &QAbstractButton::clicked, this, &Dialog::saveSettings);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(iconGroupBox);
    //mainLayout->addWidget(messageGroupBox);
    mainLayout->addWidget(gbTimer);
    mainLayout->addWidget(gbMessage);
    mainLayout->addWidget(submit);

    setLayout(mainLayout);

    setWindowTitle("Clear Pomidor - Settings");
    resize(400, 380);
}

//! [6]

void Dialog::createGBTimer() {
    gbTimer = new QGroupBox("Таймер");

    QLabel *timerWorkTitle = new QLabel(tr("Работа"));
    int workMin=0;
    int workSec=settings->value("Timer/work").toInt();
    if(workSec > 60) {
        workMin = floor(workSec/60);
        workSec-=workMin*60;
    }

    timerWorkMin = new QLineEdit(QString("%1").arg(workMin));
    timerWorkSec = new QLineEdit(QString("%1").arg(workSec));

    timerWorkMin->setValidator( new QIntValidator(0, 999, this) );
    timerWorkSec->setValidator( new QIntValidator(0, 59, this) );
    timerWorkMin->setMaximumWidth(25);
    timerWorkSec->setMaximumWidth(25);


    QLabel *timerBreakTitle = new QLabel(tr("Перерыв"));
    int breakMin=0;
    int breakSec=settings->value("Timer/break").toInt();
    if(breakSec > 60) {
        breakMin = floor(breakSec/60);
        breakSec-=breakMin*60;
    }

    timerBreakMin = new QLineEdit(QString("%1").arg(breakMin));
    timerBreakSec = new QLineEdit(QString("%1").arg(breakSec));

    timerBreakMin->setValidator( new QIntValidator(0, 999, this) );
    timerBreakSec->setValidator( new QIntValidator(0, 59, this) );
    timerBreakMin->setMaximumWidth(25);
    timerBreakSec->setMaximumWidth(25);


    QGridLayout *messageLayout = new QGridLayout;
    messageLayout->addWidget(timerWorkTitle, 0, 0);
    messageLayout->addWidget(timerWorkMin, 0, 1);
    messageLayout->addWidget(timerWorkSec, 0, 2);
    messageLayout->addWidget(timerBreakTitle, 1, 0);
    messageLayout->addWidget(timerBreakMin, 1, 1);
    messageLayout->addWidget(timerBreakSec, 1, 2);

    messageLayout->setColumnStretch(2, 0);
    messageLayout->setRowStretch(1, 1);
    gbTimer->setLayout(messageLayout);

}


void Dialog::createGBMessage() {
    gbMessage = new QGroupBox("Тексты");

    QLabel *workLabel = new QLabel(tr("Работа"));
    workTitle = new QLineEdit(settings->value("Message/workTitle").toString());
    workMsg = new QTextEdit(settings->value("Message/work").toString());


    QLabel *breakLabel = new QLabel(tr("Перерыв"));
    breakTitle = new QLineEdit(settings->value("Message/breakTitle").toString());
    breakMsg = new QTextEdit(settings->value("Message/break").toString());

    //bodyEdit = new QTextEdit;
    //bodyEdit->setPlainText(tr("Don't believe me. Honestly, I don't have a "
    //                          "clue.\nClick this balloon for details."));

    QGridLayout *messageLayout = new QGridLayout;
    messageLayout->addWidget(workLabel, 0, 0);
    messageLayout->addWidget(workTitle, 0, 1);
    messageLayout->addWidget(workMsg, 1, 0, 1, 2);
    messageLayout->addWidget(breakLabel, 2, 0);
    messageLayout->addWidget(breakTitle, 2, 1);
    messageLayout->addWidget(breakMsg, 3, 0, 1, 2);

    messageLayout->setColumnStretch(1, 0);
    messageLayout->setRowStretch(2, 1);
    gbMessage->setLayout(messageLayout);

}
/*
*/
void Dialog::saveSettings()
{
    settings->setValue("Timer/work", timerWorkSec->text().toInt() + timerWorkMin->text().toInt()*60);
    settings->setValue("Timer/break", timerBreakSec->text().toInt() + timerBreakMin->text().toInt()*60);

    settings->setValue("Message/workTitle", workTitle->text());
    settings->setValue("Message/work", workMsg->toPlainText());

    settings->setValue("Message/breakTitle", breakTitle->text());
    settings->setValue("Message/break", breakMsg->toPlainText());

    settings->save();

    close();
}



#endif
