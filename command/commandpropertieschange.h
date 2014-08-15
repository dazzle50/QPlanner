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

#ifndef COMMANDPROPERTIESCHANGE_H
#define COMMANDPROPERTIESCHANGE_H

#include <QUndoCommand>

#include "model/plan.h"
//#include "model/tasksmodel.h"

/*************************************************************************************************/
/************************ Command plan properties changes for QUndoStack *************************/
/*************************************************************************************************/

class CommandPropertiesChange : public QUndoCommand
{
public:
  CommandPropertiesChange( QString   title_new, QString   title_old,
                           DateTime  start_new, DateTime  start_old,
                           int       cal_new,   int       cal_old,
                           QString   dtf_new,   QString   dtf_old,
                           QString   notes_new, QString   notes_old )
  {
    // set private variables for new and old values
    m_title_new = title_new;
    m_title_old = title_old;
    m_start_new = start_new;
    m_start_old = start_old;
    m_cal_new   = cal_new;
    m_cal_old   = cal_old;
    m_dtf_new   = dtf_new;
    m_dtf_old   = dtf_old;
    m_notes_new = notes_new;
    m_notes_old = notes_old;

    // construct command description
    QString desc;
    if ( m_title_new != m_title_old ) desc += "Title/";
    if ( m_start_new != m_start_old ) desc += "Start/";
    if ( m_cal_new   != m_cal_old   ) desc += "Default Calendar/";
    if ( m_dtf_new   != m_dtf_old   ) desc += "DateTime Format/";
    if ( m_notes_new != m_notes_old ) desc += "Notes/";
    desc.chop( 1 );
    desc.append( " updated" );
    setText( desc );
  }

  void  redo()
  {
    // update plan with new values
    if ( m_title_new != m_title_old ) plan->setTitle( m_title_new );
    if ( m_start_new != m_start_old ) plan->setStart( m_start_new );
    if ( m_cal_new   != m_cal_old   ) plan->setCalendar( m_cal_new );
    if ( m_dtf_new   != m_dtf_old   ) plan->setDatetimeFormat( m_dtf_new );
    if ( m_notes_new != m_notes_old ) plan->setNotes( m_notes_new );
    plan->emitPlanUpdated();

    // if default calendar or start changed, need to re-schedule
    if ( m_cal_new   != m_cal_old   ||
         m_start_new != m_start_old ) plan->schedule();
  }

  void  undo()
  {
    // revert plan back to old values
    if ( m_title_new != m_title_old ) plan->setTitle( m_title_old );
    if ( m_start_new != m_start_old ) plan->setStart( m_start_old );
    if ( m_cal_new   != m_cal_old   ) plan->setCalendar( m_cal_old );
    if ( m_dtf_new   != m_dtf_old   ) plan->setDatetimeFormat( m_dtf_old );
    if ( m_notes_new != m_notes_old ) plan->setNotes( m_notes_old );
    plan->emitPlanUpdated();

    // if default calendar or start changed, need to re-schedule
    if ( m_cal_new   != m_cal_old   ||
         m_start_new != m_start_old ) plan->schedule();
  }

private:
  QString   m_title_new;
  QString   m_title_old;
  DateTime  m_start_new;
  DateTime  m_start_old;
  int       m_cal_new;
  int       m_cal_old;
  QString   m_dtf_new;
  QString   m_dtf_old;
  QString   m_notes_new;
  QString   m_notes_old;
};

#endif // COMMANDPROPERTIESCHANGE_H
