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

#ifndef COMMANDRESOURCESETDATA_H
#define COMMANDRESOURCESETDATA_H

#include <QUndoCommand>

#include "model/plan.h"
#include "model/resource.h"
#include "model/resourcesmodel.h"

/*************************************************************************************************/
/*********************** Command for setting Resource data via QUndoStack ************************/
/*************************************************************************************************/

class CommandResourceSetData : public QUndoCommand
{
public:
  CommandResourceSetData( const QModelIndex& index, const QVariant& value )
  {
    // set private variables for new and old values
    m_row       = index.row();
    m_column    = index.column();
    m_new_value = value;
    m_old_value = index.data( Qt::EditRole );

    // construct command description
    setText( QString("Resource %1 %2 = %3")
             .arg( m_row )
             .arg( Resource::headerData( m_column ).toString() )
             .arg( value.toString() ) );
  }

  void  redo()
  {
    // update resource with new value
    bool wasNull = plan->resource( m_row )->isNull();
    plan->resource( m_row )->setData( m_column, m_new_value );

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->resources()->emitDataChangedRow( m_row );
    if ( m_column != Resource::SECTION_COMMENT ) plan->schedule();

    // update plan tab to reflect increase in number of resources
    if ( wasNull ) plan->signalPlanUpdated();
  }

  void  undo()
  {
    // revert resource back to old value
    plan->resource( m_row )->setData( m_column, m_old_value );

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->resources()->emitDataChangedRow( m_row );
    if ( m_column != Resource::SECTION_COMMENT ) plan->schedule();

    // update plan tab to reflect decrease in number of resources
    if ( plan->resource( m_row )->isNull() ) plan->signalPlanUpdated();
  }

private:
  int       m_row;
  int       m_column;
  QVariant  m_new_value;
  QVariant  m_old_value;
};

#endif // COMMANDRESOURCESETDATA_H
