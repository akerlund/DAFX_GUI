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

#include "qplot.h"

Qplot::Qplot(QObject *parent) : QObject(parent)
{
  // Configure the Widget
  plot_layout = new QVBoxLayout;
  plot        = new QCustomPlot();
  plot_layout->addWidget(plot);

  // Configure the plot
  QFont legendFont("Times", 10, QFont::Bold);
  plot->addGraph();
  plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  plot->graph(0)->setPen(QPen(Qt::blue));
  plot->graph(0)->setName("Name");
  plot->legend->setVisible(true);
  plot->legend->setFont(legendFont);
  plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
  plot->legend->setBrush(QBrush(QColor(255,255,255,230)));
  plot->xAxis->setLabel("f");
  plot->yAxis->setLabel("dB");

  QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
  plot->yAxis->grid()->setSubGridVisible(true);
  plot->xAxis->grid()->setSubGridVisible(true);
  plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
  plot->xAxis->setTicker(logTicker);
  plot->xAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
  plot->xAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
//  plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
//  plot->yAxis->setTicker(logTicker);
//  plot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
//  plot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"

  // Initialize the plot data
  nr_of_x_values = 100;
  reset_x_axis();
  reset_y_axis();


  // Configure the update timer
  plot_timer = new QTimer(this);
  plot_timer->setInterval(100);
  plot_timer->start();
  connect(plot_timer, SIGNAL(timeout()), this, SLOT(plot_timer_timeout_slot()));
}

Qplot::~Qplot()
{

}

void Qplot::append_to_plot(double value) {
  shift_in_to_vector(&plot_y, value, nr_of_x_values);
  plot_update();
}

void Qplot::set_plot(QVector<double> *vector) {
  plot_y = *vector;
  plot_update();
}


void Qplot::plot_update() {
  plot->graph(0)->setData(plot_x, plot_y);
  plot->rescaleAxes();
  plot->replot();
}


void Qplot::plot_timer_timeout_slot() {
  plot_update();
}


void Qplot::shift_in_to_vector(QVector<double> *vector, double value, int max_size) {
  vector->append(value);
  if (vector->size() > max_size) {
    vector->remove(0, vector->size() - max_size);
  }
}


void Qplot::reset_x_axis() {
  double _x = max_x / nr_of_x_values;
  plot_x.clear();
  for (int i = 0; i < nr_of_x_values; i++) {
    plot_x.append(i*_x);
  }
}

void Qplot::set_max_x(double value) {
  max_x = value;
}


void Qplot::set_nr_of_x_values(int value) {
  nr_of_x_values = value;
  reset_x_axis();
  reset_y_axis();
}


void Qplot::reset_y_axis() {
  plot_y.clear();
  for (int i = 0; i < nr_of_x_values; i++) {
    plot_y.append(0);
  }
}
