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

#ifndef COMMANDDAYSETDATA_H
#define COMMANDDAYSETDATA_H

#include <QUndoCommand>

#include "model/plan.h"
#include "model/day.h"
#include "model/daysmodel.h"

/*************************************************************************************************/
/*********************** Command for setting Day type data via QUndoStack ************************/
/*************************************************************************************************/

class CommandDaySetData : public QUndoCommand
{
public:
  CommandDaySetData( int row, int col, const QVariant& new_value, const QVariant& old_value )
  {
    // set private variables for new and old values
    m_row       = row;
    m_column    = col;
    m_new_value = new_value;
    m_old_value = old_value;

    // construct command description
    setText( QString("Day %1 %2 = %3")
             .arg( row )
             .arg( Day::headerData( col ).toString() )
             .arg( new_value.toString() ) );
  }

  void  redo()
  {
    // update resource with new value
    plan->day( m_row )->setDataDirect( m_column, m_new_value );
    //plan->days()->emitDataChangedRow( m_row );

    if ( m_row != Day::SECTION_NAME ) plan->schedule();
  }

  void  undo()
  {
    // revert resource back to old value
    plan->day( m_row )->setDataDirect( m_column, m_old_value );
    //plan->days()->emitDataChangedRow( m_row );

    if ( m_row != Day::SECTION_NAME ) plan->schedule();
  }

private:
  int       m_row;
  int       m_column;
  QVariant  m_new_value;
  QVariant  m_old_value;
};

#endif // COMMANDDAYSETDATA_H
