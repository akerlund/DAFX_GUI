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

  void append_to_plot(double value);
  void plot_update();
  QVBoxLayout  *plot_layout;
  void set_nr_of_x_values(int value);


private:

  void reset_x_axis();
  void reset_y_axis();
  void shift_in_to_vector(QVector<double> *vector, double value, int max_size);


private:

  QCustomPlot *plot;
  QVector<double> plot_x;
  QVector<double> plot_y;
  int nr_of_x_values;


  QTimer *plot_timer;

public slots:
  void plot_timer_timeout_slot();
};

#endif
