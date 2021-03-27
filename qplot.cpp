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
  plot->xAxis->setLabel("X Axis");
  plot->yAxis->setLabel("Y Axis");


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

void Qplot::append_to_plot(double value)
{
  shift_in_to_vector(&plot_y, value, nr_of_x_values);
  plot_update();
}


void Qplot::plot_update()
{
  plot->graph(0)->setData(plot_x, plot_y);
  plot->rescaleAxes();
  plot->replot();
}


void Qplot::plot_timer_timeout_slot()
{
  plot_update();
}


void Qplot::shift_in_to_vector(QVector<double> *vector, double value, int max_size) {

  vector->append(value);
  if(vector->size() > max_size) {
    vector->remove(0, vector->size( ) - max_size);
  }
}


void Qplot::reset_x_axis()
{
  plot_x.clear();
  for (int i = 0; i < nr_of_x_values; i++) {
    plot_x.append(i);
  }
}


void Qplot::reset_y_axis()
{
  plot_y.clear();
  for (int i = 0; i < nr_of_x_values; i++) {
    plot_y.append(0);
  }
}
