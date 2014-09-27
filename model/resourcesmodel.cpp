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

#include "resourcesmodel.h"
#include "resource.h"
#include "plan.h"
#include "command/commandresourcesetdata.h"

#include <QXmlStreamWriter>

/*************************************************************************************************/
/**************************** Table model containing all resources *******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourcesModel::ResourcesModel() : QAbstractTableModel()
{
  // create 'unassigned' resource, also known as resource zero, usually hidden
  m_resources.append( new Resource(true) );
}

/****************************************** destructor *******************************************/

ResourcesModel::~ResourcesModel()
{
  // delete all resources in model
  foreach( Resource* r, m_resources ) delete r;
}

/****************************************** initialise ******************************************/

void ResourcesModel::initialise()
{
  // create initial blank resource
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
  m_resources.append( new Resource() );
}

/********************************************* resource ***********************************************/

Resource* ResourcesModel::resource( int n )
{
  // return pointer to n'th resource, checking n is in range first
  Q_ASSERT( n >= 0 );
  Q_ASSERT( n < m_resources.size() );
  return m_resources.at(n);
}

/******************************************** number *********************************************/

int ResourcesModel::number()
{
  // return number of non-null resources in plan
  int  count = 0;
  foreach( Resource* res, m_resources )
    if ( !res->isNull() ) count++;

  return count;
}

/***************************************** saveToStream ******************************************/

void  ResourcesModel::saveToStream( QXmlStreamWriter* stream )
{
  // write resources data to xml stream
  stream->writeStartElement( "resources-data" );

  foreach( Resource* r, m_resources )
  {
    // don't write 'unassigned' resource 0
    if ( plan->index(r) == 0 ) continue;

    stream->writeStartElement( "resource" );
    stream->writeAttribute( "id", QString("%1").arg(plan->index(r)) );
    if ( !r->isNull() ) r->saveToStream( stream );
    stream->writeEndElement();
  }

  // close resources-data element
  stream->writeEndElement();
}

/**************************************** loadFromStream *****************************************/

void  ResourcesModel::loadFromStream( QXmlStreamReader* stream )
{
  // load resources data from xml stream
  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if resource element create new resource
    if ( stream->isStartElement() && stream->name() == "resource" )
      m_resources.append( new Resource(stream) );

    // when reached end of resources data return
    if ( stream->isEndElement() && stream->name() == "resources-data" ) break;
  }

  updateAssignable();
}

/******************************************** rowCount *******************************************/

int ResourcesModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_resources.size();
}

/****************************************** columnCount ******************************************/

int ResourcesModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return Resource::SECTION_MAXIMUM + 1;
}

/********************************************** data *********************************************/

QVariant ResourcesModel::data( const QModelIndex& index,
                               int role  = Qt::DisplayRole ) const
{
  // if index matches override index, return override value
  if ( index == m_overrideIndex )
    if ( role == Qt::DisplayRole || role == Qt::EditRole )
      return m_overrideValue;

  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_resources.size() ) return QVariant();

  return m_resources.at( row )->data( index.column(), role );
}

/******************************************** setData ********************************************/

bool ResourcesModel::setData( const QModelIndex& index,
                              const QVariant& value,
                              int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;

  // if value hasn't changed, don't proceed
  if ( value == data( index, role ) ) return false;

  // set data via undo/redo command
  plan->undostack()->push( new CommandResourceSetData( index, value ) );
  return true;
}

/****************************************** headerData *******************************************/

QVariant ResourcesModel::headerData( int section,
                                     Qt::Orientation orientation,
                                     int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return resource header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Resource::headerData( section );
  return QString("%1").arg( section );
}

/********************************************* flags *********************************************/

Qt::ItemFlags ResourcesModel::flags( const QModelIndex& index ) const
{
  // if resource is null (blank), then only initials is editable, others are not
  int row = index.row();
  int col = index.column();
  if ( m_resources.at(row)->isNull() &&  col != Resource::SECTION_INITIALS )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise item is selectable/enabled/editable
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

/************************************ slotCalendarNameChange *************************************/

void ResourcesModel::slotCalendarNameChange()
{
  // slot to receive calendar name changed signal, update Calendar column
  emit dataChanged( QAbstractTableModel::index( 0, Resource::SECTION_CALENDAR ),
                    QAbstractTableModel::index( rowCount(), Resource::SECTION_CALENDAR ) );

  // also signal plan updated to ensure plan tab is also up-to-date
  plan->signalPlanUpdated();
}

/************************************** emitDataChangedRow ***************************************/

void ResourcesModel::emitDataChangedRow( int row )
{
  // emit data changed signal for row
  emit dataChanged( QAbstractTableModel::index( row, 0 ),
                    QAbstractTableModel::index( row, columnCount() ) );
}

/************************************** initialsIsDuplicate **************************************/

bool ResourcesModel::initialsIsDuplicate( const QString& id, int row )
{
  // if id matches a resources initials on any other row return true
  for ( int r = 0 ; r < m_resources.size() ; r++ )
    if ( r != row && id == m_resources.at(r)->initials() ) return true;

  // no match found, so return false
  return false;
}

/**************************************** updateAssignable ***************************************/

void ResourcesModel::updateAssignable()
{
  // start with an empty set
  m_assignable.clear();

  // determine assignable list
  foreach( Resource* res, m_resources )
    foreach( QString str, res->assignable() )
      m_assignable.insert( str );
}

/**************************************** resourceSet ***************************************/

QSet<Resource*> ResourcesModel::resourceSet( QString tag )
{
  // return set of resources that have tag
  QSet<Resource*>  set;
  foreach( Resource* res, m_resources )
    if ( res->hasTag( tag ) ) set.insert( res );

  return set;
}
