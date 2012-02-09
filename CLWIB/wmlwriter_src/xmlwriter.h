#ifndef xmlwriter_h
#define xmlwriter_h

#include<iostream>
#include <stdio.h>
#include<string>
#include<vector>
#include <stack>
using namespace std;
typedef stack<string> StackStrings;


class xmlwriter{
public:
	xmlwriter(string sTmp);
	~xmlwriter();
	void CreateChild(string sTag,string sValue);
	void Createtag(string sTag);
	void CloseLasttag();
	void CloseAlltags();
	void AddAtributes(string sAttrName, string sAttrvalue);
	void AddComment(string sComment);
private:
	string sXmlFile;
	vector<string> vectAttrData;
	FILE *fp;
	int iLevel;
	StackStrings sTagStack;
};

#endif // xmlwriter_h


