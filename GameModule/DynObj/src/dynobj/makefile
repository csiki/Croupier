
include ../generic.mk

# Settings common to both Win32 & Linux
DEFS += -DDO_DEBUG -D_CFE_ -DDO_LIB_AUTHOR=\"ATS\" 
ifneq ($(PLATFORM),win32)
    CPPFLAGS += -fvisibility=hidden 
endif
INCZ += -I. -I../
INCZPCH:= $(INCZ)

# set default value for flags if not given on cmdline
ifndef flags
  flags := d    # debug
endif

# Modify above based on cmd line input flags
ifeq ($(findstring d,$(flags)),d)
  DEBFLAGS = -g -O0 
else
  DEBFLAGS = -Os 
endif
CPPFLAGS += $(DEBFLAGS)

# This is used in all targets to clean old objects
.PHONY: clean
clean: 
	-rm $(TARGET) $(OBJS)

# This will make clean builds by default
CLEAN= clean


# Using pdoh to preprocess headers/sources
ifeq ($(findstring rpdoh, $(MAKECMDGOALS)),rpdoh)
  HDRS= DynObj.h DoBaseI.h DynStr.h DoWindow.h DoDynShared.hpp DoBase.hpp \
        DynArr.hpp DynArrImpl.hpp DortInt.h NamedRef.h DoRunTime.h 
  SRCS= DynStr.cpp DynArrImpl.cpp NamedRef.cpp 

  .PHONY:rpdoh
  rpdoh: $(HDRS) $(SRCS)

  $(HDRS) $(SRCS):: 
	./pdoh $@ -o$@ -b 
endif
	

# DynStr library
ifeq ($(findstring ds, $(MAKECMDGOALS)),ds)
  # Target 
  GOAL= ds
  OUT= DynStr
  .PHONY: ds
  TARGET= $(OUT)$(SOSUFF)

  DEFS+= -DDO_USE_RUNTIME=1 -DDO_USE_DYNSHARED=0 -DDO_MODULE 

  # Include files 
  SUBDIR := .
  SRCS = DynStr.cpp DynObj.cpp vt_test.cpp 
  SDFLAGS = -fPIC -DDO_LIB_NAME=\"DynStr\"
  include subdir.mk
	
  # Include files 
  SUBDIR := ../utils
  SRCS = utf8String.cpp unicode.cpp
  include subdir.mk
	
  # DynStr
  $(TARGET): $(CLEAN) $(OBJS) makefile
	$(CPP) -shared -fPIC $(DEBFLAGS) -o $(TARGET) $(OBJS)

  include whole-or-file.mk
endif


# DynArr dynamic library
ifeq ($(findstring da, $(MAKECMDGOALS)),da)
  GOAL=da
  OUT= DynArr
  .PHONY: da 
  TARGET= $(OUT)$(SOSUFF)

  DEFS+= -DDO_USE_RUNTIME=1 -DDO_USE_DYNSHARED=0 -DDO_MODULE 

  # Include files 
  SUBDIR := .
  SRCS = DynArrImpl.cpp DynObj.cpp vt_test.cpp
  SDFLAGS = -DDO_LIB_NAME=\"DynArr\"
  include subdir.mk
	
  # DynArr
  $(TARGET): $(CLEAN) $(OBJS) makefile
	$(CPP) -shared $(DEBFLAGS) -o $(TARGET) $(OBJS) 

  include whole-or-file.mk
endif


# A light version of DynObj library (without DoRunTime)
ifeq ($(findstring doliblt, $(MAKECMDGOALS)),doliblt)
  GOAL= doliblt
  OUT= dolt
  .PHONY: doliblt
  TARGET= $(LIBPREF)$(OUT)$(LIBSUFF)
  
  # Include files 
  SUBDIR := .
  SRCS = DynObj.cpp vt_test.cpp DynObjLib.cpp 
  SDFLAGS = -DDO_MAIN -DDO_USE_RUNTIME=0 
  include subdir.mk
	
  SUBDIR := ../pi
  SRCS = DynLib.cpp Atomic.cpp
  include subdir.mk
  
  SUBDIR := ../utils
  SRCS = CharBuf.cpp 
  include subdir.mk
  
  # libdolt.a
  $(TARGET):  $(CLEAN) $(OBJS) makefile
	$(AR) rcs $(TARGET) $(OBJS) 

  include whole-or-file.mk
endif


# A fuller version of DynObj library (with DoRunTime)
ifeq ($(findstring dolibdort, $(MAKECMDGOALS)),dolibdort)
  # DoRunTime version of DynObj library
  GOAL=dolibdort
  OUT= dodort
  .PHONY: dolibdort 
  TARGET= $(LIBPREF)$(OUT)$(LIBSUFF)
  
  # Include files 
  SUBDIR := .
  SRCS = DynObj.cpp vt_test.cpp DynObjLib.cpp DoRunTime.cpp NamedRef.cpp
  SDFLAGS = -DDO_MAIN -DDO_USE_RUNTIME=1 -DDO_ENABLE_VTABLE_DISPATCH
  include subdir.mk
	
  SUBDIR := ../pi
  SRCS = DynLib.cpp Atomic.cpp Thread.cpp SpinLock.cpp RWLock.cpp CritSect.cpp 
  include subdir.mk
  
  SUBDIR := ../utils
  SRCS = CharBuf.cpp Notifiable.cpp 
  include subdir.mk
  
  # libdodort.a
  $(TARGET):  $(CLEAN) $(OBJS) makefile
	$(AR) rcs $(TARGET) $(OBJS) 

  include whole-or-file.mk
endif


# Test program for VTable diagnostics
ifeq ($(findstring bvtblt, $(MAKECMDGOALS)),bvtblt)
  # Target executable
  GOAL= bvtblt
  .PHONY: bvtblt 
  TARGET= vtblt$(EXESUFF)

  # Source directory
  SUBDIR := .
  SDFLAGS = -DVT_ALL_TESTS
  SRCS = vt_main.cpp vt_test.cpp 
  include subdir.mk
	
  # vtblt
  $(TARGET):  $(CLEAN) $(OBJS) makefile
	$(CPP) -o $(TARGET) $(DEBFLAGS) $(OBJS) $(LINKFLAGS) $(LIBDL)

  include whole-or-file.mk
endif


# This is build pdoh, not parsing!
ifeq ($(findstring bpdoh, $(MAKECMDGOALS)),bpdoh)
  # Target executable
  GOAL= bpdoh
  .PHONY: bpdoh
  TARGET= pdoh$(EXESUFF)

  DEFS += -DEA_DEBUG_HOOK=PDOH_BP_Dummy 
  
  # Include files 
  SUBDIR := tools
  SRCS = pdoh.cpp 
  SDFLAGS = 
  include subdir.mk

  # Include files 
  SUBDIR := ../utils
  SRCS = utf8String.cpp unicode.cpp utils.cpp CharBuf.cpp StrUtils.cpp FileSections.cpp 
  include subdir.mk
		
  # pdoh
  $(TARGET):   $(CLEAN) $(OBJS) makefile
	$(CPP) -o $(TARGET) $(DEBFLAGS)  $(OBJS)  

  include whole-or-file.mk
endif

# Test program for file sections
ifeq ($(findstring bfstest, $(MAKECMDGOALS)),bfstest)
  # Target executable
  GOAL= bfstest
  .PHONY: bfstest 
  TARGET= fstest$(EXESUFF)

  # Include files 
  SUBDIR := ../utils
  SDFLAGS = -DFILESECTIONS_TEST
  SRCS = FileSections.cpp utf8String.cpp unicode.cpp utils.cpp CharBuf.cpp StrUtils.cpp 
  include subdir.mk
		
  # fstest
  $(TARGET):  $(CLEAN) $(OBJS) makefile
	$(CPP) -o $(TARGET) $(DEBFLAGS)  $(OBJS) 

  include whole-or-file.mk
endif


default:
	echo 'No target given'

print-%: ;	@$(error $* is $($*) ($(value $*)) (from $(origin $*)))


