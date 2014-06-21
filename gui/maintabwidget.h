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

#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QTabWidget>

/*************************************************************************************************/
/***************************** Tabbed widget containing main screens *****************************/
/*************************************************************************************************/

namespace Ui { class MainTabWidget; }

class MainTabWidget : public QTabWidget
{
  Q_OBJECT
public:
  explicit MainTabWidget( QWidget* parent = nullptr );          // constructor
  ~MainTabWidget();                                             // destructor

  void  endEdits();                  // end any task/resource/calendar/day edits in progress
  void  setModels();                 // ensure views are connected to correct models

private:
  Ui::MainTabWidget*   ui;           // user interface created using qt designer
};

#endif // MAINTABWIDGET_H
