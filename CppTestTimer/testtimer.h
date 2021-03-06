//---------------------------------------------------------------------------
/*
TestTimer, class that measures time a test takes
Copyright (C) 2014-2016 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppTestTimer.htm
//---------------------------------------------------------------------------
#ifndef RIBI_TESTTIMER_H
#define RIBI_TESTTIMER_H

#include <string>
#include <vector>
#include <memory>

namespace ribi {

struct TestTimerImpl;

struct TestTimer
{
  explicit TestTimer(
    const std::string& function_name,
    const std::string& file_name,
    const double max_time_sec = 1.0
  ) noexcept;
  TestTimer(const TestTimer&) = delete;
  TestTimer& operator=(const TestTimer&) = delete;
  ~TestTimer() noexcept; //But might terminate the program instead
  static int GetMaxCnt() noexcept;
  static std::string GetVersion() noexcept;
  static std::vector<std::string> GetVersionHistory() noexcept;
  static void SetMaxCnt(const int max_cnt);

  private:
  const std::unique_ptr<TestTimerImpl> m_impl;
};

using test_timer = TestTimer; //Alias

} //~namespace ribi

#endif // RIBI_TESTTIMER_H
