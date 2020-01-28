#include "thermometer.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>

Thermometer::Thermometer(QWidget *parent, double max, double min)
    :QWidget(parent)
{
    setWindowTitle(tr("Temperature"));
    this->setGeometry(QRect(20, 30, 150, 300));

    maxTemperature = max;
    minTemperature = min;

    levelUpdatingTime = new QTimer();
    levelUpdatingTime->setInterval(100);
    temperatureDisplayiTime = new QTimer();
    temperatureDisplayiTime->setInterval(100);

    levelPosition = getLevelAt(currentTemperature = (max+min)/2);

    lcdTemp = new QLCDNumber(this);
    lcdTemp->display(currentTemperature);
    lcdTemp->setDigitCount(4);
    lcdTemp->setStyleSheet("background-color: black;"
                           "color: darkred;"
                           "border: 2px solid gray;");

    connect(levelUpdatingTime, &QTimer::timeout,
            this, QOverload<>::of(&Thermometer::update));

    connect(temperatureDisplayiTime, &QTimer::timeout,
            this, &Thermometer::displayTemperature);
}

Thermometer::~Thermometer()
{
    delete levelUpdatingTime;
    delete lcdTemp;
}

void Thermometer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPoint topLeft;
    QRect rect;

    painter.setPen(Qt::black);

    // Paint frame
    topLeft = QPoint(0, 0);
    rect = QRect(topLeft, QSize(width(), height()-1));
    QLinearGradient gradientFrame;
    gradientFrame.setStart(QPoint(0, 0));
    gradientFrame.setFinalStop(QPoint(width(), 0));
    gradientFrame.setColorAt(1,  Qt::black);
    gradientFrame.setColorAt(0,  QRgb(0x43434));
    painter.setBrush(gradientFrame);
    painter.drawRoundedRect(rect, 5, 5);

    // Paint gauge
    const int marginGTop = static_cast<int>(height()*0.20);
    const int marginGBottom = static_cast<int>(height()*0.25);
    const int marginG = static_cast<int>(width()*0.45);
    const int widthGauge = width() - 2*marginG;
    const int heightGauge = height() - marginGBottom - levelPosition;
    topLeft = QPoint(marginG, levelPosition);
    rect = QRect(topLeft, QSize(widthGauge, heightGauge));

    painter.setBrush(Qt::darkRed);
    painter.drawRoundedRect(rect, 10, 10);

    //Paint gauge glass
    const int heightGaugeGlass = height() - marginGBottom - marginGTop;
    topLeft = QPoint(marginG, marginGTop);
    rect = QRect(topLeft, QSize(widthGauge, heightGaugeGlass));
    QLinearGradient gradientGauge;
    gradientGauge.setStart(QPoint(marginG, levelPosition));
    gradientGauge.setFinalStop(QPoint(marginG + widthGauge, levelPosition));
    gradientGauge.setColorAt(0, Qt::darkRed);
    gradientGauge.setColorAt(0.6, Qt::black);

    gradientGauge.setStart(topLeft);
    gradientGauge.setFinalStop(QPoint(marginG + widthGauge, marginGTop));
    gradientGauge.setSpread(QGradient::RepeatSpread);
    gradientGauge.setColorAt(0, QColor(0xff, 0xff, 0xff, 0x00));
    gradientGauge.setColorAt(0.8, QColor(0xff, 0xff, 0xff, 0x55));
    gradientGauge.setColorAt(1, QColor(0xff, 0xff, 0xff, 0x00));

    painter.setBrush(gradientGauge);
    painter.setBrushOrigin(marginG, levelPosition);
    painter.drawRoundedRect(rect, 10, 10);

   //Set LCDNumber geometry
    const int marginLCDTop = marginGTop + heightGaugeGlass
                           + static_cast<int>(height()*0.05);
    const int marginLCDBottom = static_cast<int>(height()*0.09);
    const int marginLCD = static_cast<int>(width()*0.23);
    const int widthLCD = width() - 2*marginLCD;
    const int heightLCD = height() - marginLCDBottom - marginLCDTop;

    topLeft = QPoint(marginLCD, marginLCDTop);
    rect = QRect(topLeft, QSize(widthLCD, heightLCD));
    lcdTemp->setGeometry(rect);

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
int Thermometer::getLevelAt(const double temperature)
{
    /* First we set the size and the position of the gauge accordind its
     * representation in paintEvent after
     * "Rounded Rectangle - temperature color - foreground" comment */
    const int marginGTop = static_cast<int>(height()*0.20);
    const int marginGBottom = static_cast<int>(height()*0.25);
    const int marginG = static_cast<int>(width()*0.45);
    const int H = height() - marginGBottom - marginGTop;
    const int pos_y = marginG;

    const double scale = H / (maxTemperature - minTemperature);
    const double deltaTemperature = (maxTemperature - temperature);
    return static_cast<int>(pos_y + scale * deltaTemperature);
}

/* Returns the temperature correspondingt to the level 'level'
 * Tis fuction is not the inverse of getLevelAt which is NOT
 * bijective (living in N).
 * Consider getLevelAt looks like the floor function
 */
double Thermometer::getTemperatureAt(const int level)
{
     const int marginGTop = static_cast<int>(height()*0.20);
     const int marginGBottom = static_cast<int>(height()*0.25);
     const int marginG = static_cast<int>(width()*0.45);
     const int H = height() - marginGBottom - marginGTop;
     const int pos_y = marginG;

     const double inv_scale = (maxTemperature - minTemperature)/ H;
     const int delta_y = level - pos_y;
     return maxTemperature - (delta_y*inv_scale);
}

void Thermometer::setCurrentTemperature(double newTemperature)
{
    if (newTemperature > maxTemperature)
        newTemperature = maxTemperature;
    else if(newTemperature < minTemperature)
        newTemperature = minTemperature;

    currentTemperature = newTemperature;

    levelUpdatingTime->start();
}

void Thermometer::updateLevelPosition()
{
    if(levelPosition == getLevelAt(currentTemperature))
    {
        lcdTemp->display(currentTemperature);
        levelUpdatingTime->stop();
    }
    else
    {
        if(levelPosition > getLevelAt(currentTemperature))
        levelPosition--;
        else
        levelPosition++;

        temperatureDisplayiTime->start();
        emit levelChanged();
    }
}

void Thermometer::displayTemperature()
{
   lcdTemp->display(getTemperatureAt(levelPosition));
   temperatureDisplayiTime->stop();
}

void Thermometer::setMinTemperature(double value)
{
    minTemperature = value;
}

void Thermometer::setMaxTemperature(double value)
{
    maxTemperature = value;
}
