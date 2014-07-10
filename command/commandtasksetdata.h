/***************************************************************************
 *   Copyright (C) 2014 by Richard Crook                                   *
 *   http://code.google.com/p/projectplanner                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef COMMANDTASKSETDATA_H
#define COMMANDTASKSETDATA_H

#include <QUndoCommand>

#include "model/plan.h"
#include "model/tasksmodel.h"
#include "model/task.h"

/*************************************************************************************************/
/************************* Command for setting Task data via QUndoStack **************************/
/*************************************************************************************************/

class CommandTaskSetData : public QUndoCommand
{
public:
  CommandTaskSetData( Task* old_task, int col, const QVariant& new_value )
  {
    // set private variables for new and old values
    m_old_task  = *old_task;
    m_row       = plan->index( old_task );
    m_column    = col;
    m_new_value = new_value;

    // construct command description
    setText( QString("Task %1 %2 = %3")
             .arg( m_row )
             .arg( Task::headerData( col ).toString() )
             .arg( new_value.toString() ) );
  }

  void  redo()
  {
    // update task with new value
    plan->task( m_row )->setDataDirect( m_column, m_new_value );
    plan->schedule();
  }

  void  undo()
  {
    // revert task back to old values
    Task* task = plan->task( m_row );
    *task = m_old_task;
    //if ( task->isNull() ) plan->tasks()->setSummaries();
    plan->schedule();
  }

private:
  Task      m_old_task;
  int       m_row;
  int       m_column;
  QVariant  m_new_value;
};

#endif // COMMANDTASKSETDATA_H
