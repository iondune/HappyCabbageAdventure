



#include "xmlwriter.h"


int main(int argc, char* argv[])
{
	xmlwriter MyXml("boby.xml");
	MyXml.AddAtributes("age","25");	
	MyXml.AddAtributes("Profession","Software");	
	MyXml.Createtag("Boby");
	
	MyXml.AddComment("Personal information");
	MyXml.Createtag("Home"); // create node boby
	MyXml.CreateChild("Address","Pazheparampil"); // add element
	MyXml.CreateChild("Mobile","09844400873");
	MyXml.CloseLasttag(); // close boby


	MyXml.AddComment("Office information");
	MyXml.Createtag("Office"); // create node boby
	MyXml.CreateChild("Address","Bangalore"); // add element
	MyXml.CreateChild("Ph","0091234567");
	MyXml.CloseLasttag(); // close Office
	
	
	MyXml.CloseAlltags();

	cout<<"See boby.xml for output";

	return 0;
}

