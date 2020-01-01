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

    ~Thermometer() override;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    int getLevelAt(double temperature);
    QLinearGradient setThermometerColors(QPoint, QPoint);

signals:
    void temperatureChanged(double);

public slots:
    void setCurrentTemperature(double value);

private slots:
    void updateLevelPosition();

private: /* Properties */
    double currentTemperature;
    double maxTemperature, minTemperature;
    double lowestWarmTemperature, highestColdTemperature;
    int levelPosition;
    QColor warmColor, ambiantColor, coldColor;
    unsigned int warmAlpha, ambiantAlpha, coldAlpha;
    QTimer *levelUpdatingTime;
};

#endif // THERMOMETER_H
