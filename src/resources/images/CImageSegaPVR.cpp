/* 
 * File:   CImageDreamPVR.cpp
 * Author: yevgeniy.logachev
 */

#include "CImageSegaPVR.h"
#include "Global.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************
const std::map<uint64_t, TexelProps> CImageDreamPVR::s_TexelProps =
{
    std::make_pair(0x01, TexelProps(TF_RGB565,        TT_UByte,   16)),
    std::make_pair(0x00, TexelProps(TF_RGBA5551,      TT_UByte,   16)),
    std::make_pair(0x02, TexelProps(TF_RGBA4444,      TT_UByte,   16)),
	//std::make_pair(0x03, TexelProps(TF_YUV422,      TT_UByte,   16)),
};

INL uint32_t MipMapsCountFromWidth(uint32_t width);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CImageDreamPVR::CImageDreamPVR(const std::string& filename)
: IImage(filename)
, m_Width(1)
, m_Height(1)
, m_Mipmaps(0)
, m_TexelProps(TexelProps(TF_A8, TT_UByte, 1))
, m_IsCompressed(false)
{
    
}

CImageDreamPVR::~CImageDreamPVR()
{
    
}

bool CImageDreamPVR::Load()
{
    if (IResource::Load())
    {
		const IResource::TResourceData& data = IResource::RawData();

		uint32_t offset = 0;
		GBIXHeader gbixHeader;
		if (strncmp((char*)data.data(), "GBIX", 4) == 0)
		{
			memcpy(&gbixHeader, data.data(), sizeof(GBIXHeader));
			offset += sizeof(GBIXHeader);
		}

		PVRTHeader header;
        memcpy(&header, data.data() + offset, sizeof(PVRTHeader));
        offset += sizeof(PVRTHeader);

        m_Width = header.width;
        m_Height = header.height;

        int format = ((header.textureAttributes >> 8) & 0xFF);
        
        bool hasMipMaps = ((format == 0x02) || (format == 0x04) || (format == 0x11));
        if (hasMipMaps)
        {
            m_Mipmaps = MipMapsCountFromWidth(m_Width);
        }
        else
        {
            m_Mipmaps = 1;
        }

        m_IsCompressed = ((format == 0x03) || (format == 0x04) || (format == 0x10) || (format == 0x11));
        
        std::map<uint64_t, TexelProps>::const_iterator prop = s_TexelProps.find(header.textureAttributes & 0xFF);
        assert(prop != s_TexelProps.end());
        m_TexelProps = prop->second;

        m_Data.assign(data.begin() + offset, data.end());

		IResource::Unload();

        return true;
    }

    return false;
}

const IResource::TResourceData& CImageDreamPVR::RawData()
{ 
    return m_Data; 
}

uint32_t CImageDreamPVR::Width() const
{
    return m_Width;
}

uint32_t CImageDreamPVR::Height() const
{
    return m_Height;
}

uint32_t CImageDreamPVR::Bpp() const
{
    return m_TexelProps.bpp;
}

uint32_t CImageDreamPVR::MipsCount() const
{
    return m_Mipmaps;
}

bool CImageDreamPVR::IsCompressed() const
{
    return m_IsCompressed;
}

TexelFormats CImageDreamPVR::TexelFormat() const
{
    return m_TexelProps.texelFormat;
}

TexelTypes CImageDreamPVR::TexelType() const
{
    return m_TexelProps.texelType;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

INL uint32_t MipMapsCountFromWidth(uint32_t width)
{
    uint32_t mipMapsCount = 0;
    while( width )
    {
        ++mipMapsCount;
        width /= 2;
    }
    
    return mipMapsCount;
}
