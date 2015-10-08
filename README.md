EBL_sim
=======

A Geant4 simulation for electron beam lithography.

Build and Install
-----------------

Standard cmake build assuming you have geant4 installed.

    mkdir ebl_build
    cd ebl_build
    cmake ../EBL_sim/. -DCMAKE_INSTALL_PREFIX=../ebl_run
    make install

Run the simulation

    cd ../ebl_run
    ./bin/ebl1  

Running options
---------------

The default  will run 1000 events  and save histograms to run file "0".
The run number can be specified with the "--run" flag

    ./bin/ebl1  --run=100

This creates the file EBL_sim_output_100.root.




