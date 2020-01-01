#include "thermometer.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>

Thermometer::Thermometer(QWidget *parent, double max, double min)
    :QWidget(parent)
{
    setWindowTitle(tr("Temperature"));
    this->setGeometry(QRect(0, 0, 10, 150));

    currentTemperature = 0.00;
    maxTemperature = max;
    minTemperature = min;
    lowestWarmTemperature  = (2*max + min)/3;
    highestColdTemperature = (max + 2*min)/3;

    //TODO: level =

    warmColor = QColor(QRgb(0xff4e50));
    ambiantColor= QColor(QRgb(0x0fd850));
    coldColor = QColor(QRgb(0x3ab5b0));

    warmColor.setAlpha(warmAlpha = 0xff);
    ambiantColor.setAlpha(ambiantAlpha = 0xff);
    coldColor.setAlpha(coldAlpha = 0xff);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this, QOverload<>::of(&Thermometer::update));
    timer->start(1000);

}

void Thermometer::paintEvent(QPaintEvent *)
{
    qDebug() << "In paintEvent";
    QPoint bottom_right(pos().x() + width(), pos().y() + height());
    QLinearGradient gauge_temp(pos(), bottom_right);
    gauge_temp.setColorAt(0.0, warmColor);
    gauge_temp.setColorAt(0.5, ambiantColor);
    gauge_temp.setColorAt(1, coldColor);

    /* Draw the thermometer */
    QPainter painter(this);
    painter.setBrush(QBrush(gauge_temp));
    painter.drawRoundedRect(QRectF(pos(), bottom_right), 5, 5);
}
