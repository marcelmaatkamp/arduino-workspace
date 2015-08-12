#include <ArrayList.h>

void setup(){
  ArrayList *list = new ArrayList("Hello World");
  list->add_string_item("From my Little World");
  list->add_string_item("of Arduino");
  list->add_string_item("Added Portion #1");   
  list->add_string_item("Added Portion #2");   
  list->add_string_item("Added Portion #3");   
  list->display_string_list();  
  Serial.println("-");
  delay(10);
  list->remove_selected_item(4);
  list->display_string_list();
  Serial.println("-");
  delay(10);
  list->empty_list();
  list->set_string_item("Hello Empty World",0);
  list->display_string_list();
  delete list;
}

void loop(){
}
