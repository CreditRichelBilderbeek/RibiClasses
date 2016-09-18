#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "modelfunctionparser.h"

#include <cassert>
#include <stdexcept>

#include <boost/math/constants/constants.hpp>

//Warp's function parser
#include "fparser.hh"

#include "ribi_random.h"
#include "templocale.h"
#pragma GCC diagnostic pop

ribi::ModelFunctionParser::ModelFunctionParser(
  std::string my_function,
  const std::string& variable_name)
  : m_parser{new FunctionParser}
{
  #ifndef NDEBUG
  Test();
  #endif
  assert(m_parser);

  const double pi = boost::math::constants::pi<double>();
  m_parser->AddConstant("pi",pi);
  m_parser->AddConstant("tau",2.0*pi);
  m_parser->AddFunction("rand",MyRand,1);

  //FunctionParser assumes the dot ('.') as a decimal seperator,
  //therefore, convert all comma's in my_function to dots
  //std::replace(std::begin(my_function),std::end(my_function),',','.');
  //assert(std::count(std::begin(my_function),std::end(my_function),',') == 0
  //  && "FunctionParser assumes dots as decimal seperator");

  m_parser->Parse(my_function,variable_name);
  if (m_parser->GetParseErrorType()!= FunctionParser::FP_NO_ERROR)
  {
    const std::string error
      = "InputFunctionParser cannot parse '"
      + my_function
      + "' with variable '"
      + variable_name
      + "' (note: this can have to do that FunctionParser assumes an English locale,"
      + "as '0.0' should be accepted)"
    ;
    throw std::runtime_error(error.c_str());
  }
}

double ribi::ModelFunctionParser::Evaluate(const double x) const
{
  const double xs[1] = { x };
  const double y = m_parser->Eval(xs);
  return y;
}

std::string ribi::ModelFunctionParser::GetVersion() noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::ModelFunctionParser::GetVersionHistory() noexcept
{
  return {
    "201x-xx-xx: version 1.0: initial version",
    "2015-01-03: version 1.1: set locale to English in Desktop version", //SET_LOCALE_TO_ENGLISH_MODELFUNCTIONPARSER
    "2015-01-04: version 1.2: made locale local"
  };
}

double ribi::ModelFunctionParser::MyRand(const double * const max) noexcept
{
  assert(max);
  return (*max) * Random().GetFraction();
}

#ifndef NDEBUG
void ribi::ModelFunctionParser::Test() noexcept
{
  //Set std::locale to English
  TempLocale temp_english_locale("en_US.UTF-8");

  //In case the decimal digits need to be non-Dutch
  {
    const double pi = boost::math::constants::pi<double>();
    assert(boost::lexical_cast<std::string>(pi)[1] == '.' && "No Dutch please");
  }

  {
    const ModelFunctionParser p("x * x * sin(x) * rand(x)","x");
    p.Evaluate(0.0);
  }
  try
  {
    std::string zero{boost::lexical_cast<std::string>(0.0)};
    const ModelFunctionParser p(zero,"x");
    assert(std::abs(p.Evaluate(0.0) - 0.0) < 0.0001);
  }
  catch (std::runtime_error&)
  {
    assert(!"Should not get here, '0.0' is a correct function");
  }
}
#endif
