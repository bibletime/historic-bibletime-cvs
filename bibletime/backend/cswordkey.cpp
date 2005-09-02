/********* Read the file LICENSE for license details. *********/


//own includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

#include "cswordversekey.h"
#include "cswordldkey.h"
#include "cswordtreekey.h"

#include "util/ctoolclass.h"

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>
#include <treekey.h>
#include <treekeyidx.h>

//Qt includes
#include <qregexp.h>
#include <qstring.h>

CSwordKey::CSwordKey(CSwordModuleInfo* const module) : m_module(module) {}

CSwordKey::CSwordKey(const CSwordKey& k) {
    m_module = k.m_module;
}

const QString CSwordKey::rawText() {
    if (!m_module) {
        return QString::null;
    }

    if (/*sword::SWKey* k =*/ dynamic_cast<sword::SWKey*>(this)) {
        //     m_module->module()->SetKey(k);
        m_module->module()->getKey()->setText( (const char*)key().utf8() );
    }
    if (key().isNull()) {
        return QString::null;
    }

    return QString::fromUtf8(m_module->module()->getRawEntry());
}

const QString CSwordKey::renderedText( const CSwordKey::TextRenderType mode ) {
    if (!m_module) {
        return QString::null;
    }

    using namespace sword;
    SWKey* k = dynamic_cast<sword::SWKey*>(this);
    if (k) {
        VerseKey* vk_mod = dynamic_cast<VerseKey*>(m_module->module()->getKey());
        if (vk_mod) {
            vk_mod->Headings(1);
        }

        m_module->module()->getKey()->setText( this->key().utf8() );

        if (m_module->type() == CSwordModuleInfo::Lexicon) {
            m_module->snap();
            /* In lexicons make sure that our key (e.g. 123) was successfully set to the module,
            i.e. the module key contains this key (e.g. 0123 contains 123) */
            if ( strcasecmp(m_module->module()->getKey()->getText(), (const char*)key().utf8())
                    && !strstr(m_module->module()->getKey()->getText(), (const char*)key().utf8())
               ) {
                qWarning("return an empty key for %s", m_module->module()->getKey()->getText());
                return QString::null;
            }
        }
    }


    if (!key().isNull()) { //we have valid text
        const QString text = QString::fromUtf8( m_module->module()->RenderText() );

        if (mode == HTMLEscaped) {
            //we have to encode all UTF-8 in HTML escapes
            // go though every character and write down the escaped HTML unicode entity
            // form is &#<decimal unicode value here>;
            QString ret;
            QChar c;
            const unsigned int length = text.length();
            for (unsigned int i = 0; i < length; ++i) {
                c = text.at(i);
                if (c.latin1()) { //normal latin1 character
                    ret.append(c);
                } else {//unicode character, needs to be escaped
                    ret.append("&#")
                    .append(c.unicode())
                    .append(";");
                }
            };
            return ret;
        } else {
            return text;
        }
    }

    return QString::null;
}

const QString CSwordKey::strippedText() {
    if (!m_module) {
        return QString::null;
    }

    if (/*sword::SWKey* k =*/ dynamic_cast<sword::SWKey*>(this)) {
        //   m_module->module()->SetKey(k);
        m_module->module()->getKey()->setText( (const char*)key().utf8() );
    }
    return QString::fromUtf8( m_module->module()->StripText() );
}


/** This will create a proper key object from a given module */
CSwordKey* CSwordKey::createInstance( CSwordModuleInfo* const module ) {
    if (!module)
        return 0;

    switch( module->type() ) {
    case CSwordModuleInfo::Bible://fall through
    case CSwordModuleInfo::Commentary:
        return new CSwordVerseKey( (sword::VerseKey *) ( (sword::SWKey *)(*module->module()) ), module );
    case CSwordModuleInfo::Lexicon:
        return new CSwordLDKey( (sword::SWKey *)(*module->module()), module);
    case CSwordModuleInfo::GenericBook:
        return new CSwordTreeKey( (sword::TreeKeyIdx*)((sword::SWKey *)(*module->module())), module );
    default:
        return 0;
    }
}
