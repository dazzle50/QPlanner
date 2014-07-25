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

#ifndef DAYSMODEL_H
#define DAYSMODEL_H

#include <QAbstractTableModel>

class Day;

/*************************************************************************************************/
/************************ Table model containing all calendar day types **************************/
/*************************************************************************************************/

class DaysModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  DaysModel();                                                     // constructor
  ~DaysModel();                                                    // destructor

  void         initialise();                                       // create initial default contents

  Day*         day( int n );                                       // return pointer to n'th day type
  int          index( Day* d ) { return m_days.indexOf(d); }       // return index of day type, or -1
  int          number() { return m_days.size(); }                  // return number of day types in plan
  QStringList  namesList() const;                                  // return list of day type names
  void         emitDataChangedRow( int );                          // emit data changed signal for row
  void         emitNameChanged() const;                            // emit name changed signal

  void         beginInsert( int );                                 // begin insert process
  void         endInsert() { endInsertColumns(); }                 // end insert process
  void         beginRemove( int );                                 // begin remove process
  void         endRemove() { endRemoveColumns(); }                 // end remove process

  /********************* methods to support QAbstractTableModel ************************/

  int            rowCount( const QModelIndex& parent = QModelIndex() ) const;     // implement virtual row count
  int            columnCount( const QModelIndex& parent = QModelIndex() ) const;  // implement virtual column count
  QVariant       data( const QModelIndex&, int ) const;                           // implement virtual return data
  bool           setData( const QModelIndex&, const QVariant&, int );             // implement virtual set data
  QVariant       headerData( int, Qt::Orientation, int ) const;                   // implement virtual header data
  Qt::ItemFlags  flags( const QModelIndex& ) const;                               // implement virtual return flags

signals:
  void         nameChanged() const;                                // signal that a day name has changed

private:
  QList<Day*>     m_days;      // list of day types available to calendars
};

#endif // DAYSMODEL_H
