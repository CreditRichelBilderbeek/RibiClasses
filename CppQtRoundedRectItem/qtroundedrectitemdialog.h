//---------------------------------------------------------------------------
/*
QtRoundedRectItem, rectangular-shaped QGraphicsItem
Copyright (C) 2012-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtRoundedRectItem.htm
//---------------------------------------------------------------------------
#ifndef QTROUNDEDRECTDIALOG_H
#define QTROUNDEDRECTDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#pragma GCC diagnostic pop

namespace Ui {
class QtRoundedRectItemDialog;
}

namespace ribi {

struct QtRoundedRectItem;

class QtRoundedRectItemDialog : public QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT
    
public:

  explicit QtRoundedRectItemDialog(QWidget *parent = 0);
  QtRoundedRectItemDialog(const QtRoundedRectItemDialog&) = delete;
  QtRoundedRectItemDialog& operator=(const QtRoundedRectItemDialog&) = delete;
  ~QtRoundedRectItemDialog() noexcept;

  void DisableSetSize() noexcept;
  void DoSomethingRandom() noexcept;

  boost::shared_ptr<const QtRoundedRectItem> GetItem() const noexcept;

  static int GetMinimumHeight() noexcept { return 276; }

  const Ui::QtRoundedRectItemDialog * GetUi() const noexcept { return ui; }
        Ui::QtRoundedRectItemDialog * GetUi()       noexcept { return ui; }

  ///Read the X value directly from GUI
  double GetUiX() const noexcept;
  ///Read the Y value directly from GUI
  double GetUiY() const noexcept;

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  void SetItem(const boost::shared_ptr<QtRoundedRectItem>& item) noexcept;

  ///Set the X value directly to GUI
  void SetUiX(const double x) noexcept;
  ///Set the Y value directly to GUI
  void SetUiY(const double y) noexcept;


protected:
  void keyPressEvent(QKeyEvent * event);

private slots:

  void on_box_contour_pen_width_valueChanged(double arg1);
  void on_box_focus_pen_width_valueChanged(double arg1);
  void on_box_height_including_pen_valueChanged(double arg1);
  void on_box_height_valueChanged(double arg1);
  void on_box_radius_x_valueChanged(double arg1);
  void on_box_radius_y_valueChanged(double arg1);
  void on_box_width_including_pen_valueChanged(double arg1);
  void on_box_width_valueChanged(double arg1);
  void on_box_x_valueChanged(double arg1);
  void on_box_y_valueChanged(double arg1);

private:
  Ui::QtRoundedRectItemDialog *ui;

  ///The QtRoundedRectItem to work on
  boost::shared_ptr<QtRoundedRectItem> m_item;

  void OnContourPenChanged(QtRoundedRectItem * const qtitem) noexcept;
  void OnFocusPenChanged(QtRoundedRectItem * const qtitem) noexcept;
  void OnPosChanged(QtRoundedRectItem * const qtitem) noexcept;
  void OnRadiusXchanged(QtRoundedRectItem * const qtitem) noexcept;
  void OnRadiusYchanged(QtRoundedRectItem * const qtitem) noexcept;
  void OnWidthChanged(QtRoundedRectItem * const qtitem) noexcept;
  void OnHeightChanged(QtRoundedRectItem * const qtitem) noexcept;
};

} //~namespace ribi

#endif // QTROUNDEDRECTDIALOG_H
