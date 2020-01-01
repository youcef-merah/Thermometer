#include "thermometer.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>

Thermometer::Thermometer(QWidget *parent, double max, double min)
    :QWidget(parent)
{
    setWindowTitle(tr("Temperature"));
    this->setGeometry(QRect(0, 0, 15, 150));

    maxTemperature = max;
    minTemperature = min;
    lowestWarmTemperature  = (2*max + min)/3;
    highestColdTemperature = (max + 2*min)/3;

    warmColor = QColor(QRgb(0xff4e50));
    ambiantColor= QColor(QRgb(0x0fd850));
    coldColor = QColor(QRgb(0x3ab5b0));

    warmColor.setAlpha(warmAlpha = 0xff);
    ambiantColor.setAlpha(ambiantAlpha = 0xff);
    coldColor.setAlpha(coldAlpha = 0xff);

    levelUpdatingTime = new QTimer();
    levelUpdatingTime->setInterval(1000);
    levelPosition = getLevelAt(currentTemperature = (max+min)/2);

    connect(levelUpdatingTime, &QTimer::timeout,
            this, QOverload<>::of(&Thermometer::update));
}

Thermometer::~Thermometer()
{
    delete levelUpdatingTime;
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
    painter.setBrushOrigin(pos().x(), levelPosition);
    painter.drawRoundedRect(QRectF(pos(), bottom_right), 7, 7);

    /* Draw the level cursor */
    const QPoint left(pos().x(), levelPosition);
    const QPoint right(bottom_right.x(), levelPosition);
    painter.setBrush(QBrush());
    painter.drawLine(left, right);
    updateLevelPosition();
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
int Thermometer::getLevelAt(double temperature)
{
    double scale = height() / (maxTemperature - minTemperature);
    // Now we compute delta := delta.y/scale
    double deltaTemperature = (maxTemperature - temperature);
    return static_cast<int>(deltaTemperature*scale + pos().y());
}

void Thermometer::setCurrentTemperature(double newTemperature)
{
    if (newTemperature > maxTemperature)
        newTemperature = maxTemperature;
    else if(newTemperature < minTemperature)
        newTemperature = minTemperature;

    currentTemperature = newTemperature;

    levelUpdatingTime->start();

    emit temperatureChanged(newTemperature);
}

QLinearGradient Thermometer::setThermometerColors(QPoint topLeft,
                                                  QPoint bottomRight)
{
    QColor transparentTop = QColor();
    transparentTop.setAlpha(0x00);
    QLinearGradient gaugeColor(topLeft, bottomRight);
    gaugeColor.setColorAt(0.0, transparentTop);
    gaugeColor.setColorAt(0.1, warmColor);
    gaugeColor.setColorAt(0.5, ambiantColor);
    gaugeColor.setColorAt(1, coldColor);

    return gaugeColor;
}


void Thermometer::updateLevelPosition()
{
    if(levelPosition < getLevelAt(currentTemperature))
        levelPosition++;
    else if(levelPosition > getLevelAt(currentTemperature))
        levelPosition--;
    else
        levelUpdatingTime->stop();
}
