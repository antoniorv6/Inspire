SOURCES := $(wildcard *.cpp)				#All the source files in our project (located in src folder)
FIRSTOBJECTS := $(subst  .cpp,.o,$(SOURCES))	#All the object files in our project. We pick all the cpp files and change it's extention with an o in order to obtain, for example, ../Game.cpp -> ../Game.o
OBJECTS:= $(patsubst %,../../obj/%,$(FIRSTOBJECTS))	#This was a quick soultion as the last function picked up the ../ part of the path. We change all src to obj (where our binaries are), ../Game.o -> obj/Game.o  	
OBJECTDIRECTORY:=../../obj/

OBJECTSNORMALCOMP:= ../../obj/OrionHTTPRequester.o ../../obj/OrionClientPredictor.o
#IRRLICHT INCLUDE PATHS AND LIBRARY PATHS
#--------------------------------------------------------------------------------------------------------------------
#Where our Irrlicht .h files are located
IRRLICHTINCTDIR=include/irrlicht
#GLM directory
GLMDIR=include/GLM
#RapidXML directory
RAPIDXMLINCDIR=include/RapidXML
#FMOD directory
FMODDINCDIR=include/fmod
#Where our Irrlicht libraries are located
IRRLICHTLIBDIR=lib/irrlicht/Linux
#Same with FMOD
FMODLIBDIR=lib/fmod
#Imguiforirrlicht
IRRIMGUIINCDIR=include/IRRIMGUI
#Raknet 
RAKNETINCDIR = /include/raknet
RAKNETLIBDIR = lib/raknet
#The instruction needed to include this path in compilation (Will extent with code growing)
IRRINCFLAG= -I../../$(IRRLICHTINCTDIR)
#Same with FMOD Studio
FMODINCFLAG= -I../../$(FMODDINCDIR)
#Instruction needed to include GLM
GLMINCFLAG= -I../../$(GLMDIR)
#Include RapidXML
RAPIDXMLFLAG = -I../../$(RAPIDXMLINCDIR)
#Irrimguinclude
IRRIMGUIFLAG = -I../../$(IRRIMGUIINCDIR)
#Raknet
RAKNETINCFLAG = -I../../$(RAKNETINCDIR)

RAPIDJSONINCDIR=include/rapidjson
RAPIDJSONINCFLAG=-I../../$(RAPIDJSONINCDIR)

GLFWINCDIR= include/GLFW
GLEWINCDIR= include/GLEW

GLEWLIBDIR = lib/GLEW
GLFWLIBDIR = lib/GLFW

GLFWINCFLAG = -I../../$(GLFWINCDIR)
GLEWINCFLAG = -I../../$(GLEWINCDIR)



SHAREDLIBS= -Wl,-rpath=lib/fmod -Wl,-rpath=lib/irrimgui -Wl,-rpath=lib/raknet -Wl,-rpath=lib/GLFW -Wl,-rpath=lib/assimp

COMMANDBASH="export LD_LIBRARY_PATH=lib/fmod/:lib/raknet:lib/irrimgui && ./$(GAMENAME)"
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

RELEASEFOLDER = ../../server/
DEBUGFOLDER   = ../../debugServer/
GAMENAME = InspireServer

INCLUDEFOLDERS := -I ../Managers -I ../Components -I ../FMODEngine -I ../GraphicEngine -I ../PICASSO -I ../StatesMachine -I ../States -I ../ -I ../AthenAI/Fuzzy -I ../AthenAI/DecTree -I ../AthenAI/FSM -I ../AthenAI/SteeringBehaviors -I ../AthenAI/Sensors

INTERNAL_INCLUDES= -I.

LIBS= -lIrrlicht -lGLU -lGL -lX11 -lXxf86vm -lfmod -lfmodstudio -lRakNetDLL -lGLEW -lglfw3 -lXrandr -pthread -lXi -ldl -lXinerama -lXcursor -lassimp

all:make_folders $(RELEASEFOLDER)$(GAMENAME)

http:$(OBJECTSNORMALCOMP)

debug:$(DEBUGFOLDER)$(GAMENAME)

$(RELEASEFOLDER)$(GAMENAME):$(OBJECTS)
													 				         #Target -> ejecutable : Sources -> All the files that are in the obj folder
	@printf "%b" "$(COM_COLOR)$(LINK_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n " #We print a message to tell the procces that we are making. In this case, Linking
	@$(CXX) -Wall -std=c++14 -o $@ ../../obj/*.o -L../../lib/irrimgui -L../../lib/assimp -L../../lib/GLFW -L../../lib/GLEWmake -lIrrIMGUI -L../../$(IRRLICHTLIBDIR) -L../../lib/IRRIMGUI -L../../$(FMODLIBDIR) -L../../$(RAKNETLIBDIR) $(SHAREDLIBS) $(LIBS)
	@$(call link_compilation_test)										     #We call our checking function to trigger the message that is needed in this
	
	@printf "%b" "$(WARNING_COLOR)"Creating_Bash_Execuion_File"$(NO_COLOR)\n"
	@touch $(RELEASEFOLDER)execute.sh
	@echo $(COMMANDBASH) > $(RELEASEFOLDER)execute.sh
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n"
	@printf "%b" "$(WARNING_COLOR)"Copying_Assets_folder"$(NO_COLOR)\n"
	@cp -R ../../assets $(RELEASEFOLDER)
	@cp -R ../../lib	  $(RELEASEFOLDER)
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n" 
	@printf "%b" "$(SUCCESS_COLOR)"Compilation_proccess_successfull"$(NO_COLOR)\n"

$(DEBUGFOLDER)$(GAMENAME):$(OBJECTS)
													 				         #Target -> ejecutable : Sources -> All the files that are in the obj folder
	@printf "%b" "$(COM_COLOR)$(LINK_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n " #We print a message to tell the procces that we are making. In this case, Linking
	@$(CXX) -g -Wall -std=c++14 -o $@ ../../obj/*.o -L../../lib/irrimgui -lIrrIMGUI -L../../$(IRRLICHTLIBDIR) -L../../lib/IRRIMGUI -L../../$(FMODLIBDIR) -L../../$(RAKNETLIBDIR) $(SHAREDLIBS) $(LIBS)
	@$(call link_compilation_test)										     #We call our checking function to trigger the message that is needed in this
	
	@printf "%b" "$(WARNING_COLOR)"Creating_Bash_Execuion_File"$(NO_COLOR)\n"
	@touch $(DEBUGFOLDER)execute.sh
	@echo $(COMMANDBASH) > $(DEBUGFOLDER)execute.sh
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n"
	@printf "%b" "$(WARNING_COLOR)"Copying_Assets_folder"$(NO_COLOR)\n"
	@cp -R ../../assets $(DEBUGFOLDER)
	@cp -R ../../lib	  $(DEBUGFOLDER)
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n" 
	@printf "%b" "$(SUCCESS_COLOR)"Compilation_proccess_successfull"$(NO_COLOR)\n"


$(OBJECTDIRECTORY)%.o:%.cpp												 #Target -> all the objects in the obj folder : Sources -> all the cpp files in the src folder
	@printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n "	 #We print a message to tell the procces that we are making. In this case, Compiling
	@$(CXX) -g -Wall -std=c++14 -o $@ -c $^ $(IRRINCFLAG) $(IRRIMGUIFLAG) $(RAPIDJSONINCFLAG) $(GLMINCFLAG) $(FMODINCFLAG) $(INCLUDEFOLDERS) $(RAPIDXMLFLAG) $(RAKNETINCFLAG) $(INCLUDEFOLDERS) -I../ $(INTERNAL_INCLUDES) #Shell prommpt to compile our source files with the inclusions that are needed 
	@$(call test_compilation)

make_folders:
	@$(MAKE) -C ../AthenAI
	@$(MAKE) -C ../Managers
	@$(MAKE) -C ../Components
	@$(MAKE) -C ../PICASSO
	@$(MAKE) -C ../GraphicEngine
	@$(MAKE) -C ../FMODEngine
	@$(MAKE) -C ../StatesMachine
	@$(MAKE) -C ../States

info:
	$(info $(FIRSTOBJECTS))
	$(info $(OBJECTS))