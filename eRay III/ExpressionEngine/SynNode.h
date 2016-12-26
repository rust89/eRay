#ifndef _SYNNODE_H_
#define _SYNNODE_H_

#include "CustomSynItem.h"

/**
* \brief This class is designed to work with syntactic nodes. The class is a recursive structure, that making possible to store an arbitrarily large expressions.
*
* \author Rustam Z. 2012
* \file SynNode.h
*/
class SynNode : public CustomSynItem
{
	protected:
		/**
		* \brief Associated with the node value.
		*/
		double m_fValue;
		
		/**
		* \brief Pointer to the Next SynNode item.
		*/
		SynNode    *m_pNext;

		/**
		* \brief Pointer to the first child item.
		*/
		SynNode *m_pSubitem;

		/**
		* \brief Pointer to owner item.
		*/
		SynNode   *m_pOwner;

		SynNode* getClone(const SynNode* Item);

	public:
		/**
		* \brief Default constructor for SynNode class
		* \param Text SynNode text.
		* \param Type Common type of the syntactic unit.
		* \param Spec Specialization of the syntactic unit.
		*/
		SynNode(parser_string Text = PARSER_TEXT(""),int Type = 0, int Spec = 0);

		/**
		* \brief The default copy constructor.
		* \param Other Reference to the cloned object.
		*/
		SynNode(const SynNode &Other);

		/**
		* \brief This method create full copy of the SynNode object.
		* \return Cloned object.
		*/
		SynNode* Clone();
		
		/**
		* \brief This method produces an addendum to the host child.
		* \param Owner The parent element which will be adding a child element.
		* \param Text SynNode text.
		* \param Type Common type of the syntactic unit.
		* \param Spec Specialization of the syntactic unit.
		* \return Pointer to inserted item.
		*/
		static SynNode* AddSubItem(SynNode* Owner, parser_string Text, int Type = 0, int Spec = 0);
		
		/**
		* \brief This method produces an addendum to the host child.
		* \param Owner The parent element which will be adding a child element.
		* \param Node Child item.
		* \return Pointer to inserted item.
		*/		
		static SynNode* AddSubItem(SynNode* Owner, SynNode* Node);
		
		/**
		* \brief Destroys the node, as well as all subsequent nodes behind it and their children.
		* \param Item Pointer to the destination node.
		*/
		static void Destroy(SynNode *Item);

		/**
		* \brief This method gets a pointer to the next in order of syntactic a node.
		* \return Pointer to the next in order of syntactic a node if success, NULL overwise.
		*/
		const SynNode* Next() const;

		/**
		* \brief This method receives a pointer to a list of child nodes of syntax.
		* \return Pointer to the first in order of the child syntactic node if success, NULL overwise.
		*/
		const SynNode* SubItem() const;

		/**
		* \brief This method gets the parent node for this node.
		* \return Pointer to the owner node if success, NULL overwise.
		*/
		const SynNode* Owner() const;

		/**
		* \brief This method gets a digit value associated with a node.
		* \return Real value.
		*/
		const double getValue() const;

		/**
		* \brief This method set a digit value associated with a node.
		* \param Value Som real value.
		*/
		void setValue(double Value);

};


#endif