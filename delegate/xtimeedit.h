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

#ifndef XTIMEEDIT_H
#define XTIMEEDIT_H

#include <QTimeEdit>

#include "model/datetime.h"

/*************************************************************************************************/
/******************** Editor to allow entry of Time 00:00 to 24:00 inclusive *********************/
/*************************************************************************************************/

class XTimeEdit : public QTimeEdit
{
public:
  XTimeEdit( QWidget*, Time, Time );     // constructor

  void     stepBy( int );                // reimplemented to have rollover wrapping on stepping
  QString  toString();                   // get time in string "hh:mm" format
  void     setTime( QString );           // set time in from string "hh:mm" format
};

#endif // XTIMEEDIT_H
