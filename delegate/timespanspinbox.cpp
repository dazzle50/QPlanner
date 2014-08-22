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

#include "timespanspinbox.h"
#include "model/timespan.h"

/*************************************************************************************************/
/**************************** SpinBox designed to handle TimeSpan type ***************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TimeSpanSpinBox::TimeSpanSpinBox( QWidget* parent = nullptr ) : QDoubleSpinBox( parent )
{
  // set default characteristics
  setMaximum( 9999.0 );
}

/******************************************* setUnits ********************************************/

void  TimeSpanSpinBox::setUnits( char units )
{
  // units displayed in QDoubleSpinBox suffix
  setSuffix( QString(" %1").arg(units) );

  // if units is seconds, no decimal places otherwise two decimal places
  if ( units == TimeSpan::UNIT_MINUTES ) setDecimals( 0 );
  else                                   setDecimals( 2 );
}

/***************************************** textFromValue *****************************************/

QString  TimeSpanSpinBox::textFromValue( double value ) const
{
  // remove any unnecessary zeros and decimal point
  QString str = QDoubleSpinBox::textFromValue( value );
  while ( str.contains(".") && str.right(1) == "0" )
    str.chop(1);
  if ( str.right(1) == "." ) str.chop(1);
  return str;
}

/******************************************* validate ********************************************/

QValidator::State  TimeSpanSpinBox::validate( QString& text, int& pos ) const
{
  // if pos less than one then couldn't have just entered new units
  if ( pos < 1 ) return QDoubleSpinBox::validate( text, pos );

  // check if new units entered
  char  units = text.at(pos-1).toLatin1();

  if ( units == 'h' ) units = TimeSpan::UNIT_HOURS;

  if ( units == TimeSpan::UNIT_MINUTES ||
       units == TimeSpan::UNIT_HOURS   ||
       units == TimeSpan::UNIT_DAYS    ||
       units == TimeSpan::UNIT_WEEKS   ||
       units == TimeSpan::UNIT_MONTHS  ||
       units == TimeSpan::UNIT_YEARS ) const_cast<TimeSpanSpinBox*>( this )->setUnits( units );

  // suppress minus sign as TimeSpanSpinBox doesn't accept negative durations
  if ( units == '-' ) return QValidator::Invalid;

  // return QDoubleSpinBox validator state
  return QDoubleSpinBox::validate( text, pos );
}
