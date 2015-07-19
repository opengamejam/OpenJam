/* 
 * File:   CImagePVR.h
 * Author: yevgeniy.logachev
 */

#ifndef CIMAGEPVR_H
#define	CIMAGEPVR_H

#include "Global.h"
#include "IIMage.h"

namespace jam
{

class CImagePVR : public IImage
{    
public:
    virtual ~CImagePVR();
    
    virtual bool            Load();
    virtual const TResourceData& RawData();
    virtual unsigned int    Width() const;
    virtual unsigned int    Height() const;
    virtual unsigned int    Bpp() const;
    virtual unsigned int    MipsCount() const;
	virtual unsigned int    IsCompressed() const;
    virtual TexelFormats    TexelFormat() const;
    virtual TexelTypes      TexelType() const;
    
    CImagePVR(const std::string& filename, const std::string& loader = "");

    
private:
    unsigned int            m_Width;
    unsigned int            m_Height;
    unsigned int            m_Mipmaps;
    TResourceData           m_Data;
    TexelProps              m_TexelProps;
    
    #pragma pack(push,4)
    struct PVRTHeader
    {
        unsigned int        version;		//Version of the file header, used to identify it.
        unsigned int        flags;			//Various format flags.
        unsigned long long	pixelformat;	//The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
        unsigned int        colourspace;	//The Colour Space of the texture, currently either linear RGB or sRGB.
        unsigned int        channeltype;	//Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
        unsigned int        height;			//Height of the texture.
        unsigned int        width;			//Width of the texture.
        unsigned int        depth;			//Depth of the texture. (Z-slices)
        unsigned int        numsurfaces;	//Number of members in a Texture Array.
        unsigned int        numfaces;		//Number of faces in a Cube Map. Maybe be a value other than 6.
        unsigned int        mipmapcount;	//Number of MIP Maps in the texture - NB: Includes top level.
        unsigned int        metasize;       //Size of the accompanying meta data.
    };
    #pragma pack(pop)
    
    static const size_t     s_PVRT_HEADER_SIZE = 52;
    
    static const std::map<unsigned long long, TexelProps> s_TexelProps;
};

}; // namespace jam

#endif	/* CIMAGEPVR_H */
