#ifndef QPLOT_H
#define QPLOT_H

#include <QObject>
#include <QTimer>
#include <QFont>
#include "qcustomplot.h"

class Qplot : public QObject
{
  Q_OBJECT

public:

  explicit Qplot(QObject *parent = nullptr);
  ~Qplot();

  void plot_update();
  QVBoxLayout  *plot_layout;


private:

  QCustomPlot *plot;
  QVector<double> plot_x;
  QVector<double> plot_y;

  QTimer *plot_timer;

public slots:
  void plot_timer_timeout_slot();
};

#endif
