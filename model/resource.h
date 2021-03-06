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

#ifndef RESOURCE_H
#define RESOURCE_H

#include "datetime.h"

class Calendar;
class QXmlStreamWriter;
class QXmlStreamReader;

/*************************************************************************************************/
/************************************* Single plan resource **************************************/
/*************************************************************************************************/

class Resource
{
public:
  Resource();                                                        // constructor (normal)
  Resource( bool );                                                  // constructor (unassigned)
  Resource( QXmlStreamReader* );                                     // constructor

  void              saveToStream( QXmlStreamWriter* );               // write resource data to xml stream
  static QVariant   headerData( int );                               // return column header data
  QVariant          data( int, int );                                // return data for column & role
  void              setData( int, const QVariant& );                 // set data value for column

  bool              isNull() const { return m_initials.isNull(); }   // is the task null (blank)
  QString           initials() const { return m_initials; }          // return initials
  Calendar*         calendar() const { return m_calendar; }          // return resource calendar
  Date              start() const;                                   // return resource start date
  Date              end() const;                                     // return resource end date
  QList<QString>    assignable() const;                              // return assignable names
  bool              hasTag( QString ) const;                         // return true if tag matches

  enum sections                            // sections to be displayed by view
  {
    SECTION_MINIMUM  = 0,
    SECTION_INITIALS = 0,
    SECTION_NAME     = 1,
    SECTION_ORG      = 2,
    SECTION_GROUP    = 3,
    SECTION_ROLE     = 4,
    SECTION_ALIAS    = 5,
    SECTION_START    = 6,
    SECTION_END      = 7,
    SECTION_AVAIL    = 8,
    SECTION_COST     = 9,
    SECTION_CALENDAR = 10,
    SECTION_COMMENT  = 11,
    SECTION_MAXIMUM  = 11
  };

private:
  QString            m_initials;           // must be unique across all resources in model
  QString            m_name;               // free text
  QString            m_org;                // free text
  QString            m_group;              // free text
  QString            m_role;               // free text
  QString            m_alias;              // free text
  Date               m_start;              // date availability starts inclusive
  Date               m_end;                // date availability end inclusive
  float              m_availability;       // number available
  float              m_cost;               // cost TODO
  Calendar*          m_calendar;           // calendar for resource
  QString            m_comment;            // free text
};

#endif // RESOURCE_H
