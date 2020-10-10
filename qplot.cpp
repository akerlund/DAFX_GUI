#include "qplot.h"

Qplot::Qplot(QObject *parent) : QObject(parent)
{
  plot_layout = new QVBoxLayout;
  plot        = new QCustomPlot();
  plot_layout->addWidget(plot);

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

  plot_timer = new QTimer(this);
  plot_timer->setInterval(100);
  plot_timer->start();
  connect(plot_timer, SIGNAL(timeout()), this, SLOT(plot_timer_timeout_slot()));
}

Qplot::~Qplot()
{

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

