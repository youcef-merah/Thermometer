#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <QWidget>
#include <QLCDNumber>

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
    int getLevelAt(const double temperature);
    double getTemperatureAt(const int level);


signals:
    void levelChanged(void);

public slots:
    void setCurrentTemperature(double value);

private slots:
    void updateLevelPosition();
    void displayTemperature();

private: /* Properties */
    double currentTemperature;
    double maxTemperature, minTemperature;
    int levelPosition;
    QTimer *levelUpdatingTime;
    QTimer *temperatureDisplayiTime;
    QLCDNumber *lcdTemp;
};

#endif // THERMOMETER_H
