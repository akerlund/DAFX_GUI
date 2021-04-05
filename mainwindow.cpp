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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  this->setWindowTitle("DAFX");

  _main_widget = new QWidget();
  _main_layout = new QVBoxLayout;
  _main_tab = new QTabWidget();

  setCentralWidget(_main_widget);
  _main_widget->setLayout(_main_layout);
  _main_layout->addWidget(_main_tab);

  // Tab 0
  _console = new Console();
  _tab0 = new QWidget();
  _tab0->setLayout(_console->main_layout);
  ui->statusbar->addPermanentWidget(_console->serial_label);

  // Tab 1
  _filter = new Filter();
  _tab1 = new QWidget();
  _tab1->setLayout(_filter->main_layout);

  // Tab 2
  _mixer = new Mixer();
  _tab2 = new QWidget();
  _tab2->setLayout(_mixer->main_layout);

  _main_tab->addTab(_tab0, "Console");
  _main_tab->addTab(_tab1, "Filter");
  _main_tab->addTab(_tab2, "Mixer");
  _main_tab->setCurrentIndex(1);
  _main_tab->setTabPosition(QTabWidget::North);
}

MainWindow::~MainWindow() {
  delete ui;
}
