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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serial.h"
#include "qplot.h"
#include "cmdline.h"
#include "vbytearray.h"
#include "mixchannel.h"
#include "biquad.h"
#include "filter.h"
#include "mixer.h"
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QComboBox>
#include <QLabel>
#include <QTabWidget>
#include <QTextBrowser>
#include <QCheckBox>
#include <QList>
#include <QFrame>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
  Q_OBJECT

  const int MIXER_CHANNELS_C = 5;

  QWidget      *main_widget;
  QVBoxLayout  *main_layout;
  QTabWidget   *main_tab;

  Serial       *serial_connection;
  QLabel       *serial_label;

  Qplot        *plot;
  CmdLine      *cmd_line;

  QWidget      *tab0;
  QVBoxLayout  *tab0_main_layout;
  QHBoxLayout  *tab0_header_layout;
  QComboBox    *tab0_serial_list;
  QPushButton  *tab0_btn_update;
  QPushButton  *tab0_btn_connect;
  QPushButton  *tab0_btn_disconnect;
  QCheckBox    *tab0_chk_parse_as_string;
  QCheckBox    *tab0_chk_crc_enabled;
  QLabel       *tab0_lbl_parse_as_string;
  QLabel       *tab0_lbl_crc_enabled;
  QGridLayout  *tab0_ckh_layout;
  QTextBrowser *tab0_text_browser;
  QHBoxLayout  *tab0_footer_layout;
  QComboBox    *tab0_combo_input;
  QPushButton  *tab0_btn_clear;

  Filter       *_filter;
  Mixer        *_mixer;

  QWidget      *tab1;
  QWidget      *tab2;
  QHBoxLayout  *tab2_layout;
  MixChannel   *_mix_channel;
  QFrame       *_qframe;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
  void when_read_received(QByteArray);
  void when_port_error(QString);
  void when_tab0_btn_update_clicked();
  void when_tab0_btn_connect_clicked();
  void when_tab0_btn_disconnect_clicked();
  void when_tab0_btn_clear_clicked();
  void when_tab0_chk_parse_as_string_state(int state);
  void when_tab0_chk_crc_enabled_state(int state);
  void when_cmd_return_pressed(QString line);

};

#endif
