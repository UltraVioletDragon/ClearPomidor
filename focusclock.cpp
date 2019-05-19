#include "focusclock.h"
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
/*
 *
 */
//FocusClock::FocusClock(QWindow *parent, Settings *setting)
//    : QWindow(parent)
//      , m_backingStore(new QBackingStore(this))
//{
FocusClock::FocusClock(Settings *setting)
    : m_backingStore(new QBackingStore(this))
{

    settings = setting;
    m_timerId = startTimer(200);

    timeMax = settings->value("Timer/work").toInt()*1000;
    mainTimer.setTimerType(Qt::PreciseTimer);
    mainTimerBreak.setTimerType(Qt::PreciseTimer);

    connect(&mainTimer, SIGNAL(timeout()), this, SLOT(timerEnd()));
    connect(&mainTimerBreak, SIGNAL(timeout()), this, SLOT(timerEndBreak()));
    connect(settings, &Settings::callChangeSettings, this, &FocusClock::updateSettings);

    _isTimer=true;
    _isBreak=false;

    calcLeftTime();
    // start();

}

/*
 *
 */
bool FocusClock::event(QEvent *event)
{

    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

/*
 *
 */
void FocusClock::renderLater()
{
    requestUpdate();
}

/*
 *
 */
void FocusClock::resizeEvent(QResizeEvent *resizeEvent)
{
    m_backingStore->resize(resizeEvent->size());
}

/*
 *
 */
void FocusClock::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        renderNow();
}

/*
 *
 */
void FocusClock::renderNow()
{

    calcLeftTime();

    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    QColor colorWhite(255, 255, 255);
    QColor colorGray(192,192,192);
    //QColor colorRed(255,99,71);

    painter.fillRect(0, 0, width(), height(), colorWhite);

    painter.setRenderHint(QPainter::Antialiasing);
    int side = qMin(width(), height());
    painter.scale(side / 300.0, side / 300.0);

    painter.setPen(QPen(colorGray, 8, Qt::SolidLine, Qt::FlatCap, Qt::BevelJoin));
    painter.drawArc( 25,25, 250, 250, (0)*16, (360)*16 );

    if(_isBlur) {
        painter.setOpacity(0.2);
    }
    render(&painter);

    painter.end();

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}


/*
 *
 */
void FocusClock::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId && (mainTimer.isActive() || mainTimerBreak.isActive())) {
        calcLeftTime();
        renderLater();
    }
}

/*
 *
 */
void FocusClock::start()
{
    if(_isTimer) {
        //qDebug()<<"Start Timer";
        if( mainTimerLeft > 0 ) {
            mainTimer.start(mainTimerLeft);
        } else {
            mainTimer.start(timeMax);
        }
        emit callStart();
    } else if(_isBreak) {
       // qDebug()<<"Start Break";
        if( mainTimerLeft > 0 ) {
            mainTimerBreak.start(mainTimerLeft);
        } else {
            mainTimerBreak.start(timeMax);
        }
        emit callStart();
    }
    _isBlur=false;
}

/*
 *
 */
void FocusClock::timerEnd()
{
    //qDebug()<<"Timer End";

    _isTimer=false;
    _isBreak=true;

    mainTimer.stop();
    mainTimerLeft=0;
    timeMax = settings->value("Timer/break").toInt()*1000;

    _isBlur=true;

    renderNow();
    emit callTimerEnd();
}
/*
 *
 */
void FocusClock::timerEndBreak()
{
   // qDebug()<<"Break End";

    _isTimer=true;
    _isBreak=false;

    mainTimerBreak.stop();
    mainTimerLeft=0;
    timeMax = settings->value("Timer/work").toInt()*1000;

    _isBlur=true;

    renderNow();
    emit callTimerEnd();

    //stop();

}
/*
 *
 */
void FocusClock::pause()
{
    if(mainTimer.isActive()) {
        mainTimerLeft=mainTimer.remainingTime();
        mainTimer.stop();
    }
    if(mainTimerBreak.isActive()) {
        mainTimerLeft=mainTimerBreak.remainingTime();
        mainTimerBreak.stop();
    }

    emit callPause();
}
/*
 *
 */
void FocusClock::stop()
{
    //emit changeIcon("off");
    emit callStop();
    mainTimerLeft=0;
    timeMax=settings->value("Timer/work").toInt()*1000;

    _isTimer=true;
    _isBreak=false;

   // qDebug()<<"Timer STOP";
    if(mainTimer.isActive()) {
        mainTimer.stop();
    }
    if(mainTimerBreak.isActive()) {
        mainTimerBreak.stop();
    }

    renderNow();

}
/*
 *
 */
void FocusClock::render(QPainter *p)
{
    if(_isBreak) {
        colorTimer = QColor(102,255,102); // green
    } else {
        colorTimer = QColor(255,99,71); // red
    }

    // Draw timeLeft
    p->save();
    p->setPen(QPen(colorTimer, 8, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    p->drawArc( 25, 25, 250, 250, 90*16, 16 * (360 * (timeMax-getRemainingTime())/timeMax));

    // Draw timer
    QFont font = p->font();
    font.setPixelSize(40);
    p->setFont(font);
    p->setPen(QPen(colorTimer, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));

    //calcLeftTime();
    p->drawText(90, 160, prepeareNumber(leftMinutes));
    p->drawText(138, 160, ":");
    p->drawText(155, 160, prepeareNumber(leftSeconds));

    //p->drawText(155, 200, prepeareNumber(((timeLeft-leftMinutes*60000-leftSeconds*1000))));
    //p->drawText(90, 100, QString("%1").arg((360*(timeMax-getRemainingTime())/timeMax), 3, 10, QChar(' ')));

    p->restore();

}

/*
*/
void FocusClock::calcLeftTime()
{
    if(mainTimer.isActive()) {
        timeLeft = mainTimer.remainingTime();
    } else if(mainTimerBreak.isActive()) {
        timeLeft = mainTimerBreak.remainingTime();
    } else {
        timeLeft = timeMax;
    }

    leftMinutes = (timeLeft/60000);
    leftSeconds = ((timeLeft-leftMinutes*60000)/1000);
}
/*
 *
 */
int FocusClock::getRemainingTime() {
    if(mainTimer.isActive()) {
        return round(mainTimer.remainingTime()/1000)*1000;
    } else if(mainTimerBreak.isActive()) {
        return mainTimerBreak.remainingTime();
    } else return 0;
}
/*
 *
 */
QString FocusClock::prepeareNumber(int number) {
    return QString("%1").arg(number, 2, 10, QChar('0'));
}
/*
 *
 */
QString FocusClock::timeLeftString() {
    return prepeareNumber(leftMinutes) + ":" + prepeareNumber(leftSeconds);
}
/*
 *
 */
bool FocusClock::isTimer()
{
    return _isTimer;
}
/*
 *
 */
bool FocusClock::isBreak()
{
    return _isBreak;
}


/**/
void FocusClock::updateSettings()
{
    int oldTimeMax = timeMax;

    if(isTimer()) {
        timeMax = settings->value("Timer/work").toInt()*1000;
    } else {
        timeMax = settings->value("Timer/break").toInt()*1000;
    }

    int diff = timeMax-oldTimeMax;

    if(mainTimer.isActive() || mainTimerBreak.isActive()) {
        pause();
        if(timeMax < mainTimerLeft) {
            mainTimerLeft=timeMax;
        } else {
            mainTimerLeft+=diff;
        }
        start();
    } else {
        mainTimerLeft+=diff;
        renderNow();
    }

}
