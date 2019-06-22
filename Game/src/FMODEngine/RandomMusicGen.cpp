#include <RandomMusicGen.hpp>
#include <Clock.hpp>
#include <SoundEngine.h>

/**
 * Construct a new Random Music Gen:: Random Music Gen object
 * 
 * @param c_tempoToPlay 
 * @param c_initialScale 
 * @param c_rootNote 
 * @param c_eventIdentifier 
 */
RandomMusicGen::RandomMusicGen(int c_tempoToPlay, int c_initialScale, int c_rootNote, const std::string& c_eventIdentifier, SoundEngineFacade*& c_soundEngine)
{
    m_FMODEventIdentifier = c_eventIdentifier;
    m_tempo = CalculateTempoInSeconds(c_tempoToPlay);
    m_rootTransposition = c_rootNote;
    m_scalePlayed = c_initialScale;
    CreateScales();

    //INITIALIZATIONS OF THE ALGORITHM
    m_R = 3.7;
    m_xValue = 0.5;
    //--------------------------------

    //Engine oriented initializations
    m_soundEngine = c_soundEngine;

    m_tempoClock = new Clock();
    m_tempoClock->Init();

    m_isPlayingNote = false;
    //-------------------------------
}

RandomMusicGen::~RandomMusicGen()
{
    delete m_tempoClock;
}

/**
 * Calculates the ammount of time that lasts a beat (in miliseconds)
 * 
 * @param c_tempo 
 * @return double 
 */
double RandomMusicGen::CalculateTempoInSeconds(int c_tempo)
{
    return ((double)60/(double)c_tempo); //It just makes a simple conversion
}

/**
 * Creates the scales for the random music generator
 * 
 * Maybe this method will change, it depends in the final scales we are going to use during our project
 * It now maps Major Scale, Lidian Major Scale and Mixolidian Major Scale
 */
void RandomMusicGen::CreateScales()
{
    //Major Scale = T - T - ST - T - T - T - ST
    float l_toneInterval = 0.5;
    int l_isSemitone = 0;
    int l_localScaleCounter = 1;
    for(int i = 0; i < 13; i++)
    {
        m_scaleModes[0][i] = l_toneInterval;

        l_isSemitone = ((l_localScaleCounter == 3 || l_localScaleCounter == 7) && l_localScaleCounter != 0);
        
        l_toneInterval += 0.05 * (l_isSemitone) + 0.15 * (!l_isSemitone);

        if(l_localScaleCounter > 8)
            l_localScaleCounter = 1;
        else
            l_localScaleCounter++;
        
    }

    m_scaleModes[0][13] = 2.0; //Maximum that FMOD allows us

    //Lidian Scale = T - T - T - ST - T - T - ST
    l_toneInterval = 0.5;
    l_isSemitone   = 0;
    l_localScaleCounter = 1;
    for(int i = 0; i < 13; i++)
    {
        m_scaleModes[1][i] = l_toneInterval;

        l_isSemitone = ((l_localScaleCounter == 3 || l_localScaleCounter == 4) && l_localScaleCounter != 0);

        l_toneInterval += 0.05 * (l_isSemitone) + 0.15 * (!l_isSemitone);

         if(l_localScaleCounter > 8)
            l_localScaleCounter = 1;
        else
            l_localScaleCounter++;
    }

    m_scaleModes[0][13] = 2.0; //Maximum that FMOD allows us

    //Mixolidian Scale = T - T - ST - T - T - ST - T
    l_toneInterval = 0.5;
    l_isSemitone   = 0;
    l_localScaleCounter = 1;
    for(int i = 0; i < 13; i++)
    {
        m_scaleModes[2][i] = l_toneInterval;

        l_isSemitone = ((l_localScaleCounter == 3 || l_localScaleCounter == 7) && l_localScaleCounter != 0);

        l_toneInterval += 0.05 * (l_isSemitone) + 0.15 * (!l_isSemitone);

         if(l_localScaleCounter > 7)
            l_localScaleCounter = 1;
        else
            l_localScaleCounter++;
    }

    m_scaleModes[0][13] = 2.0; //Maximum that FMOD allows us
}

/**
 * Generates and orders the FMOD Engine to play a note in the evet that has been assigned
 * 
 */
void RandomMusicGen::Play()
{
    if(m_tempoClock->GetElapsedTime() > m_tempo) // If we have reached the metronome tick, we can play a note
    {
        int l_positionToPlay = LogisticEQ();                                    // We apply the logistic equation to get our next note to play
        float l_semiToneToPlay = m_scaleModes[m_scalePlayed][l_positionToPlay]; // We get the semitone mapped in the actual scale
        std::cout<< "Note playing in scale - " << m_scalePlayed << " --> " << l_semiToneToPlay<<std::endl;
        
        m_tempoClock->Restart();                                                //Restart the clock for new tempo metrics

        m_isPlayingNote = true; //We are playing the note!
        m_soundEngine->ChangeEventVariable(m_FMODEventIdentifier, "Play", m_isPlayingNote);     //|
        m_soundEngine->ChangeEventVariable(m_FMODEventIdentifier, "Pitch", l_semiToneToPlay);   //| Instructions to the sound engine to play music
    }
    else
    {
        if(m_isPlayingNote) //Check if we are playing a note
        {
            m_isPlayingNote = false;
            m_soundEngine->ChangeEventVariable(m_FMODEventIdentifier, "Play", m_isPlayingNote); // Tell the engine to stop it
        }
    }
    
}

/**
 * @brief Applies the logistic equation for this time and determines the matrix position that has to be played
 * 
 */
int RandomMusicGen::LogisticEQ()
{
    m_xValue = m_R * m_xValue * (1 - m_xValue); //LOGISTIC EQUATION
    return (m_xValue * 14);                     //The value mapped in the matrix
                                                //Max value of m_XValue is 1, and there are now 14 positions in our matrix, so
                                                //we have to resolve mappedValue = 14*m_xValue / 1 -> 14*m_xValue
}   