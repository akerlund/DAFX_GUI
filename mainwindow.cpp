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

  // MainWindow
  main_widget              = new QWidget();
  main_layout              = new QVBoxLayout;
  main_tab                 = new QTabWidget();

  serial_label             = new QLabel(this);
  serial_connection        = new Serial();
  cmd_line                 = new CmdLine();

  setCentralWidget(main_widget);
  main_widget->setLayout(main_layout);
  main_layout->addWidget(main_tab);
  ui->statusbar->addPermanentWidget(serial_label);
  serial_label->setText("NO_CONNECTION");

  // Tab 0
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
  tab0->setLayout(tab0_main_layout);
  tab0_main_layout->addLayout(tab0_header_layout);
  tab0_main_layout->addWidget(tab0_text_browser);
  tab0_main_layout->addLayout(tab0_footer_layout);

  // Tab 1
  samples = 500;

  tab1             = new QWidget();
  tab1_layout      = new QHBoxLayout;
  tab1_ctrl_layout = new QVBoxLayout();

  plot = new Qplot();
  plot->set_max_x(44100/2);
  plot->set_nr_of_x_values(samples);

  _tab1_lbl_f = new QLabel("F");
  tab1_ctrl_layout->addWidget(_tab1_lbl_f);
  _dia_f = new QDial();
  _dia_f->setMinimum(20);
  _dia_f->setMaximum(20000);
  _dia_f->setValue(1000);
  _dia_f->setMaximumWidth(50);
  _dia_f->setMaximumHeight(50);
  tab1_ctrl_layout->addWidget(_dia_f);
  _tab1_qle_f = new QLineEdit("0");
  _tab1_qle_f->setText(QString::number(_dia_f->value()));
  _tab1_qle_f->setMaximumWidth(100);
  tab1_ctrl_layout->addWidget(_tab1_qle_f);
  _tab1_lbl_q = new QLabel("Q");
  tab1_ctrl_layout->addWidget(_tab1_lbl_q);
  _dia_q = new QDial();
  _dia_q->setMinimum(0);
  _dia_q->setMaximum(500);
  _dia_q->setMaximumWidth(50);
  _dia_q->setMaximumHeight(50);
  _dia_q->setValue(100);
  tab1_ctrl_layout->addWidget(_dia_q);
  _tab1_qle_q = new QLineEdit("0");
  _tab1_qle_q->setText(QString::number(_dia_q->value()/100)+"."+QString::number(_dia_q->value()%100));
  _tab1_qle_q->setMaximumWidth(100);
  tab1_ctrl_layout->addWidget(_tab1_qle_q);
  _tab1_lbl_t = new QLabel("Wave");
  tab1_ctrl_layout->addWidget(_tab1_lbl_t);
  _cmb_f_type = new QComboBox();
  _cmb_f_type->insertItem(0, "LP");
  _cmb_f_type->insertItem(1, "BP");
  _cmb_f_type->insertItem(2, "HP");
  _cmb_f_type->setMaximumWidth(50);
  tab1_ctrl_layout->addWidget(_cmb_f_type);

  _bq_timer = new QTimer(this);
  _bq_timer->setInterval(100);
  connect(_bq_timer, SIGNAL(timeout()), this, SLOT(update_filter_plot()));

  tab1_ctrl_layout->addStretch();
  tab1_layout->addLayout(plot->plot_layout);
  tab1_layout->addLayout(tab1_ctrl_layout);
  tab1->setLayout(tab1_layout);

  connect(_dia_f,  &QSlider::valueChanged, this, &MainWindow::when_filter_changed);
  connect(_dia_q,  &QSlider::valueChanged, this, &MainWindow::when_filter_changed);
  connect(_cmb_f_type, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged),
          this,      static_cast<void (MainWindow::*)()>(&MainWindow::when_filter_changed));

  _bq = new Biquad();
  update_filter_plot();

  // Tab 2
  tab2        = new QWidget();
  tab2_layout = new QHBoxLayout;

  for (int i = 0; i < MIXER_CHANNELS_C; i++) {
    _qframe = new QFrame();
    _qframe->setFrameShadow(QFrame::Sunken);
    _qframe->setFrameShape(QFrame::VLine);
    _mix_channel = new MixChannel(i);
    tab2_layout->addLayout(_mix_channel->main_layout);
    tab2_layout->addWidget(_qframe);
    connect(_mix_channel, &MixChannel::pan_value_changed,  this, &MainWindow::when_pan_changed);
    connect(_mix_channel, &MixChannel::gain_value_changed, this, &MainWindow::when_gain_changed);
    connect(_mix_channel, &MixChannel::freq_value_changed, this, &MainWindow::when_freq_changed);
    connect(_mix_channel, &MixChannel::wave_value_changed, this, &MainWindow::when_wave_changed);
    connect(_mix_channel, &MixChannel::duty_value_changed, this, &MainWindow::when_duty_changed);
  }

  tab2_layout->addStretch();
  tab2->setLayout(tab2_layout);

  main_tab->addTab(tab0, "Console");
  main_tab->addTab(tab1, "Other");
  main_tab->addTab(tab2, "Mixer");
  main_tab->setTabPosition(QTabWidget::North);
  main_tab->setCurrentIndex(1);

  // Parsing RX data as strings
  tab0_chk_parse_as_string->setChecked(true);
  tab0_chk_crc_enabled->setChecked(true);
  serial_connection->rx_set_parse_as_string(true);
  serial_connection->rx_set_crc_enabled(true);

  when_tab0_btn_update_clicked();

  connect(serial_connection,        SIGNAL(error_message(QString)),    this, SLOT(when_port_error(QString)));
  connect(serial_connection,        SIGNAL(read_received(QByteArray)), this, SLOT(when_read_received(QByteArray)));
  connect(tab0_btn_update,          SIGNAL(clicked()),                 this, SLOT(when_tab0_btn_update_clicked()));
  connect(tab0_btn_connect,         SIGNAL(clicked()),                 this, SLOT(when_tab0_btn_connect_clicked()));
  connect(tab0_btn_disconnect,      SIGNAL(clicked()),                 this, SLOT(when_tab0_btn_disconnect_clicked()));
  connect(tab0_btn_clear,           SIGNAL(clicked()),                 this, SLOT(when_tab0_btn_clear_clicked()));
  connect(cmd_line,                 SIGNAL(return_pressed(QString)),   this, SLOT(when_cmd_return_pressed(QString)));
  connect(tab0_chk_parse_as_string, SIGNAL(stateChanged(int)),         this, SLOT(when_tab0_chk_parse_as_string_state(int)));
  connect(tab0_chk_crc_enabled,     SIGNAL(stateChanged(int)),         this, SLOT(when_tab0_chk_crc_enabled_state(int)));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::when_tab0_btn_update_clicked() {
  QList<QSerialPortInfo> ports = serial_connection->list_serial_devices();

  tab0_serial_list->clear();

  foreach(const QSerialPortInfo &port, ports) {
    tab0_serial_list->addItem(port.portName(), port.systemLocation());
  }
}

void MainWindow::when_tab0_btn_connect_clicked() {
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

void MainWindow::when_tab0_btn_disconnect_clicked() {
  if (serial_connection->port_is_open()) {
    serial_connection->port_disconnect();
    tab0_text_browser->append("INFO [serial] Disconnected");
    serial_label->setText("Disconnected");
  } else {
    tab0_text_browser->append("INFO [serial] Not connected");
    serial_label->setText("Unconnected");
  }
}


void MainWindow::when_tab0_btn_clear_clicked() {
  tab0_text_browser->clear();
}


void MainWindow::when_tab0_chk_parse_as_string_state(int state) {
  if (state) {
    serial_connection->rx_set_parse_as_string(true);
    tab0_text_browser->append("INFO [serial] Parsing RX data as strings");
  } else {
    serial_connection->rx_set_parse_as_string(false);
    tab0_text_browser->append("INFO [serial] Parsing RX data as packets");
  }
}


void MainWindow::when_tab0_chk_crc_enabled_state(int state) {
  if (state) {
    serial_connection->rx_set_crc_enabled(true);
    tab0_text_browser->append("INFO [serial] Enabled CRC");
  } else {
    serial_connection->rx_set_crc_enabled(false);
    tab0_text_browser->append("INFO [serial] Disabled CRC");
  }
}


void MainWindow::when_cmd_return_pressed(QString line) {
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

void MainWindow::when_pan_changed(int id, int value) {
  tab0_text_browser->append(QString::number(id));
}
void MainWindow::when_gain_changed(int id, int value) {
    tab0_text_browser->append(QString::number(id));
}
void MainWindow::when_freq_changed(int id, int value) {
    tab0_text_browser->append(QString::number(id));
}
void MainWindow::when_wave_changed(int id, int value) {
    tab0_text_browser->append(QString::number(id));
}
void MainWindow::when_duty_changed(int id, int value) {
    tab0_text_browser->append(QString::number(id));
}

void MainWindow::update_filter_plot() {

  bq_coefficients_t bq_coefficients;
  QVector<double>   _m;
  bq_coefficients = _bq->bq_coefficients(bq_type_t(_cmb_f_type->currentIndex()), _dia_f->value(), 44100.0, _dia_q->value()/100.0);
  _bq->bq_normalize(bq_coefficients);
  _m = _bq->bq_magnitude_response(bq_coefficients, 44100.0, samples);

  for (int i = 0; i < samples; i++) {
    _m[i] = 10*log10(_m[i]/1.0);
  }
  plot->set_plot(&_m);
}


void MainWindow::when_filter_changed() {
  update_filter_plot();
  _tab1_qle_f->setText(QString::number(_dia_f->value()));
  _tab1_qle_q->setText(QString::number(_dia_q->value()/100)+"."+QString::number(_dia_q->value()%100));
}
