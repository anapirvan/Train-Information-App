#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parse_update_plecare(xmlNodePtr cur, char msg[], char id_cautat[], int intarziere)
{
    cur = cur->xmlChildrenNode;
    char str[10];

    while (cur != NULL)
    {
        if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp(cur->name, (const xmlChar *)"IntarzierePlecare"))
        {
            sprintf(str, "%d", intarziere);
            xmlNodeSetContent(cur, BAD_CAST str);
            sprintf(msg, "Am modificat intarzierea la plecare a trenului cu id-ul = %s\n", id_cautat);
            return;
        }
        cur = cur->next;
    }
}

void update_plecare(char *docname, char msg[], char id_cautat[], int intarziere)
{

    xmlDocPtr doc;
    xmlNodePtr cur;
    int gasit = 0;

    doc = xmlParseFile(docname);

    if (doc == NULL)
    {
        fprintf(stderr, "Document not parsed successfully. \n");
        xmlFreeDoc(doc);
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
        fprintf(stderr, "document of the wrong type, root node != story");
        xmlFreeDoc(doc);
        return;
    }

    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Tren")))
        {
            printf("ok\n");
            fflush(stdout);
            xmlChar *id = xmlGetProp(cur, BAD_CAST "id");
            if (id != NULL && !strcmp(id_cautat, (char *)id))
            {
                xmlFree(id);
                parse_update_plecare(cur, msg, id_cautat, intarziere);
                xmlSaveFormatFile(docname, doc, 0);
                gasit = 1;
                break;
            }
        }

        cur = cur->next;
    }

    if (!gasit)
        sprintf(msg, "Nu exista niciun tren cu acest id!\n");

    xmlFreeDoc(doc);
}

void parse_update_sosire(xmlNodePtr cur, char msg[], char id_cautat[], int intarziere)
{
    cur = cur->xmlChildrenNode;
    char str[10];

    while (cur != NULL)
    {
        if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp(cur->name, (const xmlChar *)"IntarziereSosire"))
        {
            sprintf(str, "%d", intarziere);
            xmlNodeSetContent(cur, BAD_CAST str);
            sprintf(msg, "Am modificat intarzierea la sosire a trenului cu id-ul = %s\n", id_cautat);
            return;
        }
        cur = cur->next;
    }
}

void update_sosire(char *docname, char msg[], char id_cautat[], int intarziere)
{

    xmlDocPtr doc;
    xmlNodePtr cur;
    int gasit = 0;

    doc = xmlParseFile(docname);

    if (doc == NULL)
    {
        fprintf(stderr, "Document not parsed successfully. \n");
        xmlFreeDoc(doc);
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
        fprintf(stderr, "document of the wrong type, root node != story");
        xmlFreeDoc(doc);
        return;
    }

    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Tren")))
        {
            xmlChar *id = xmlGetProp(cur, BAD_CAST "id");
            if (id != NULL && !strcmp(id_cautat, (char *)id))
            {
                xmlFree(id);
                parse_update_sosire(cur, msg, id_cautat, intarziere);
                xmlSaveFormatFile(docname, doc, 0);
                gasit = 1;
                break;
            }
        }

        cur = cur->next;
    }

    if (!gasit)
        sprintf(msg, "Nu exista niciun tren cu acest id!\n");

    xmlFreeDoc(doc);
}