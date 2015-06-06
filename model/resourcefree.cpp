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

#include "resourcefree.h"

/*************************************************************************************************/
/************************** Remaining free assignable resource quantity **************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourceFree::ResourceFree()
{
  // initialse private variables
  m_free.clear();
  m_reliable = false;
}

/****************************************** initialise *******************************************/

void ResourceFree::initialise( Date start, Date end, float quantity )
{
  // initialise with resource basics
  if ( start == XDate::NULL_DATE ) start = XDate::MIN_DATE;
  DateTime s = start * 1440u;

  if ( end == XDate::NULL_DATE ) end = XDate::MAX_DATE;
  DateTime e = end * 1440u;

  m_free.clear();
  Free  free = { e, quantity };
  m_free.insert( s, free );
  m_reliable = true;
}

/******************************************** assign *********************************************/

bool ResourceFree::assign(DateTime, DateTime, float)
{
  // assign resource, returning true if sufficient, else false

  return m_reliable;
}

/********************************************* free **********************************************/

bool ResourceFree::free( DateTime dt, DateTime& end, float& quantity )
{
  // return remaining free
  QMap<DateTime, Free>::const_iterator i = m_free.constBegin();
  while ( i != m_free.constEnd() )
  {
    DateTime s = i.key();
    DateTime e = i.value().end;
    float    q = i.value().free;

    if ( dt < s )  // before first block
    {
      end = e;
      quantity = 0.0f;
      return m_reliable;
    }

    if ( dt >= s && dt <= e ) // dt in this block
    {
      end = e;
      quantity = q;
      return m_reliable;
    }

    ++i;
  }

  return m_reliable;
}
