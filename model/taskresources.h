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

#ifndef TASKRESOURCES_H
#define TASKRESOURCES_H

#include <QString>
#include <QList>
#include <QHash>

class Resource;

/*************************************************************************************************/
/***************************** Resources assigned to task with plan ******************************/
/*************************************************************************************************/

class TaskResources
{
public:
  TaskResources();                                   // constructor
  TaskResources( QString );                          // constructor

  QString         toString() const;                  // return string for display in tasks view
  bool            isEmpty() const;                   // return true if no resources allocated
  static QString  validate( const QString& );        // return any validation failures
  void            process();                         // process internal string format into alloc

  QHash<Resource*,float>    alloc;                   // processed assignments in easy efficient access format

  struct Assignment
  {
    QString   tag;        // initials or name or org or group or alias or role etc
    float     max;        // 0 (zero) means unlimited
  };

private:
  QList<Assignment>    m_res;       // list of resource assignments in original string format
};

#endif // TASKRESOURCES_H
