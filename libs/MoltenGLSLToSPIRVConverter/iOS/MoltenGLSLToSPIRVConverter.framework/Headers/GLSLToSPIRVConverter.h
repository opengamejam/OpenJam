/*
 * GLSLToSPIRVConverter.h
 *
 * Copyright (c) 2014-2017 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Use of this document is governed by the Molten License Agreement, as included
 * in the Molten distribution package. CAREFULLY READ THAT LICENSE AGREEMENT BEFORE
 * READING AND USING THIS DOCUMENT. BY READING OR OTHERWISE USING THIS DOCUMENT,
 * YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS AND CONDITIONS OF THAT LICENSE
 * AGREEMENT. IF YOU DO NOT ACCEPT THE TERMS AND CONDITIONS OF THAT LICENSE AGREEMENT,
 * DO NOT READ OR USE THIS DOCUMENT.
 */

#ifndef __GLSLToSPIRVConverter_h_
#define __GLSLToSPIRVConverter_h_ 1


#include "GLSLConversion.h"
#include <string>
#include <vector>


namespace molten {

#pragma mark -
#pragma mark GLSLToSPIRVConverter

	/** Converts GLSL code to SPIR-V code. */
	class GLSLToSPIRVConverter {

	public:

		/** Sets the GLSL source code that is to be converted to the specified null-terminated string. */
		void setGLSL(const std::string& glslSrc);

		/** Returns the GLSL source code that was set using the setGLSL() function. */
		const std::string& getGLSL();

		/**
		 * Converts GLSL code, set with setGLSL(), to SPIR-V code, which can be retrieved using getSPIRV().
		 *
		 * The boolean flags indicate whether the original GLSL code and resulting SPIR-V code should
		 * be logged to the result log of this converter. This can be useful during shader debugging.
		 */
		bool convert(MLNShaderStage shaderStage, bool shouldLogGLSL, bool shouldLogSPIRV);

		/** Returns the SPIRV code most recently converted by the convert() function. */
		const std::vector<uint32_t>& getSPIRV();

		/**
		 * Returns whether the most recent conversion was successful.
		 *
		 * The initial value of this property is NO. It is set to YES upon successful conversion.
		 */
		bool getWasConverted();

		/**
		 * Returns a human-readable log of the most recent conversion activity.
		 * This may be empty if the conversion was successful.
		 */
		const std::string& getResultLog();

	protected:
		void logMsg(const char* logMsg);
		bool logError(const char* errMsg);
		void logGLSL(const char* opDesc);
		void logSPIRV(const char* opDesc);
		bool validateSPIRV();
		void initGLSLCompilerResources();

		std::string _glsl;
		std::vector<uint32_t> _spirv;
		std::string _resultLog;
		bool _wasConverted = false;
	};

}

#endif
