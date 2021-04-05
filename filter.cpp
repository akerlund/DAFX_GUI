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

#include "filter.h"

Filter::Filter(QObject *parent) : QObject(parent) {

  _samples = 500;
  _fs      = 44100.0;

  _plot = new Qplot();
  _plot->set_max_x(_fs/2);
  _plot->set_nr_of_x_values(_samples);

  _body_layout = new QHBoxLayout;
  _lbl_f = new QLabel("F");
  _body_layout->addWidget(_lbl_f);
  _dia_f = new QDial();
  _dia_f->setMinimum(20);
  _dia_f->setMaximum(20000);
  _dia_f->setValue(1000);
  _dia_f->setMaximumWidth(50);
  _dia_f->setMaximumHeight(50);
  _body_layout->addWidget(_dia_f);
  _qle_f = new QLineEdit("0");
  _qle_f->setText(QString::number(_dia_f->value()));
  _qle_f->setMaximumWidth(100);
  _body_layout->addWidget(_qle_f);
  _lbl_q = new QLabel("Q");
  _body_layout->addWidget(_lbl_q);
  _dia_q = new QDial();
  _dia_q->setMinimum(0);
  _dia_q->setMaximum(500);
  _dia_q->setMaximumWidth(50);
  _dia_q->setMaximumHeight(50);
  _dia_q->setValue(100);
  _body_layout->addWidget(_dia_q);
  _qle_q = new QLineEdit("0");
  _qle_q->setText(QString::number(_dia_q->value()/100)+"."+QString::number(_dia_q->value()%100));
  _qle_q->setMaximumWidth(100);
  _body_layout->addWidget(_qle_q);
  _lbl_t = new QLabel("Wave");
  _body_layout->addWidget(_lbl_t);
  _cmb_f_type = new QComboBox();
  _cmb_f_type->insertItem(0, "LP");
  _cmb_f_type->insertItem(1, "BP");
  _cmb_f_type->insertItem(2, "HP");
  _cmb_f_type->setMaximumWidth(50);
  _body_layout->addWidget(_cmb_f_type);
  _body_layout->addStretch();

  main_layout = new QVBoxLayout;
  main_layout->addLayout(_plot->plot_layout);
  main_layout->addLayout(_body_layout);

  connect(_dia_f,  &QSlider::valueChanged, this, &Filter::when_filter_changed);
  connect(_dia_q,  &QSlider::valueChanged, this, &Filter::when_filter_changed);
  connect(_cmb_f_type, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
          this,        static_cast<void (Filter::*)()>(&Filter::when_filter_changed));

  _bq = new Biquad();
  update_filter_plot();
}

void Filter::set_nr_of_samples(int value) {
  _samples = value;
  _plot->set_nr_of_x_values(value);
}

void Filter::set_fs(double value) {
  _fs = value;
  _plot->set_max_x(value);
}

void Filter::when_filter_changed() {
  update_filter_plot();
  _qle_f->setText(QString::number(_dia_f->value()));
  _qle_q->setText(QString::number(_dia_q->value()/100)+"."+QString::number(_dia_q->value()%100));
}


void Filter::update_filter_plot() {

  bq_coefficients_t bq_coefficients;
  QVector<double> _m;
  bq_coefficients = _bq->bq_coefficients(bq_type_t(_cmb_f_type->currentIndex()), _dia_f->value(), _fs, _dia_q->value()/100.0);
  _bq->bq_normalize(bq_coefficients);
  _m = _bq->bq_magnitude_response(bq_coefficients, _fs, _samples);

  for (int i = 0; i < _samples; i++) {
    _m[i] = 10*log10(_m[i]/1.0);
  }
  _plot->set_plot(&_m);
}
