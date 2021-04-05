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

#include "serial.h"
#include <QDebug>

Serial::Serial(QObject *parent) : QObject(parent) {

  serial_port = new QSerialPort(this);

  rx_state           = RX_IDLE_E;
  rx_length          = 0;
  rx_addr            = 0;
  rx_timeout         = 0;
  rx_crc_low         = 0;
  rx_crc_high        = 0;
  rx_crc_enabled     = false;
  rx_parse_as_string = false;

  rx_timer = new QTimer(this);
  rx_timer->setInterval(10);
  rx_timer->start();

  re_axi_wr = QRegularExpression("^wr\\s+(\\d*)\\s+(\\d*)");
  re_axi_rd = QRegularExpression("^rd\\s+(\\d*)$");

  connect(serial_port, SIGNAL(readyRead()), this, SLOT(port_read()));
  connect(rx_timer, SIGNAL(timeout()), this, SLOT(rx_timer_timeout_slot()));
}


QList<QSerialPortInfo> Serial::list_serial_devices() {
  QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
  return ports;
}


bool Serial::port_connect(QString port_name) {
  serial_port->setPortName(port_name);
  serial_port->open(QIODevice::ReadWrite);

  if (!serial_port->isOpen()) { return false; }

  serial_port->setBaudRate(QSerialPort::Baud115200);
  serial_port->setDataBits(QSerialPort::Data8);
  serial_port->setParity(QSerialPort::NoParity);
  serial_port->setStopBits(QSerialPort::OneStop);
  serial_port->setFlowControl(QSerialPort::NoFlowControl);

  return true;
}


void Serial::port_disconnect() {
  if (serial_port->isOpen()) { serial_port->close(); }
}


bool Serial::port_is_open() {
  return serial_port->isOpen();
}


QString Serial::port_name() {
  return serial_port->portName();
}


bool Serial::cmd_read(QByteArray &data) {

  QByteArray tx_packet;
  QByteArray tx_payload;
  int number;
  unsigned short crc;

  // Checking if this is an AXI read command
  match    = re_axi_rd.match(QString::fromStdString(data.toStdString()).trimmed());
  hasMatch = match.hasMatch();

  if (hasMatch) {

    QString araddr = match.captured(1);

    // Append read command
    tx_payload.append('R');

    // Append araddr
    number = araddr.toUInt();
    tx_payload.append((char)((number >> 24) & 0xFF));
    tx_payload.append((char)((number >> 16) & 0xFF));
    tx_payload.append((char)((number >> 8)  & 0xFF));
    tx_payload.append((char) (number        & 0xFF));

    // CRC
    crc = crc_16((const unsigned char*)tx_payload.constData(), 5);

    // Packet
    tx_packet.append(LENGTH_8_BITS_C);
    tx_packet.append(5);
    tx_packet.append(tx_payload);
    tx_packet.append((char)((crc >> 8)  & 0xFF));
    tx_packet.append((char) (crc        & 0xFF));

    if (serial_port->isOpen()) {
      serial_port->write(tx_packet);
      serial_port->flush();
    }

    return true;

  } else {
    return false;
  }
}


bool Serial::cmd_write(QByteArray &data) {

  QByteArray tx_packet;
  QByteArray tx_payload;
  int number;
  unsigned short crc;

  // Checking if this is an AXI write command
  match    = re_axi_wr.match(QString::fromStdString(data.toStdString()).trimmed());
  hasMatch = match.hasMatch();

  if (hasMatch) {

    QString awaddr = match.captured(1);
    QString wdata  = match.captured(2);

    // Append write command
    tx_payload.append('W');

    // Append awaddr
    number = awaddr.toUInt();
    tx_payload.append((char)((number >> 24) & 0xFF));
    tx_payload.append((char)((number >> 16) & 0xFF));
    tx_payload.append((char)((number >> 8)  & 0xFF));
    tx_payload.append((char) (number        & 0xFF));

    // Append wdata
    number = wdata.toUInt();
    tx_payload.append((char)((number >> 24) & 0xFF));
    tx_payload.append((char)((number >> 16) & 0xFF));
    tx_payload.append((char)((number >> 8)  & 0xFF));
    tx_payload.append((char) (number        & 0xFF));

    // CRC
    crc = crc_16((const unsigned char*)tx_payload.constData(), 9);

    // Packet
    tx_packet.append(LENGTH_8_BITS_C);
    tx_packet.append(9);
    tx_packet.append(tx_payload);
    tx_packet.append((char)((crc >> 8)  & 0xFF));
    tx_packet.append((char) (crc        & 0xFF));

    if (serial_port->isOpen()) {
      serial_port->write(tx_packet);
      serial_port->flush();
    }

    return true;

  } else {
    return false;
  }
}


void Serial::port_write(QByteArray &data) {

  QByteArray tx_data;
  int tx_length = data.size();

  if (cmd_write(data)) { return; }
  if (cmd_read(data))  { return; }

  tx_data.append(LENGTH_8_BITS_C);
  tx_data.append((char)tx_length);
  tx_data.append(data);

  if (serial_port->isOpen()) {
    serial_port->write(tx_data);
  }
  serial_port->flush();
}


void Serial::port_read() {

  while (serial_port->bytesAvailable() != 0) {
    QByteArray data = serial_port->readAll();
    rx_parser(data);
  }
}


void Serial::port_error(QSerialPort::SerialPortError error) {

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


void Serial::rx_set_crc_enabled(bool setting) {
  rx_crc_enabled = setting;
}


void Serial::rx_set_parse_as_string(bool setting) {
  rx_parse_as_string = setting;
}


bool Serial::rx_get_parse_as_string() {
  return rx_parse_as_string;
}


void Serial::rx_parser(QByteArray &data) {
  unsigned char rx_data;
  bool          crc_enabled;

  for (int i = 0; i < data.length(); i++) {

    rx_data = data[i];

    switch (rx_state) {

      case RX_IDLE_E:

        rx_addr    = 0;
        rx_length  = 0;
        rx_string.clear();
        rx_string.append(rx_data);
        rx_timeout = RX_TIMEOUT_C;

        //if (rx_parse_as_string) {
        //  rx_state = RX_FIND_NEWLINE_E;
        //} else

        crc_enabled = rx_data & CRC_ENABLED_BIT_C; //

        if (rx_data & STRING_ENABLED_BIT_C) {
            qDebug() << "is_string = true";
        } else {
            qDebug() << "is_string = false";
        }
        qDebug() << rx_data;

        if (rx_data & STRING_ENABLED_BIT_C) {
          rx_state = RX_FIND_NEWLINE_E;
          rx_string.clear();
        } else if ((rx_data & LENGTH_8_BITS_C) == LENGTH_8_BITS_C) {
          rx_state = RX_LENGTH_LOW_E;
        } else if ((rx_data & LENGTH_16_BITS_C) == LENGTH_16_BITS_C) {
          rx_state = RX_LENGTH_HIGH_E;
        }
        break;


      case RX_FIND_NEWLINE_E:

        rx_string.append(rx_data);
        if (rx_data == '\n' || rx_data == '\r') {
          rx_state = RX_IDLE_E;
          emit read_received(rx_string);
        }
        rx_timeout = RX_TIMEOUT_C;
        break;


      case RX_LENGTH_HIGH_E:

        rx_length  = (unsigned int)rx_data << 8;
        rx_state   = RX_LENGTH_LOW_E;
        rx_timeout = RX_TIMEOUT_C;
        break;


      case RX_LENGTH_LOW_E:

        rx_length |= (unsigned int)rx_data;

        if (rx_length <= RX_BUFFER_SIZE_C && rx_length > 0) {
          rx_state   = RX_READ_PAYLOAD_E;
          rx_timeout = RX_TIMEOUT_C;
        } else {
          rx_state = RX_LENGTH_LOW_E;
        }
        break;


      case RX_READ_PAYLOAD_E:

        rx_buffer[rx_addr++] = rx_data;
        if (rx_addr == rx_length) {
          if (crc_enabled) {
            rx_state = RX_READ_CRC_LOW_E;
          } else {
            emit read_received(data);
            rx_state = RX_IDLE_E;
          }
        }
        rx_timeout = RX_TIMEOUT_C;
        break;


      case RX_READ_CRC_LOW_E:

        rx_state    = RX_READ_CRC_HIGH_E;
        rx_timeout  = RX_TIMEOUT_C;
        rx_crc_high = rx_data;
        break;


      case RX_READ_CRC_HIGH_E:

        rx_crc_low = rx_data;
        rx_timeout = RX_TIMEOUT_C;
        if (crc_16(rx_buffer, rx_length) == ((unsigned short)rx_crc_high << 8 | (unsigned short)rx_crc_low)) {
          emit read_received(data);
        }
        break;


      default:
        rx_state = RX_IDLE_E;
        break;
    }
  }
}


void Serial::rx_timer_timeout_slot() {

  if (rx_timeout) {
    rx_timeout--;
  } else {
    rx_state = RX_IDLE_E;
  }
}


unsigned short Serial::crc_16(const unsigned char *buf, unsigned int len) {

  unsigned short checksum = 0;
  for (int i = 0; i < int(len); i++) {
    checksum = crc_16_table[(((checksum >> 8) ^ *buf++) & 0xFF)] ^ (checksum << 8);
  }
  return checksum;
}
