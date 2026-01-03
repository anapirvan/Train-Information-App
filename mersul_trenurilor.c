#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parse_mersul_trenurilor(xmlDocPtr doc, xmlNodePtr cur,char msg[])
{

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        xmlChar* content=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Plecare")))
        {
            strcat(msg,"Plecare: ");
            strcat(msg,(char*)content);
            strcat(msg,"\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Destinatie")))
        {
            strcat(msg,"Destinatie: ");
            strcat(msg,(char*)content);
            strcat(msg,"\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OraPlecare")))
        {
            strcat(msg,"OraPlecare: ");
            strcat(msg,(char*)content);
            strcat(msg,"\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OraSosire")))
        {
            strcat(msg,"OraSosire: ");
            strcat(msg,(char*)content);
            strcat(msg,"\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"IntarzierePlecare")))
        {
            strcat(msg,"InatrzierePlecare: ");
            strcat(msg,(char*)content);
            strcat(msg,"\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"IntarziereSosire")))
        {
            strcat(msg,"InatrziereSosire: ");
            strcat(msg,(char*)content);
            strcat(msg,"\n\n");
        }

        cur = cur->next;
        xmlFree(content);
    }
}

void mersul_trenurilor(char *docname,char msg[])
{

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(docname);
 
    if (doc == NULL)
    {
        fprintf(stderr, "Document not parsed successfully. \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *)"MersulTrenurilor"))
    {
        fprintf(stderr, "document of the wrong type, root node != parole");
        xmlFreeDoc(doc);
        return;
    }

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Tren")))
        {

            xmlChar *id = xmlGetProp(cur, BAD_CAST "id");
            if (id != NULL)
            {
                strcat(msg,"id = ");
                strcat(msg,(char *)id);
                strcat(msg,"\n");
                xmlFree(id);
            }

            parse_mersul_trenurilor(doc, cur,msg);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
}

/*int main()
{
    int x=parseDoc("log.xml","maria","000");
    printf("%d\n",x);
}*/