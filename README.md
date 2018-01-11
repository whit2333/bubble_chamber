Bubble Sim
==========

A Geant4 simulation for bubble chamber experiment.

Build and Install
-----------------

Standard cmake build assuming you have geant4 installed.

```
git clone ###remote###
mkdir -p bubble_chamber/build
cd    bubble_chamber/build
cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME
make install
```

Run the simulation

```
bubble_chamber -h
```

Running options
---------------

The default  will run 1000 events  and save histograms to run file "0".
The run number can be specified with the "--run" flag

```
bubble_chamber  --run=100
````

This creates the file EBL_sim_output_100.root.




