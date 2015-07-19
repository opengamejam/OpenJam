/* 
 * File:   CImagePVR.cpp
 * Author: yevgeniy.logachev
 */

#include "CImagePVR.h"
#include "Global.h"
#include "ILoader.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************
const std::map<unsigned long long, TexelProps> CImagePVR::s_TexelProps =
{
    std::make_pair(0x0000000000000000ULL, TexelProps(TF_PVRTC2,        TT_UByte,   2)),
    std::make_pair(0x0000000000000001ULL, TexelProps(TF_PVRTC2Alpha,   TT_UByte,   2)),
    std::make_pair(0x0000000000000002ULL, TexelProps(TF_PVRTC4,        TT_UByte,   4)),
    std::make_pair(0x0000000000000003ULL, TexelProps(TF_PVRTC4Alpha,   TT_UByte,   4)),
    std::make_pair(0x0000000000000006ULL, TexelProps(TF_ETC,           TT_UByte,   24)),
    std::make_pair(0x0000000800000061ULL, TexelProps(TF_A8,            TT_UByte,   8)),
    std::make_pair(0x000000080000006CULL, TexelProps(TF_L8,            TT_UByte,   8)),
    std::make_pair(0x000008080000616CULL, TexelProps(TF_LA88,          TT_UByte,   32)),
    std::make_pair(0x0005060500626772ULL, TexelProps(TF_RGB565,        TT_UShort,  32)),
    std::make_pair(0x0008080800626772ULL, TexelProps(TF_RGB888,        TT_UByte,   24)),
    std::make_pair(0x0105050561626772ULL, TexelProps(TF_RGBA5551,      TT_UByte,   16)),
    std::make_pair(0x0404040461626772ULL, TexelProps(TF_RGBA4444,      TT_UByte,   16)),
    std::make_pair(0x0808080861626772ULL, TexelProps(TF_RGBA8888,      TT_UByte,   32)),
    std::make_pair(0x2020202061626772ULL, TexelProps(TF_RGBA32323232,  TT_Float,   128)),
    std::make_pair(0x0808080861726762ULL, TexelProps(TF_BGRA8888,      TT_UByte,   32)),
};

// *****************************************************************************
// Public Methods
// *****************************************************************************

CImagePVR::CImagePVR(const std::string& filename, const std::string& loader)
: IImage(filename, loader)
, m_Width(1)
, m_Height(1)
, m_Mipmaps(0)
, m_TexelProps(TexelProps(TF_A8, TT_UByte, 1))
{
    
}

CImagePVR::~CImagePVR()
{
    
}

bool CImagePVR::Load()
{
    if (IResource::Load())
    {
		const IResource::TResourceData& data = IResource::RawData();

		PVRTHeader header;
		memset(&header, 0, sizeof(header));
        memcpy(&header, &data[0], s_PVRT_HEADER_SIZE);

        assert(header.numsurfaces == 1); // "Multiple Surfaces not supported");
        assert(header.numfaces == 1); // "Cubemaps not supported");

        m_Width = header.width;
        m_Height = header.height;
        m_Mipmaps = header.mipmapcount;
        
        std::map<unsigned long long, TexelProps>::const_iterator prop = s_TexelProps.find(header.pixelformat);
        assert(prop != s_TexelProps.end());
        m_TexelProps = prop->second;

        m_Data.assign(data.begin() + s_PVRT_HEADER_SIZE + header.metasize, data.end());
        
		IResource::Unload();
        
        return true;
    }

    return false;
}

const IResource::TResourceData& CImagePVR::RawData()
{ 
    return m_Data; 
}

unsigned int CImagePVR::Width() const
{
    return m_Width;
}

unsigned int CImagePVR::Height() const
{
    return m_Height;
}

unsigned int CImagePVR::Bpp() const
{
    return m_TexelProps.bpp;
}

unsigned int CImagePVR::MipsCount() const
{
    return m_Mipmaps;
}

unsigned int CImagePVR::IsCompressed() const
{
    return (m_TexelProps.texelFormat > TF_BGRA8888);
}

TexelFormats CImagePVR::TexelFormat() const
{
    return m_TexelProps.texelFormat;
}

TexelTypes CImagePVR::TexelType() const
{
    return m_TexelProps.texelType;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
