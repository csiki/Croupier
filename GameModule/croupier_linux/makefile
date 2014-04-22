CXX = g++
LD = g++

CXXFLAGS = -Wall -W -fexceptions -std=c++11
LDFLAGS = -ldl -rdynamic -pthread

CXXFLAGS_DEBUG = $(CXXFLAGS) -g
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
OUT_DEBUG = bin/croupier_debug

CXXFLAGS_RELEASE = $(CXXFLAGS) -O3
LDFLAGS_RELEASE = -s $(LDFLAGS)
OBJDIR_RELEASE = obj/Release
OUT_RELEASE = bin/croupier_release

SOURCES = \
	./AliveBotLoader.cpp \
	./BotCommunicationTranslator.cpp \
	./BotCommunicator.cpp \
	./Bot.cpp \
	./BotData.cpp \
	./BotDataXMLHandler.cpp \
	./BotHandler.cpp \
	./BotInfo.cpp \
	./BotKnowledgeHandler.cpp \
	./BotManager.cpp \
	./BroadcastMember.cpp \
	./BroadcastStation.cpp \
	./Card.cpp \
	./CBotLoader.cpp \
	./CommonLispBotLoader.cpp \
	./ConcreteBot.cpp \
	./CppBotLoader.cpp \
	./Croupier.cpp \
	./CSharpBotLoader.cpp \
	./Deck.cpp \
	./Entity.cpp \
	./Event.cpp \
	./GameData.cpp \
	./GameDataXMLHandler.cpp \
	./GameModule.cpp \
	./GameOwner.cpp \
	./HandEvaluator.cpp \
	./Hostess.cpp \
	./JavaBotLoader.cpp \
	./KnowledgeTable.cpp \
	./KnowledgeTableXMLHandler.cpp \
	./Log.cpp \
	./Logger.cpp \
	./LogXMLHandler.cpp \
	./LuaBotLoader.cpp \
	./PascalBotLoader.cpp \
	./PerlBotLoader.cpp \
	./PhpBotLoader.cpp \
	./Process.cpp \
	./PrologBotLoader.cpp \
	./pugixml.cpp \
	./PythonBotLoader.cpp \
	./Results.cpp \
	./ResultsXMLHandler.cpp \
	./RubyBotLoader.cpp \
	./Rulz.cpp \
	./RulzXMLHandler.cpp \
	./SchemeBotLoader.cpp \
	./stdafx.cpp \
	./Table.cpp \
	./TimerBotProxy.cpp \
	./XYTranslator.cpp

OBJ_DEBUG = $(patsubst %.cpp, $(OBJDIR_DEBUG)/%.o, $(SOURCES))
OBJ_RELEASE = $(patsubst %.cpp, $(OBJDIR_RELEASE)/%.o, $(SOURCES))

DEPS_DEBUG = $(OBJ_DEBUG:.o=.o.d)
DEPS_RELEASE = $(OBJ_RELEASE:.o=.o.d)

CREATE_DIRECTORY = @mkdir -p $(@D)

all: debug release

debug: $(OUT_DEBUG)

release: $(OUT_RELEASE)

clean: clean_debug clean_release

clean_debug:
	rm -f $(OBJ_DEBUG)
	rm -f $(DEPS_DEBUG)
	rm -f $(OUT_DEBUG)
	
clean_release:
	rm -f $(OBJ_RELEASE)
	rm -f $(DEPS_RELEASE)
	rm -f $(OUT_RELEASE)

-include $(DEPS_DEBUG)
-include $(DEPS_RELEASE)

$(OUT_DEBUG): $(OBJ_DEBUG)
	mkdir -p bin
	$(LD) -o $(OUT_DEBUG) $(OBJ_DEBUG) $(LDFLAGS_DEBUG)

$(OUT_RELEASE): $(OBJ_RELEASE)
	mkdir -p bin
	$(LD) -o $(OUT_RELEASE) $(OBJ_RELEASE) $(LDFLAGS_RELEASE)

$(OBJDIR_DEBUG)/%.o: %.cpp
	$(CREATE_DIRECTORY)
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@ -MMD -MF $@.d

$(OBJDIR_RELEASE)/%.o: %.cpp
	$(CREATE_DIRECTORY)
	$(CXX) $(CXXFLAGS_RELEASE) -c $< -o $@ -MMD -MF $@.d
