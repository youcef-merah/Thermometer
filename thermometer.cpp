#include "thermometer.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>

Thermometer::Thermometer(QWidget *parent, double max, double min)
    :QWidget(parent)
{
    setWindowTitle(tr("Temperature"));
    this->setGeometry(QRect(20, 50, 200, 500));

    maxTemperature = max;
    minTemperature = min;
    lowestWarmTemperature  = (2*max + min)/3;
    highestColdTemperature = (max + 2*min)/3;

    temperatureColor = Qt::darkRed;

    levelUpdatingTime = new QTimer();
    levelUpdatingTime->setInterval(100);
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
    QPoint topLeft;
    QRect rect;

    QLinearGradient gradientFrame;
    // Paint frame
    //background-image: linear-gradient(to right, #434343 0%, black 100%);
    topLeft = QPoint(0, 0);
    rect = QRect(topLeft, QSize(width(), height()));
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    gradientFrame.setStart(QPoint(0, 0));
    gradientFrame.setFinalStop(QPoint(width(), 0));
    gradientFrame.setColorAt(1,  Qt::black);
    gradientFrame.setColorAt(0,  QRgb(0x43434));
    painter.setBrush(gradientFrame);

    painter.drawRoundedRect(rect, 50, 50);

    //Paint circle
    painter.setPen(Qt::transparent);
    const int marginC = static_cast<int>(width()*0.25);
    const int diameter = width() - 2*marginC;
    topLeft = QPoint(marginC, height() - diameter - marginC);
    rect = QRect(topLeft, QSize(diameter, diameter));
    painter.setBrush(temperatureColor);
    painter.drawEllipse(rect);

    //Paint gauge
    const int marginG = static_cast<int>(width()*0.45);
    const int widthGauge = width() - 2*marginG;
    topLeft = QPoint(marginG, levelPosition);
    rect = QRect(topLeft, QSize(widthGauge,
                                height() - diameter - marginC - levelPosition));

    painter.setBrush(temperatureColor);
    painter.drawRect(rect);

    //Paint gauge glass
    topLeft = QPoint(marginG, marginG);
    rect = QRect(topLeft, QSize(widthGauge,
                                height() - diameter - marginC - marginG));

    QLinearGradient gradientGauge;
    gradientGauge.setStart(QPoint(marginG, levelPosition));
    gradientGauge.setFinalStop(QPoint(marginG + widthGauge, levelPosition));
    gradientGauge.setColorAt(0, temperatureColor);
    gradientGauge.setColorAt(0.6, Qt::black);

    gradientGauge.setStart(topLeft);
    gradientGauge.setFinalStop(QPoint(marginG + widthGauge, marginG));
    gradientGauge.setSpread(QGradient::RepeatSpread);
    gradientGauge.setColorAt(0, QColor(0xff, 0xff, 0xff, 0x00));
    gradientGauge.setColorAt(0.8, QColor(0xff, 0xff, 0xff, 0x55));
    gradientGauge.setColorAt(1, QColor(0xff, 0xff, 0xff, 0x00));

    painter.setBrush(gradientGauge);
    painter.setBrushOrigin(marginG, levelPosition);
    painter.drawRect(rect);

    //Paint Circle glass
    topLeft = QPoint(marginC, height() - diameter - marginC);
    rect = QRect(topLeft, QSize(diameter, diameter));

    QLinearGradient gradientCircle;
    gradientCircle.setStart(topLeft);
    gradientCircle.setFinalStop(QPoint(width() - marginC, height() - diameter - marginC ));
    gradientCircle.setColorAt(0, temperatureColor);
    gradientCircle.setColorAt(0.6, Qt::black);

    gradientCircle.setColorAt(0, QColor(0xff, 0xff, 0xff, 0x00));
    gradientCircle.setColorAt(0.8, QColor(0xff, 0xff, 0xff, 0x55));
    gradientCircle.setColorAt(1, QColor(0xff, 0xff, 0xff, 0x00));

    painter.setBrush(gradientCircle);
    //painter.setBrush(temperatureColor);


    painter.drawEllipse(rect);
    updateLevelPosition();
}

/* Given 'pos_y' the ordinatee of the top left corner of the thermometer gauge,
 * and given 'H' its height.
 * Given max and min the respective maximum and minimum temperature level,
 * and given temp the current temperature and temp_y its corresponding
 * ordinatee on the widget.
 *
 * The scale between the height of the gauge and the
 * correspondings real values range (in celcius) is:
 *               scale := H / (max - min)
 *
 * We define the distance between pos_y and temp_y (according to the QT graphic
 * representation) by:
 *               delta_y := temp_y - pos_y
 * And the distance: deltaTemp =  max - temp
 * Obviously     deltaTemp = scale * delta_y
 *           <=> temp_y - pos_y = scale * (max - min)
 *           <=> temp_y = pos_y + scale * deltaTemp
 */
int Thermometer::getLevelAt(double temperature)
{
    /* First we set the size and the position of the gauge accordind its
     * representation in paintEvent after
     * "Rounded Rectangle - temperature color - foreground" comment */
    const int marginC = static_cast<int>(width()*0.25);
    const int marginG = static_cast<int>(width()*0.45);
    const int diameter = width() - 2*marginC;
    const int H = height() - diameter - marginC - marginG;
    const int pos_y = marginG;

    double scale = H / (maxTemperature - minTemperature);
    double deltaTemperature = (maxTemperature - temperature);
    return static_cast<int>(pos_y + scale * deltaTemperature);
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

void Thermometer::updateLevelPosition()
{
    if(levelPosition < getLevelAt(currentTemperature))
        levelPosition++;
    else if(levelPosition > getLevelAt(currentTemperature))
        levelPosition--;
    else
        levelUpdatingTime->stop();
}
