//
// C++ Interface: autoptrvector
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UTILAUTOPTRVECTOR_H
#define UTILAUTOPTRVECTOR_H

namespace util {

/**
 * This class provides a simple vector which works on pointers.
 * @author The BibleTime team
*/
template<class T> class AutoPtrVector {
public:
	/** Default constructor
	* The default constructor. This creates an empty vector
	*/
	explicit AutoPtrVector() : m_first(0), m_current(0), m_end(0) {};
	
	AutoPtrVector(AutoPtrVector& old) : m_first(0), m_current(0), m_end(0) {
		if (this != &old) {
			Item* last = m_first;
			Item* prev = 0;
			
			for (T* c = old.first(); c; c = old.next()) {
				last = new Item( new T(*c) );
				
				if (prev) {
					prev->next = last;
				}
	
				prev = last;
			}
		}
	};
	
	AutoPtrVector& operator=(AutoPtrVector& old) {	
		//at first delete all items, then copy old into new items
		clear();
		
		if (this != &old) {
			Item* last = m_first;
			Item* prev = 0;
			
			for (T* c = old.first(); c; c = old.next()) {
				last = new Item( new T(*c) );
				
				if (prev) {
					prev->next = last;
				}
	
				prev = last;
			}
		}
		
		return *this;
	};
	
	/** Destructor
	* Deletes all the objects which belong to the stored pointers
	*/	
	~AutoPtrVector() {
		clear();
	};
	
	
	/** Append an item
	*
	* Append a new item to this vector.
	*/	
	inline void append(T* type) {
		if (!m_first) { //handle the first item special
			m_first = new Item( type );
			
			m_end = m_first;
		}
		else {
			m_end->next = new Item( type );
			m_end = m_end->next;
		}
	};
	
	/** The first item
	*
	* @return The first item of this vector
	*/
	inline T* const first() const {
		m_current = m_first;
		return m_current->value;
	};
	
	/** The current item
	*
	* @return The current item reached by first() and next() calls
	*/
	inline T* const current() const {
		return m_current->value;
	};
	
	/** Moves to the next item.
	*
	* @return Moves to the next item
	*/
	inline T* const next() const {
		if (m_current && m_current->next) {
			m_current = m_current->next;
			return m_current->value;
		}
		
		return 0;
	};
	
	/** Returns if this conainer is empty
	*
	* @return If this vector has items or not
	*/
	inline const bool isEmpty() const {
		return (bool)m_first;
	};

	/** Clear this vector
	* This deletes all objects which belong to the stored pointers.
	*/
	inline void clear() {
		Item* i = m_first;
		while (i) {
			delete i->value;

			Item* current = i;
			i = current->next;			
			delete current;
		}
	};

private:
	struct Item {
		Item(T* t = 0) : value(t), next(0) {
		};
		
		T* value;
		Item* next;
	};
	
	mutable Item* m_first;
	mutable Item* m_current;
	mutable Item* m_end;
};

}; //end of namespace             


#endif
