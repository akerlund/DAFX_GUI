#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

class Serial : public QObject
{
  Q_OBJECT

public:
  explicit Serial(QObject *parent = nullptr);
  QList<QSerialPortInfo> list_serial_devices();
  bool port_connect(QString port_name);
  void port_disconnect();
  void port_write(QByteArray &data);
  void port_error(QSerialPort::SerialPortError error);


private:
  QSerialPort *serial_port;

signals:
  void read_recieved(QByteArray data);
  void error_message(QString msg);

private slots:
  void read();

};

#endif // SERIAL_H
