cmput411-assignment2
====================


This program reads in a skeleton animation file of the .bvh formate, specified as a single argument in command line.
This animation may be played back, with variable playback speed. See the pdf description of the assignment for full features.

Development was done in eclipse, hence this folder actually can be imported into eclipse as a project rather simply, and then built and run there. (Do not forget to specify which model to load in the command line arguments.)

Alternatively, the code can be compiled and then run with the following commands (THIS DOES NOT WORK YET!):
```
make
./motionviewer <path_to_bvh_file>
```

###### Assumptions about the project
1. All the bvh files we load either have "CHANNELS 6 Xposition Yposition Zposition Zrotation Yrotation Xrotation" or "CHANNELS 3 Zrotation Yrotation Xrotation"
2. Any root is not a leaf (this is valid assumption as that would not make sense)
3. I do NOT assume that there's only one root in the file, however I never tested whether my parser works with multiple roots or not. In case there are multiple roots, I assume that their animation descriptions are interleaved, as in the first line contains the information about the first frame for all the trees, then the second line describes the second frame for all trees, etc.
4. For calculating the initial viewing position I make the assumption that only ROOT's have have translation transformations

--------------------
Developed by David Szepesvari. Completed October 21, 2012.