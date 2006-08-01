#
# Use this line to specify the C compiler.
#
#CC=cc
#
# Use this line to specify options for the C compiler.  You'll probably
# want to turn on optimizations. You may also have to use some of the 
# following flags:
#
#  -DCAPSBLAS         if BLAS routine names are capitalized.
#  -DCAPSLAPACK       if LAPACK routine names are capitalized.
#  -DNOUNDERBLAS      if BLAS routine names have no underscore.
#  -DNOUNDERLAPACK    if LAPACK routine names have no underscore.
#  -DBIT64            For I32LP64 systems.
#
CFLAGS=
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
# It's extremely likely that you'll have to change the LIBS= line for
# your particular system.  
#
LIBS=-L../lib -lsdp -llapack -lblas -lm
#
#  This builds the csdp program.  
#
csdp: csdp.o 
	$(CC) csdp.o $(LIBS) -o csdp
#
# To clean out the directory:
#
clean:
	rm -f *.o
	rm -f csdp


