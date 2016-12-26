#ifndef _CMESSAGEMAP_H_
#define _CMESSAGEMAP_H_

#include <Windows.h>

namespace RClasses {

template <class TMethod, class TMethodOwner> class EVENTHANDLER
{
	public:
		UINT MessageCode;
		TMethod Func;
		TMethodOwner Owner;
};

template <class TMethod, class TMethodOwner> class RMessageMap
{
	protected:
		struct Node
		{
			EVENTHANDLER <TMethod, TMethodOwner> Event;
			Node *Next;
		} *root;
	public:
		RMessageMap();
		RMessageMap(const RMessageMap &Copy);
		~RMessageMap();
		void AddMessage(EVENTHANDLER <TMethod, TMethodOwner> Event);
		void AddMessage(UINT MessageCode, TMethod Func, TMethodOwner Owner);
		void Clear();
		TMethod Find(UINT MessageCode);
		bool Find(UINT MessageCode, TMethod &Func, TMethodOwner &Owner);
};

template <class TMethod, class TMethodOwner> RMessageMap <TMethod, TMethodOwner>::RMessageMap()
{
	root = 0;
}

template <class TMethod, class TMethodOwner> RMessageMap <TMethod, TMethodOwner> ::RMessageMap(const RMessageMap &Copy)
{
	root = 0;
}

template <class TMethod, class TMethodOwner> RMessageMap <TMethod, TMethodOwner> ::~RMessageMap()
{
	Clear();
}

template <class TMethod, class TMethodOwner> void RMessageMap <TMethod, TMethodOwner> ::AddMessage(EVENTHANDLER <TMethod, TMethodOwner> Event)
{
	// инициализация структуры
	Node *tmp = new Node;
	tmp->Event = Event;
	tmp->Next = 0;

	if (root)
	{
		Node *Iterator = root;
		while (Iterator->Next)
		{
			Iterator = Iterator->Next; 
		}
		Iterator->Next = tmp;
	}
	else root = tmp;
}

template <class TMethod, class TMethodOwner> void RMessageMap <TMethod, TMethodOwner> ::AddMessage(UINT MessageCode, TMethod Func, TMethodOwner Owner)
{
	// инициализация структуры
	Node *tmp = new Node;
	tmp->Event.MessageCode = MessageCode;
	tmp->Event.Func = Func;
	tmp->Event.Owner = Owner;
	tmp->Next = 0;

	if (root)
	{
		Node *Iterator = root;
		while (Iterator->Next)
		{
			Iterator = Iterator->Next; 
		}
		Iterator->Next = tmp;
	}
	else root = tmp;
}

template <class TMethod, class TMethodOwner> void RMessageMap <TMethod, TMethodOwner> ::Clear()
{
	if (!root) return;
	while (root)
	{
		Node *tmp = root->Next;
		delete root;
		root = tmp;
	}
	root = 0;
}

template <class TMethod, class TMethodOwner> TMethod RMessageMap <TMethod, TMethodOwner>::Find(UINT MessageCode)
{
	Node *tmp = root;
	while (tmp)
	{
		if (tmp->Event.MessageCode == MessageCode)
			return tmp->Event.Func;
		tmp = tmp->Next;
	}
	return (TMethod) 0;
}

template <class TMethod, class TMethodOwner> bool RMessageMap <TMethod, TMethodOwner>::Find(UINT MessageCode, TMethod &Func, TMethodOwner &Owner)
{
	Node *tmp = root;
	while (tmp)
	{
		if (tmp->Event.MessageCode == MessageCode)
		{
			Func = tmp->Event.Func;
			Owner = tmp->Event.Owner;
			return true;
		}
		tmp = tmp->Next;
	}
	return false;
}

}

#endif