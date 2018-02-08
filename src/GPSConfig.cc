#include "GPSConfig.hh"
#include "BubbleChamberSim.h"

void PrintSettings(const Settings& X)
{
  std::cout << " vertex " << " (" << 
  X.vertex.at(0) << ", " << 
  X.vertex.at(1) << ", " << 
  X.vertex.at(2) << ")\n" ;

}

