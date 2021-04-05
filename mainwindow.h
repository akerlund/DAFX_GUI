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
#include "console.h"

#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QStatusBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {

  Q_OBJECT

  const int MIXER_CHANNELS_C = 5;

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;

  private:
    QWidget     *_main_widget;
    QVBoxLayout *_main_layout;
    QTabWidget  *_main_tab;
    QWidget     *_tab0;
    QWidget     *_tab1;
    QWidget     *_tab2;

    Filter      *_filter;
    Mixer       *_mixer;
    Console     *_console;

  private slots:

};

#endif
