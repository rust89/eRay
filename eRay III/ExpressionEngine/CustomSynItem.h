#ifndef _CUSTOMSYNITEM_H_
#define _CUSTOMSYNITEM_H_

#include "parser_string.h"

/**
* \class CustomSynItem
* \brief The CustomSynItem class provides a representation of the token and the functions to work with him.
*
* This is the base class for more detailed views. With this class, you can store the text of the token, the lexical type and lexical type specialization.
*
* \author Rustam Z. 2012
* \file CustomSynItem.h
*/
class CustomSynItem
{
	protected:

		/**
		* \brief The text of the token.
		*/
		parser_string m_sText;

		/**
		* \brief Lexical type.
		*/
		int m_nType;

		/**
		* \brief Specialization of the lexical type.
		*/
		int m_nSpec;	
	public:

		/**
		* \brief The default constructor for CustomSynItem object.
		*/
		CustomSynItem();

		/**
		* \brief The custom constructor with params
		* \param Text The text of the token.
		* \param Type Lexical type.
		* \param Spec Specialization of the lexical type.
		*/
		CustomSynItem(parser_string Text, int Type, int Spec);

		/**
		* \brief Copy constructor.
		* \param Other The copied object.
		*/
		CustomSynItem(const CustomSynItem &Other);

		/**
		* \brief Default class destructor.
		*/
		~CustomSynItem();

		/**
		* \brief The setText method set a new text of the token.
		* \param Text New text of the token.
		*/
		void setText(parser_string Text);

		/**
		* \brief The setType method set a new lexical type of the token.
		* \param Type New lexical type of the token.
		*/
		void setType(int Type);

		/**
		* \brief The setSpec method set a new lexical Specialization of the token.
		* \param Spec New lexical Specialization of the token.
		*/
		void setSpec(int Spec);

		/**
		* \brief The getText method returns text of the token.
		* \return Text of the token.
		*/
		const parser_string getText() const;

		/**
		* \brief The getType method returns a lexical type of the token.
		* \return Type of the token.
		*/
		const int getType() const;
		
		/**
		* \brief The getSpec method returns a lexical specialization of the token.
		* \return Specialization of the token.
		*/
		const int getSpec() const;

		/**
		* \brief Reset token information.
		*/
		void Clear();
};

#endif