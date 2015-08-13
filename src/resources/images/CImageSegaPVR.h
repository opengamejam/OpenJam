/* 
 * File:   CImageDreamPVR.h
 * Author: yevgeniy.logachev
 */

#ifndef CImageDreamPVR_H
#define	CImageDreamPVR_H

#include "Global.h"
#include "IIMage.h"

namespace jam
{

class CImageDreamPVR : public IImage
{    
public:
    virtual ~CImageDreamPVR();
    
    virtual bool            Load();
    virtual const ResourceData_t& RawData();
    virtual unsigned int    Width() const;
    virtual unsigned int    Height() const;
    virtual unsigned int    Bpp() const;
    virtual unsigned int    MipsCount() const;
	virtual unsigned int    IsCompressed() const;
    virtual TexelFormats    TexelFormat() const;
    virtual TexelTypes      TexelType() const;
    
    CImageDreamPVR(const std::string& filename, const std::string& loader = "");

private:
    unsigned int 			TextureFormat(unsigned char* header);

    
private:
    unsigned int            m_Width;
    unsigned int            m_Height;
    unsigned int            m_Mipmaps;
    ResourceData_t          m_Data;
    TexelProps              m_TexelProps;
    bool 					m_IsCompressed;
    
	#pragma pack(push,1)
	struct GBIXHeader                       // Global index header
	{
		unsigned int        version;        // ID, "GBIX" in ASCII
		unsigned int        nextTagOffset;  // Bytes number to the next tag
		unsigned long long	globalIndex;
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct PVRTHeader
	{
		unsigned int        version;            // ID, "PVRT" in ASCII
		unsigned int        textureDataSize;
		unsigned int        textureAttributes;
		unsigned short      width;              // Width of the texture.
		unsigned short      height;             // Height of the texture.
	};
	#pragma pack(pop)
    
    static const size_t     s_PVRT_HEADER_SIZE = sizeof(PVRTHeader);
    
    static const std::map<unsigned long long, TexelProps> s_TexelProps;
};

}; // namespace jam

#endif	/* CImageDreamPVR_H */
