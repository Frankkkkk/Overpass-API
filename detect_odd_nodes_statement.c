#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "expat_justparse_interface.h"
#include "script_datatypes.h"
#include "script_queries.h"
#include "script_tools.h"
#include "detect_odd_nodes_statement.h"

#include <mysql.h>

using namespace std;

void Detect_Odd_Nodes_Statement::set_attributes(const char **attr)
{
  map< string, string > attributes;
  
  attributes["from"] = "_";
  attributes["into"] = "_";
  
  eval_cstr_array(get_name(), attributes, attr);
  
  input = attributes["from"];
  output = attributes["into"];
}

void Detect_Odd_Nodes_Statement::execute(MYSQL* mysql, map< string, Set >& maps)
{
  set< Node > nodes;
  set< Way > ways;
  set< Relation > relations;
  set< Area > areas;
  if (input == output)
  {
    nodes = maps[output].get_nodes();
    ways = maps[output].get_ways();
    relations = maps[output].get_relations();
    areas = maps[output].get_areas();
  }
  
  map< string, Set >::const_iterator mit(maps.find(input));
  if (mit == maps.end())
  {
    maps[output] = Set(nodes, ways, relations, areas);
    return;
  }
  const set< Node >& in_nodes(mit->second.get_nodes());
  
  set< int > node_parity_control;
  for (set< Way >::const_iterator it(mit->second.get_ways().begin());
       it != mit->second.get_ways().end(); ++it)
  {
    vector< int >::const_iterator iit(it->members.begin());
    if (iit != it->members.end())
    {
      if (node_parity_control.find(*iit) != node_parity_control.end())
	node_parity_control.erase(*iit);
      else
	node_parity_control.insert(*iit);
      
      iit = --(it->members.end());
      if (node_parity_control.find(*iit) != node_parity_control.end())
	node_parity_control.erase(*iit);
      else
	node_parity_control.insert(*iit);
    }
  }
  if (node_parity_control.size() > 0)
  {
    for (set< int >::const_iterator it(node_parity_control.begin());
	 it != node_parity_control.end(); ++it)
    {
      set< Node >::const_iterator nit(in_nodes.find(Node(*it, 0, 0)));
      if (nit != in_nodes.end())
	nodes.insert(*nit);
      else
	cout<<"Error: Node "<<*it
	    <<" is not contained in set \""<<input<<"\".\n";
    }
  }
  
  maps[output] = Set(nodes, ways, relations, areas);
}
