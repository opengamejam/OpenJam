//
//  IGameObject.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef ILOADER_H
#define	ILOADER_H

#include "Global.h"

namespace jam
{

class ILoader
{
public:
	ILoader() = default;
	virtual ~ILoader() = default;

	virtual bool	Map(const std::string& url, std::vector<unsigned char>& data) = 0;
	
	static ILoader* GetLoader(const std::string& loader);
	static void     RegisterLoader(const std::string& loaderName, ILoader* loader);
	static void     RegisterDefaultLoader(ILoader* defaultLoader);
    static ILoader* DefaultLoader();

private:
	static std::map<std::string, ILoader*> m_Loaders;
	static ILoader* m_DefaultLoader;
};

}; // namespace jam

#endif	/* ILOADER_H */