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

#include <QVector>

#ifndef BIQUAD_H
#define BIQUAD_H

typedef struct {
  double b0;
  double b1;
  double b2;
  double a0;
  double a1;
  double a2;
} bq_coefficients_t;

typedef enum {
  BQ_LP_E,
  BQ_BP_E,
  BQ_HP_E
} bq_type_t;

class Biquad {

  public:
    Biquad();
    bq_coefficients_t bq_coefficients(bq_type_t bq_type, double f0, double fs, double q);
    void bq_normalize(bq_coefficients_t &bqc);
    QVector<double> bq_magnitude_response(bq_coefficients_t bqc, double fs, int n);
};

#endif
