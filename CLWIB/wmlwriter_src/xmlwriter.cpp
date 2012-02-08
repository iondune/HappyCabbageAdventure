// xmlwriter.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "xmlwriter.h"
#include <stdarg.h>

xmlwriter::xmlwriter(string sTmp)
{
    sXmlFile = sTmp;
    fp = NULL;
    iLevel = 0;
    fp = fopen(sXmlFile.c_str(),"w");
    if(fp == NULL)
    {
		std::cout<<"Unable to open output file";
		return;
  	}
	else
	{
		fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\" \?>");
	}
}

xmlwriter::~xmlwriter()
{
    if(fp != NULL)
        fclose(fp);
    vectAttrData.clear();
}


void xmlwriter::Createtag(string sTag)
{
	fprintf(fp,"\n");
	//Indent properly
	for(int iTmp =0;iTmp<iLevel;iTmp++)
		fprintf(fp,"\t");
	fprintf(fp,"<%s",sTag.c_str());
	//Add Attributes
	while(0 < vectAttrData.size()/2)
	{
		string sTmp = vectAttrData.back();
		fprintf(fp," %s=", sTmp.c_str());
		vectAttrData.pop_back();
		sTmp = vectAttrData.back();
		fprintf(fp,"\"%s\"", sTmp.c_str());
		vectAttrData.pop_back();
	}
	vectAttrData.clear();
	fprintf(fp,">");
	sTagStack.push(sTag);
	iLevel++;

}
void xmlwriter::CloseLasttag()
{
	fprintf(fp,"\n");
	iLevel--;
    //Indent properly
	for(int iTmp =0;iTmp<iLevel;iTmp++)
		fprintf(fp,"\t");
	fprintf(fp,"</%s>",sTagStack.top().c_str());
	sTagStack.pop();//pop out the last tag
	return;
}

void xmlwriter::CloseAlltags()
{
	while(sTagStack.size() != 0)
	{
	   fprintf(fp,"\n");
	   iLevel--;
        //Indent properly
	   for(int iTmp =0;iTmp<iLevel;iTmp++)
	       fprintf(fp,"\t");
	   fprintf(fp,"</%s>",sTagStack.top().c_str());
	   sTagStack.pop();//pop out the last tag
    }
	return;
}
void xmlwriter::CreateChild(string sTag,string sValue)
{
	fprintf(fp,"\n");
	//Indent properly
	for(int iTmp =0;iTmp<iLevel;iTmp++)
		fprintf(fp,"\t");
	fprintf(fp,"<%s",sTag.c_str());
	//Add Attributes
	while(0 < vectAttrData.size()/2)
	{
		string sTmp = vectAttrData.back();
		fprintf(fp," %s=", sTmp.c_str());
		vectAttrData.pop_back();
		sTmp = vectAttrData.back();
		fprintf(fp,"\"%s\"", sTmp.c_str());
		vectAttrData.pop_back();
	}
	vectAttrData.clear();
	//add value and close tag
	fprintf(fp,">%s</%s>",sValue.c_str(),sTag.c_str());
}

void xmlwriter::AddAtributes(string sKey, string sVal)
{
	vectAttrData.push_back(sVal);
	vectAttrData.push_back(sKey);
}


void xmlwriter::AddComment(string sComment)
{
	fprintf(fp,"\n");
	//Indent properly
	for(int iTmp =0;iTmp<iLevel;iTmp++)
		fprintf(fp,"\t");
	fprintf(fp,"<!--%s-->",sComment.c_str());
}
