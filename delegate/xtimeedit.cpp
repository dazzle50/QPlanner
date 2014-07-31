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

#include "xtimeedit.h"

/*************************************************************************************************/
/******************** Editor to allow entry of Time 00:00 to 24:00 inclusive *********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

XTimeEdit::XTimeEdit( QWidget* parent, Time min, Time max ) : QTimeEdit( parent )
{
  // uses QTimeEdit but with minutes as hours and seconds as minutes so can hold 24:00
  setDisplayFormat( "mm:ss" );
  setMinimumTime( QTime( 0, 0 ).addSecs( min ) );
  setMaximumTime( QTime( 0, 0 ).addSecs( max ) );
  setWrapping( true );
}

/******************************************** stepBy *********************************************/

void XTimeEdit::stepBy( int steps )
{
  // reimplemented to have rollover wrapping on stepping
  QTime newTime;
  switch ( currentSection() )
  {
    case QTimeEdit::SecondSection:  // really minutes!
      newTime = time().addSecs( steps );
      if ( newTime > maximumTime() ) newTime = maximumTime();
      if ( newTime < minimumTime() ) newTime = minimumTime();
      QTimeEdit::setTime( newTime );
      break;

    case QTimeEdit::MinuteSection:  // really hours!
      newTime = time().addSecs( steps*60 );
      if ( newTime > maximumTime() ) newTime = maximumTime();
      if ( newTime < minimumTime() ) newTime = minimumTime();
      QTimeEdit::setTime( newTime );
      break;

    default:
      qDebug("XTimeEdit::stepBy - unexpected current section");
      QTimeEdit::stepBy( steps );
  }
}

/****************************************** toString *********************************************/

QString XTimeEdit::toString()
{
  // get editor time converting to new format and string format "hh:mm"
  int hours = time().minute();
  int mins  = time().second();
  return QString("%1:%2").arg( hours, 2, 10, QChar('0') ).arg( mins, 2, 10, QChar('0') );
}

/******************************************* setTime *********************************************/

void XTimeEdit::setTime( QString time )
{
  // set time from string "hh:mm" format but with minutes as hours and seconds as minutes
  int hours = time.left(2).toInt();
  int mins  = time.right(2).toInt();
  QTimeEdit::setTime( QTime( 0, hours, mins ) );
}
