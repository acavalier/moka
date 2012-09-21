/*
 * Homology : Homology computation of a 3D objects
 * Copyright (C) 2012, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of Moka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "g-map-vertex.hh"
#include "extraction-images.hh"
#include "compute-homology.hh"
#include "chrono.hh"

using namespace std;
using namespace GMap3d;

int main(int argc, char** argv)
{
  if ( argc==1 || !strcmp(argv[1],"-?") || !strcmp(argv[1],"-h") )
  {
    cout<<"Usage1 : a.out mokafile (.moka). Pour calculer l'homology de"<<endl
        <<"  la G-carte."<<endl;
    exit(EXIT_FAILURE);
  }

  // First we load the map.
  CGMapVertex g;
  if ( !g.load(argv[1]) )
  {
    cout<<"Problem during loading of "<<argv[1]<<endl;
    return EXIT_FAILURE;
  }

  g.randomizeDarts();
  
  int nbdarts, nbvertices, nbedges, nbfaces, nbvolumes, nbcc;
  g.getGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                             &nbfaces,&nbvolumes,&nbcc,
                             NULL,NULL,NULL,NULL);
  std::cout<<"Map before simplification: darts="<<nbdarts
           <<", vertices="<<nbvertices
           <<", edges="<<nbedges
           <<", faces="<<nbfaces
           <<", volumes="<<nbvolumes
           <<", cc="<<nbcc<<std::endl;

  // g.save("extract-map.moka");

  CChrono c;
  c.start();
  // Here simplify the map
  g.simplify3DObject();
  c.stop();
  c.display("Simplification time");

  // g.save("simplify-map.moka");

  g.getGlobalCharacteristics(&nbdarts,&nbvertices,&nbedges,
                             &nbfaces,&nbvolumes,&nbcc,
                             NULL,NULL,NULL,NULL);
  std::cout<<"Map after simplification: darts="<<nbdarts
           <<", vertices="<<nbvertices
           <<", edges="<<nbedges
           <<", faces="<<nbfaces
           <<", volumes="<<nbvolumes
           <<", cc="<<nbcc<<std::endl;

  // Now compute the homology on the simplified representation.
  CChrono c2;
  c.start();
  c2.start();
  
  CHomology h(&g);
  h.computeVolumicHomology();

  c.stop();
  c2.stop();
  c2.display("Homology computation time");

  c.display("TOTAL:");  
  
  std::cout<<"Memory used: "<<h.size()<<" bytes."<<endl;

  // Display Betti numbers
  std::cout<<"Betti numbers Free (0,1,2,3): "<<h.getH0FreeGenerators()
           <<", "<<h.getH1FreeGenerators()<<", "<<h.getH2FreeGenerators()
           <<", "<<h.getH3FreeGenerators()<<endl;
  std::cout<<"Betti numbers Torsion (1,2): "<<h.getH1TorsionGenerators()
           <<", "<<h.getH2TorsionGenerators()<<endl;
  
  exit(EXIT_SUCCESS);
}
