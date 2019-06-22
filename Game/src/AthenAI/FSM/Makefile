SOURCES := $(wildcard *.cpp)				#All the source files in our project (located in src folder)
FIRSTOBJECTS := $(subst  .cpp,.o,$(SOURCES))	#All the object files in our project. We pick all the cpp files and change it's extention with an o in order to obtain, for example, src/Game.cpp -> src/Game.o
OBJECTS:= $(patsubst %,../../../obj/%,$(FIRSTOBJECTS))	#This was a quick soultion as the last function picked up the src/ part of the path. We change all src to obj (where our binaries are), src/Game.o -> obj/Game.o  	
OBJECTDIRECTORY:=../../../obj/
#IRRLICHT INCLUDE PATHS AND LIBRARY PATHS
#--------------------------------------------------------------------------------------------------------------------
#Where our Irrlicht .h files are located
IRRLICHTINCTDIR=include/irrlicht
#GLM directory
GLMDIR=include/GLM
#FMOD directory
FMODDINCDIR=include/fmod
#Where our Irrlicht libraries are located
IRRLICHTLIBDIR=lib/irrlicht/Linux
#Same with FMOD
FMODLIBDIR=lib/fmod
#Imguiforirrlicht
IRRIMGUIINCDIR=include/IRRIMGUI
#The instruction needed to include this path in compilation (Will extent with code growing)
IRRINCFLAG= -I../../../$(IRRLICHTINCTDIR)
#Same with FMOD Studio
FMODINCFLAG= -I../../../$(FMODDINCDIR)
#Instruction needed to include IRRIMGUI
IRRIMGUIFLAG= -I../../../$(IRRIMGUIINCDIR)
#Instruction needed to include GLM
GLMINCFLAG= -I../../../$(GLMDIR)

SHAREDLIBS= -Wl,-rpath=lib/fmod
#--------------------------------------------------------------------------------------------------------------------

#Color for the compliation message -> Blue with emphasis
COM_COLOR = \033[1;34m	
#Color for the success message -> Green with emphasis				   
SUCCESS_COLOR = \033[1;32m
#Color for the error message -> Red with emphasis			   
ERROR_COLOR = \033[1;31m
#Color for the warning message -> Yelow with emphasis			  
WARNING_COLOR = \033[1;33m
#Color for the files that are taking part in de proccess -> Dark blue			
OBJ_COLOR = \033[0;36m
#No special color				  
NO_COLOR =  \033[m					  

#Generic compilation message string that won't change for now
COM_STRING = "COMPILING..."
#Generic success message string that won't change for now			   
COM_END = "SUCCESS!"
#Generic error message string that won't change for now				   
ERROR_STRING = "ERROR"

#TEST LINKING
#Checks if the linking proccess is successfull or not - It gives a success message if it went OK, a error message if it didn't and a warninig if we have to be concerned about something

define test_compilation
		RESULT=$$?; \
        if [ $$RESULT -ne 0 ]; then \
            printf "%-60b%b" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n"; \
        elif [ -s $@.log ]; then \
            printf "%-60b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $@" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n"; \
        else  \
            printf "$(SUCCESS_COLOR)$(COM_END)$(NO_COLOR)\n"; \
        fi; \

		exit $$RESULT

endef

INCLUDEFOLDERS := -I ../../. -I ../../Managers -I ../../Components -I ../../FMODEngine -I ../../GraphicEngine -I ../../PICASSO -I ../../StatesMachine -I ../../States -I ../Fuzzy -I ../DecTree -I ../AthenAI/FSM -I ../SteeringBehaviors -I ../Sensors

all:$(OBJECTS)

$(OBJECTDIRECTORY)%.o:%.cpp
	
	@printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n " #We print a message to tell the procces that we are making. In this case, Linking
	@g++ -g -Wall -O3 -std=c++14 -o $@ -c $^ $(IRRINCFLAG) $(IRRIMGUIFLAG) $(GLMINCFLAG) $(FMODINCFLAG) $(INTERNAL_INCLUDES) -I../ -I. $(INCLUDEFOLDERS)
	@$(call test_compilation)

info:
	$(info $(FIRSTOBJECTS))
	$(info $(OBJECTS))