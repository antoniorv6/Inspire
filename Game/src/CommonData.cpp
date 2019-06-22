#include <CommonData.hpp>

/**
 * @brief 
 * 
 */
void RenderUtilities::UpdateFreezeValues()
{
    m_mixPct += 0.01f;
    
    if(m_mixPct >= 0.6f)
    {
        if(m_offsetDenom <= 300.0f)
        {
            m_offsetDenom += 5.0f;
        }
    }
}

/**
 * @brief 
 * 
 */
void RenderUtilities::ResetFreezeValues()
{
    m_mixPct      = 0;
    m_offsetDenom = 1.0f;
}

/**
 * @brief 
 * 
 */
void RenderUtilities::UpdateMagnetValues()
{   
    if(m_amplitude >= 26.0f && m_refraction >= 0.26f && m_width >= 1.3f)
    {
        m_desactivateMagnet = true;
    }

    m_amplitude  += 1.0f;
    m_refraction += 0.01f;
    m_width      += 0.05f;
}   

/**
 * @brief 
 * 
 */
void RenderUtilities::ResetMagnetValues()
{
    m_amplitude         = 0.0f;
    m_refraction        = 0.0f;
    m_width             = 0.0f;
    m_desactivateMagnet = false;
}

/**
 * @brief 
 * 
 */
void RenderUtilities::UpdateThiefValues()
{
    // 0.2126, 0.7152, 0.0722
    if(m_averageRGB.x >= 0.2126)
    {
        m_averageRGB.x -= 0.002;
    }
    if(m_averageRGB.y >= 0.7152)
    {
        m_averageRGB.y -= 0.002;
    }
    if(m_averageRGB.z >= 0.0722)
    {
        m_averageRGB.z -= 0.002;
    }
}

/**
 * @brief 
 * 
 */
void RenderUtilities::ResetThiefValues()
{
    m_averageRGB = glm::vec3(1.0f);
}

/**
 * @brief 
 * 
 */
void RenderUtilities::UpdateShieldValues()
{
    m_shildAlpha += 0.05f;
    m_redPct  += 0.01f;
    m_bluePct += 0.01f;

    if(m_redPct >= 0.4f || m_shildAlpha >= 1.0f)
    {
        m_desactivateShield = true;
    }
}

/**
 * @brief 
 * 
 */
void RenderUtilities::ResetShieldValues()
{
    m_shildAlpha = 0.0f;
    m_redPct = 0.0f;
    m_bluePct = 0.0f;
    m_desactivateShield = false;
}

/**
 * @brief 
 * 
 */
void RenderUtilities::UpdateChargingValues()
{
    if(m_expand >= 0.2f)
    {
        m_expand -= 0.035f;
    }
}

/**
 * @brief 
 * 
 */
void RenderUtilities::ResetChargingValues()
{
    m_expand = 1.0f;
}

/**
 * @brief 
 * 
 */
void RenderUtilities::UpdateMirrorValues()
{
    m_mirrorAlpha += 0.03f;
    m_greenPct    += 0.01f;
    m_bluePct2    += 0.01f;

    if(m_greenPct >= 0.5f || m_mirrorAlpha >= 1.0f)
    {
        m_desactivateMirror = true;
    }
}

/**
 * @brief 
 * 
 */
void RenderUtilities::ResetMirrorValues()
{
    m_mirrorAlpha = 0.0f;
    m_greenPct = 0.0f;
    m_bluePct2 = 0.0f;
    m_desactivateMirror = false;
}

/**
 * @brief 
 * 
 * @param l_meshName 
 * @param l_colCount 
 * @param l_rowCount 
 * @return std::vector<std::vector<std::string>> 
 */
 std::vector<std::vector<std::string>> 
 RenderUtilities::GetLODNamesMatrix
(
    const std::string& l_meshName, 
    uint32_t l_colCount,
    uint32_t l_rowCount
)
{
    std::vector<std::vector<std::string>> l_namesMatrix;
    l_namesMatrix.reserve(l_colCount);

    for(uint32_t i = 0; i < l_colCount; i++)
    {
        l_namesMatrix.emplace_back(std::vector<std::string>());

        for(uint32_t j = 0; j < l_rowCount; j++)
        {
            l_namesMatrix[i].push_back(l_meshName + "_" + std::to_string(i + 1) + "_LOD_" + std::to_string(j + 1));    
        }
    }

    return l_namesMatrix;
}

/**
 * @brief 
 * 
 * @param l_meshName 
 * @param l_count 
 * @return std::vector<std::string> 
 */
std::vector<std::string>
RenderUtilities::GetLODNamesVector
(
    const std::string& l_meshName,
    uint32_t l_count
)
{
    std::vector<std::string> l_namesVector;
    l_namesVector.reserve(l_count);

    for(uint32_t i = 0; i < l_count; i++)
    {
        l_namesVector.emplace_back(l_meshName + "_" + std::to_string(i + 1) + "_LOD_1");
    }

    return l_namesVector;
}

/**
 * @brief 
 * 
 * @param l_matrix 
 * @param l_firtsName 
 * @param l_lastName 
 * @param l_framesNumer 
 */
std::vector<std::string> 
RenderUtilities::SetUpNameVector
(
    const std::string& l_firtsName, 
    const std::string& l_lastName,
    uint32_t l_framesNumer
)
{
    std::vector<std::string> l_framesVector;

    for(uint32_t i = 0; i < l_framesNumer; i++)
    {
        l_framesVector.push_back(l_firtsName + l_lastName + std::to_string(i + 1));
    }

    return l_framesVector;
}

/**
 * @brief 
 * 
 * @param l_textureNames 
 * @return std::vector<std::string> 
 */
std::vector<std::string> 
RenderUtilities::GetLODTexturesNames(const std::string& l_textureNames)
{
    std::vector<std::string> l_names;
    std::vector<std::string> l_resolutions;
    l_resolutions.push_back("_1024.png");
    l_resolutions.push_back("_256.png");
    l_resolutions.push_back("_128.png");

    for(uint32_t i = 0; i < 3; i++)
    {
        l_names.push_back(l_textureNames + l_resolutions[i]);
    }

    return l_names;
}