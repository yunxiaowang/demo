////////////////////////////////////////////////////////
//  File name:      Identifier.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: NPP identifier to deal with string and variable
////////////////////////////////////////////////////////

#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "faststrdup.h"

class Identifier
{
public:
        static Identifier * get(int);
        static Identifier * get(const char *);

        Identifier(int number):
            m_isString(false)
        {
            m_value.m_number = number;
        }

        Identifier(const char * name):
            m_isString(true)
        {
            m_value.m_string =  faststrdup(name);
        }

        ~Identifier();

        bool isValid(Identifier *);

        bool isString() const
        {
            return m_isString;
        }

        int getNumber() const
        {
            return m_isString ? 0: m_value.m_number;
        }

        const char * getString() const
        {
            return m_isString ? m_value.m_string : 0;
        }

private:

        union
        {
            const char* m_string;
			int m_number;
        } m_value;
		
        bool m_isString;
};



#endif //identifier_h_
