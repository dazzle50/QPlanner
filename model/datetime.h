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

#ifndef DATETIME_H
#define DATETIME_H

#include <QtGlobal>

typedef  quint32 DateTime;   // simple date-time between 1AD and ~8000AD with one minute resolution
typedef  qint32  Date;       // simple date between 1AD and ~8000AD with one day resolution
typedef  qint16  Time;       // simple time between 00:00H and 24:00H with one minute resolution

/*************************************************************************************************/
/******************************** XTime provides an enhanced QTime *******************************/
/*************************************************************************************************/

class XTime
{
public:
  static Time      time( int h, int m )    // return qint16 Time hours & minutes
                    { return h*60 + m; }
};


#endif // DATETIME_H
