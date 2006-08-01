#
# Use this line to specify the C compiler.
#
#CC=
#
# Use this line to specify options for the C compiler.  
#
#  -DCAPSBLAS         if BLAS routine names are capitalized.
#  -DCAPSLAPACK       if LAPACK routine names are capitalized.
#  -DNOUNDERBLAS      if BLAS routine names have no underscore.
#  -DNOUNDERLAPACK    if LAPACK routine names have no underscore.
#  -DBIT64            For I32LP64 systems.
#
#CFLAGS=
#
# Use this line to specify where the SDP and linear algebra libraries are
# to be found. 
#
# -L../lib            look in the ../lib directory
# -lsdp               get libsdp.a
# -llapack            get liblapack.a
# -lblas              get libblas.a
# -lm                 Get C math library.
#
LIBS=-L../lib -lsdp -llapack -lblas -lm
#
# Target all builds everything.
#
all: theta complement complement rand_graph graphtoprob
#
# This builds the theta number code. 
#
theta: theta.o 
	$(CC) $(CFLAGS) theta.o $(LIBS) -o theta
#
# Complement computes the complement of a graph.
#
complement: complement.o 
	$(CC) $(CFLAGS) complement.o $(LIBS) -o complement
#
# rand_graph generates a random graph.  
#
rand_graph: rand_graph.o
	$(CC) $(CFLAGS) rand_graph.o $(LIBS) -o rand_graph
#
# graphtoprob converts a file in the graph format to an SDP problem in our
# SDP format.
#
graphtoprob: graphtoprob.o 
	$(CC) $(CFLAGS) graphtoprob.o $(LIBS) -o graphtoprob
#
# To clean up the directory.
#
clean:
	rm -f *.o
	rm -f theta
	rm -f complement
	rm -f rand_graph
	rm -f graphtoprob


