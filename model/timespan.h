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

#ifndef TIMESPAN_H
#define TIMESPAN_H

#include <QString>
#include <QChar>
#include <QDateTime>
#include <cmath>

/*************************************************************************************************/
/********************************** Quantity of time with units **********************************/
/*************************************************************************************************/

class TimeSpan
{
public:
  enum span_type
  {
    UNIT_MINUTES   = 'M',
    UNIT_HOURS     = 'H',
    UNIT_DAYS      = 'd',
    UNIT_WEEKS     = 'w',
    UNIT_MONTHS    = 'm',
    UNIT_YEARS     = 'y',
    UNIT_INVALID   = 'I'      // invalid time-span
  };

/****************************************** constructor ******************************************/

  TimeSpan()
  {
    // initialise default invalid time-span
    m_units = UNIT_INVALID;
    m_num   = 0.0;
  }

/****************************************** constructor ******************************************/

  TimeSpan( float num, char units )
  {
    // construct time-span from number and units
    m_units = units;
    m_num   = num;
  }

/****************************************** constructor ******************************************/

  TimeSpan( QString str )
  {
    // construct time-span from string
    str = str.simplified();
    QString lastchr = str.right(1);
    if ( QString("0123456789.").contains(lastchr) )
      m_units = UNIT_DAYS;   // no units specified so assume 'days'
    else
    {
      if ( QString("MHdwmy").contains(lastchr) )   // check if valid units
      {
        m_units = lastchr.at(0).toLatin1();
        str.chop(1);
      }
      else
      {
        m_units = UNIT_INVALID;
        m_num   = 0.0;
        return;
      }
    }

    bool  ok;
    m_num = str.toDouble( &ok );
    if ( !ok ) m_units = UNIT_INVALID;    // check remainder converted to number ok

    // only allow integer minutes
    if ( m_units == UNIT_MINUTES ) m_num = floor(m_num);
  }

/******************************************** methods ********************************************/

  TimeSpan   operator-() const
  {
    return TimeSpan( -m_num, m_units );
  }

  QString    toString() const
  {
    if ( m_units == UNIT_INVALID ) return "-";
    return QString("%1 %2").arg( m_num ).arg( m_units );
  }

  bool       isValid() const { return ( m_units != UNIT_INVALID ); }
  float      number() const  { return m_num; }
  char       units() const   { return m_units; }

private:
  float      m_num;        // number of units time quantity
  char       m_units;      // units for time quantity
};

#endif // TIMESPAN_H
