#include <vector>
#include <string>

class SoundEngineFacade;
class Clock;

class RandomMusicGen 
{
    public:
        RandomMusicGen(int c_tempoToPlay, int c_initialScale, int c_rootNote, const std::string& c_eventIdentifier, SoundEngineFacade*& c_soundEngine);
        ~RandomMusicGen();

        void Play();
        double CalculateTempoInSeconds(int c_tempo);
        int LogisticEQ();
        void CreateScales();
    
    private:
        float    m_R;                  //Growing speed of the function, suggested values are between 3 and 4 
        float    m_xValue;             //Starting point of the logistic equation

        int      m_scalePlayed;        //We need to know which scale will be played first, maybe this will vary depending if we add modulations or not
        double   m_tempo;              //Tempo that will be played in the program
        int      m_rootTransposition;  //This is the root note transposition. If we put 0, the root note will be C3 (for now)
        
        //0 = Major Scale
        //1 = Lidian Major Scale
        //2 = Mixolidian Major Scale
        float m_scaleModes[3][14];     //Modes of the scales that the randomizer will use, for now, we are using 3 scales of three octaves each one (C3 - C5)
                                       //In this program we are going to store the Major, Lidian and Mixolidian scales 
        
        Clock* m_tempoClock;           //Clock that will handle the tempo in order to have sort of rithm
        std::string m_FMODEventIdentifier; 

        SoundEngineFacade*    m_soundEngine; //Our sound engine, we need a reference in order to change the parameter values

        bool    m_isPlayingNote;       //This boolean serves to adjust more or less the melody to tempo, needs improvement
};