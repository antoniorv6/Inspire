#include <OrionClientPredictor.hpp>
#include <iostream>
#include <fstream>

ClientPredictor::ClientPredictor()
{
    m_clientSideReg.m_lastPosition = glm::vec3(0,0,0);
    m_clientSideReg.m_lastRotation = glm::vec3(0,0,0);
    m_clientSideReg.m_lastVvelocity = glm::vec3(0,0,0);
    m_clientSideReg.m_timeStamp = 0.0;

    m_serverSideReg.m_lastPosition = glm::vec3(0,0,0);
    m_serverSideReg.m_lastRotation = glm::vec3(0,0,0);
    m_serverSideReg.m_lastVvelocity = glm::vec3(0,0,0);
    m_serverSideReg.m_timeStamp = 0.0;

    m_lagLimit = 100.0;
    m_IsPredicting = false;
}

ClientPredictor::~ClientPredictor()
{
    
}

void ClientPredictor::RegisterEvent(bool isPrediction, const EventRegister &c_eventRegister)
{
    if(isPrediction)
    {
        m_clientSideReg = c_eventRegister;
    }
    else
    {
       m_serverSideReg = c_eventRegister;
    } 
}

EventRegister ClientPredictor::GetResult()
{
    double result = (m_clientSideReg.m_timeStamp*1000) - (m_serverSideReg.m_timeStamp*1000);
    
    if(result > 0.0 && result > m_lagLimit)
    {
        std::cout<<"LAG - "<<result<<std::endl;
        m_IsPredicting = true;
        LogResult();
        return m_clientSideReg;
    }
    
    if(m_IsPredicting)
    {
        m_IsPredicting = false;
        LogResult();
    }

    return m_serverSideReg;
}

void ClientPredictor::LogResult()
{
    std::ofstream c_streamFile("PredictorLogs.log", std::ofstream::app);
    c_streamFile << "--------------- REGISTER OF CLIENT ---------------\n";
    c_streamFile << "Client Prediction with timestamp - " << m_clientSideReg.m_timeStamp <<"\n";
    c_streamFile << "Position - "<<m_clientSideReg.m_lastPosition.x<<" - "<<m_clientSideReg.m_lastPosition.y<<" - "<<m_clientSideReg.m_lastPosition.z<<"\n";
    c_streamFile << "Rotation - "<<m_clientSideReg.m_lastRotation.x<<" - "<<m_clientSideReg.m_lastRotation.y<<" - "<<m_clientSideReg.m_lastRotation.z<<"\n";
    c_streamFile << "Velocity - "<<m_clientSideReg.m_lastVvelocity.x<<" - "<<m_clientSideReg.m_lastVvelocity.y<<" - "<<m_clientSideReg.m_lastVvelocity.z<<"\n";
    c_streamFile << "--------------------------------------------------\n";
    
    c_streamFile << "--------------- REGISTER OF SERVER ---------------\n";
    c_streamFile << "Server position packet with timestamp - " << m_serverSideReg.m_timeStamp <<"\n";
    c_streamFile << "Position - "<<m_serverSideReg.m_lastPosition.x<<" - "<<m_serverSideReg.m_lastPosition.y<<" - "<<m_serverSideReg.m_lastPosition.z<<"\n";
    c_streamFile << "Rotation - "<<m_serverSideReg.m_lastRotation.x<<" - "<<m_serverSideReg.m_lastRotation.y<<" - "<<m_serverSideReg.m_lastRotation.z<<"\n";
    c_streamFile << "Velocity - "<<m_serverSideReg.m_lastVvelocity.x<<" - "<<m_serverSideReg.m_lastVvelocity.y<<" - "<<m_serverSideReg.m_lastVvelocity.z<<"\n";
    c_streamFile << "--------------------------------------------------\n";

    c_streamFile << "--------------- BALANCING RESULT ---------------\n";
    double result = m_clientSideReg.m_timeStamp*1000 - m_serverSideReg.m_timeStamp*1000;
    c_streamFile<<"DELAY: " << result <<std::endl;
    if(!m_IsPredicting)
        c_streamFile<<"CONNECTION BALANCED"<<std::endl;
    c_streamFile<<"------------------------------------------------"<<std::endl;
    
    c_streamFile.close();
}

bool ClientPredictor::IsPredicting()
{
    return m_IsPredicting;
}