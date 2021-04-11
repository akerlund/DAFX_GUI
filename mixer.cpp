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

#include "mixer.h"
#include "base_addresses.h"
#include "dafx_address.h"

Mixer::Mixer(QObject *parent) : QObject(parent) {

  _body_layout = new QHBoxLayout;

  for (int i = 0; i < MIXER_CHANNELS_C; i++) {
    _qframe = new QFrame();
    _qframe->setFrameShadow(QFrame::Sunken);
    _qframe->setFrameShape(QFrame::VLine);
    _mix_channel = new MixChannel(i);
    _body_layout->addLayout(_mix_channel->main_layout);
    _body_layout->addWidget(_qframe);
    connect(_mix_channel, &MixChannel::pan_value_changed,  this, &Mixer::when_pan_changed);
    connect(_mix_channel, &MixChannel::gain_value_changed, this, &Mixer::when_gain_changed);
    connect(_mix_channel, &MixChannel::freq_value_changed, this, &Mixer::when_freq_changed);
    connect(_mix_channel, &MixChannel::wave_value_changed, this, &Mixer::when_wave_changed);
    connect(_mix_channel, &MixChannel::duty_value_changed, this, &Mixer::when_duty_changed);
  }
  _body_layout->addStretch();
  main_layout = new QVBoxLayout;
  main_layout->addLayout(_body_layout);

  _qtimer = new QTimer(this);
  _qtimer->setInterval(100);
  connect(_qtimer, &QTimer::timeout, this, &Mixer::timer_timeout_slot);
}


void Mixer::when_pan_changed(int id, int value) {

}


void Mixer::when_gain_changed(int id, int value) {
  _emit_str = "wr " + QString::number(MIXER_CHANNEL_GAIN_0_ADDR) + " " + QString::number((value<<Q_BITS_C)/100);
  _qtimer->start();
}


void Mixer::when_freq_changed(int id, int value) {
  _emit_str = "wr " + QString::number(OSC0_FREQUENCY_ADDR) + " " + QString::number(value<<Q_BITS_C);
  _qtimer->start();
}


void Mixer::when_wave_changed(int id, int value) {
  _emit_str = "wr " + QString::number(OSC0_WAVEFORM_SELECT_ADDR) + " " + QString::number(value);
  _qtimer->start();
}


void Mixer::when_duty_changed(int id, int value) {
  _emit_str = "wr " + QString::number(OSC0_DUTY_CYCLE_ADDR) + " " + QString::number(value);
  _qtimer->start();
}


void Mixer::timer_timeout_slot() {
  emit serial_message(_emit_str.toUtf8());
  _qtimer->stop();
}
