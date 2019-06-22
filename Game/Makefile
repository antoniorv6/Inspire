.PHONY: info clean debug server genesis			  	   #.PHONY is used to call a makefile function whenever there exists a physical file called the same or not
									   #https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html

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
#Generic linking message string that won't change for now				  
LINK_STRING = "LINKING..."

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

#TEST COMPILATION
#Checks if the compilation proccess is successfull or not - It gives a success message if it went OK, a error message if it didn't and a warninig if we have to be concerned about something
define link_compilation_test
		
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
#TEST FOLDER EXISTENCE
#Checks if the folder exists, if not then it creates it
define test_folder_existence

	if [ ! -d "obj" ]; then\
		printf "%b" "$(WARNING_COLOR)"The_object_folder_does_not_exist"$(NO_COLOR)\n"; \
		printf "%b" "$(COM_COLOR)"Creating..."$(NO_COLOR)\n ";\
    	mkdir obj;\
		printf "%b" "$(SUCCESS_COLOR)"Created!"$(NO_COLOR)\n ";\
	fi;

	if [ ! -d "GenisisRelease" ]; then\
		printf "%b" "$(WARNING_COLOR)"The_object_folder_does_not_exist"$(NO_COLOR)\n"; \
		printf "%b" "$(COM_COLOR)"Creating..."$(NO_COLOR)\n ";\
    	mkdir GenesisRelease;\
		printf "%b" "$(SUCCESS_COLOR)"Created!"$(NO_COLOR)\n ";\
	fi;

	if [ ! -d "release" ]; then\
		printf "%b" "$(WARNING_COLOR)"The_release_folder_does_not_exist"$(NO_COLOR)\n"; \
		printf "%b" "$(COM_COLOR)"Creating..."$(NO_COLOR)\n ";\
    	mkdir release;\
		printf "%b" "$(SUCCESS_COLOR)"Created!"$(NO_COLOR)\n ";\
	fi;

	if [ ! -d "debug" ]; then\
		printf "%b" "$(WARNING_COLOR)"The_release_folder_does_not_exist"$(NO_COLOR)\n"; \
		printf "%b" "$(COM_COLOR)"Creating..."$(NO_COLOR)\n ";\
    	mkdir debug;\
		printf "%b" "$(SUCCESS_COLOR)"Created!"$(NO_COLOR)\n ";\
	fi;

	if [ ! -d "server" ]; then\
		printf "%b" "$(WARNING_COLOR)"The_server_folder_does_not_exist"$(NO_COLOR)\n"; \
		printf "%b" "$(COM_COLOR)"Creating..."$(NO_COLOR)\n ";\
    	mkdir server;\
		printf "%b" "$(SUCCESS_COLOR)"Created!"$(NO_COLOR)\n ";\
	fi;

endef

SOURCES := $(wildcard src/*.cpp)				#All the source files in our project (located in src folder)
FIRSTOBJECTS := $(subst  .cpp,.o,$(SOURCES))	#All the object files in our project. We pick all the cpp files and change it's extention with an o in order to obtain, for example, src/Game.cpp -> src/Game.o
OBJECTS:= $(subst src/,obj/,$(FIRSTOBJECTS))	#This was a quick soultion as the last function picked up the src/ part of the path. We change all src to obj (where our binaries are), src/Game.o -> obj/Game.o  	

#FOLDERS FOR RELEASE AND DEBUG
RELEASEFOLDER=release/
DEBUGFOLDER=debug/
SERVERFOLDER=server/
DEBUGNAME=debug
GAMENAME=Inspire

#BASH COMMANDS
COMMANDBASH="export LD_LIBRARY_PATH=lib/fmod/:lib/raknet:lib/irrimgui:lib/assimp && ./$(GAMENAME)"

#INCLUDE FOLDERS
INCLUDEFOLDERS := -I src/Managers -I src/Components -I src/FMODEngine -I src/GraphicEngine -I src/PICASSO -I src/GenesisEngine -I src/StatesMachine -I src/States -I src/AthenAI/DecTree -I src/AthenAI/FSM -I src/AthenAI/Fuzzy -I src/AthenAI/Sensors -I src/AthenAI/SteeringBehaviors

#IRRLICHT INCLUDE PATHS AND LIBRARY PATHS
#--------------------------------------------------------------------------------------------------------------------
#Where our Irrlicht .h files are located
IRRLICHTINCTDIR=include/irrlicht
#GLM directory
GLMDIR=include/GLM
#Assimp directory
ASSIMPINCDIR=include/assimp
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
RAKNETINCDIR = include/raknet
RAKNETLIBDIR = lib/raknet

GLFWINCDIR= include/GLFW
GLEWINCDIR= include/GLEW

GLEWLIBDIR = lib/GLEW
GLFWLIBDIR = lib/GLFW

FREETYPEINCDIR = include/freetype
#The instruction needed to include this path in compilation (Will extent with code growing)
IRRINCFLAG= -I$(IRRLICHTINCTDIR)
#Same with FMOD Studio
FMODINCFLAG= -I$(FMODDINCDIR)
#Instruction needed to include GLM
GLMINCFLAG= -I$(GLMDIR)
#Include Assimp
ASSIMPINCFLAG= -I$(ASSIMPINCDIR)
#Include RapidXML
RAPIDXMLFLAG = -I$(RAPIDXMLINCDIR)
#Irrimguinclude
IRRIMGUIFLAG = -I$(IRRIMGUIINCDIR)
#Raknet
RAKNETINCFLAG = -I$(RAKNETINCDIR)

GLFWINCFLAG = -I$(GLFWINCDIR)
GLEWINCFLAG = -I$(GLEWINCDIR)

FREETYPEINCLFAG= -I$(FREETYPEINCDIR)
FREETYPELIBDIR=  lib/freetype

SHAREDLIBS= -Wl,-rpath=lib/fmod -Wl,-rpath=lib/irrimgui -Wl,-rpath=lib/raknet -Wl,-rpath=lib/GLFW -Wl,-rpath=lib/assimp
#--------------------------------------------------------------------------------------------------------------------

#CRUCIAL FOLDERS
#.o files directory
OBJDIR=obj
#.cpp files directory
SRCDIR=src

#Include flags for internal .h files -- Will extent as the project grows
INTERNAL_INCLUDES:= -I $(SRCDIR)/ 
#Library flags -- Will extent as the project grows
LIBS= -lIrrlicht -lGLU -lGL -lX11 -lXxf86vm -lfmod -lfmodstudio -lRakNetDLL -lGLEW -lglfw3 -lXrandr -pthread -lXi -ldl -lXinerama -lXcursor -lassimp -lfreetype

all:erase_release create_folders make_folders $(RELEASEFOLDER)$(GAMENAME)

server:erase_release create_folders make_server

debug:erase_debug create_folders make_folders $(DEBUGFOLDER)$(DEBUGNAME)

genesis:erase_release create_folders make_gengine

#LINKING
$(RELEASEFOLDER)$(GAMENAME):$(OBJECTS)
												 				         #Target -> ejecutable : Sources -> All the files that are in the obj folder
	@printf "%b" "$(COM_COLOR)$(LINK_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n " #We print a message to tell the procces that we are making. In this case, Linking
	@g++ -Wall -std=c++14 -o $@ obj/*.o -Llib/irrimgui -lIrrIMGUI -L$(GLFWLIBDIR) -L$(FREETYPELIBDIR) -L$(GLEWLIBDIR) -L$(IRRLICHTLIBDIR) -L lib/IRRIMGUI -L lib/assimp -L$(FMODLIBDIR) -L$(RAKNETLIBDIR) $(SHAREDLIBS) $(LIBS)	 #Shell prommpt to link our binary files with the libraries we need
	@$(call link_compilation_test)										     #We call our checking function to trigger the message that is needed in this
	
	@printf "%b" "$(WARNING_COLOR)"Creating_Bash_Execuion_File"$(NO_COLOR)\n"
	@touch $(RELEASEFOLDER)execute.sh
	@echo $(COMMANDBASH) > $(RELEASEFOLDER)execute.sh
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n"
	@printf "%b" "$(WARNING_COLOR)"Copying_Assets_folder"$(NO_COLOR)\n"
	@cp -R assets $(RELEASEFOLDER)
	@cp -R lib	  $(RELEASEFOLDER)
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n" 
	@printf "%b" "$(SUCCESS_COLOR)"Compilation_proccess_successfull"$(NO_COLOR)\n"	

$(DEBUGFOLDER)$(DEBUGNAME):$(OBJECTS) 														 #Target -> ejecutable : Sources -> All the files that are in the obj folder
	@printf "%b" "$(COM_COLOR)$(LINK_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n " #We print a message to tell the procces that we are making. In this case, Linking
	@printf "%b" "$(COM_COLOR)DEBUG_VERSION$(NO_COLOR)\n "
	@g++ -g -Wall -std=c++14 -o $@ obj/*.o  -Llib/irrimgui -lIrrIMGUI -L$(GLFWLIBDIR) -L$(GLEWLIBDIR) -L$(IRRLICHTLIBDIR) -L lib/IRRIMGUI -L lib/assimp -L$(FMODLIBDIR) -L$(RAKNETLIBDIR) $(SHAREDLIBS) $(LIBS)								 #Shell prommpt to link our binary files with the libraries we need
	@$(call link_compilation_test)										     #We call our checking function to trigger the message that is needed in this
	@printf "%b" "$(WARNING_COLOR)"Copying_Assets_folder"$(NO_COLOR)\n"
	@cp -R assets $(DEBUGFOLDER)
	@cp -R lib	  $(DEBUGFOLDER)
	@printf "%b" "$(SUCCESS_COLOR)"Copied!"$(NO_COLOR)\n" 
	@printf "%b" "$(SUCCESS_COLOR)"Debug_compilation_proccess_successfull"$(NO_COLOR)\n"						 							 		


$(OBJDIR)/%.o:$(SRCDIR)/%.cpp												 #Target -> all the objects in the obj folder : Sources -> all the cpp files in the src folder
	@printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(^)$(NO_COLOR)\n "	 #We print a message to tell the procces that we are making. In this case, Compiling
	@g++ -g -Wall -O3 -std=c++14 -o $@ -c $^ $(IRRINCFLAG) $(FREETYPEINCFLAG) $(GLFWINCFLAG) $(GLEWINCFLAG) $(ASSIMPINCFLAG) $(IRRIMGUIFLAG) $(GLMINCFLAG) $(FMODINCFLAG) $(INCLUDEFOLDERS) $(RAPIDXMLFLAG) $(INTERNAL_INCLUDES) $(RAKNETINCFLAG) #Shell prommpt to compile our source files with the inclusions that are needed 
	@$(call test_compilation)												 #We call our checking function to trigger the message that is needed in this

erase_release:
	@rm -f release/Inspire
	@rm -f obj/main.o
erase_debug:
	@rm -f debug/debug

create_folders:
		@$(call test_folder_existence)									     #We call first our function to check if the obj folder exists

make_folders:
		@$(MAKE) -C src/AthenAI
		@$(MAKE) -C src/Managers
		@$(MAKE) -C src/Components
		@$(MAKE) -C src/PICASSO
		@$(MAKE) -C src/GraphicEngine
		@$(MAKE) -C src/FMODEngine
		@$(MAKE) -C src/StatesMachine
		@$(MAKE) -C src/States
		@$(MAKE) genesis -C src/GenesisEngine
		@$(MAKE) http -C src/Orion
		@$(MAKE) -C src/HUD
		@$(MAKE) -C src/GUI

make_server:
		@$(MAKE) -C src/Orion

make_gengine:
		@$(MAKE) -C src/GenesisEngine


#INFO PROCCESS TO DEBUG DIRECTORY LOCATIONS
info:
	$(info $(SOURCES))									
	$(info $(OBJECTS))

#CLEANING FUNCTION
clean:
	@printf "%b" "$(WARNING_COLOR)"Erasing" " " "all" " " "binaries"$(OBJ_COLOR)$(^)$(NO_COLOR)\n " #Message for erasing binaries

	@rm -rf $(OBJDIR)																				#Erase OBJ folder
	@printf "%b" "$(WARNING_COLOR)"BINARY" " " "FOLDER" " " "ERASED"$(OBJ_COLOR)$(^)$(NO_COLOR)\n " #Confirm erasing. Success message
	
	@rm -rf $(RELEASEFOLDER)																					#Erase executable file
	@printf "%b" "$(WARNING_COLOR)"RELEASE" " " "FOLDER" " " "ERASED"$(OBJ_COLOR)$(^)$(NO_COLOR)\n "			#Confirm erasing. Success message

	@rm -rf $(DEBUGFOLDER)																					#Erase executable file
	@printf "%b" "$(WARNING_COLOR)"DEBUG" " " "FOLDER" " " "ERASED"$(OBJ_COLOR)$(^)$(NO_COLOR)\n "			#Confirm erasing. Success message

	@rm -rf $(SERVERFOLDER)																					#Erase executable file
	@printf "%b" "$(WARNING_COLOR)"SERVER" " " "FOLDER" " " "ERASED"$(OBJ_COLOR)$(^)$(NO_COLOR)\n "			#Confirm erasing. Success message

	@rm -rf GenesisRelease/																				#Erase executable file
	@printf "%b" "$(WARNING_COLOR)"GRAPHIC ENGINE" " " "FOLDER" " " "ERASED"$(OBJ_COLOR)$(^)$(NO_COLOR)\n "			#Confirm erasing. Success message

	@rm -rf assets/binaries/models/*.bin
	@rm -rf assets/binaries/textures/*.bin

	@rm -rf ProfilerResults/*.html

cleanbin:
	@rm -rf assets/binaries/models/*.bin
	@rm -rf assets/binaries/textures/*.bin

