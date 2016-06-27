//
//  CMemoryFileInfo.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CMemoryFileInfo.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMemoryFileInfo::CMemoryFileInfo()
{
    
}

CMemoryFileInfo::~CMemoryFileInfo()
{
    
}

CMemoryFileInfo::CMemoryFileInfo(const std::string& filePath, bool isDir)
{
    std::size_t found = filePath.rfind("/");
    if (found != std::string::npos)
    {
        const std::string basePath = filePath.substr(0, found + 1);
        std::string fileName;
        if (found != filePath.length())
        {
            fileName = filePath.substr(found + 1, filePath.length() - found - 1);
        }
        
        Initialize(basePath, fileName, isDir);
    }
}

const std::string& CMemoryFileInfo::Name() const
{
    return m_Name;
}

const std::string& CMemoryFileInfo::BaseName() const
{
    return m_BaseName;
}

const std::string& CMemoryFileInfo::Extension() const
{
    return m_Extension;
}

const std::string& CMemoryFileInfo::AbsolutePath() const
{
    return m_AbsolutePath;
}

const std::string& CMemoryFileInfo::BasePath() const
{
    return m_BasePath;
}

bool CMemoryFileInfo::IsDir() const
{
    return m_IsDir;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CMemoryFileInfo::Initialize(const std::string& basePath, const std::string& fileName, bool isDir)
{
    m_BasePath = basePath;
    m_Name = fileName;
    m_IsDir = isDir;
    
    if (!StringEndsWith(m_BasePath, "/"))
    {
        m_BasePath += "/";
    }
    
    m_AbsolutePath = m_BasePath + m_Name;
    
    size_t dotsNum = std::count(m_Name.begin(), m_Name.end(), '.');
    bool isDotOrDotDot = (dotsNum == m_Name.length() && isDir);
    
    if (!isDotOrDotDot)
    {
        std::size_t found = m_Name.rfind(".");
        if (found != std::string::npos)
        {
            m_BaseName = m_Name.substr(0, found);
            if (found < m_Name.length())
            {
                m_Extension = m_Name.substr(found, m_Name.length() - found);
            }
        }
    }
}
