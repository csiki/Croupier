
include ../../generic.mk

# Platform specific
ifeq ($(PLATFORM),win32)
else
  LIBDL+= -lpthread
  DEFS += -DPTHREADS
  CPPFLAGS += -fvisibility=hidden 
endif 


# Settings common to both Win32 & Linux
DEFS += -DDO_DEBUG -D_CFE_
INCZ += -I. -I$(PROJ)/home/MiniAppManager
INCZPCH:= $(INCZ)

# set default value for flags if not given on cmdline
ifndef flags
  flags := d
endif

# Modify above based on cmd line input flags
ifeq ($(findstring d,$(flags)),d)
  CPPFLAGS += -g -O0 
endif

MOD_FLAGS:= -fPIC -DDO_MODULE -DDO_ENABLE_VTABLE_DISPATCH=0 -DDO_USE_DYNSHARED=0
MAIN_FLAGS:= -DDO_MAIN -DDO_USE_RUNTIME=0
MAIN_RT_FLAGS:= -DDO_MAIN  -DDO_USE_RUNTIME=1

ifneq ($(filter $(MAKECMDGOALS), main1 main2 main3),)
  $(error Use bmain1, ... instead of main1, ...)
endif

# Generic clean target
clean: 
	-rm $(TARGET) $(OBJS)


ifeq ($(findstring pimpl1, $(MAKECMDGOALS)),pimpl1)
  # Target library
  OUT= pimpl1
  TARGET= $(OUT)$(SOSUFF)

  MOD_FLAGS += -DDO_USE_RUNTIME=0
  
  # Include files in this dir
  SUBDIR := .
  SRCS = PersonImpl1.cpp
  SDFLAGS = $(MOD_FLAGS)
  include ../subdir.mk
	
  # Include files in DynObj
  SUBDIR := ..
  SRCS = DynObj.cpp vt_test.cpp
  SDFLAGS = $(MOD_FLAGS)  -DDO_LIB_NAME=\"pimpl1\"
  include ../subdir.mk

  # pimpl1 library
  $(TARGET): clean $(OBJS) makefile
	$(CPP) -shared -fPIC -g -o $(TARGET) $(OBJS)

  pimpl1: $(TARGET)
  
endif


ifeq ($(findstring pimpl2, $(MAKECMDGOALS)),pimpl2)
  # Target library
  OUT= pimpl2
  TARGET= $(OUT)$(SOSUFF)

  MOD_FLAGS += -DDO_USE_RUNTIME=1 
  
  # Include files in this dir
  SUBDIR := .
  SRCS = PersonImpl2.cpp 
  SDFLAGS = $(MOD_FLAGS) 
  include ../subdir.mk
	
  # Include files in DynObj
  SUBDIR := ..
  SRCS = DynObj.cpp vt_test.cpp 
  SDFLAGS = $(MOD_FLAGS) -DDO_LIB_NAME=\"pimpl2\"
  include ../subdir.mk

  # pimpl2 dynamic library
  $(TARGET): clean $(OBJS) makefile
	$(CPP) -shared -fPIC -g -o $(TARGET) $(OBJS)

  pimpl2: $(TARGET)
  
endif


ifeq ($(findstring pimpl3, $(MAKECMDGOALS)),pimpl3)
  # Target library
  OUT= pimpl3
  TARGET= $(OUT)$(SOSUFF)

  MOD_FLAGS += -DDO_USE_RUNTIME=1 
  
  # Include files in this dir
  SUBDIR := .
  SRCS = PersonImpl3.cpp 
  SDFLAGS = $(MOD_FLAGS) 
  include ../subdir.mk
	
  # Include files in DynObj
  SUBDIR := ..
  SRCS = DynObj.cpp vt_test.cpp 
  SDFLAGS = $(MOD_FLAGS) -DDO_LIB_NAME=\"pimpl3\"
  include ../subdir.mk

  # pimpl3 dynamic library
  $(TARGET): clean $(OBJS) makefile
	$(CPP) -shared -fPIC -g -o $(TARGET) $(OBJS)

  pimpl3: $(TARGET)
  
endif


ifeq ($(findstring bmain1, $(MAKECMDGOALS)),bmain1)
  # Target executable
  OUT= main1
  TARGET= $(OUT)$(EXESUFF)

  # Include files this dir
  SUBDIR := .
  SRCS = main1.cpp 
  SDFLAGS = $(MAIN_FLAGS)
  include ../subdir.mk
	
  # main1 exe
  $(TARGET): clean $(OBJS) makefile
	$(CPP) -g -rdynamic -o $(TARGET) $(OBJS) -L../ -ldolt $(LIBDL)

  bmain1: $(TARGET)
  
endif


ifeq ($(findstring bmain2, $(MAKECMDGOALS)),bmain2)
  # Target executable
  OUT= main2
  TARGET= $(OUT)$(EXESUFF)

  # Include files this dir
  SUBDIR := .
  SRCS = main2.cpp 
  SDFLAGS = $(MAIN_RT_FLAGS)
  include ../subdir.mk
		
  # main2 exe
  $(TARGET): clean $(OBJS) makefile
	$(CPP) -g -rdynamic -o $(TARGET) $(OBJS) -L../ -ldodort $(LIBDL)

  bmain2: $(TARGET)
  
endif


ifeq ($(findstring bmain3, $(MAKECMDGOALS)),bmain3)
  # Target executable
  OUT= main3
  TARGET= $(OUT)$(EXESUFF)

  # Include files this dir
  SUBDIR := .
  SRCS = main3.cpp 
  SDFLAGS = $(MAIN_RT_FLAGS)
  include ../subdir.mk
	
  # main3 exe
  $(TARGET): clean $(OBJS) makefile
	$(CPP) -g -rdynamic -o $(TARGET) $(OBJS) -L../ -ldodort $(LIBDL)

  bmain3: $(TARGET)
  
endif


default:
	echo 'No target given'

print-%: ;	@$(error $* is $($*) ($(value $*)) (from $(origin $*)))


