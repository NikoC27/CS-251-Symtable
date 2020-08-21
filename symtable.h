/*symtable.h*/

// 
// << Niko Castellana >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
//
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.
//

#pragma once

#include <iostream>
#include <deque>
#include <map>
#include <utility>

using namespace std;

template<typename KeyT, typename SymbolT>
class symtable
{
public:
  //
  // A symbol table is a stack of scopes.  Every scope has a name, and 
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  class Scope
  {
  public:
    string              Name;
    map<KeyT, SymbolT>  Symbols;

    // constructors:
    Scope()
    {
      // default empty string and an empty map:
     
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
    }
  };

private:
  int Size = 0;
  deque <Scope> MyScopes;

public:
  enum class ScopeOption
  {
    ALL, 
    CURRENT,
    GLOBAL
  };

  //
  // default constructor:
  //
  // Creates a new, empty symbol table.  No scope is open.
  //
  symtable()
  {
    
  }

  //
  // size
  //
  // Returns total # of symbols in the symbol table.
  //
  // Complexity: O(1)
  //
  int size() const
  {
	return Size;  // Return total # of symbols
	
  } // End of size()

  //
  // numscopes
  //
  // Returns the # of open scopes.
  //
  // Complexity: O(1)
  //
  int numscopes() const
  {
	return MyScopes.size();  // Return # of open scopes
	
  } // End of numscopes()

  //
  // enterScope
  //
  // Enters a new, open scope in the symbol table, effectively "pushing" on
  // a new scope.  You must provide a name for the new scope, although
  // the name is currently used only for debugging purposes.
  //
  // NOTE: the first scope you enter is known as the GLOBAL scope, since this
  // is typically where GLOBAL symbols are stored.  
  //
  // Complexity: O(1)
  //
  void enterScope(string name)
  {
	Scope myScope(name);  // Declare new scope and pass name 
	MyScopes.push_front(myScope);  // Push new scope to front of deque
	
  } // End of enterScope

  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
  //
  void exitScope()
  {
	if(MyScopes.empty())  // Throw error when no scope is currently open
		throw runtime_error("symtable::exitScope: empty");
	
	Size = Size - MyScopes.front().Symbols.size();  // Lower the size of deque
	MyScopes.pop_front();  // Pop the front of the deque
	
  } // End of exitScope()

  //
  // curScope
  //
  // Returns a copy of the current scope.  A runtime_error is thrown if
  // no scope is currently open.
  //
  // Complexity: O(N) where N is the # of symbols in the current scope
  //
  Scope curScope() const
  {
	if(MyScopes.empty())  // Throw error when no scope is currently open
		throw runtime_error("symtable::curScope: empty");
	
   	return MyScopes.front();  // Return the front of the deque
	
  } // End of curScope()

  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  void insert(KeyT key, SymbolT symbol)
  {
	/* Check if the key already exists*/
	if(MyScopes.front().Symbols.count(key) == 1)
	{
	    MyScopes.front().Symbols.at(key) = symbol;  // Override symbol if key found
	}
	else 
	{
	    MyScopes.front().Symbols.emplace(key,symbol);  // Insert symbol into key
		Size++;  // Increase size of symbols
	}
	
  } // End of insert()

  //
  // lookup
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
  // pair is found, true is returned along with a copy of the symbol (via 
  // "symbol" reference parameter).  If not found, false is returned and
  // the "symbol" parameter is left unchanged.
  //
  // NOTE: the search can be controlled by the "option" parameter.  By 
  // default, the entire symbol table is searched as described above.
  // However, the search can also be limited to just the current scope, 
  // or just the GLOBAL scope, via the "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   string                   symbol;
  //   bool                     found;
  //   ...
  //   found = table.lookup("i", 
  //                        symbol, 
  //                        symtable<string,string>::ScopeOption::CURRENT);
  //
  // Complexity: O(SlgN) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  bool lookup(KeyT key, 
              SymbolT& symbol, 
              ScopeOption option = ScopeOption::ALL) const
  {
	/* Check option is ALL to search all the scopes in deque */
	if(option == ScopeOption::ALL)
	{
		for(auto &x : MyScopes) // Check all the scopes in the deque
		{
			auto ptr = x.Symbols.find(key);  // Find key in each scope
			
		/* Checks each scope for a matching key and symbol pair */
			if(ptr != x.Symbols.end())  
			{
				symbol = ptr->second;  // Update symbol
				return true;  // Matching pair found
			} 
		}
	}
	/* Check option is CURRENT to search the current scope in deque */
	else if (option == ScopeOption::CURRENT)
	{
		auto ptr1 = MyScopes.front().Symbols.find(key); // Find key in front of deque
		
		/* Checks current scope for a matching key and symbol pair */
		if(ptr1 == MyScopes.front().Symbols.end())
		{
			return false;  // Pair not found
		}
		else
		{
			symbol = ptr1->second;  // Update symbol
			return true;  // Matching pair found
		}
	}
	/* Check option is GLOBAL to search the global scope in deque */
	else if(option == ScopeOption::GLOBAL)
	{ 
		auto ptr2 = MyScopes.back().Symbols.find(key); // Find key in back of deque
		
		/* Checks global scope for a matching key and symbol pair */
		if(ptr2 == MyScopes.back().Symbols.end())
		{
			return false;  // Pair not found
		}
		else
		{
			symbol = ptr2->second;  // Update symbol
			return true;  // Matching pair found
		}
	}
	
	return false;  // Return false if (key,symbol) pair isn't found
	
  } // End of lookup()

  //
  // dump
  // 
  // Dumps the contents of the symbol table to the output stream, 
  // starting with the current scope and working "outward" to the GLOBAL
  // scope.  You can dump the entire symbol table (the default), or dump
  // just the current scope or global scope; this is controlled by the 
  // "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   ...
  //   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
  //
  // Complexity: O(S*N) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
  {
	output << "**************************************************" << endl;

	/* Checks the key and symbol pairs in every scope of the deque and prints */
   	if (option == ScopeOption::ALL)
	{
		output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
		// Print # of open scopes
		output << "** # of scopes: " << this->numscopes() << endl; 
		// Print # of symbols
		output << "** # of symbols: " << this->size() << endl;  
	    
		// Loop through scopes in deque
		for(auto &scopename : MyScopes)
		{ 
			// Print out scope name
			output << "** "<<scopename.Name<<" **" << endl;
			// Loop and print out each key and symbol pair for each scope
			for(auto &p : scopename.Symbols)
			{
				output << p.first << ": " << p.second << endl;
			}
		}
		  
	}
    /* Checks the key and symbol pairs in the current scope and prints */ 
	else if (option == ScopeOption::CURRENT)
	{
		output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
		// Print # of open scopes
		output << "** # of scopes: " << this->numscopes() << endl;
		// Print # of symbols
        	output << "** # of symbols: " << this->size() << endl;
		// Print name of CURRENT scope
		output << "** " << MyScopes.front().Name << " **" << endl;
		
		// Loop and print out each key and symbol pair for CURRENT 
		for(auto &p : MyScopes.front().Symbols)
		{
			output << p.first << ": " << p.second << endl;
		}
			  
	}
    /* Checks the key and symbol pairs in the global scope and prints */
    	else 
	{
		output << "*************** SYMBOL TABLE (GBL) ***************" << endl;
		// Print # of open scopes
		output << "** # of scopes: " << this->numscopes() << endl;
		// Print # of symbols
		output << "** # of symbols: " << this->size() << endl;
		// Print name of GLOBAL scope
		output << "** "<<MyScopes.back().Name<<" **" << endl;
		
		// Loop and print out each key and symbol pair for GLOBAL
		for(auto &p : MyScopes.back().Symbols)
		{
			output << p.first << ": " << p.second << endl;
		}
			  
	}
	  
	output << "**************************************************" << endl;
	  
  }  // End of dump()

}; // End of symtable
