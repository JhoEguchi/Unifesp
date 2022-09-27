#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

#define SIZE 211
#define SHIFT 4

static int hash ( char * name, char * scope)
{ int temp = 0;
  int i = 0;
  while (name[i] != '\0')
  { temp = ((temp << SHIFT) + name[i]) % SIZE;
    ++i;
  }
  i=0;
  while (scope[i] != '\0')
  { temp = ((temp << SHIFT) + scope[i]) % SIZE;
    ++i;
  }
  return temp;
}

typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ;
     char *scope;
     char *IDtype;
     char *DATAtype;
     struct BucketListRec * next;
   } * BucketList;

static BucketList hashTable[SIZE];

void st_insert( char *name, int lineno, int loc, char *scope, char *dtype, char *itype )
{ int h = hash(name, scope);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL)
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->scope = scope;
    l->DATAtype = dtype;
    l->IDtype = itype;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
}


int st_lookup ( char * name, char * scope)
{ int h = hash(name, scope);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}

char * st_lookup_type (char * name, char * scope){
  int h = hash(name, scope);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return "NULL";
  else return l->DATAtype;
}

char * st_lookup_typeID (char * name, char * scope){
  int h = hash(name, scope);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return "NULL";
  else return l->IDtype;
}

void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Local   Nome da variável  Escopo        Tipo ID  Tipo Dado  Linhas \n");
  fprintf(listing,"------  ----------------  -----         -------  ---------  ------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-9d", l->memloc);
        fprintf(listing,"%-17s", l->name);
        fprintf(listing,"%-14s", l->scope);
        fprintf(listing,"%-10s", l->IDtype);
        fprintf(listing,"%-12s", l->DATAtype);
        while (t != NULL)
        { fprintf(listing,"%d, ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} 
