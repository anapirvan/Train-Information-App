#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

int parseStory(xmlDocPtr doc, xmlNodePtr cur, char username[], char parola[])
{
    cur = cur->xmlChildrenNode;
    int nr = 0;
    while (cur != NULL)
    {
        xmlChar* content=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"username")))
        {
            if (xmlStrcmp(content, (const xmlChar*)username) == 0)
                nr++;
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"parola")))
        {
            if (xmlStrcmp(content, (const xmlChar*)parola) == 0)
                nr++;
        }

        if (nr == 2)
            return 1;
        cur = cur->next;
        xmlFree(content);
    }
    return 0;
}

int login(char *docname, char username[], char parola[])
{

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(docname);

    if (doc == NULL)
    {
        fprintf(stderr, "Document not parsed successfully. \n");
        return  -1;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return -1;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"parole"))
    {
        fprintf(stderr, "document of the wrong type, root node != parole");
        xmlFreeDoc(doc);
        return -1;
    }

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"persoana")))
        {
            if (parseStory(doc, cur, username, parola))
            {
                xmlFreeDoc(doc);
                return 1;
            }
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return 0;
}

/*int main()
{
    int x=parseDoc("log.xml","maria","000");
    printf("%d\n",x);
}*/