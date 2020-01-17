#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "menu.h"

//1
typedef int bool ;
#define false 0
#define true 1 

typedef struct menu_s menu_t;



typedef struct {
	char* name;
	void (*ptr_f) () ;
}action_t;

typedef union {
	action_t action;
	menu_t  *menu;
}choice_t;

typedef struct {
	choice_t choice ;
  
    int type;
}item_t;


struct menu_s{
 menu_t *ptr_dad;
char* name;
int pos;
item_t item[9] ;
};




//2
menu_t * createMenu(const char* text){
	menu_t mnu;
	mnu.name=(char*)malloc(100*sizeof(char));
	strcpy(mnu.name,text);
	mnu.ptr_dad=NULL;

	menu_t* m = malloc(sizeof(menu_t));
	if(m== NULL){
		printf("System error ! :/ \n");
		exit (EXIT_FAILURE) ;
	}
    mnu.pos=0;
	*m = mnu;

	return m;
}
//3
void addMenuAction(menu_t* m, const char* text, void(*f)()){
	
	if(m->pos <9){
	
	m->item[m->pos].choice.action.name=(char*)malloc(100*sizeof(char));
	strcpy(m->item[m->pos].choice.action.name,text);
	m->item[m->pos].choice.action.ptr_f=f ;

	m->item[m->pos].type=0;
	m->pos++;
}else{
	printf("not enough place!");
}

}
//4
void addSubMenu(menu_t* m, menu_t* sm){
	if(sm->ptr_dad == NULL && m != NULL && sm != NULL ){
		if(m->pos<9){
		
		sm->ptr_dad=m;

	    m->item[m->pos].choice.menu=sm;
        m->item[m->pos].type=1;
        m->pos++;
	   }else{
		printf("not enough place!");
	}
}
}
//5
void deleteMenu(menu_t* m){
    if(m == NULL){
    	free(m);
    }
	if(m->ptr_dad == NULL){
		for(int i=0;i<m->pos;i++){
        if(m->item[i].type == 1){
            m->ptr_dad=NULL;
        	deleteMenu(m->item[i].choice.menu);
        }
		}
		free(m);
	}
}
//6

void launchMenu(menu_t* m){
	
	printf("%s : \n",m->name);
	for(int i=1;i< m->pos+1;i++) {
		if(m->item[i-1].type==0){
		printf("%d -%s \n",i,m->item[i-1].choice.action.name);
	}else{
		printf("%d -%s \n",i,m->item[i-1].choice.menu->name);
	}
    
	}
	char a;
	do 
		scanf("%c",&a);
	while(a == '\n');
	if ( m->ptr_dad != NULL && a== 'p'){
		launchMenu(m->ptr_dad);
	}
	if(a>='1' && a<='9'){
	   if(m->item[a-'1'].type == 0){
		m->item[a-'1'].choice.action.ptr_f();
		
		
	  }else if ( m->item[a-'1'].type == 1){
		launchMenu(m->item[a-'1'].choice.menu);
		
	  }else{
		printf("there is no  item in this spot ! please enter another");
		scanf("%c",&a);
	  }
	} else {
     launchMenu(m);
	}
    
  }


void f1() {
  printf("Functionality 1 is called\n");
}
 
void f2() {
  printf("Functionality 2 is called\n");
}
 
bool cont = true;
 
void quit() {
  cont = false;
}
 


int main() {
  menu_t* m;
  menu_t* sm;
 
  m = createMenu("Main menu");
 
  addMenuAction(m,"Apply functionnality 1",f1);
  addMenuAction(m,"Apply functionnality 2",f2);
 
  sm = createMenu("First submenu");
  addMenuAction(sm,"Apply functionnality 2",f2);
  addSubMenu(m,sm);
 
  sm = createMenu("Second submenu");
  addMenuAction(sm,"Apply functionnality 1",f1);
  addSubMenu(m,sm);
 
  addMenuAction(m,"Quit",quit);
 
  while(cont) launchMenu(m);
 
  deleteMenu(m);
 
  return EXIT_SUCCESS;
}
