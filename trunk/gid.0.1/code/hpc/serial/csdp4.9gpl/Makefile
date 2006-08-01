#
# This Makefile can be used to automatically build the entire package.  
#
# If you make changes in the "Makefile" under any subdirectory, you can
# rebuild the system with "make clean" followed by "make all".
#
#
# Generic. On most systems, this should handle everything.
#
all:
	cd lib; make libsdp.a
	cd solver; make csdp
	cd theta; make all
#
# Clean out all of the directories.
# 
clean:
	cd lib; make clean
	cd solver; make clean
	cd theta; make clean







