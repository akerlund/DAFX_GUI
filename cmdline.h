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

#ifndef CMDLINE_H
#define CMDLINE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QKeyEvent>
#include <QFile>
#include <QMap>
#include <QRegularExpression>

class CmdLine : public QWidget {

  Q_OBJECT

  public:
    explicit CmdLine(QWidget *parent = nullptr);
    bool cmd_parser(QString cmd);
    void load_cmd_file();
    bool eventFilter(QObject *object, QEvent *e);

  private:
    QMap<QString, int> commands;
    QComboBox    *cmd_line;
    QHBoxLayout  *cmd_layout;

  signals:
    void return_pressed(QString line);
};

#endif
