/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDKEY_H
#define CSWORDKEY_H

//Own includes


//Qt includes
#include <qstring.h>


class CSwordModuleInfo;
/**
 * The base class for all Sword based keys.
 * @author The BibleTime team
 * @version $Id$
 */
class CSwordKey {

protected:
	CSwordKey(CSwordModuleInfo* const module = 0); //protected constructor, because CSwordKey shouldn't be used (it's an abstract base class).
	CSwordKey(const CSwordKey&); //copy constructor

public:

  enum TextRenderType {
    Normal = 0,
    HTMLEscaped
  };
	virtual ~CSwordKey() {};
	
	//pure virtual functions		
	virtual const QString key() const = 0;
	virtual void key(const QString&) = 0;
	virtual void key(const char*) = 0;
	virtual CSwordKey* copy() const = 0;	
	
	//implemented functions
	inline virtual CSwordModuleInfo* const module(CSwordModuleInfo* const newModule = 0);
	virtual const QString rawText();
  virtual const QString renderedText( const CSwordKey::TextRenderType mode = CSwordKey::Normal );
	virtual const QString strippedText();
  /**
  * This returns a new object of the right CSwordKey* implementation (e.g. CSwordVerseKey or CSwordLDKey)
  * The type is determined by the type of the module.
  * @see CSwordModuleInfo, CSwordBibleModuleInfo, CSwordCommentaryModuleInfo, CSwordLexiconModukleInfo
  */
  static CSwordKey* createInstance(CSwordModuleInfo * const module);
  /**
  * The assignment operator for more easy use of the key classes.
  */
  inline virtual CSwordKey& operator = ( const QString& );

protected:
	CSwordModuleInfo* m_module; //module pointer used by all keys
};


/** The assignment operator for more easy use of the key classes. */
inline CSwordKey& CSwordKey::operator = ( const QString& newKey ){
  key(newKey);
  return *this;
}

inline CSwordModuleInfo* const CSwordKey::module(CSwordModuleInfo* const newModule) {
	if (newModule) {
		m_module = newModule;
	}
	
	return m_module;
}


#endif
