#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <QWidget>

class Thermometer : public QWidget
{
    Q_OBJECT
public:
    explicit Thermometer(QWidget *parent = nullptr,
                         double maxTemperature = 10,
                         double minTemperature = -5);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

signals:

public slots:
    void on_levelMoving_time();

private:
    int setLevelAt(double temperature);
    QLinearGradient setThermometerColors(QPoint, QPoint);

private:
    double currentTemperature;
    //TODO: make it static
    double maxTemperature, minTemperature;
    double lowestWarmTemperature, highestColdTemperature;
    int levelPosition;
    QColor warmColor, ambiantColor, coldColor;
    unsigned int warmAlpha, ambiantAlpha, coldAlpha;
    QTimer *paintTimer, *levelMovingTimer;
};

#endif // THERMOMETER_H
