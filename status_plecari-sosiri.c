#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parse_status_plecari(xmlDocPtr doc, xmlNodePtr cur, char msg[])
{
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        xmlChar* content=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Plecare")))
        {
            strcat(msg, "Plecare: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Destinatie")))
        {
            strcat(msg, "Destinatie: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OraPlecare")))
        {
            strcat(msg, "OraPlecare: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OraSosire")))
        {
            strcat(msg, "OraSosire: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"IntarzierePlecare")))
        {
            strcat(msg, "InatrzierePlecare: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"IntarziereSosire")))
        {
            strcat(msg, "InatrziereSosire: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n\n");
        }
        cur = cur->next;
        xmlFree(content);
    }
    return;
}

void status_plecari(char *docname, char msg[], int ora, int min)
{

    xmlDocPtr doc;
    xmlNodePtr cur, cur1;

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
    int ora_plecare, min_plecare,gasit=0;
    char str[10];

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Tren")))
        {
            cur1 = cur->xmlChildrenNode;
            while (cur1 != NULL)
            {
                if (!xmlStrcmp(cur1->name, (const xmlChar *)"OraPlecare"))
                {
                    bzero(str, sizeof(str));
                    xmlChar* content=xmlNodeListGetString(doc, cur1->xmlChildrenNode, 1);
                    strcpy(str, (char*)content);
                    sscanf(str, "%d:%d", &ora_plecare, &min_plecare);
                    xmlFree(content);

                    if ((ora_plecare == ora + 1 && min_plecare <= min) || (ora_plecare == ora && min_plecare >= min))
                    {
                        gasit=1;
                        xmlChar *id = xmlGetProp(cur, BAD_CAST "id");
                        if (id != NULL)
                        {
                            strcat(msg, "id = ");
                            strcat(msg, (char *)id);
                            strcat(msg, "\n");
                            xmlFree(id);
                        }
                        parse_status_plecari(doc, cur, msg);
                    }
                    break;
                }
                cur1 = cur1->next;
            }
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);

    if(gasit==0)
    {
        strcat(msg,"Nu sunt trenuri care sa plece in urmatoarea ora!\n");
    }

    return;
}




void parse_status_sosiri(xmlDocPtr doc, xmlNodePtr cur, char msg[])
{

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        xmlChar* content=xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Plecare")))
        {
            strcat(msg, "Plecare: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Destinatie")))
        {
            strcat(msg, "Destinatie: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OraPlecare")))
        {
            strcat(msg, "OraPlecare: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OraSosire")))
        {
            strcat(msg, "OraSosire: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"IntarzierePlecare")))
        {
            strcat(msg, "InatrzierePlecare: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n");
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"IntarziereSosire")))
        {
            strcat(msg, "InatrziereSosire: ");
            strcat(msg, (char*)content);
            strcat(msg, "\n\n");
        }
        cur = cur->next;
        xmlFree(content);
    }
    return;
}

void status_sosiri(char *docname, char msg[], int ora, int min)
{

    xmlDocPtr doc;
    xmlNodePtr cur, cur1;

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
    int ora_plecare, min_plecare,gasit=0;
    char str[10];

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Tren")))
        {
            cur1 = cur->xmlChildrenNode;
            while (cur1 != NULL)
            {
                if (!xmlStrcmp(cur1->name, (const xmlChar *)"OraSosire"))
                {
                    bzero(str, sizeof(str));
                    xmlChar* content=xmlNodeListGetString(doc, cur1->xmlChildrenNode, 1);
                    strcpy(str, (char*)content);
                    sscanf(str, "%d:%d", &ora_plecare, &min_plecare);
                    xmlFree(content);

                    if ((ora_plecare == ora + 1 && min_plecare <= min) || (ora_plecare == ora && min_plecare >= min))
                    {
                        gasit=1;
                        xmlChar *id = xmlGetProp(cur, BAD_CAST "id");
                        if (id != NULL)
                        {
                            strcat(msg, "id = ");
                            strcat(msg, (char *)id);
                            strcat(msg, "\n");
                            xmlFree(id);
                        }
                        parse_status_sosiri(doc, cur, msg);
                    }
                    break;
                }
                cur1 = cur1->next;
            }
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);

    if(gasit==0)
    {
        strcat(msg,"Nu sunt trenuri care sa soseasca in urmatoarea ora!\n");
    }   

    return;
}