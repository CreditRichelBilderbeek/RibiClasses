#include "beast.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wextra"
#include "RInside.h"
#pragma GCC diagnostic pop

#include "fileio.h"
#include "ribi_rinside.h"

const std::string ribi::Beast::sm_beast_path{"../../../../Programs/BEAST/bin"};

ribi::Beast::Beast()
{
  #ifndef NDEBUG
  Test();
  #endif
  const std::string executable{sm_beast_path + "/beast"};
  if (!ribi::fileio::FileIo().IsRegularFile(executable))
  {
    std::stringstream s;
    s
      << "Error: BEAST executable '" << executable << "' not found\n"
      << "Please change the path\n"
    ;
    throw std::logic_error(s.str().c_str());
  }
}

void ribi::Beast::AnalyzeBirthDeath(
  const std::string& log_file,
  const std::string png_filename
) const
{
  AnalyzeBirthDeathRinside(log_file,png_filename);
}

void ribi::Beast::AnalyzeBirthDeathRinside(
  const std::string& log_file,
  const std::string png_filename
) const
{
  auto& r = ribi::Rinside().Get();
  ribi::fileio::FileIo f;
  assert(f.IsRegularFile(log_file));

  r.parseEval("library(ape)");
  r.parseEval("library(geiger)");
  r.parseEval("library(phangorn)");
  r["log_file"] = log_file;
  r["png_filename"] = png_filename;
  r.parseEval("data_raw <- read.table(log_file,header = TRUE)");
  r.parseEval("data <- tail(data_raw,n = nrow(data_raw) * 0.9)");
  r.parseEval("png(filename=png_filename)");
  r.parseEval("hist(data$BirthDeath)");
  r.parseEval("dev.off()");

  if (!f.IsRegularFile(png_filename))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create PNG file "
      << "with filename '" << png_filename << "'. "
      << "Perhaps not all packages (ape,geiger,phangorn) needed are installed? "
    ;
    throw std::runtime_error(s.str().c_str());
  }

}


void ribi::Beast::AnalyzeBirthDeathRscript(
  const std::string& log_file,
  const std::string png_filename
) const
{
  using ribi::fileio::FileIo;
  assert(FileIo().IsRegularFile(log_file));

  //TODO: Test if the user has all required packages

  const std::string temp_r_filename{
    FileIo().GetTempFileName(".R")
  };

  //Create the R script
  {
    std::ofstream f(temp_r_filename.c_str());
    f
      << "library(ape)" << '\n'
      << "library(geiger)" << '\n'
      << "library(phangorn)" << '\n'
      << "data_raw <- read.table(\"" << log_file << "\",header = TRUE)" << '\n'
      << "data <- tail(data_raw,n = nrow(data_raw) * 0.9)" << '\n'
      << "png(filename=\"" << png_filename << "\")" << '\n'
      << "hist(data$BirthDeath)" << '\n'
      << "dev.off()" << '\n'
    ;
  }

  if (!FileIo().IsRegularFile(temp_r_filename))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create temporary R script file "
      << "with filename '" << temp_r_filename << "'";
    throw std::runtime_error(s.str().c_str());
  }

  //Execute the R script
  {
    std::stringstream cmd;
    cmd << "Rscript " << temp_r_filename;
    const int error{std::system(cmd.str().c_str())};
    if (error)
    {
      std::clog << __FILE__ << ": error " << error << '\n';
    }
  }

  if (!FileIo().IsRegularFile(png_filename))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create SVG file "
      << "with filename '" << png_filename << "'. "
      << "Perhaps not all packages (ape) needed are installed? "
      << "You can try to run the temporary R script file '"
      << temp_r_filename
      << "' yourself to see which error it gives"
    ;
    throw std::runtime_error(s.str().c_str());
  }
  //After detection. so user can check script
  FileIo().DeleteFile(temp_r_filename);
}

void ribi::Beast::AnalyzeCoalescent(
  const std::string& log_file,
  const std::string png_filename_coalescent_constant,
  const std::string png_filename_popsize
) const
{
  AnalyzeCoalescentRinside(
    log_file,
    png_filename_coalescent_constant,
    png_filename_popsize
  );
}

void ribi::Beast::AnalyzeCoalescentRinside(
  const std::string& log_file,
  const std::string png_filename_coalescent_constant,
  const std::string png_filename_popsize
) const
{
  ribi::fileio::FileIo f;
  auto& r = ribi::Rinside().Get();

  r.parseEval("library(ape)");
  r.parseEval("library(geiger)");
  r.parseEval("library(phangorn)");
  r["log_file"] = log_file;
  r["png_filename_popsize"] = png_filename_popsize;
  r["png_filename_coalescent_constant"] = png_filename_coalescent_constant;

  r.parseEval("data_raw <- read.table(log_file,header = TRUE)");
  r.parseEval("data <- tail(data_raw,n = nrow(data_raw) * 0.9)");
  r.parseEval("png(filename=png_filename_popsize)");
  r.parseEval("hist(data$popSize)");
  r.parseEval("dev.off()");
  r.parseEval("png(filename=png_filename_coalescent_constant)");
  r.parseEval("hist(data$CoalescentConstant)");
  r.parseEval("dev.off()");


  if (!f.IsRegularFile(png_filename_coalescent_constant))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create file "
      << "with filename '" << png_filename_coalescent_constant << "'. "
    ;
    throw std::runtime_error(s.str().c_str());
  }
  if (!f.IsRegularFile(png_filename_popsize))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create file "
      << "with filename '" << png_filename_popsize << "'. "
    ;
    throw std::runtime_error(s.str().c_str());
  }


}

void ribi::Beast::AnalyzeCoalescentRscript(
  const std::string& log_file,
  const std::string png_filename_coalescent_constant,
  const std::string png_filename_popsize
) const
{
  using ribi::fileio::FileIo;

  //TODO: Test if the user has all required packages

  const std::string temp_r_filename{
    FileIo().GetTempFileName(".R")
  };

  //Create the R script
  {
    std::ofstream f(temp_r_filename.c_str());
    f
      << "library(ape)" << '\n'
      << "library(geiger)" << '\n'
      << "library(phangorn)" << '\n'
      << "data_raw <- read.table(\"" << log_file << "\",header = TRUE)" << '\n'
      << "data <- tail(data_raw,n = nrow(data_raw) * 0.9)" << '\n'
      << "png(filename=\"" << png_filename_popsize << "\")" << '\n'
      << "hist(data$popSize)" << '\n'
      << "dev.off()" << '\n'
      << "png(filename=\"" << png_filename_coalescent_constant << "\")" << '\n'
      << "hist(data$CoalescentConstant)" << '\n'
      << "dev.off()" << '\n'
    ;
  }

  if (!FileIo().IsRegularFile(temp_r_filename))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create temporary R script file "
      << "with filename '" << temp_r_filename << "'";
    throw std::runtime_error(s.str().c_str());
  }

  //Execute the R script
  {
    std::stringstream cmd;
    cmd << "Rscript " << temp_r_filename;
    const int error{std::system(cmd.str().c_str())};
    if (error)
    {
      std::clog << __FILE__ << ": error " << error << '\n';
    }
  }

  if (!FileIo().IsRegularFile(png_filename_coalescent_constant))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create SVG file "
      << "with filename '" << png_filename_coalescent_constant << "'. "
      << "You can try to run the temporary R script file '"
      << temp_r_filename
      << "' yourself to see which error it gives"
    ;
    throw std::runtime_error(s.str().c_str());
  }
  if (!FileIo().IsRegularFile(png_filename_popsize))
  {
    std::stringstream s;
    s << __FILE__ << "(" << __LINE__ << "): "
      << "Could not create SVG file "
      << "with filename '" << png_filename_popsize << "'. "
      << "You can try to run the temporary R script file '"
      << temp_r_filename
      << "' yourself to see which error it gives"
    ;
    throw std::runtime_error(s.str().c_str());
  }
  //After detection. so user can check script
  FileIo().DeleteFile(temp_r_filename);
}

std::string ribi::Beast::GetVersion() const noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::Beast::GetVersionHistory() const noexcept
{
  return {
    "2015-06-13: Version 1.0: started versioning",
    "2015-10-05: Version 1.1: BeastParameterFile renames duplicate sequence descriptions, fixed bug"
    "2015-10-06: Version 1.2: BeastParameterFile can detect two version of parameter files"
  };
}

void ribi::Beast::Run(
  const std::string& xml_input_filename,
  const std::string& log_output_filename,
  const std::string& trees_output_filename
) const noexcept
{
  const ribi::fileio::FileIo fileio;
  const std::string executable{sm_beast_path + "/beast"};
  assert(ribi::fileio::FileIo().IsRegularFile(executable)
    && "Checked in constructor");

  const std::string cmd{executable + " -overwrite " + xml_input_filename};
  const int error{std::system(cmd.c_str())};
  if (error)
  {
    std::clog << __FILE__ << ": error " << error << '\n';
  }

  assert(fileio.IsRegularFile(xml_input_filename));


  if (fileio.IsRegularFile("alignment.log"))
  {
    fileio.CopyFile("alignment.log",log_output_filename);
    fileio.DeleteFile("alignment.log");
  }
  if (fileio.IsRegularFile("alignment.trees"))
  {
    fileio.CopyFile("alignment.trees",trees_output_filename);
    fileio.DeleteFile("alignment.trees");
  }
}
