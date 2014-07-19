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
  CommandDaySetData( const QModelIndex& index, const QVariant& value )
  {
    // set private variables for new and old values
    m_row     = index.row();
    m_column  = index.column();
    m_old_day = *( plan->day( m_row ) );
    m_value   = value;

    // construct command description
    setText( QString("Day %1 %2 = %3")
             .arg( m_row+1 )
             .arg( Day::headerData( m_column ).toString() )
             .arg( value.toString() ) );
  }

  void  redo()
  {
    // update day with new value
    plan->day( m_row )->setData( m_column, m_value );

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->days()->emitDataChangedRow( m_row );
    if ( m_column != Day::SECTION_NAME ) plan->schedule();
  }

  void  undo()
  {
    // revert day type to old definition
    *( plan->day( m_row ) ) = m_old_day;

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->days()->emitDataChangedRow( m_row );
    if ( m_column != Day::SECTION_NAME ) plan->schedule();
  }

private:
  int       m_row;
  int       m_column;
  Day       m_old_day;  // need whole copy for undo since reducing number of periods will lose data
  QVariant  m_value;
};

#endif // COMMANDDAYSETDATA_H
