#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  main_widget         = new QWidget();
  main_layout         = new QVBoxLayout;
  main_tab            = new QTabWidget();

  serial_label        = new QLabel(this);
  serial_connection   = new Serial();

  tab0                = new QWidget();
  tab0_main_layout    = new QVBoxLayout;
  tab0_header_layout  = new QHBoxLayout;
  tab0_serial_list    = new QComboBox();
  tab0_btn_update     = new QPushButton();
  tab0_btn_connect    = new QPushButton();
  tab0_btn_disconnect = new QPushButton();
  tab0_text_browser   = new QTextBrowser();
  tab0_combo_input    = new QComboBox();

  tab1                = new QWidget();
  tab1_layout         = new QVBoxLayout;

  connect(tab0_btn_update,     SIGNAL(clicked()), this, SLOT(on_tab0_btn_update_clicked()));
  connect(tab0_btn_connect,    SIGNAL(clicked()), this, SLOT(on_tab0_btn_connect_clicked()));
  connect(tab0_btn_disconnect, SIGNAL(clicked()), this, SLOT(on_tab0_btn_disconnect_clicked()));

  setCentralWidget(main_widget);
  main_widget->setLayout(main_layout);
  main_layout->addWidget(main_tab);
  ui->statusbar->addPermanentWidget(serial_label);

  serial_label->setText("STATUS_OK");

  main_tab->addTab(tab0, "Console");
  main_tab->addTab(tab1, "Other");
  main_tab->setTabPosition(QTabWidget::North);

  tab0->setLayout(tab0_main_layout);
  tab0_main_layout->addLayout(tab0_header_layout);
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
  tab0_main_layout->addWidget(tab0_text_browser);
  tab0_text_browser->setText("Hello");
  tab0_main_layout->addWidget(tab0_combo_input);
  tab0_combo_input->setEditable(true);

  tab1->setLayout(tab1_layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tab0_btn_update_clicked()
{
  tab0_text_browser->append("on_tab0_btn_update_clicked");
  QList<QSerialPortInfo> ports = serial_connection->list_serial_devices();

  foreach(const QSerialPortInfo &port, ports) {
    tab0_serial_list->addItem(port.portName(), port.systemLocation());
  }
}

void MainWindow::on_tab0_btn_connect_clicked()
{
  tab0_text_browser->append("on_tab0_btn_connect_clicked");
  serial_connection->port_connect(tab0_serial_list->currentData().toString());
}

void MainWindow::on_tab0_btn_disconnect_clicked()
{
  tab0_text_browser->append("on_tab0_btn_disconnect_clicked");
  serial_connection->port_disconnect();
}


