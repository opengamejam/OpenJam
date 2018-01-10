/* 
 * File:   CImagePVR.h
 * Author: yevgeniy.logachev
 */

#ifndef CIMAGEPVR_H
#define CIMAGEPVR_H

#include "Global.h"
#include "IIMage.h"

namespace jam {

class CImagePVR : public IImage {
    JAM_OBJECT
public:
    CImagePVR(const std::string& filename);
    virtual ~CImagePVR();

    virtual bool Load() override;
    virtual const TResourceData& RawData() override;
    virtual uint32_t Width() const override;
    virtual uint32_t Height() const override;
    virtual uint32_t Bpp() const override;
    virtual uint32_t MipsCount() const override;
    virtual bool IsCompressed() const override;
    virtual TexelFormats TexelFormat() const override;
    virtual TexelTypes TexelType() const override;

private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Mipmaps;
    TResourceData m_Data;
    TexelProps m_TexelProps;

#pragma pack(push, 4)
    struct PVRTHeader {
        uint32_t version; // Version of the file header, used to identify it.
        uint32_t flags; // Various format flags.
        uint64_t pixelformat; // The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
        uint32_t colourspace; // The Colour Space of the texture, currently either linear RGB or sRGB.
        uint32_t channeltype; // Variable type that the channel is stored in.
        uint32_t height; // Height of the texture.
        uint32_t width; // Width of the texture.
        uint32_t depth; // Depth of the texture. (Z-slices)
        uint32_t numsurfaces; // Number of members in a Texture Array.
        uint32_t numfaces; // Number of faces in a Cube Map. Maybe be a value other than 6.
        uint32_t mipmapcount; // Number of MIP Maps in the texture - NB: Includes top level.
        uint32_t metasize; // Size of the accompanying meta data.
    };
#pragma pack(pop)

    static const size_t s_PVRT_HEADER_SIZE;
    static const std::map<uint64_t, TexelProps> s_TexelProps;
};

}; // namespace jam

#endif /* CIMAGEPVR_H */
