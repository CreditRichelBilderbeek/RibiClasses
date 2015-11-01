//---------------------------------------------------------------------------
/*
Container, class with container class helper functions
Copyright (C) 2013-2015 Richel Bilderbeek

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
//From http://www.richelbilderbeek.nl/CppContainer.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "container.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "fuzzy_equal_to.h"
#include "testtimer.h"
#include "trace.h"
#pragma GCC diagnostic pop

template <class Container>
std::string ContainerToStrImpl(const Container& c, const std::string& seperator)
{
  std::stringstream s;
  std::copy(c.begin(),c.end(),
    std::ostream_iterator<typename Container::value_type>(s,seperator.c_str()));
  return s.str();
}

ribi::Container::Container()
{
  #ifndef NDEBUG
  Test();
  #endif
}

bool ribi::Container::AllAboutEqual(
  const std::vector<double>& v,
  const double tolerance) const noexcept
{
  assert(!v.empty());
  fuzzy_equal_to f(tolerance);
  const double first_value{v[0]};

  return std::count_if(
    std::begin(v),
    std::end(v),
    [f,first_value](const double x) { return f(first_value,x); }
  )
    == boost::numeric_cast<int>(v.size())
  ;
}

std::string ribi::Container::Concatenate(const std::vector<std::string>& v, const std::string& seperator) const noexcept
{
  std::stringstream s;
  for (const auto& t: v) { s << t << seperator; }
  std::string str = s.str();
  //Remove seperator
  if (!str.empty())
  {
    str.resize(str.size() - seperator.size());
  }
  return str;
}

std::string ribi::Container::ContainerToStr(
  const std::vector<std::string>& s,
  const std::string& seperator
) const noexcept
{
  return ContainerToStrImpl(s,seperator);
}


std::string ribi::Container::GetVersion() const noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::Container::GetVersionHistory() const noexcept
{
  return {
    "2014-xx-xx: Version 1.0: initial version",
    "2014-06-14: Version 1.1: added SeperateString"
    "2014-07-30: Version 1.2: added Concatenate"
  };
}

std::string ribi::Container::RemoveWhitespace(std::string s) const noexcept
{
  boost::algorithm::erase_all(s," ");
  boost::algorithm::erase_all(s,"\t");
  boost::algorithm::erase_all(s,"\n");
  return s;
}

std::vector<std::string> ribi::Container::SeperateString(
  const std::string& input,
  const char seperator) const noexcept
{
  std::vector<std::string> v;
  boost::algorithm::split(v,input,
    std::bind2nd(std::equal_to<char>(),seperator),
    boost::algorithm::token_compress_on);
  return v;
}

std::string ribi::Container::ToUpper(std::string s) const
{
  std::transform(
    std::begin(s), std::end(s), std::begin(s),
    std::ptr_fun<int,int>(std::toupper)
  );
  return s;
}

#ifndef NDEBUG
void ribi::Container::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};
  const Container c;
  if (verbose) { TRACE("Concatenate: empty vector with empty seperator must result in an empty string"); }
  {
    const std::vector<std::string> v{};
    const std::string s{c.Concatenate(v,"")};
    assert(s.empty());
  }
  if (verbose) { TRACE("Concatenate: empty vector with longer seperator must result in an empty string"); }
  {
    const std::vector<std::string> v{};
    const std::string s{c.Concatenate(v,"[wont be used]")};
    assert(s.empty());
  }
  if (verbose) { TRACE("Concatenate: vector with one string and empty seperator must result in that string"); }
  {
    const std::string s{"any string"};
    const std::vector<std::string> v{s};
    const std::string t{c.Concatenate(v,"")};
    assert(s == t);
  }
  if (verbose) { TRACE("Concatenate: vector with one string and longer seperator must result in that string"); }
  {
    const std::string s{"any string again"};
    const std::vector<std::string> v{s};
    const std::string t{c.Concatenate(v,"[wont be used]")};
    assert(s == t);
  }
  if (verbose) { TRACE("Concatenate: vector with two string and empty seperator must result in the summed string"); }
  {
    const std::string s{"any string"};
    const std::string t{"goes on"};
    const std::string expected{s+t};
    const std::vector<std::string> v{s,t};
    const std::string u{c.Concatenate(v,"")};
    assert(u == expected);
  }
  if (verbose) { TRACE("Concatenate: vector with two string and longer seperator must result in the summed string"); }
  {
    const std::string s{"any string"};
    const std::string t{"goes on"};
    const std::string seperator{" "};
    const std::string expected{s+seperator+t};
    const std::vector<std::string> v{s,t};
    const std::string u{c.Concatenate(v,seperator)};
    assert(u == expected);
  }
  //SeperateString
  {
    { //Single input, seperator of type char
      const auto v = c.SeperateString("a",',');
      assert(v.size() == 1);
      assert(v[0]=="a");
    }
    { //Two inputs, seperator of type char
      const auto v = c.SeperateString("a,b",',');
      assert(v.size() == 2);
      assert(v[0]=="a");
      assert(v[1]=="b");
    }
    {
      //Five inputs, seperator of type char
      const auto v = c.SeperateString("a,bb,ccc,dddd,eeeee",',');
      assert(v.size() == 5);
      assert(v[0]=="a");
      assert(v[1]=="bb");
      assert(v[2]=="ccc");
      assert(v[3]=="dddd");
      assert(v[4]=="eeeee");
    }
    { //Three inputs, of which one empty, seperator of type char
      const auto v = c.SeperateString("a, ,ccc",',');
      assert(v.size() == 3);
      assert(v[0]=="a");
      assert(v[1]==" ");
      assert(v[2]=="ccc");
    }
  }
  //AllAboutEqual
  {
    std::vector<double> v = { 0.9, 1.0, 1.1 };
    assert(c.AllAboutEqual(v,1.0));
    assert(!c.AllAboutEqual(v,0.01));
  }
  //AllUnique
  {
    assert( c.AllUnique( std::vector<int>( {} ) ));
    assert( c.AllUnique( std::vector<int>( {1} ) ));
    assert( c.AllUnique( std::vector<int>( {1,2} ) ));
    assert( c.AllUnique( std::vector<int>( {1,2,3} ) ));
    assert(!c.AllUnique( std::vector<int>( {1,2,1} ) ));
  }
  //HasNoOverlap
  {
    assert( c.HasNoOverlap( std::vector<int>( {} ),std::vector<int>( {} )));
    assert( c.HasNoOverlap( std::vector<int>( {1} ),std::vector<int>( {} )));
    assert( c.HasNoOverlap( std::vector<int>( {1} ),std::vector<int>( {2} )));
    assert(!c.HasNoOverlap( std::vector<int>( {1} ),std::vector<int>( {1} )));
    assert( c.HasNoOverlap( std::vector<int>( {1,2} ),std::vector<int>( {3} )));
    assert(!c.HasNoOverlap( std::vector<int>( {1,2,3} ),std::vector<int>( {1,2,3} )));
    assert( c.HasNoOverlap( std::vector<int>( {1,2,3} ),std::vector<int>( {4,5,6} )));
  }
  //ToUpper
  {
    assert(c.ToUpper("abc") == "ABC");
    assert(c.ToUpper("ABC") == "ABC");
  }
  //RemoveWhitespace
  {
    assert(c.RemoveWhitespace("abc") == "abc");
    assert(c.RemoveWhitespace("a\tb \nc") == "abc");
  }
  //Sort
  {
    std::vector<int> v = {3,1,2};
    const std::vector<int> w = {1,2,3};
    c.Sort(v);
    assert(v == w);
  }
  //Count
  {
    const auto v = {3,1,2};
    assert(c.Count(v,1) == 1);
    assert(c.Count(v,4) == 0);
  }
  //CountIf
  {
    const auto v = {3,1,2};
    assert(c.CountIf(v, [](const auto i) { return i > 0; }) == 3);
    assert(c.CountIf(v, [](const auto i) { return i > 1; }) == 2);
    assert(c.CountIf(v, [](const auto i) { return i > 2; }) == 1);
    assert(c.CountIf(v, [](const auto i) { return i > 3; }) == 0);
  }
  //CountIf
  {
    const auto v = {3,1,2};
    assert(c.FindIf(v, [](const auto i) { return i > 0; }) != std::end(v));
    assert(c.FindIf(v, [](const auto i) { return i > 1; }) != std::end(v));
    assert(c.FindIf(v, [](const auto i) { return i > 2; }) != std::end(v));
    assert(c.FindIf(v, [](const auto i) { return i > 3; }) == std::end(v));
  }
}
#endif
