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

#ifndef MIXER_H
#define MIXER_H

#include "mixchannel.h"
#include <QObject>
#include <QFrame>
#include <QTimer>

class Mixer : public QObject {

  Q_OBJECT

  const int MIXER_CHANNELS_C = 5;
  const int Q_BITS_C         = 11;

  public:
    explicit Mixer(QObject *parent = nullptr);

  public:
    QVBoxLayout *main_layout;

  private:
    QHBoxLayout *_body_layout;
    MixChannel  *_mix_channel;
    QFrame      *_qframe;
    QTimer      *_qtimer;
    QString     _emit_str;

  signals:
    void serial_message(QByteArray msg);

  private slots:
    void when_pan_changed(int id, int value);
    void when_gain_changed(int id, int value);
    void when_freq_changed(int id, int value);
    void when_wave_changed(int id, int value);
    void when_duty_changed(int id, int value);
    void timer_timeout_slot();
};

#endif
