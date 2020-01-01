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

private:
    double currentTemperature;
    double maxTemperature, minTemperature;
    double lowestWarmTemperature, highestColdTemperature;
    QPoint level;
    QColor warmColor, ambiantColor, coldColor;
    unsigned int warmAlpha, ambiantAlpha, coldAlpha;

};

#endif // THERMOMETER_H
