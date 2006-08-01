Thanks for showing interest in the graph invariant database project.

All codes are licensed under the GPL version 2.0, except for those in the /nongpl/ directory.  See gpl.txt for more info.


To get started run:
./makescript.sh

Invariant executables will be put in bin/invars/
Some sample graphs are located in data/


Overview:
/code/invars holds the code for graph invariants. Each graph invariant is named in UPPER_CASE_WITH_UNDERSCORES_FOR_SPACES. 
A graph invariant has the following behavior:
      It takes as input a list of graphs from STDIN. For each graph it outputs one or more numbers followed by a newline character to STDOUT.  Right now the format for a graph is:
NUMBER_OF_VERTEXES
EDGE
EDGE
....
-1

Example graph on 5 vertexes with 4 edges:
5
0 4
1 4
2 4
3 4
-1

Once the project gets more mature we will probably have a common API allowing many of the other popular formats like DIMACS,graph6,... 

/code/invars/         holds graph invariants

/code/analysis/       holds data mining tools. It's currently empty, but we wish to fill it with SQL,R, and C code for data mining.

/code/web/            holds files for maintaining web interfaces.  Also, we want to be able to compile a graph database into on-line book form.

/code/viz/            holds visualization code. This is where graph drawing code goes. The current plan is to add utilities that interface with ATT graphviz.

/code/hpc             holds high performance codes for larger graphs

/code/lib             holds support libraries

/data/                holds graphs and computed invariants

/nongpl/              holds software not covered under the GPL.  Contributers, make sure that you honor their licences, and don't copy any of this code to other directories.

Compile instructions:
Sorry for the lack of a proper Makefile. This will have to do for now
Option 1: Run the make script
./makescript.sh

Option 2: Compile files individually by hand

If you want to contribute to the project see contrib.txt.