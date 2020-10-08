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
