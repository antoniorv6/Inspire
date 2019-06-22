#include <glm.hpp>

struct EventRegister
{
    glm::vec3   m_lastPosition;
    glm::vec3   m_lastRotation;
    glm::vec3   m_lastVvelocity;
    float       m_rotSpin;
    double      m_timeStamp;
};

class ClientPredictor 
{
    public:
        ClientPredictor();
        ~ClientPredictor();
        void RegisterEvent(bool isPrediction, const EventRegister &c_event);
        EventRegister GetResult();
        void LogResult();
        bool IsPredicting();

    private:
        EventRegister m_clientSideReg, m_serverSideReg;
        bool m_IsPredicting;
        double m_lagLimit;
};