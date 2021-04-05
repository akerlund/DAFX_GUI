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

#include "console.h"

Console::Console(QObject *parent) : QObject(parent) {


  _serial = new Serial();
  _serial->rx_set_parse_as_string(true);
  _serial->rx_set_crc_enabled(true);
  serial_label = new QLabel();
  serial_label->setText("NO_CONNECTION");

  _header_layout = new QHBoxLayout;

  _serial_list = new QComboBox();
  _header_layout->addWidget(_serial_list);
  _btn_update = new QPushButton();
  _btn_update->setText("Update");
  _btn_update->setMaximumWidth(100);
  _header_layout->addWidget(_btn_update);
  _btn_connect = new QPushButton();
  _btn_connect->setText("Connect");
  _btn_connect->setMaximumWidth(100);
  _header_layout->addWidget(_btn_connect);
  _btn_disconnect = new QPushButton();
  _btn_disconnect->setText("Disconnect");
  _btn_disconnect->setMaximumWidth(100);
  _header_layout->addWidget(_btn_disconnect);

  _ckh_layout = new QGridLayout();
  _chk_parse_as_string = new QCheckBox();
  _chk_parse_as_string->setMaximumWidth(15);
  _chk_parse_as_string->setChecked(true);
  _ckh_layout->addWidget(_chk_parse_as_string, 0,0);
  _chk_crc_enabled = new QCheckBox();
  _chk_crc_enabled->setMaximumWidth(15);
  _chk_crc_enabled->setChecked(true);
  _ckh_layout->addWidget(_chk_crc_enabled, 1,0);
  _lbl_parse_as_string = new QLabel("String Mode");
  _lbl_parse_as_string->setMaximumWidth(100);
  _ckh_layout->addWidget(_lbl_parse_as_string, 0,1);
  _lbl_crc_enabled = new QLabel("CRC Enabled");
  _lbl_crc_enabled->setMaximumWidth(100);
  _ckh_layout->addWidget(_lbl_crc_enabled, 1,1);

  _header_layout->addLayout(_ckh_layout);

  _footer_layout = new QHBoxLayout;
  _cmd_line      = new CmdLine();
  _footer_layout->addWidget(_cmd_line);
  _btn_clear = new QPushButton();
  _btn_clear->setText("Clear");
  _btn_clear->setMaximumWidth(100);
  _footer_layout->addWidget(_btn_clear);

  main_layout  = new QVBoxLayout;
  main_layout->addLayout(_header_layout);
  _text_browser = new QTextBrowser();
  main_layout->addWidget(_text_browser);
  main_layout->addLayout(_footer_layout);

  connect(_serial,              SIGNAL(error_message(QString)),    this, SLOT(when_port_error(QString)));
  connect(_serial,              SIGNAL(read_received(QByteArray)), this, SLOT(when_read_received(QByteArray)));
  connect(_btn_update,          SIGNAL(clicked()),                 this, SLOT(when_btn_update_clicked()));
  connect(_btn_connect,         SIGNAL(clicked()),                 this, SLOT(when_btn_connect_clicked()));
  connect(_btn_disconnect,      SIGNAL(clicked()),                 this, SLOT(when_btn_disconnect_clicked()));
  connect(_btn_clear,           SIGNAL(clicked()),                 this, SLOT(when_btn_clear_clicked()));
  connect(_cmd_line,            SIGNAL(return_pressed(QString)),   this, SLOT(when_cmd_return_pressed(QString)));
  connect(_chk_parse_as_string, SIGNAL(stateChanged(int)),         this, SLOT(when_chk_parse_as_string_state(int)));
  connect(_chk_crc_enabled,     SIGNAL(stateChanged(int)),         this, SLOT(when_chk_crc_enabled_state(int)));

  when_btn_update_clicked();
}

void Console::when_btn_update_clicked() {
  QList<QSerialPortInfo> ports = _serial->list_serial_devices();

  _serial_list->clear();

  foreach(const QSerialPortInfo &port, ports) {
    _serial_list->addItem(port.portName(), port.systemLocation());
  }
}

void Console::when_btn_connect_clicked() {
  if (!_serial->port_connect(_serial_list->currentData().toString())) {
    _text_browser->append("INFO [serial] CONNECTION_FAILED");
  }

  if (!_serial->port_is_open()) {
    serial_label->setText("CONNECTION_FAILED");
  } else {
    _text_browser->append("INFO [serial] Connected to" + _serial_list->currentData().toString());
    serial_label->setText(_serial->port_name());
  }
}


void Console::when_btn_disconnect_clicked() {
  if (_serial->port_is_open()) {
    _serial->port_disconnect();
    _text_browser->append("INFO [serial] Disconnected");
    serial_label->setText("Disconnected");
  } else {
    _text_browser->append("INFO [serial] Not connected");
    serial_label->setText("Unconnected");
  }
}


void Console::when_btn_clear_clicked() {
  _text_browser->clear();
}


void Console::when_chk_parse_as_string_state(int state) {
  if (state) {
    _serial->rx_set_parse_as_string(true);
    _text_browser->append("INFO [serial] Parsing RX data as strings");
  } else {
    _serial->rx_set_parse_as_string(false);
    _text_browser->append("INFO [serial] Parsing RX data as packets");
  }
}


void Console::when_chk_crc_enabled_state(int state) {
  if (state) {
    _serial->rx_set_crc_enabled(true);
    _text_browser->append("INFO [serial] Enabled CRC");
  } else {
    _serial->rx_set_crc_enabled(false);
    _text_browser->append("INFO [serial] Disabled CRC");
  }
}


void Console::when_cmd_return_pressed(QString line) {
  QByteArray tx_data = line.toUtf8();
  _text_browser->append(line);
  if (_serial->port_is_open()) {
    _serial->port_write(tx_data);
  } else {
    _text_browser->append("ERROR [serial] Port is not open");
  }
}


void Console::when_read_received(QByteArray data) {

  VByteArray byte_array;
  unsigned char PLOT_0_C = 0xA0;

  if (_serial->rx_get_parse_as_string()) {
    _text_browser->append(QString::fromStdString(data.toStdString()).trimmed());
    return;
  }

  if (data[0] == PLOT_0_C) {
    byte_array = VByteArray();
    byte_array.vbAppendUint8(data[0]);
    byte_array.vbAppendUint8(data[1]);
    byte_array.vbAppendUint8(data[2]);
    byte_array.vbAppendUint8(data[3]);
  }

}

void Console::when_port_error(QString error) {
  _text_browser->append(error);
}
