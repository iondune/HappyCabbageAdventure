// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __CABBAGE_CORE_CXMLWRITER_H_INCLUDED__
#define __CABBAGE_CORE_CXMLWRITER_H_INCLUDED__

#include <string>
#include <vector>
#include <cstdio>


namespace Cabbage
{
namespace Core
{

	//! Interface providing methods for making it easier to write XML files.
	class CXMLWriter
	{
	public:

		//! Constructor
		CXMLWriter(std::string const & fileName);

		//! Destructor
		virtual ~CXMLWriter();

		//! Writes a xml 1.0 header like <?xml version="1.0"?>
		virtual void writeXMLHeader();

		//! Writes an xml element with maximal 5 attributes
		virtual void writeElement(const char* name, bool empty=false,
			const char* attr1Name = 0, const char* attr1Value = 0,
			const char* attr2Name = 0, const char* attr2Value = 0,
			const char* attr3Name = 0, const char* attr3Value = 0,
			const char* attr4Name = 0, const char* attr4Value = 0,
			const char* attr5Name = 0, const char* attr5Value = 0);

		//! Writes an xml element with any number of attributes
		virtual void writeElement(const char* name, bool empty,
				std::vector<std::string> &names, std::vector<std::string> &values);

		//! Writes a comment into the xml file
		virtual void writeComment(const char* comment);

		//! Writes the closing tag for an element. Like </foo>
		virtual void writeClosingTag(const char* name);

		//! Writes a text into the file. All occurrences of special characters like
		//! & (&amp;), < (&lt;), > (&gt;), and " (&quot;) are automaticly replaced.
		virtual void writeText(const char* text);

		//! Writes a line break
		virtual void writeLineBreak();

		struct XMLSpecialCharacters
		{
			char Character;
			const char* Symbol;
		};

	private:

		void writeAttribute(const char* att, const char* name);

		int Tabs;
		FILE * File;

		bool TextWrittenLast;

	};

}
}

#endif

