#include <NewList.h>

void setup() {

  Serial.begin(115600);
  Serial.println("Prueba con NewList"); // NewList Test

  NewList<int> listaEnteros;
  Serial.println("Objero creado"); //Created object

  // Añadiendo al final de la lista.
  listaEnteros.add(1); //Adding to end of list.
  listaEnteros.add(2);
  listaEnteros.add(50);
  listaEnteros.add(32);

  Serial.println("");
  
  Serial.println("Desde el primero: "); //From first
  listaEnteros.goFirst();
  Serial.println(listaEnteros.getCurrent());
  while(listaEnteros.hasNext()){
    listaEnteros.goNext();
    Serial.println(listaEnteros.getCurrent());
  }

  Serial.println("");

  Serial.print("El de la segunda posicion: "); //Second possition
  Serial.println(listaEnteros.get(2));

  Serial.println("");
  
  Serial.print("El último: "); // Last
  listaEnteros.goLast();
  Serial.println(listaEnteros.getCurrent());

  Serial.println("");
  
  Serial.print("El primero: "); //First possition (start from 0)
  listaEnteros.goFirst();
  Serial.println(listaEnteros.getCurrent());

  Serial.println("");
  
  Serial.println("Desde el ultimo: ");//From last
  listaEnteros.goLast();
  Serial.println(listaEnteros.getCurrent());
  while(listaEnteros.hasPrevious()){
    listaEnteros.goPrevious();
    Serial.println(listaEnteros.getCurrent());
  }

  Serial.println("");
  
  // La última posición es la 3, como no existe el 4º se posiciona en el último.
  // The last possition is 3, than there are'n the 4, stay in last element.
  Serial.print("El de la cuarta posicion: ");
  Serial.println(listaEnteros.get(4));

  Serial.println("Se posiciona en el 2º elemento. (el nº 50)");
  listaEnteros.goTo(2);
  Serial.println("Insertar un 43 delante del 2º elemento.");
  listaEnteros.insert(43);

  Serial.println("Recorrer la lista para comprobar que el 43 está delante del 50.");
  Serial.println("Desde el primero: "); //From first
  listaEnteros.goFirst();
  Serial.println(listaEnteros.getCurrent());
  while(listaEnteros.hasNext()){
    listaEnteros.goNext();
    Serial.println(listaEnteros.getCurrent());
  }

  Serial.println("");

  Serial.println("Buscar el elemento de valor 50");
  listaEnteros.search(50);
  Serial.print("Lo encontré: ");
  Serial.println(listaEnteros.getCurrent());

  // Destructor.
  listaEnteros.~NewList();

  Serial.println("");
  Serial.println("Objeto destruido");

  listaEnteros.goFirst();
  Serial.println(listaEnteros.getCurrent());

}

void loop() {

}
