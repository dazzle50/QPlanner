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

#include "taskresources.h"
#include "plan.h"
#include "resourcesmodel.h"

/*************************************************************************************************/
/***************************** Resources assigned to task with plan ******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TaskResources::TaskResources()
{
}

/****************************************** constructor ******************************************/

TaskResources::TaskResources( QString text )
{
  // split text into individual assignments
  m_res.clear();
  QString tag, max;
  foreach( QString part, text.split( ',', QString::SkipEmptyParts ) )
  {
    // split part into tag and max assignment
    if ( part.contains('[') )
    {
      tag = part.section( '[', 0, 0 ).simplified();
      max = part.section( '[', 1 ).remove( ']' );
    }
    else
    {
      tag = part.simplified();
      max = "0";
    }

    Assignment ass;
    ass.tag = tag;
    ass.max = max.toFloat();
    m_res.append( ass );
  }
}

/******************************************** toString *******************************************/

QString TaskResources::toString() const
{
  QString str;

  // build up string equivalent
  foreach( Assignment ass, m_res )
  {
    str += ass.tag;
    if ( ass.max > 0.0 ) str += QString( "[%1]" ).arg( ass.max );
    str += ", ";
  }

  // remove final ", " and return string equivalent
  str.chop(2);
  return str;
}

/******************************************** isEmpty ********************************************/

bool TaskResources::isEmpty() const
{
  // return true if no resources allocated
  return m_res.isEmpty();
}

/******************************************** validate *******************************************/

QString TaskResources::validate( const QString& text )
{
  // split text into individual assignments
  QString error, tag, max;
  foreach( QString part, text.split( ',', QString::SkipEmptyParts ) )
  {
    // split part into tag and max assignment
    if ( part.contains( '[' ) )
    {
      tag = part.section( '[', 0, 0 ).simplified();
      max = part.section( '[', 1 ).remove( ']' );
    }
    else
    {
      tag = part.simplified();
      max = "0";
    }

    if ( !plan->resources()->isAssignable( tag ) )
      error += QString( "'%1' is not an assignable resource.\n" ).arg( tag );

    bool ok;
    float num = max.toFloat( &ok );
    if ( !ok || num < 0.0 )
      error += QString( "'%1' is not a valid number for '%2'.\n" ).arg( max ).arg( tag );
  }

  // remove final '\n' and return validation error text
  error.chop(1);
  return error;
}

/******************************************** process ********************************************/

void TaskResources::process()
{
  // process internal string format into alloc in easy efficient access format
  alloc.clear();

  foreach( Assignment assign, m_res )
    foreach( Resource* res, plan->resources()->resourceSet( assign.tag ) )
    {
      float num = assign.max;
      if ( num == 0.0 ) num = 1e9;
      if ( alloc.contains(res) ) num += alloc.value(res);
      alloc.insert( res, num );
    }
}
