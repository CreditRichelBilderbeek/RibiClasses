//---------------------------------------------------------------------------
/*
QtKeyboardFriendlyGraphicsView, an keyboard friendly QGraphicsView
Copyright (C) 2012-2015 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppQtKeyboardFriendlyGraphicsView.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtkeyboardfriendlygraphicsview.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSimpleTextItem>

#include "trace.h"

#pragma GCC diagnostic pop

ribi::QtKeyboardFriendlyGraphicsView::QtKeyboardFriendlyGraphicsView(QWidget* parent)
  : QGraphicsView(new QGraphicsScene,parent),
    m_signal_update{},
    m_verbose{false}
{
  #ifndef NDEBUG
  Test();
  #endif
}

QGraphicsItem* ribi::QtKeyboardFriendlyGraphicsView::GetClosest(
  const QGraphicsItem* const focus_item,
  const std::vector<QGraphicsItem *>& items) const
{
  assert(!items.empty());
  assert(std::count(items.begin(),items.end(),focus_item) == 0);

  QGraphicsItem* best = nullptr;
  double best_distance = std::numeric_limits<double>::max();
  for (QGraphicsItem* const item:items)
  {
    if (!(item->flags() & QGraphicsItem::ItemIsFocusable)) continue;
    if (!(item->isVisible())) continue;

    assert(item != focus_item);
    const double distance = GetDistance(focus_item->pos(),item->pos());
    if (distance < best_distance)
    {
      best_distance = distance;
      best = item;
    }
  }
  return best; //best can be nullptr
}

double ribi::QtKeyboardFriendlyGraphicsView::GetDistance(const QPointF& a, const QPointF& b)
{
  const double dx = a.x() - b.x();
  const double dy = a.y() - b.y();
  return std::sqrt((dx * dx) + (dy * dy));
}

std::vector<QGraphicsItem *> ribi::QtKeyboardFriendlyGraphicsView::GetItemsAbove(const QGraphicsItem* const focus_item) const
{
  std::vector<QGraphicsItem *> v;
  const QList<QGraphicsItem *> items = this->items();
  //Look for items between North-West and North-East first
  for(QGraphicsItem* const item: items)
  {
    const double dy = item->pos().y() - focus_item->pos().y();
    if (dy < 0.0)  //Use '<' (instead of '<=') to leave out focus_item
    {
      const double dx = item->pos().x() - focus_item->pos().x();
      if (std::abs(dx) < std::abs(dy))
      {
        assert(item != focus_item);
        v.push_back(item);
      }
    }
  }
  if (!v.empty()) return v;
  //Look for items North (from West, through North, through East)
  for(QGraphicsItem* const item: items)
  {
    const double dy = item->pos().y() - focus_item->pos().y();
    if (dy < 0.0)  //Use '<' (instead of '<=') to leave out focus_item
    {
      assert(item != focus_item);
      v.push_back(item);
    }
  }
  return v;
}

std::vector<QGraphicsItem *> ribi::QtKeyboardFriendlyGraphicsView::GetItemsBelow(const QGraphicsItem* const focus_item) const
{
  std::vector<QGraphicsItem *> v;
  const QList<QGraphicsItem *> items = this->items();
  //Look for items between South-East and South-West first
  for(QGraphicsItem* const item: items)
  {
    const double dy = item->pos().y() - focus_item->pos().y();
    if (dy > 0.0)  //Use '>' (instead of '>=') to leave out focus_item
    {
      const double dx = item->pos().x() - focus_item->pos().x();
      if (std::abs(dx) < std::abs(dy))
      {
        assert(item != focus_item);
        v.push_back(item);
      }
    }
  }
  if (!v.empty()) return v;
  //Look for items South
  for(QGraphicsItem* const item: items)
  {
    const double dy = item->pos().y() - focus_item->pos().y();
    if (dy > 0.0)  //Use '>' (instead of '>=') to leave out focus_item
    {
      assert(item != focus_item);
      v.push_back(item);
    }
  }
  return v;
}

std::vector<QGraphicsItem *> ribi::QtKeyboardFriendlyGraphicsView::GetItemsLeft(const QGraphicsItem* const focus_item) const
{
  std::vector<QGraphicsItem *> v;
  const QList<QGraphicsItem *> items = this->items();
  //Look for items between South-West and North-West first
  for(QGraphicsItem* const item: items)
  {
    const double dx = item->pos().x() - focus_item->pos().x();
    if (dx < 0.0)  //Use '<' (instead of '<=') to leave out focus_item
    {
      const double dy = item->pos().y() - focus_item->pos().y();
      if (std::abs(dy) < std::abs(dx))
      {
        assert(item != focus_item);
        v.push_back(item);
      }
    }
  }
  //Look for items Westwards
  for(QGraphicsItem* const item: items)
  {
    const double dx = item->pos().x() - focus_item->pos().x();
    if (dx < 0.0)  //Use '<' (instead of '<=') to leave out focus_item
    {
      assert(item != focus_item);
      v.push_back(item);
    }
  }
  return v;
}

std::vector<QGraphicsItem *> ribi::QtKeyboardFriendlyGraphicsView::GetItemsRight(const QGraphicsItem* const focus_item) const
{
  std::vector<QGraphicsItem *> v;
  const QList<QGraphicsItem *> items = this->items();
  //Look for items between North-East and South-East first
  for(QGraphicsItem* const item: items)
  {
    const double dx = item->pos().x() - focus_item->pos().x();
    if (dx > 0.0) //Use '>' (instead of '>=') to leave out focus_item
    {
      const double dy = item->pos().y() - focus_item->pos().y();
      if (std::abs(dy) < std::abs(dx))
      {
        assert(item != focus_item);
        v.push_back(item);
      }
    }
  }
  //Look for items Eastwards
  for(QGraphicsItem* const item: items)
  {
    const double dx = item->pos().x() - focus_item->pos().x();
    if (dx > 0.0) //Use '>' (instead of '>=') to leave out focus_item
    {
      assert(item != focus_item);
      v.push_back(item);
    }
  }
  return v;
}

std::string ribi::QtKeyboardFriendlyGraphicsView::GetVersion() noexcept
{
  return "1.3";
}

std::vector<std::string> ribi::QtKeyboardFriendlyGraphicsView::GetVersionHistory() noexcept
{
  return {
    "2012-12-13: version 1.0: initial version",
    "2012-12-31: version 1.1: improved moving focus",
    "2015-08-24: version 1.2: move item with CTRL, add selected with SHIFT, can move multiple items",
    "2015-09-18: version 1.3: added verbosity"
  };
}

void ribi::QtKeyboardFriendlyGraphicsView::keyPressEvent(QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier) {
    KeyPressEventCtrl(event);
  }
  else if (event->modifiers() & Qt::ShiftModifier) {
    KeyPressEventShift(event);
  }
  else {
    KeyPressEventNoModifiers(event);
  }

}

void ribi::QtKeyboardFriendlyGraphicsView::KeyPressEventCtrl(QKeyEvent *event) noexcept
{
  //CTRL: Move items
  assert(event->modifiers() & Qt::ControlModifier);
  if (m_verbose) { std::clog << "CTRL pressed: try to move items" << std::endl; }
  double delta_x{0.0};
  double delta_y{0.0};
  switch (event->key())
  {
    case Qt::Key_Up:
      if (m_verbose) { std::clog << "Moving selected item (s) up" << std::endl; }
      delta_y = -10.0;
      break;
    case Qt::Key_Right:
      if (m_verbose) { std::clog << "Moving selected item (s) right" << std::endl; }
      delta_x =  10.0;
      break;
    case Qt::Key_Down:
      if (m_verbose) { std::clog << "Moving selected item (s) down" << std::endl; }
      delta_y =  10.0;
      break;
    case Qt::Key_Left:
      if (m_verbose) { std::clog << "Moving selected item (s) left" << std::endl; }
      delta_x = -10.0;
      break;
  }
  for (const auto item: scene()->selectedItems())
  {
    assert(item);
    if (!(item->flags() & QGraphicsItem::ItemIsMovable)) { continue; }
    item->setPos(item->pos() + QPointF(delta_x,delta_y));
    if (m_verbose) { std::clog << "(1) m_signal_update(item)" << std::endl; }
    m_signal_update(item);
  }
  scene()->update();
}

void ribi::QtKeyboardFriendlyGraphicsView::KeyPressEventNoModifiers(QKeyEvent *event) noexcept
{
  assert(!(event->modifiers() & Qt::ShiftModifier));
  assert(!(event->modifiers() & Qt::ControlModifier));
  //Can be nullptr
  QGraphicsItem* const current_focus_item = scene()->focusItem();

  if (!current_focus_item &&
    ( event->key() == Qt::Key_Up || event->key() == Qt::Key_Right || event->key() == Qt::Key_Down || event->key() == Qt::Key_Right )
  )
  {
    if (m_verbose) { std::clog << "Cannot tranfer selectedness when there is no focus" << std::endl; }
    return;
  }

  const auto current_selected_items = scene()->selectedItems();

  std::vector<QGraphicsItem *> new_selected_items;

  switch (event->key())
  {
    case Qt::Key_Up:
      assert(current_focus_item);
      if (m_verbose) { std::clog << "Move focus up" << std::endl; }
      new_selected_items = GetItemsAbove(current_focus_item);
    break;
    case Qt::Key_Tab:
    case Qt::Key_Right:
      assert(current_focus_item);
      if (m_verbose) { std::clog << "Move focus right" << std::endl; }
      new_selected_items = GetItemsRight(current_focus_item);
      break;
    case Qt::Key_Down:
      assert(current_focus_item);
      if (m_verbose) { std::clog << "Move focus down" << std::endl; }
      new_selected_items = GetItemsBelow(current_focus_item);
      break;
    case Qt::Key_Left:
    case Qt::Key_Backtab:
      assert(current_focus_item);
      if (m_verbose) { std::clog << "Move focus left" << std::endl; }
      new_selected_items = GetItemsLeft(current_focus_item);
      break;
    case Qt::Key_Space:
      if (m_verbose) { std::clog << "Set random focus" << std::endl; }
      SetRandomFocus();
      return;
    case Qt::Key_Question:
    {
      if (const QGraphicsItem* const item = scene()->focusItem())
      {
        std::clog << __func__ << ": QGraphicsItem selected at ("
          << item->pos().x() << ","
          << item->pos().y() << ")"
          << std::endl;
      }
      else
      {
        std::clog << __func__ << ": no QGraphicsItem selected" << std::endl;
      }
    }
    return;
    default:
    {
      if (m_verbose) { std::clog << "Calling QGraphicsView::keyPressEvent" << std::endl; }
      QGraphicsView::keyPressEvent(event);
      this->update();
      return;
    }
  }

  //assert(current_focus_item);
  //if (m_verbose) { std::clog << "clearFocus of current_focus_item" << std::endl; }
  //current_focus_item->setEnabled(false);
  //current_focus_item->clearFocus();
  //current_focus_item->setEnabled(true);

  //Unselect currently selected
  for (const auto item: current_selected_items)
  {
    item->setSelected(false);
    m_signal_update(item);
  }
  //Select newly selected
  for (const auto item: new_selected_items)
  {
    item->setSelected(true);
    m_signal_update(item);
  }
  //Transfer focus
  current_focus_item->clearFocus();
  if (new_selected_items.empty())
  {
    //Nothing, focus is already lost
  }
  else
  {
    QGraphicsItem* const new_focus_item = GetClosest(current_focus_item,new_selected_items);
    if (new_focus_item) { new_focus_item->setFocus(); }
  }
}

void ribi::QtKeyboardFriendlyGraphicsView::KeyPressEventShift(QKeyEvent *event) noexcept
{
  assert(event->modifiers() & Qt::ShiftModifier);
  //Can be nullptr
  QGraphicsItem* const current_focus_item = scene()->focusItem();
  if (!current_focus_item)
  {
    if (m_verbose) { std::clog << "Cannot add items without a focus" << std::endl; }
    return;
  }

  std::vector<QGraphicsItem *> new_added_selected_items;

  switch (event->key())
  {
    case Qt::Key_Up:
      if (m_verbose) { std::clog << "Move focus up" << std::endl; }
      new_added_selected_items = GetItemsAbove(current_focus_item);
    break;
    case Qt::Key_Tab:
    case Qt::Key_Right:
      if (m_verbose) { std::clog << "Move focus right" << std::endl; }
      new_added_selected_items = GetItemsRight(current_focus_item);
      break;
    case Qt::Key_Down:
      if (m_verbose) { std::clog << "Move focus down" << std::endl; }
      new_added_selected_items = GetItemsBelow(current_focus_item);
      break;
    case Qt::Key_Left:
    case Qt::Key_Backtab:
      if (m_verbose) { std::clog << "Move focus left" << std::endl; }
      new_added_selected_items = GetItemsLeft(current_focus_item);
      break;
    default:
    {
      if (m_verbose) { std::clog << "Calling QGraphicsView::keyPressEvent" << std::endl; }
      QGraphicsView::keyPressEvent(event);
      this->update();
      return;
    }
  }

  //Add selectedness
  for (auto item: new_added_selected_items)
  {
    item->setSelected(true);
    m_signal_update(item);
  }

   //Transfer focus
  current_focus_item->clearFocus();
  if (new_added_selected_items.empty())
  {
    //Nothing, focus is already lost
  }
  else
  {
    QGraphicsItem* const new_focus_item = GetClosest(current_focus_item,new_added_selected_items);
    if (new_focus_item) { new_focus_item->setFocus(); }
  }


  this->update();
}

void ribi::QtKeyboardFriendlyGraphicsView::SetRandomFocus()
{
  if (QGraphicsItem* const item = scene()->focusItem())
  {
    if (m_verbose) { std::clog << "Removing current focus" << std::endl; }
    //Really lose focus
    item->setEnabled(false);
    item->setSelected(false); // #239
    item->clearFocus();
    item->setEnabled(true);
    m_signal_update(item);
  }
  else
  {
    if (m_verbose) { std::clog << "No focused item to remove focus of" << std::endl; }
  }

  if (m_verbose) { std::clog << "Remove selectedness of all " << scene()->selectedItems().size() << " selected items" << std::endl; }
  for (auto selected_item: scene()->selectedItems())
  {
    selected_item->setSelected(false);
    m_signal_update(selected_item);
  }


  //Let a random item receive focus
  const QList<QGraphicsItem *> all_items = this->items();
  QList<QGraphicsItem *> items;
  std::copy_if(all_items.begin(),all_items.end(),std::back_inserter(items),
    [](const QGraphicsItem* const item)
    {
      return (item->flags() & QGraphicsItem::ItemIsFocusable)
        && item->isVisible();
    }
  );
  if (m_verbose) { std::clog << "Obtained " << all_items.size() << " focusable items" << std::endl; }

  if (!items.empty())
  {
    const int i = std::rand() % items.size();
    if (m_verbose) { std::clog << "Giving the " << i << "th item focus" << std::endl; }
    assert(i >= 0);
    assert(i < items.size());
    auto& new_focus_item = items[i];
    new_focus_item->setSelected(true); // #239
    new_focus_item->setFocus();
    m_signal_update(new_focus_item);
  }
  else
  {
    if (m_verbose) { std::clog << "No focusable items" << std::endl; }
  }
}
