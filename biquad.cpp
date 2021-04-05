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

#include "biquad.h"
#include <cmath>
#include <stdint.h>

Biquad::Biquad() {

}

bq_coefficients_t Biquad::bq_coefficients(bq_type_t bq_type, double f0, double fs, double q) {

  bq_coefficients_t bqc;
  double w0 = 2*M_PI*f0/fs;
  double alfa = sin(w0) / (2*q);

  if (bq_type == BQ_LP_E) {
    bqc.b0 = (1 - cos(w0)) / 2;
    bqc.b1 = 1 - cos(w0);
    bqc.b2 = (1 - cos(w0)) / 2;
    bqc.a0 = 1 + alfa;
    bqc.a1 = -2*cos(w0);
    bqc.a2 = 1 - alfa;
  }

  if (bq_type == BQ_BP_E) {
    bqc.b0 = sin(w0) / 2;
    bqc.b1 = 0;
    bqc.b2 = -sin(w0) / 2;
    bqc.a0 = 1 + alfa;
    bqc.a1 = -2*cos(w0);
    bqc.a2 = 1 - alfa;
  }

  if (bq_type == BQ_HP_E) {
    bqc.b0 = (1 + cos(w0)) / 2;
    bqc.b1 = -(1 + cos(w0));
    bqc.b2 = (1 + cos(w0)) / 2;
    bqc.a0 = 1 + alfa;
    bqc.a1 = -2*cos(w0);
    bqc.a2 = 1 - alfa;
  }

  return bqc;
}

void Biquad::bq_normalize(bq_coefficients_t &bqc) {
  bqc.b0 = bqc.b0 / bqc.a0;
  bqc.b1 = bqc.b1 / bqc.a0;
  bqc.b2 = bqc.b2 / bqc.a0;
  bqc.a1 = bqc.a1 / bqc.a0;
  bqc.a2 = bqc.a2 / bqc.a0;
  bqc.a0 = 1.0;
}

// http://rs-met.com/documents/dsp/BasicDigitalFilters.pdf
QVector<double> Biquad::bq_magnitude_response(bq_coefficients_t bqc, double fs, int n) {

  QVector<double> res;
  double numerator;
  double divisor;
  double w0;
  double f;
  double num0 = bqc.b0*bqc.b0 + bqc.b1*bqc.b1 + bqc.b2*bqc.b2;
  double num1 = 2*(bqc.b0*bqc.b1 + bqc.b1*bqc.b2);
  double num2 = 2*bqc.b0*bqc.b2;
  double div0 = 1 + bqc.a1*bqc.a1 + bqc.a2*bqc.a2;
  double div1 = 2*(bqc.a1 + bqc.a1*bqc.a2);
  double div2 = 2*bqc.a2;

  for (int i = 0; i < n; i++) {
    f  = fs/2/n*i;
    w0 = 2*M_PI*f/fs;

    numerator = num0 + num1*cos(w0) + num2*cos(2*w0);
    divisor   = div0 + div1*cos(w0) + div2*cos(2*w0);
    res.append(sqrt(numerator/divisor));
  }

  return res;
}
