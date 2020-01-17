#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>


#define prl { printf("\n"); }

#define length 1000
#define file "xml.txt"


typedef struct xattribute_t xattribute_t;
typedef struct xelement_t xelement_t;
typedef union contenu contenu;


struct xattribute_t {
	char *nom;
	char *valeur;
	xattribute_t *next;
};

struct  xelement_t {
	char *nom ;
	xattribute_t *liste;
	xelement_t *pere;  
	xelement_t *frere;  //next
	union contenu {
		char *raw ;
		xelement_t *fils;
	} *contenu;
};


xelement_t *create_xelement (const char *name) {
	xelement_t *my_xml =  (xelement_t*)malloc(sizeof(xelement_t));
	my_xml->nom = malloc(sizeof(strlen(name)+1));
	strcpy (my_xml->nom, name);
	my_xml->liste = NULL;
	my_xml->pere = NULL;
	my_xml->frere = NULL;
	my_xml->contenu = NULL;
	return my_xml;
}

void add_xattribute (xelement_t* e, const char *name, const char *value) {
	xattribute_t *my_xml = (xattribute_t*)malloc(sizeof(xattribute_t));
	my_xml->nom = malloc(sizeof(strlen(name)+1));
	my_xml->valeur = malloc(sizeof(strlen(value)+1));
	strcpy (my_xml->nom, name);
	strcpy (my_xml->valeur, value);
	my_xml->next = NULL;
	//Ajouter au queue 
	xattribute_t *c = e->liste;
	xattribute_t *p = NULL;
	while (c != NULL) {
		p = c;
		c = c->next;
	}
	if (p != NULL)
		p->next = my_xml;
	else
		e->liste = my_xml;
}
//Ajouter S dans une branch de e
void add_sub_xelement (xelement_t *e, xelement_t *s) {
	if (e->pere != NULL) return;
	else {
		e->contenu->fils = malloc(sizeof(xelement_t));
		e->contenu->fils->liste = malloc(sizeof(xattribute_t));
		e->contenu->fils->liste->nom = malloc(sizeof(strlen(s->liste->nom))+1);
		e->contenu = malloc(sizeof(contenu));
		s->pere = e;
		//ajouter au queue
		xelement_t *c = e->contenu->fils;
		xelement_t *p = NULL;
		while (c != NULL) {
			p = c;
			c = c->frere;
		}
		if (p != NULL) p->frere = s;
		else e->contenu->fils = s;
		printf("%s\n",e->contenu->fils->liste->nom);
	}
}

void add_raw (xelement_t *e, const char *r) {
	e->contenu = (contenu*)malloc(sizeof(contenu));
	e->contenu->raw = malloc(sizeof(strlen(r)+1));
	strcpy (e->contenu->raw, r);
}

void delete_xelement(xelement_t *e) {
	free (e);
	free (e->pere);
	free (e->frere);
	free (e->contenu);
	free (e->contenu->fils);
}

// Input - OUtpt
void save_xelement (FILE *fd, xelement_t *e) {
	//note element
	if (e == NULL) printf("IL Y A DES ERRORS \n");
	else { 
		e->contenu = (contenu*)malloc(sizeof(contenu));
		if (e->liste != NULL) {
			fprintf (fd, "<%s ", e->nom);
			xattribute_t *c = e->liste;
			if (c-> next == NULL) fprintf(fd, "%s = \"%s\">",c->nom, c->valeur);
			else {
				while (c->next != NULL) {
					fprintf(fd, "%s = %s /> ",c->next->nom, c->next->valeur);
					c->next = c->next->next;
				}
			}
			if (e->pere != NULL) 
				save_xelement(fd, e->pere);
			if (e->frere != NULL) 
				save_xelement (fd, e->frere);
			if(e->contenu->raw == NULL) 
				return;
			else 
				fprintf(fd, " %s ",e->contenu->raw);
		}
		else if (e->liste == NULL ) {
			fprintf (fd, "<%s> ", e->nom);
			if (e->pere != NULL) 
				save_xelement(fd, e->pere);
			if (e->frere != NULL) 
				save_xelement (fd, e->frere);
			if(e->contenu->raw == NULL)
			       	return;
			else 
				fprintf(fd, " %s ",e->contenu->raw);
			fprintf (fd, "</%s> ", e->nom);
		} 
	}
}


void save_xml (const char *fname, xelement_t *e) {
	//OUVRIR LES FICHIER ECRIT
	FILE *flop = NULL;
	flop = fopen (fname,"w");
	if (fname == NULL) printf("\nFAILURE"); 
	else {
		save_xelement(flop, e);
		printf("\nREUSSIT\n");
		fclose(flop);
	}
}

void print_xelement(xelement_t* e) {
	save_xelement(stdout, e);
}


//Import

char next_char (FILE *fd) {
	if (fd == NULL) exit(1);
	else {
		char car = fgetc(fd);
		fseek(fd, 0L, SEEK_SET);
		do	{
			if (car == '\'') exit(1);
			else if (car == '\n') exit(1);
			else if (car == '\r') exit(1);
			else if (car == '\t') exit(1);
			else return car; break;
		}while (car != EOF) ;
	}
}


void check_next_char (FILE *fd, char c) {
	if ( c != next_char(fd)) return;
}

bool is_next_char (FILE *fd, char c, bool cons) {
	if (next_char(fd) == c) return true;
	else cons = false; ungetc (next_char(fd), fd); return false;
}

//renvoie nom quand rencontrer <>/=
char *next_word (FILE *fd) {
	if (fd == NULL) exit(1);
	else {
		int i = 0;
		int dim = 0;
		char str[dim];
		char car;
		car = fgetc(fd);
		fseek(fd, 0L, SEEK_SET);
		while (car != EOF) {
			if (car == '<') {i++; str[dim++] = car;}
			else if (car == '>') {i++; str[dim++] = car;}
			else if (car == '/') {i++; str[dim++] = car;}
			else if (car == '=') {i++; str[dim++] = car;}
			else {str[dim++] = car;}
			if (i == 2) ungetc(car, fd); break;
		}
		str[dim++] = '\0';
		char *str1 = str;
		return str1;
	}
}


void check_next_word (FILE *fd, const char *w) {
	int c = strcmp(next_word(fd),w);
	if (c != 0) return;
}


char *next_string (FILE *fd) {
	if (fd == NULL) exit(1);
	else {
		int i = 0;
		int dim = 0;
		char str[dim];
		char c;
		c = fgetc(fd);
		fseek(fd, 0L, SEEK_SET);
		while (c != EOF) {
			if (c == '\'') {i++; str[dim++] = c;}
			else {str[dim++] = c;}
			if (i == 3)  break;
		}
		str[dim++] = '\0';
		char *str1 = str;
		return str1;
	}
}


char *next_raw (FILE *fd) {
	if (fd == NULL) exit(1);
	else {
		int i = 0;
		int dim = 0;
		char str[dim];
		char c;
		c = fgetc(fd);
		fseek(fd, 0L, SEEK_SET);
		while (c != EOF) {
			if (c == '<') {i++; str[dim++] = c;}
			else {str[dim++] = c;}
			if (i == 2)  break;
		}
		str[dim++] = '\0';
		char *str1 = str;
		return str1;
	}
}


// IMPORT
xelement_t* load_xelement(FILE* fd, const char* end_tag);

void load_xelement_raw(FILE* fd, xelement_t* e) {
	char* w = next_raw(fd);
	check_next_char(fd, '<');
	check_next_char(fd, '/');
	check_next_word(fd, e->nom);
	check_next_char(fd, '>');
	add_raw(e,w);
	free(w);
}

void load_xelement_sub(FILE* fd, xelement_t* e) {
	xelement_t* f = load_xelement(fd, e->nom);
	if (f != NULL) {
		add_sub_xelement(e,f);
		load_xelement_sub(fd, e);
	}
}

void load_xelement_content(FILE* fd, xelement_t* e) {
	if (is_next_char(fd,'<',false))
		load_xelement_sub(fd, e);
	else
		load_xelement_raw(fd, e);
}

xelement_t* load_xelement(FILE* fd, const char* end_tag) {
	xelement_t* e = NULL;
	char c;

	check_next_char(fd,'<');

	if ((end_tag) && (is_next_char(fd,'/',true))) {
		check_next_word(fd,end_tag);
		check_next_char(fd,'>');
		return NULL;
	}

	char* name = next_word(fd);
	if (name == NULL) {
		fprintf(stderr, "load_xelement: tag name expected\n");
		exit(EXIT_FAILURE);
	}
	e = create_xelement(name);
	free(name);

	while((name = next_word(fd)) != NULL) {
		check_next_char(fd,'=');
		char* value = next_string(fd);
		add_xattribute(e,name,value);
	}

	c = next_char(fd);

	if (c == '/') {
		check_next_char(fd,'>');
		return e;
	}

	if (c == '>') {
		load_xelement_content(fd, e);
		return e;
	}

	fprintf(stderr, "load_xelement: end of markup expected ('>' or '/>'), but got %c\n", c);
	exit(EXIT_FAILURE);
}

xelement_t* load_xml(const char* fname) {
	FILE* fd = fopen(fname, "r");
	xelement_t* e = load_xelement(fd,NULL);
	fclose(fd);
	return e;
}


// ***************************************** TEST'S XML ***********************************//
void test_xml() {
	xelement_t *livre = create_xelement ("book");
	printf ("%s\t",livre->nom);
	add_xattribute(livre,"id","102");
	printf ("%s = %s\t",livre->liste->nom,livre->liste->valeur);
	add_xattribute(livre,"ii","101");
	printf ("%s = %s\n",livre->liste->next->nom,livre->liste->next->valeur);
	xelement_t *livre1 = create_xelement ("titre");

	printf ("%s\t",livre1->nom);
	add_raw (livre1,"ten phim");
	printf ("%s\n",livre1->contenu->raw);
	xelement_t *livre2 = create_xelement ("book");
	printf ("%s\t",livre2->nom);
	add_xattribute(livre2,"id","102");
	printf ("%s = %s\t",livre2->liste->nom,livre2->liste->valeur);
	
}

int main () {
	test_xml();
	FILE *fin ;
	fin = fopen(file,"w");
	xelement_t *catalog = create_xelement ("catalog");
	xelement_t *book = create_xelement ("book");
	add_xattribute (book,"id","bk101");
	xelement_t *author = create_xelement ("author");
	add_xattribute (author,"lastname","Gambardella");
	add_xattribute (author,"firstname","Matthew");

	print_xelement(catalog);
	save_xml (file,catalog);


	fclose (fin);

	return 0;
}
