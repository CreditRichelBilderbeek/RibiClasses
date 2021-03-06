//---------------------------------------------------------------------------
/*
Counter, an incrementing counter
Copyright (C) 2011-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppCounter.htm
//---------------------------------------------------------------------------
#ifndef COUNTER_H
#define COUNTER_H

#include <string>
#include <vector>

namespace ribi {

///Counter is a counter
///It may appear to be a rather trivial class, but I use it mostly to test signals:
///  MyClass my_class;
///  Counter c{0}; //For receiving the signal
///  c.SetVerbosity(true);
///  my_class.m_signal.connect(
///    boost::bind(&MyClass::AnyMemberFunction,&c)
///  );
///  my_class.DoSomethingThatEmitsAsignal();
///  assert(c.Get() == 1);
struct Counter
{
  ///Counter is constructed with an initial count, which is initialized to
  ///zero by default
  explicit Counter(const int initial_count = 0) noexcept;

  ///Increments count
  void Inc() noexcept;

  //Prefix
  Counter& operator++()
  {
    Inc();
    return *this;
  }

  ///Get the count
  int Get() const noexcept { return m_count; }

  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;

  void SetVerbosity(const bool verbosity) noexcept { m_verbosity = verbosity; }

  private:
  ///The count
  int m_count;

  bool m_verbosity;
};

bool operator==(const Counter& lhs, const Counter& rhs) noexcept;
bool operator!=(const Counter& lhs, const Counter& rhs) noexcept;

} //~namespace ribi

#endif // COUNTER_H
