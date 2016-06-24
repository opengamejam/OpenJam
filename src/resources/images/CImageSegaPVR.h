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
    CImageDreamPVR(const std::string& filename);
    virtual ~CImageDreamPVR();
    
    virtual bool Load();
    virtual const TResourceData& RawData();
    virtual uint32_t Width() const;
    virtual uint32_t Height() const;
    virtual uint32_t Bpp() const;
    virtual uint32_t MipsCount() const;
	virtual bool IsCompressed() const;
    virtual TexelFormats TexelFormat() const;
    virtual TexelTypes TexelType() const;

private:
    uint32_t TextureFormat(unsigned char* header);

    
private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Mipmaps;
    TResourceData m_Data;
    TexelProps m_TexelProps;
    bool m_IsCompressed;
    
	#pragma pack(push,1)
	struct GBIXHeader               // Global index header
	{
		uint32_t version;           // ID, "GBIX" in ASCII
		uint32_t nextTagOffset;     // Bytes number to the next tag
		uint64_t globalIndex;
	};
	#pragma pack(pop)

	#pragma pack(push,1)
	struct PVRTHeader
	{
		uint32_t version;            // ID, "PVRT" in ASCII
		uint32_t textureDataSize;
		uint32_t textureAttributes;
		uint16_t width;              // Width of the texture.
		uint16_t height;             // Height of the texture.
	};
	#pragma pack(pop)
    
    static const std::map<uint64_t, TexelProps> s_TexelProps;
};

}; // namespace jam

#endif	/* CImageDreamPVR_H */
