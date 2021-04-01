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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setWindowTitle("SerialPortInterface");

  main_widget              = new QWidget();
  main_layout              = new QVBoxLayout;
  main_tab                 = new QTabWidget();

  serial_label             = new QLabel(this);
  serial_connection        = new Serial();
  cmd_line                 = new CmdLine();

  tab0                     = new QWidget();
  tab0_main_layout         = new QVBoxLayout;
  tab0_header_layout       = new QHBoxLayout;
  tab0_serial_list         = new QComboBox();
  tab0_btn_update          = new QPushButton();
  tab0_btn_connect         = new QPushButton();
  tab0_btn_disconnect      = new QPushButton();
  tab0_ckh_layout          = new QGridLayout();
  tab0_chk_parse_as_string = new QCheckBox();
  tab0_chk_crc_enabled     = new QCheckBox();
  tab0_lbl_parse_as_string = new QLabel("String Mode", this);
  tab0_lbl_crc_enabled     = new QLabel("CRC Enabled", this);
  tab0_text_browser        = new QTextBrowser();
  tab0_footer_layout       = new QHBoxLayout;
  tab0_combo_input         = new QComboBox();
  tab0_btn_clear           = new QPushButton();

  plot                     = new Qplot();
  tab1                     = new QWidget();
  tab1_layout              = new QVBoxLayout;

  setCentralWidget(main_widget);
  main_widget->setLayout(main_layout);
  main_layout->addWidget(main_tab);
  ui->statusbar->addPermanentWidget(serial_label);

  serial_label->setText("NO_CONNECTION");


  tab0->setLayout(tab0_main_layout);

  tab0_header_layout->addWidget(tab0_serial_list);
  tab0_header_layout->addWidget(tab0_btn_update);
  tab0_header_layout->addWidget(tab0_btn_connect);
  tab0_header_layout->addWidget(tab0_btn_disconnect);
  tab0_btn_update->setText("Update");
  tab0_btn_update->setMaximumWidth(100);
  tab0_btn_connect->setText("Connect");
  tab0_btn_connect->setMaximumWidth(100);
  tab0_btn_disconnect->setText("Disconnect");
  tab0_btn_disconnect->setMaximumWidth(100);
  tab0_ckh_layout->addWidget(tab0_chk_parse_as_string, 0,0);
  tab0_ckh_layout->addWidget(tab0_chk_crc_enabled, 1,0);
  tab0_ckh_layout->addWidget(tab0_lbl_parse_as_string, 0,1);
  tab0_ckh_layout->addWidget(tab0_lbl_crc_enabled, 1,1);
  tab0_chk_parse_as_string->setMaximumWidth(15);
  tab0_chk_crc_enabled->setMaximumWidth(15);
  tab0_lbl_parse_as_string->setMaximumWidth(100);
  tab0_lbl_crc_enabled->setMaximumWidth(100);

  tab0_header_layout->addLayout(tab0_ckh_layout);
  tab0_footer_layout->addWidget(cmd_line);
  tab0_combo_input->setEditable(true);
  tab0_footer_layout->addWidget(tab0_btn_clear);
  tab0_btn_clear->setText("Clear");
  tab0_btn_clear->setMaximumWidth(100);
  tab0_main_layout->addLayout(tab0_header_layout);
  tab0_main_layout->addWidget(tab0_text_browser);
  tab0_main_layout->addLayout(tab0_footer_layout);

  tab1->setLayout(plot->plot_layout);

  tab2                     = new QWidget();
  tab2_layout              = new QVBoxLayout;
  _mix_channel             = new MixChannel(42);
  tab2->setLayout(_mix_channel->main_layout);

  main_tab->addTab(tab0, "Console");
  main_tab->addTab(tab1, "Other");
  main_tab->addTab(tab2, "Mixer");
  main_tab->setTabPosition(QTabWidget::North);
  main_tab->setCurrentIndex(2);

  // Parsing RX data as strings
  tab0_chk_parse_as_string->setChecked(true);
  tab0_chk_crc_enabled->setChecked(true);
  serial_connection->rx_set_parse_as_string(true);
  serial_connection->rx_set_crc_enabled(true);

  when_tab0_btn_update_clicked();

  connect(serial_connection, SIGNAL(error_message(QString)), this, SLOT(when_port_error(QString)));
  connect(serial_connection, SIGNAL(read_received(QByteArray)), this, SLOT(when_read_received(QByteArray)));
  connect(tab0_btn_update, SIGNAL(clicked()), this, SLOT(when_tab0_btn_update_clicked()));
  connect(tab0_btn_connect, SIGNAL(clicked()), this, SLOT(when_tab0_btn_connect_clicked()));
  connect(tab0_btn_disconnect, SIGNAL(clicked()), this, SLOT(when_tab0_btn_disconnect_clicked()));
  connect(tab0_btn_clear, SIGNAL(clicked()), this, SLOT(when_tab0_btn_clear_clicked()));
  connect(cmd_line, SIGNAL(return_pressed(QString)), this, SLOT(when_cmd_return_pressed(QString)));
  connect(tab0_chk_parse_as_string, SIGNAL(stateChanged(int)), this, SLOT(when_tab0_chk_parse_as_string_state(int)));
  connect(tab0_chk_crc_enabled, SIGNAL(stateChanged(int)), this, SLOT(when_tab0_chk_crc_enabled_state(int)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::when_tab0_btn_update_clicked()
{
  QList<QSerialPortInfo> ports = serial_connection->list_serial_devices();

  tab0_serial_list->clear();

  foreach(const QSerialPortInfo &port, ports) {
    tab0_serial_list->addItem(port.portName(), port.systemLocation());
  }
}

void MainWindow::when_tab0_btn_connect_clicked()
{
  if (!serial_connection->port_connect(tab0_serial_list->currentData().toString())) {
    tab0_text_browser->append("INFO [serial] CONNECTION_FAILED");
  }

  if (!serial_connection->port_is_open()) {
    serial_label->setText("CONNECTION_FAILED");
  } else {
    tab0_text_browser->append("INFO [serial] Connected to" + tab0_serial_list->currentData().toString());
    serial_label->setText(serial_connection->port_name());
  }
}

void MainWindow::when_tab0_btn_disconnect_clicked()
{

  if (serial_connection->port_is_open()) {
    serial_connection->port_disconnect();
    tab0_text_browser->append("INFO [serial] Disconnected");
    serial_label->setText("Disconnected");
  } else {
    tab0_text_browser->append("INFO [serial] Not connected");
    serial_label->setText("Unconnected");
  }
}


void MainWindow::when_tab0_btn_clear_clicked()
{
  tab0_text_browser->clear();
}


void MainWindow::when_tab0_chk_parse_as_string_state(int state)
{
  if (state) {
    serial_connection->rx_set_parse_as_string(true);
    tab0_text_browser->append("INFO [serial] Parsing RX data as strings");
  } else {
    serial_connection->rx_set_parse_as_string(false);
    tab0_text_browser->append("INFO [serial] Parsing RX data as packets");
  }
}


void MainWindow::when_tab0_chk_crc_enabled_state(int state)
{
  if (state) {
    serial_connection->rx_set_crc_enabled(true);
    tab0_text_browser->append("INFO [serial] Enabled CRC");
  } else {
    serial_connection->rx_set_crc_enabled(false);
    tab0_text_browser->append("INFO [serial] Disabled CRC");
  }
}


void MainWindow::when_cmd_return_pressed(QString line)
{
  QByteArray tx_data = line.toUtf8();
  tab0_text_browser->append(line);
  if (serial_connection->port_is_open()) {
    serial_connection->port_write(tx_data);
  } else {
    tab0_text_browser->append("ERROR [serial] Port is not open");
  }
}


void MainWindow::when_read_received(QByteArray data) {

  VByteArray byte_array;
  unsigned char PLOT_0_C = 0xA0;

  if (serial_connection->rx_get_parse_as_string()) {
    tab0_text_browser->append(QString::fromStdString(data.toStdString()).trimmed());
    return;
  }

  if (data[0] == PLOT_0_C) {
    byte_array = VByteArray();
    byte_array.vbAppendUint8(data[0]);
    byte_array.vbAppendUint8(data[1]);
    byte_array.vbAppendUint8(data[2]);
    byte_array.vbAppendUint8(data[3]);
    plot->append_to_plot((double)byte_array.vbPopFrontUint32());
  }

}



void MainWindow::when_port_error(QString error) {

  tab0_text_browser->append(error);
}


