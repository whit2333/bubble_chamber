#include "GPSConfig.hh"
#include "BubbleChamberSim.h"

void PrintSettings(const Settings& X)
{

  std::cout << "Run Number : " << X.run_number << "\n"
            << "  N events : " << X.number_of_events << "\n"
            << "    E beam : " << X.gps_conf.energy << "\n"
            << "   delta_E : " << X.delta_E << "\n"
            << "    vertex : (" << X.vertex.at(0) << ", " << X.vertex.at(1) << ", " << X.vertex.at(2) << ") [mm]\n"
            << "  vert rms : (" << X.vertex_rms.at(0) << ", " << X.vertex_rms.at(1) << ", " << X.vertex_rms.at(2) << ") [mm]\n";

}

