#include "item.h"

using namespace std;

void Item_Statement::set_attributes(const char **attr)
{
  map< string, string > attributes;
  
  attributes["set"] = "_";
  
  eval_cstr_array(get_name(), attributes, attr);
  
  output = attributes["set"];
}

void Item_Statement::forecast()
{
/*  declare_union_set(output);
  
  finish_statement_forecast();
  
  display_full();
  display_state();*/
}
