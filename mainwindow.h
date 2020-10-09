////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2020 Fredrik Åkerlund
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

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QComboBox>
#include <QLabel>
#include <QTabWidget>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  QWidget      *main_widget;
  QVBoxLayout  *main_layout;
  QTabWidget   *main_tab;

  Serial       *serial_connection;
  QLabel       *serial_label;

  QWidget      *tab0;
  QVBoxLayout  *tab0_main_layout;
  QHBoxLayout  *tab0_header_layout;
  QComboBox    *tab0_serial_list;
  QPushButton  *tab0_btn_update;
  QPushButton  *tab0_btn_connect;
  QPushButton  *tab0_btn_disconnect;
  QTextBrowser *tab0_text_browser;
  QComboBox    *tab0_combo_input;

  QWidget      *tab1;
  QVBoxLayout  *tab1_layout;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
  void on_tab0_btn_update_clicked();
  void on_tab0_btn_connect_clicked();
  void on_tab0_btn_disconnect_clicked();
};
#endif // MAINWINDOW_H
