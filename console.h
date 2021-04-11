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

#ifndef CONSOLE_H
#define CONSOLE_H

#include "serial.h"
#include "cmdline.h"
#include "vbytearray.h"
#include <QObject>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTextBrowser>
#include <QCheckBox>

class Console : public QObject {

  Q_OBJECT

  public:
    explicit Console(QObject *parent = nullptr);
    QVBoxLayout  *main_layout;
    QLabel       *serial_label;
    Serial       *_serial;

  private:
    CmdLine      *_cmd_line;
    QHBoxLayout  *_header_layout;
    QComboBox    *_serial_list;
    QPushButton  *_btn_update;
    QPushButton  *_btn_connect;
    QPushButton  *_btn_disconnect;
    QCheckBox    *_chk_parse_as_string;
    QCheckBox    *_chk_crc_enabled;
    QLabel       *_lbl_parse_as_string;
    QLabel       *_lbl_crc_enabled;
    QGridLayout  *_ckh_layout;
    QTextBrowser *_text_browser;
    QHBoxLayout  *_footer_layout;
    QPushButton  *_btn_clear;

  signals:

  public slots:
    void when_serial_message(QByteArray);

  private slots:
    void when_read_received(QByteArray);
    void when_port_error(QString);
    void when_btn_update_clicked();
    void when_btn_connect_clicked();
    void when_btn_disconnect_clicked();
    void when_btn_clear_clicked();
    void when_chk_parse_as_string_state(int state);
    void when_chk_crc_enabled_state(int state);
    void when_cmd_return_pressed(QString line);
};

#endif
