//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
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
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "conceptmapconcept.h"

#include <boost/lexical_cast.hpp>

#include "conceptmapconceptfactory.h"
#include "conceptmaphelper.h"
#include "conceptmapcompetency.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmapexamplesfactory.h"
#include "trace.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::cmap::Concept::Concept(
  const std::string& name,
  const Examples& examples,
  const bool is_complex,
  const int rating_complexity,
  const int rating_concreteness,
  const int rating_specificity)
  : m_examples{examples},
    m_is_complex{is_complex},
    m_name{name},
    m_rating_complexity{rating_complexity},
    m_rating_concreteness{rating_concreteness},
    m_rating_specificity{rating_specificity}

{
  #ifndef NDEBUG
  Test();
  assert(m_rating_complexity   >= -1);
  assert(m_rating_complexity   <=  2);
  assert(m_rating_concreteness >= -1);
  assert(m_rating_concreteness <=  2);
  assert(m_rating_specificity  >= -1);
  assert(m_rating_specificity  <=  2);
  #endif
}


void ribi::cmap::Concept::SetExamples(const Examples& examples) noexcept
{
  m_examples = examples;
}

void ribi::cmap::Concept::SetIsComplex(const bool is_complex) noexcept
{
  m_is_complex = is_complex;
}


void ribi::cmap::Concept::SetName(const std::string& name) noexcept
{
  m_name = name;
}

void ribi::cmap::Concept::SetRatingComplexity(const int rating_complexity) noexcept
{
  m_rating_complexity = rating_complexity;
  assert(m_rating_complexity >= -1);
  assert(m_rating_complexity <=  2);
}

void ribi::cmap::Concept::SetRatingConcreteness(const int rating_concreteness) noexcept
{
  m_rating_concreteness = rating_concreteness;
  assert(m_rating_concreteness >= -1);
  assert(m_rating_concreteness <=  2);
}

void ribi::cmap::Concept::SetRatingSpecificity(const int rating_specificity) noexcept
{
  m_rating_specificity = rating_specificity;
  assert(m_rating_specificity >= -1);
  assert(m_rating_specificity <=  2);
}

std::string ribi::cmap::Concept::ToStr() const noexcept
{
  std::stringstream s;
  s
    << GetName() << " "
    << GetExamples().ToStr() << " "
    << GetIsComplex() << " "
    << GetRatingComplexity() << " "
    << GetRatingConcreteness() << " "
    << GetRatingSpecificity()
  ;
  return s.str();
}

std::string ribi::cmap::Concept::ToXml() const noexcept
{
  std::stringstream s;
  s
    << "<concept>"
    <<   "<name>"
    <<     GetName()
    <<   "</name>"
    <<   GetExamples().ToXml()
    <<   "<concept_is_complex>"
    <<     GetIsComplex()
    <<   "</concept_is_complex>"
    <<   "<complexity>"
    <<     GetRatingComplexity()
    <<   "</complexity>"
    <<   "<concreteness>"
    <<     GetRatingConcreteness()
    <<   "</concreteness>"
    <<   "<specificity>"
    <<     GetRatingSpecificity()
    <<   "</specificity>"
    << "</concept>"
  ;
  const std::string r = s.str();

  assert(r.size() >= 19);
  assert(r.substr(0,9) == "<concept>");
  assert(r.substr(r.size() - 10,10) == "</concept>");
  return r;
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const Concept& concept) noexcept
{
  os << concept.ToStr();
  return os;
}

bool ribi::cmap::operator==(const ribi::cmap::Concept& lhs, const ribi::cmap::Concept& rhs)
{
  const bool verbose{false};
  if (lhs.GetIsComplex() != rhs.GetIsComplex())
  {
    if (verbose) { TRACE("Concept::IsComplex differs"); }
    return false;
  }
  if (lhs.GetName() != rhs.GetName())
  {
    if (verbose) { TRACE("Concept::Name differs"); }
    return false;
  }
  if (lhs.GetRatingComplexity() != rhs.GetRatingComplexity())
  {
    if (verbose) { TRACE("Concept::RatingComplexity differs"); }
    return false;
  }
  if (lhs.GetRatingConcreteness() != rhs.GetRatingConcreteness())
  {
    if (verbose) { TRACE("Concept::RatingConcreteness differs"); }
    return false;
  }
  if (lhs.GetRatingSpecificity() != rhs.GetRatingSpecificity())
  {
    if (verbose) { TRACE("Concept::RatingSpecificity differs"); }
    return false;
  }
  const auto lhs_examples = lhs.GetExamples();
  const auto rhs_examples = rhs.GetExamples();
  if (lhs_examples == rhs_examples)
  {
    return true;
  }
  else
  {
    if (verbose) { TRACE("Concept::Examples differs: content is different"); }
    return false;
  }
}

bool ribi::cmap::operator!=(const ribi::cmap::Concept& lhs, const ribi::cmap::Concept& rhs)
{
  return !(lhs == rhs);
}

bool ribi::cmap::operator<(const ribi::cmap::Concept& lhs, const ribi::cmap::Concept& rhs)
{
  if (lhs.GetName() < rhs.GetName()) return true;
  if (lhs.GetName() > rhs.GetName()) return false;
  if (lhs.GetExamples() < rhs.GetExamples()) return true;
  if (lhs.GetExamples() != rhs.GetExamples()) return false;
  assert(lhs.GetExamples() == rhs.GetExamples());
  if (lhs.GetRatingComplexity() < rhs.GetRatingComplexity()) return true;
  if (lhs.GetRatingComplexity() > rhs.GetRatingComplexity()) return false;
  if (lhs.GetRatingConcreteness() < rhs.GetRatingConcreteness()) return true;
  if (lhs.GetRatingConcreteness() > rhs.GetRatingConcreteness()) return false;
  return lhs.GetRatingSpecificity() < rhs.GetRatingSpecificity();
}
