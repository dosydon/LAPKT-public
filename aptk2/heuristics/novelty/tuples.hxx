#ifndef __TUPLES__
#define __TUPLES__

#include <set>
#include <utility>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace aptk 
{

	typedef		unsigned 	VariableIndex;
	typedef		unsigned	ValueIndex;

	class ValuesTuple {
	public:

		struct Entry {
			
			VariableIndex 	x;
			ValueIndex	v;
			
			bool	operator<( const Entry& other ) const {
				if ( x < other.x ) return true;
				if ( x == other.x ) 
					return v < other.v;
				return false;
			}

			bool	operator>( const Entry& other ) const {
				if ( x > other.x ) return true;
				if ( x == other.x ) 
					return v > other.v;
				return false;
			}
	
			bool	operator==( const Entry& other ) const {
				return x == other.x && v == other.v;
			}

			bool	operator!=( const Entry& other ) const {
				return x != other.x || v != other.v;
			}

		};
		
		typedef		std::vector< Entry >	Container;

		ValuesTuple();
		ValuesTuple( size_t sz, bool preallocate = true );
		ValuesTuple( const ValuesTuple& other );
		ValuesTuple( ValuesTuple&& other );

		ValuesTuple&	operator=( ValuesTuple&& other ) {
			elements = other.elements;
			return *this;
		}

		const ValuesTuple&	operator=( const ValuesTuple& other ) {
			elements = other.elements;
			return *this;
		}

		~ValuesTuple();

		void	add( VariableIndex x, ValueIndex v ) {
			Entry e = {x,v};
			elements.push_back( e );
		}

		void	set( unsigned i, VariableIndex x, ValueIndex v ) {
			elements[i] = {x,v};
		}

		void	finish() {
			std::sort( elements.begin(), elements.end() );
		}

		bool	operator==( const ValuesTuple& t ) const {
			assert( elements.size() == t.elements.size() );
			for ( unsigned i = 0; i < elements.size(); i++ )
				if ( elements[i] != t.elements[i] ) return false;
			return true; 
		}

		bool	operator<( const ValuesTuple& t ) const {
			for ( unsigned i = 0; i < elements.size(); i++ ) {
				if ( elements[i] < t.elements[i]) return true;
				if ( elements[i] > t.elements[i]) return false;
			}
			return false;
		}

		Container::iterator
		begin()	{ return elements.begin(); }
		
		Container::iterator
		end() { return elements.end(); }

		Container::const_iterator 
		begin() const { return elements.begin(); }
	
		Container::const_iterator
		end() const { return elements.end(); }

		friend std::ostream& 	operator<<( std::ostream& stream, const ValuesTuple& t );

	protected:
		
		Container	elements;
	};

	std::ostream& 	operator<<( std::ostream& stream, const ValuesTuple& t );


	typedef	std::set< ValuesTuple >	Tuple;

	class ValuesTupleIterator {
	public:

		ValuesTupleIterator( const std::vector<VariableIndex>& X, const std::vector<ValueIndex>& v, size_t sz );
 
		~ValuesTupleIterator();

		void	reset() {
			for ( unsigned k = 0; k < tuple_sz; k++ ) {
				offsets[k] = k;	
				index[k] = offsets[k];
			}			
			count = 0;
		}

		bool	next( ValuesTuple& t ) {
			/*
			for ( unsigned k = 0; k < tuple_sz; k++ )
				std::cout << index[k] << " ";
			std::cout << std::endl;
			*/
			for ( unsigned k = 0; k < tuple_sz; k++ ) {
				t.set( k, index[k], values[index[k]] );
			}
			index[tuple_sz-1]++;
			for ( unsigned k = tuple_sz-1; k > 0; k-- ) {
				if ( index[k] < vars.size() )
					break;
				index[k-1]++;
				for ( unsigned j = k; j < tuple_sz; j++ )
					index[j] = index[j-1]+1;//offsets[j];		
			}
			count++;	
			return index[0] < vars.size();
		}

		unsigned current_tuple_index() const { return count; }

	protected:
		const 	std::vector<VariableIndex>&			vars;
		const   std::vector<ValueIndex>&			values;
		size_t							tuple_sz;
		std::vector<unsigned>					offsets;
		std::vector<unsigned>					index;
		unsigned						count;
	};

	

}

#endif