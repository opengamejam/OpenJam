//
//  ILoader.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#include "ILoader.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

class CDummyLoader : public ILoader
{
public:
	CDummyLoader()
		: ILoader()
	{}
	virtual ~CDummyLoader()
	{}

	virtual bool Map(const std::string& url, std::vector<unsigned char>& data) { return false; }

	static CDummyLoader& Instance()
	{
		static CDummyLoader loader;
		return loader;
	}
};


std::map<std::string, ILoader*> ILoader::m_Loaders;
ILoader* ILoader::m_DefaultLoader = &CDummyLoader::Instance();


ILoader* ILoader::GetLoader(const std::string& loader)
{
    if (m_Loaders.find(loader) != m_Loaders.end())
    {
        return m_Loaders[loader];
    }
    return m_DefaultLoader;
}

void ILoader::RegisterLoader(const std::string& loaderName, ILoader* loader)
{
    if (m_Loaders.find(loaderName) == m_Loaders.end())
    {
        m_Loaders[loaderName] = loader;
    }
}

void ILoader::RegisterDefaultLoader(ILoader* defaultLoader)
{
    m_DefaultLoader = defaultLoader;
}

ILoader* ILoader::DefaultLoader()
{
    return m_DefaultLoader;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************