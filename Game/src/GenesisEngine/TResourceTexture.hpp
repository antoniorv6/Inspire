#include <TResource.hpp>

class TResourceTexture: public TResource
{
    public:
        TResourceTexture();
        ~TResourceTexture();
        void LoadResource(const std::string& c_resourceDocument_str);

        void LoadFromSTBI(const std::string& c_resourceDocument_str);
        void LoadFromBinary(const std::string& c_resourceDocument_str);
        void ParseToBinary(const std::string& c_binaryPath_str);

        void BindTexture();
        
        unsigned char* GetImageChar() {return m_imageData_cch;};
        int GetWidth()      {  return m_width_i;  };
        int GetHeight()     {  return m_height_i; };
        int GetBitDepth()   {  return m_bDepth_i; };
        unsigned int GetID(){  return m_textureID;};

    private:
        unsigned char* m_imageData_cch     = nullptr;
        unsigned int   m_textureID         = 0;
        int m_width_i = 0, m_height_i = 0, m_bDepth_i = 0;
}; 