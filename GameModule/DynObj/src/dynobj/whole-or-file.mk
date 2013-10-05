
# Builds either:
# - the whole target (default) 
# - One (or several) specified objects
#
# GOAL    - the goal given on command line (usually a .PHONY goal)
# TARGET  - output file with all qualifiers qualifiers (libfoo.a)
# OBJS    - list of all object files

ifeq ($(findstring $(BINDIR),$(MAKECMDGOALS)),)
    # Default, full target (No object files specified)
    $(GOAL): $(TARGET)            # This target is used when compiling the whole target
else
    # Single/multiple file (object files specified)
    OBJS:= $(filter $(OBJS), $(MAKECMDGOALS))
    $(GOAL): clean $(OBJS)        # This target is used when compiling a single file 
endif
