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

#ifndef RESOURCEFREE_H
#define RESOURCEFREE_H

#include "datetime.h"

#include <QMap>

/*************************************************************************************************/
/************************** Remaining free assignable resource quantity **************************/
/*************************************************************************************************/

class ResourceFree
{
public:
  ResourceFree();                                       // constructor

  void   initialise( Date, Date, float );               // initialise with resource basics
  bool   assign( DateTime, DateTime, float );           // assign resource
  bool   free( DateTime, DateTime&, float& );           // return remaining free

private:
  struct Free
  {
    DateTime   end;
    float      free;
  };

  QMap<DateTime, Free>   m_free;      // ordered list of remaining assignable resource
  bool                   m_reliable;  // set false after failed assign() etc
};

#endif // RESOURCEFREE_H
