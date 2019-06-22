#pragma once

class State {
    public:
        virtual ~State(){};
        virtual void Init() = 0;
        virtual void Update(float&) = 0;
        virtual void Draw() = 0;
        virtual void ChangeState() = 0;

        virtual void Pause(){};
        virtual void Resume(){};
        
        virtual void DrawStateGUI(){};

        virtual void CloseState(){};

        virtual void ProcessGUIEvent(int, int){};
};