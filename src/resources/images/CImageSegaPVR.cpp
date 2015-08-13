/* 
 * File:   CImageDreamPVR.cpp
 * Author: yevgeniy.logachev
 */

#include "CImageDreamPVR.h"
#include "Global.h"
#include "ILoader.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************
const std::map<unsigned long long, TexelProps> CImageDreamPVR::s_TexelProps =
{
    std::make_pair(0x01, TexelProps(TF_RGB565,        TT_UByte,   16)),
    std::make_pair(0x00, TexelProps(TF_RGBA5551,      TT_UByte,   16)),
    std::make_pair(0x02, TexelProps(TF_RGBA4444,      TT_UByte,   16)),
	//std::make_pair(0x03, TexelProps(TF_YUV422,      TT_UByte,   16)),
};

// *****************************************************************************
// Public Methods
// *****************************************************************************

CImageDreamPVR::CImageDreamPVR(const std::string& filename, const std::string& loader)
: IImage(filename, loader)
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
		const IResource::ResourceData_t& data = IResource::RawData();

		unsigned int offset = 0;
		struct GBIXHeader gbixHeader;
		if (strncmp((char*)data.data(), "GBIX", 4) == 0)
		{
			memcpy(&gbixHeader, data.data(), sizeof(struct GBIXHeader));
			offset += sizeof(struct GBIXHeader);
		}

		PVRTHeader header;
		memset(&header, 0, s_PVRT_HEADER_SIZE);
        memcpy(&header, data.data() + offset, s_PVRT_HEADER_SIZE);

        m_Width = header.width;
        m_Height = header.height;
        m_Mipmaps = 0;//((header.textureAttributes & 0x02) || (header.textureAttributes & 0x04) || (header.textureAttributes & 0x11));

        int format = ((header.textureAttributes >> 8) & 0xFF);
        m_IsCompressed = ((format == 0x03) || (format == 0x04) || (format == 0x10) || (format == 0x11));
        
        std::map<unsigned long long, TexelProps>::const_iterator prop = s_TexelProps.find(header.textureAttributes & 0xFF);
        assert(prop != s_TexelProps.end());
        m_TexelProps = prop->second;

        m_Data.assign(data.begin() + s_PVRT_HEADER_SIZE + offset, data.end());

		IResource::Unload();

		printf("Loaded image: %s\n", Filename().c_str());

        return true;
    }

    return false;
}

const IResource::ResourceData_t& CImageDreamPVR::RawData()
{ 
    return m_Data; 
}

unsigned int CImageDreamPVR::Width() const
{
    return m_Width;
}

unsigned int CImageDreamPVR::Height() const
{
    return m_Height;
}

unsigned int CImageDreamPVR::Bpp() const
{
    return m_TexelProps.bpp;
}

unsigned int CImageDreamPVR::MipsCount() const
{
    return m_Mipmaps;
}

unsigned int CImageDreamPVR::IsCompressed() const
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
