#include "thermometer.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>

Thermometer::Thermometer(QWidget *parent, double max, double min)
    :QWidget(parent)
{
    setWindowTitle(tr("Temperature"));
    this->setGeometry(QRect(0, 0, 15, 150));

    currentTemperature = 0.00;
    maxTemperature = max;
    minTemperature = min;
    lowestWarmTemperature  = (2*max + min)/3;
    highestColdTemperature = (max + 2*min)/3;

    this->setLevelAt(currentTemperature);

    warmColor = QColor(QRgb(0xff4e50));
    ambiantColor= QColor(QRgb(0x0fd850));
    coldColor = QColor(QRgb(0x3ab5b0));

    warmColor.setAlpha(warmAlpha = 0xff);
    ambiantColor.setAlpha(ambiantAlpha = 0xff);
    coldColor.setAlpha(coldAlpha = 0xff);

    connect(paintTimer, &QTimer::timeout,
            this, QOverload<>::of(&Thermometer::update));
    connect(levelMovingTimer, &QTimer::timeout,
            this, &Thermometer::on_levelMoving_time);
    paintTimer->start(1000);
    paintTimer->start(1000);


}
void Thermometer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    const QPoint bottom_right(pos().x() + width(), pos().y() + height());

    /* Draw the thermometer gauge*/
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setBrush(Qt::black);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    QLinearGradient gaugeColor =
                    this->setThermometerColors(pos(), bottom_right);
    painter.setBrush(QBrush(gaugeColor));
    painter.drawRoundedRect(QRectF(pos(), bottom_right), 7, 7);

    /* Draw the level cursor */
    const QPoint left(pos().x(), levelPosition);
    const QPoint right(bottom_right.x(), levelPosition);
    painter.setBrush(QBrush());
    painter.drawLine(left, right);
}


/* Given 'pos.y' the ordinatee of the top right corner of the thermometer,
 * and given 'h' its height.
 * Given max and min the respective maximum and minimum temperature level,
 * and given level the current temperature and level.y its corresponding
 * ordinatee on the widget.
 *
 * The the scale between the height of the thermometer widget and the
 * correspondings real values is:
 *               scale := h / (max - min)
 * We define the distance between pos.y and level.y by
 *               delta.y := level.y - pos.y
 * Obviously     level = max - delta.y/scale
 *
 * Then we obtain:
 *      level.y = (max - level)*scale + pos.y
 */
int Thermometer::setLevelAt(double temperature)
{
    double scale = height() / (maxTemperature - minTemperature);
    // Now we compute delta := delta.y/scale
    double deltaTemperature = (maxTemperature - temperature);
    levelPosition = static_cast<int>(deltaTemperature*scale + pos().y());
    return levelPosition;
}

QLinearGradient Thermometer::setThermometerColors(QPoint topLeft,
                                                  QPoint bottomRight)
{
    QLinearGradient gaugeColor(topLeft, bottomRight);
    gaugeColor.setColorAt(0.0, warmColor);
    gaugeColor.setColorAt(0.5, ambiantColor);
    gaugeColor.setColorAt(1, coldColor);

    return gaugeColor;
}

