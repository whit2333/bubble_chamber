#ifndef Bubble_GPSConfig_HH
#define Bubble_GPSConfig_HH 1

#include "BubbleChamberSim.h"
#include <map>
#include <array>
#include <vector>
#include <string>
#include <iostream>

struct GPSConfig {
  enum class EnergyType  { Mono, Lin, Exp, Pow, Gauss, Brem, Bbody, Cdg };
  enum class AngDistType { iso , cos, planar, beam1d, beam2d, focused, user };
  enum class PosDistType { Point, Plane, Beam, Surface, Volume };
  EnergyType   energy_dist = EnergyType ::Mono; 
  AngDistType  ang_dist = AngDistType::iso;
  PosDistType  pos_dist = PosDistType::Point;
   // Sets the angular distribution type () to either isotropic, cosine-law or user-defined.
  double energy = 8.0;
  std::vector<double> energy_params;
  std::vector<double> angle_params;
  std::vector<double> position_params;
};

struct Settings {
  enum class Mode { none, help, copy, list, run };
  enum class CopyMode { none, vis, v0, v1, v2 };
  enum class GenMode  { none, gps, gps1, gps2 };
  Mode         selected = Mode::none; 
  CopyMode     copy_sel = CopyMode::none;
  GenMode      gen_mode = GenMode::none;
  int          run_number        = 0;
  int          number_of_events  = -1;
  std::string  output_file_name  = "";
  std::string  output_tree_name  = "";
  std::string  theRest           = "";
  std::string  macroFile         = "";
  bool         run_manager_init  = false;
  bool         use_gui           = true;
  bool         use_vis           = true;
  bool         is_interactive    = true;
  bool         has_macro_file    = false;
  bool         help              = false;
  bool         is_command        = false;
  bool         is_copy_command   = false;
  bool         use_gps           = false;
  bool         vertex_set        = false;
  std::array<double,3> vertex    = {{0, 0, 0}};
  std::array<double,3> vertex_rms = {{0, 0, 0}};
  double       delta_E = 0.003;
  GPSConfig    gps_conf;


  std::map<CopyMode, std::vector<std::string>> copymode_file_names = {
    {CopyMode::vis, { BUBBLESIM_MACRO_DIR "/examples/vis.mac",
                      BUBBLESIM_MACRO_DIR "/examples/vis2.mac"} },
    {CopyMode::v1,  { BUBBLESIM_MACRO_DIR "/examples/vis.mac"} },
    {CopyMode::v2,  { BUBBLESIM_MACRO_DIR "/examples/vis2.mac"} }
  };
};

void PrintSettings(const Settings& X);

#endif
