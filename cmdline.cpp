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

#include "cmdline.h"

CmdLine::CmdLine(QWidget *parent) : QWidget(parent)
{
  cmd_layout    = new QHBoxLayout;
  cmd_line      = new QComboBox();

  cmd_line->setEditable(true);
  cmd_layout->addWidget(cmd_line);

  this->setLayout(cmd_layout);
  this->installEventFilter(this);
}

bool CmdLine::eventFilter(QObject *object, QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {

    QKeyEvent* key = static_cast<QKeyEvent*>(event);

    // Enter
    if ((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return)) {
      emit return_pressed(cmd_line->currentText());
      cmd_line->setEditText("");
    } else {
      return QObject::eventFilter(object, event);
    }
    return true;
  }
  return QObject::eventFilter(object, event);
}
