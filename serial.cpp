#include "serial.h"

Serial::Serial(QObject *parent) : QObject(parent)
{
  serial_port = new QSerialPort(this);

  connect(serial_port, SIGNAL(readyRead()), this, SLOT(read()));
}

QList<QSerialPortInfo> Serial::list_serial_devices()
{
  QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
  return ports;
}

bool Serial::port_connect(QString port_name)
{
  serial_port->setPortName(port_name);
  serial_port->open(QIODevice::ReadWrite);

  if(!serial_port->isOpen()) {
    return false;
  }

  serial_port->setBaudRate(QSerialPort::Baud115200);
  serial_port->setDataBits(QSerialPort::Data8);
  serial_port->setParity(QSerialPort::NoParity);
  serial_port->setStopBits(QSerialPort::OneStop);
  serial_port->setFlowControl(QSerialPort::NoFlowControl);

  return true;
}


void Serial::port_disconnect()
{
  if (serial_port->isOpen()) {
    serial_port->close();
  }
}

void Serial::port_write(QByteArray &data)
{
  if (serial_port->isOpen()) {
    serial_port->write(data);
   }
}


void Serial::read()
{
  while (serial_port->bytesAvailable() != 0) {
    QByteArray data = serial_port->readAll();
    emit read_recieved(data);
  }
}

void Serial::port_error(QSerialPort::SerialPortError error)
{
  QString message;

  switch (error) {

    case QSerialPort::NoError:
      break;

    case QSerialPort::DeviceNotFoundError:
      message = tr("Device not found");
    break;

    case QSerialPort::OpenError:
      message = tr("Can't open device");
    break;

    case QSerialPort::NotOpenError:
      message = tr("Not open error");
    break;

    case QSerialPort::ResourceError:
      message = tr("Port disconnected");
    break;

    case QSerialPort::UnknownError:
      message = tr("Unknown error");
    break;

    default:
      message = "Serial port error: " + QString::number(error);
    break;
  }

  if(!message.isEmpty()) {

    emit error_message(message);

    if(serial_port->isOpen()) {
      serial_port->close();
    }
  }
}
