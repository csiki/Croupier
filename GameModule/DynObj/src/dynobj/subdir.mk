#expects: 
# SRCS - list of source files
# SUBDIR - Path to directory with source files
# BINDIR - Path to output directory
# CPPBUILD - Build command for C++ files
# SDFLAGS - SubDir flags for compiling

# Flatten path in output bin dir
SDFN := $(subst /,_,$(SUBDIR))
SDFN := $(subst .,-,$(SDFN))
LOBJS = $(addprefix $(BINDIR)/$(SDFN)_, $(SRCS:.cpp=.o))
OBJS += $(LOBJS)

-include $(addsuffix .d,$(LOBJS))
$(BINDIR)/$(SDFN)_%.o: tspec:=$(SDFLAGS) 
$(BINDIR)/$(SDFN)_%.o: $(SUBDIR)/%.cpp 
	$(CPP) $(CPPFLAGS) $(tspec) $(DEFS) $(INCZPCH) $(wxCPPFLAGS) -MT$@ -MF$@.d -MD -c $< -o $@
	echo -
#$(BINDIR)/$(SDFN)_%.o.d:    

$(warning $(BINDIR)/$(SDFN)_  $(SUBDIR)/)
# Reset local compile flags after using it
SDFLAGS = 

