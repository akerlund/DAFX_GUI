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

#ifndef FILTER_H
#define FILTER_H

#include "biquad.h"
#include "qplot.h"
#include <QObject>
#include <QHBoxLayout>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

class Filter : public QObject {
  Q_OBJECT
  public:
    explicit Filter(QObject *parent = nullptr);
    void set_nr_of_samples(int value);
    void set_fs(double value);

  public:
    QVBoxLayout  *main_layout;

  private:
    int          _samples;
    double       _fs;
    QHBoxLayout  *_body_layout;
    Qplot        *_plot;
    Biquad       *_bq;
    QDial        *_dia_f;
    QDial        *_dia_q;
    QComboBox    *_cmb_f_type;
    QLabel       *_lbl_f;
    QLabel       *_lbl_q;
    QLabel       *_lbl_t;
    QLineEdit    *_qle_f;
    QLineEdit    *_qle_q;

  signals:

  public slots:
    void when_filter_changed();
    void update_filter_plot();
};

#endif
