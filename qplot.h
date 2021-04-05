////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2021 Fredrik Åkerlund
// https://github.com/akerlund/DAFX_GUI
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Description:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef QPLOT_H
#define QPLOT_H

#include <QObject>
#include <QTimer>
#include <QFont>
#include "qcustomplot.h"

class Qplot : public QObject {

  Q_OBJECT

  public:
    explicit Qplot(QObject *parent = nullptr);
    ~Qplot();
    void append_to_plot(double value);
    void set_plot(QVector<double> *vector);
    void plot_update();
    QVBoxLayout  *plot_layout;
    void set_max_x(double value);
    void set_nr_of_x_values(int value);

  private:
    void reset_x_axis();
    void reset_y_axis();
    void shift_in_to_vector(QVector<double> *vector, double value, int max_size);

  private:
    QCustomPlot *plot;
    QVector<double> plot_x;
    QVector<double> plot_y;
    double max_x;
    int nr_of_x_values;
    QTimer *plot_timer;

  public slots:
    void plot_timer_timeout_slot();
};

#endif
