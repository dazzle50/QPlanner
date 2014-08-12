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

#ifndef TIMESPANSPINBOX_H
#define TIMESPANSPINBOX_H

#include <QDoubleSpinBox>

/*************************************************************************************************/
/**************************** SpinBox designed to handle TimeSpan type ***************************/
/*************************************************************************************************/

class TimeSpanSpinBox : public QDoubleSpinBox
{
public:
  TimeSpanSpinBox( QWidget* );                            // constructor

  QValidator::State  validate( QString&, int& ) const;    // reimplement virtual to check for units entry
  QString            textFromValue( double ) const;       // reimplement virtual to suppress unneeded zeros
  void               setUnits( char );                    // update suffix with units
};

#endif // TIMESPANSPINBOX_H
