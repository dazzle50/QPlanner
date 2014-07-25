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

#ifndef COMMANDCALENDARSETDATA_H
#define COMMANDCALENDARSETDATA_H

#include <QUndoCommand>

#include "model/plan.h"
#include "model/calendar.h"
#include "model/calendarsmodel.h"

/*************************************************************************************************/
/*********************** Command for setting Calendar data via QUndoStack ************************/
/*************************************************************************************************/

class CommandCalendarSetData : public QUndoCommand
{
public:
  CommandCalendarSetData( const QModelIndex& index, const QVariant& value )
  {
    // set private variables for new and old values
    m_row     = index.row();
    m_column  = index.column();
    m_old_cal = *( plan->calendar( m_column ) );
    m_value   = value;

    // construct command description
    setText( QString("Calendar %1 %2 = %3")
             .arg( m_column+1 )
             .arg( Calendar::headerData( m_row ).toString() )
             .arg( value.toString() ) );
  }

  void  redo()
  {
    // update calendar with new value
    plan->calendar( m_column )->setData( m_row, m_value );

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->calendars()->emitDataChangedColumn( m_column );
    if ( m_row == Calendar::SECTION_NAME ) plan->calendars()->emitNameChanged();
    if ( m_row != Calendar::SECTION_NAME ) plan->schedule();
  }

  void  undo()
  {
    // revert calendar to old definition, checking if table model number of rows changes
    int oldRows = plan->calendars()->rowCount();
    *( plan->calendar( m_column ) ) = m_old_cal;
    int newRows = plan->calendars()->rowCount();
    if ( oldRows != newRows )
    {
      // table model number of rows has changed, so needs special handling
      plan->calendar( m_column )->m_cycleLength = m_value.toInt();

      if ( newRows > oldRows ) plan->calendars()->beginInsert( newRows - oldRows );
      if ( newRows < oldRows ) plan->calendars()->beginRemove( oldRows - newRows );

      *( plan->calendar( m_column ) ) = m_old_cal;

      if ( newRows > oldRows ) plan->calendars()->endInsert();
      if ( newRows < oldRows ) plan->calendars()->endRemove();
    }

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->calendars()->emitDataChangedColumn( m_column );
    if ( m_row == Calendar::SECTION_NAME ) plan->calendars()->emitNameChanged();
    if ( m_row != Calendar::SECTION_NAME ) plan->schedule();
  }

private:
  int       m_row;
  int       m_column;
  Calendar  m_old_cal;  // need whole copy for undo since reducing cycle length will lose data
  QVariant  m_value;
};

#endif // COMMANDCALENDARSETDATA_H
