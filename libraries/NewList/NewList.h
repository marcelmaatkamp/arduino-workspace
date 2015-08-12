/* NewList.h -- Versión 1.0

   Librería para usar listas doblemente enlazadas en Arduino (1.0.5 o superior).
   Creada por: José Alfonso Suárez Moreno (joseasuarez@gmail.com).
   Fecha: 21.ABR.2014

   Uso:
   NewList<tipo> lista; // <tipo> es el tipo de dato a usar en la lista. Desde un
                           tipo primitivo como int, char, long... hasta un objeto
                           o una estructura. Los símbolos <> son necesarios.

  Métodos:
  ~NewList()          ->  Destructor. 
  add()               ->  Añade un nuevo elemento al final de la lista.
  insert()            ->  Inserta un nuevo elemento delante del actual.
  goFirst()           ->  Se posiciona al principio de la lista.
  goLast()            ->  Se posiciona al final de la lista.
  goTo(int n)         ->  Se posiciona en la posición n. La primera posición es 0.Si el valor es 
                          mayor que el número de elementos-1 se posiciona en el último.
  search(tipo v)      ->  Busca el valor v en la lista. Debe ser del mismo tipo que el declarado
                          al crear el objeto. Devuelve el valor. Si no lo encuentra se posiciona en
                          el último elemento.
  goPrevious()        ->  Se posiciona en el elemento anterior al actual.
  goNext()            ->  Se posiciona en el elemento siguiente al actual.
  isFirst()           ->  Indica si está posicionado en el primer elemento. (true/false).
  isLast()            ->  Indica si está posicionado en el último elemento. (true/false).
  isEmpty()           ->  Indica si la lista esta vacía. (true/false).
  getCurrent()        ->  Devuelve el elemento actual.
  get(int n)          ->  Devuelve el elemento n. La primera posición es 0. Si el valor es mayor que 
                          el número de elementos-1 se posiciona en el último.
  hasPrevious()       ->  Indica si hay un elemento dealante del actual. (true/false).
  hasNext()           ->  Indica si hay un elemento detrás del actual.

  TO-DO:
  erase(int n)        ->  Eliminar el elemento n.
  eraseCurrent()      ->  Eliminar el elemento actual.
  eraseValue(tipo v)  ->  Eliminar el elemento que tenga el valor v (del mismo tipo que el
                          especificado al crear la lista).
*/

#ifndef NewList_H
#define NewList_H

#include "Arduino.h"

template<class uType> class NewList;

template<class uType>
class objNode {
   public:
    objNode(uType v, objNode<uType> *nxt = NULL, objNode<uType> *prv = NULL) :
       value(v), next(nxt), previous(prv) {}

   private:
    uType value;
    objNode<uType> *next;
    objNode<uType> *previous;

   friend class NewList<uType>;
};

template<class uType>
class NewList {
   public:
    NewList() : current(NULL) {}
    ~NewList();

    void add(uType v);
    void insert(uType v);
    void erase(uType v);
    void goNext();
    void goPrevious();
    bool goFirst();
    bool goLast();
    bool goTo(int n);
    bool search(uType v);
    uType getCurrent() { return current->value; }
    uType get(int n);
    bool isEmpty() { return current == NULL; }
    bool isFirst();
    bool isLast();
    bool hasPrevious() { return current->previous != NULL; }
    bool hasNext() { return current->next != NULL; }

   private:
    objNode<uType> *current;
};
#endif

template<class uType>
NewList<uType>::~NewList(){

   objNode<uType> *aux;
   goFirst();
   while(current->next) {
      aux = current;
      current = aux->next;
      delete aux;
   }
   if(!current->next){
      delete current;
   }               
}

template<class uType>
void NewList<uType>::add(uType v){
   objNode<uType> *newObj;

   if(!isEmpty()){
      goFirst();
   }
   if(isEmpty()) {
      newObj = new objNode<uType>(v, NULL, NULL);
   }
   else {
      goLast();
      newObj = new objNode<uType>(v, current->next, current);
      current->next = newObj;
   }
   current = newObj; 
}

template<class uType>
void NewList<uType>::insert(uType v){
   objNode<uType> *newObj;

   if(isEmpty()) {
      newObj = new objNode<uType>(v, NULL, NULL);
   }
   else {
      if( isFirst() ){
         newObj = new objNode<uType>(v, current, NULL);
         current->previous->next = newObj;
      }else{
         newObj = new objNode<uType>(v, current, current->previous);
         current->previous->next = newObj;
         current->previous = newObj;
      }
   }
   current=newObj;
}

template<class uType>
void NewList<uType>::erase(uType v){
}

template<class uType>
void NewList<uType>::goNext(){
   if(current->next){
      current = current->next;
   }
}

template<class uType>
void NewList<uType>::goPrevious(){
   if(current->previous){
      current = current->previous;
   }
}

template<class uType>
bool NewList<uType>::goFirst(){
   bool ok = false;
   if(!current->previous){
      ok=true;
   }else{
      while(current->previous){
         current=current->previous;
      }
      ok=true;
   }
   return ok;
}

template<class uType>
bool NewList<uType>::goLast(){
   bool ok = false;
   if(!current->next){
      ok=true;
   }else{
      while(current->next){
         current=current->next;
      }
      ok=true;
   }
   return ok;
}


template<class uType>
bool NewList<uType>::goTo(int n){
   int i=0;
   goFirst();
   while(i<n){
      i++;
      goNext();
   }
   return (i==n);
}

template<class uType>
bool NewList<uType>::search(uType v){ 
   goFirst();
   while(current->value!=v && current->next){
      goNext();
   }
   return (current->value==v);
}

template<class uType>
uType NewList<uType>::get(int n){ 
   goTo(n);
   return current->value;
}

template<class uType>
bool NewList<uType>::isFirst(){ 
   return (current->previous==NULL);
}

template<class uType>
bool NewList<uType>::isLast(){
   return (current->next==NULL);
}

