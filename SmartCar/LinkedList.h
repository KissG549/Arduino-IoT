#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_

template <class T>
class mListNode
{
public:
	mListNode() :
		mNext(NULL), mPrev(NULL), myVal(NULL) {};

	mListNode(T* pVal):
		mNext(NULL), mPrev(NULL), myVal(pVal) {};

	mListNode(const T& pVal) :
		mNext(NULL), mPrev(NULL), myVal(new T(pVal)) {};

	~mListNode()
	{
		// remove pointers
		// delete value
		if (mNext) { mNext = NULL; }
		if (mPrev) { mPrev = NULL; }
		if (myVal) { delete myVal; }
	};
	
	mListNode* mNext;
	mListNode* mPrev;
	T* myVal;
};

template <class T>
class LinkedList
{
public:
	LinkedList() :
		mHead(NULL), mTail(NULL), mCurrent(NULL), mSize(0) {};

	//LinkedList(const LinkedList& mOther)
	//{
	//	// TODO
	//};

	~LinkedList()
	{
		clear();
	};

	//void assign(uint8_t count, const T& value)
	//{
	//	// TODO
	//}

	T& front() const
	{
		return *(mHead->myVal);
	};

	T& back() const
	{
		return *(mTail->myVal);
	};

	bool isEnd() const
	{
		return mCurrent == mTail;
	}

	T& next()
	{
		if (mCurrent == NULL)
		{
			mCurrent = mHead;
		}
		else
		{
			mCurrent = mCurrent->mNext;
		}
		return *(mCurrent->myVal);
	};

	bool empty() const
	{
		return mSize <= 0;
	};

	uint8_t size() const
	{
		return mSize;
	};

	void clear()
	{
		while (!empty())
		{
			pop_front();
		}
	};

	// insert front
	void insert(T& value)
	{
		_insert(value, mHead);
	};

	//void insert(uint8_t pos, const T& value)
	//{
	//	// TODO
	//}

	void push_back(T value)
	{
		_insert(value, NULL);
	};
	
	void pop_back()
	{
		_remove(mTail);
	};

	void push_front(const T value)
	{
		_insert(value, mHead);
	};
		
	void pop_front()
	{
		_remove(mHead);
	};

	//void resize(uint8_t count)
	//{
	//	// TODO
	//}

	//void remove(const T& value)
	//{
	//	// TODO
	//}


private:
	mListNode<T>* mHead;
	mListNode<T>* mTail;
	mListNode<T>* mCurrent;
	uint8_t mSize;

	
	// insert before
	void _insert(T pValue, mListNode<T>* pPos)
	{
		mListNode<T>* newNode = new mListNode<T>(pValue);
		++mSize;

		// if the list is empty
		if (mHead == NULL && mTail == NULL)
		{
			mHead = newNode;
			mTail = newNode;
		}
		// insert to the end
		// we does not have iterators yet to identify the next item after the Tail
		else if (pPos == NULL)
		{
			newNode->mPrev = mTail;
			mTail->mNext = newNode;
			mTail = newNode;
		}
		// insert between two items
		else
		{
			newNode->mNext = pPos;
			newNode->mPrev = pPos->mPrev;
			if (pPos->mPrev != NULL)
			{
				pPos->mPrev->mNext = newNode;
			}
			pPos->mPrev = newNode;
		}
	};

	void _remove(mListNode<T>* pPos)
	{
		if (pPos == NULL) { return; }
		
		// remove from the Head
		if (pPos == mHead)
		{
			_removeHead();
		}
		// remove from the end
		else if (pPos == mTail)
		{
			_removeTail();
		}
		else
		{
			pPos->mNext->mPrev = pPos->mPrev;
			pPos->mPrev->mNext = pPos->mNext;
		}
		delete pPos;
		--mSize;
	};

	void _removeHead()
	{
		if (mHead->mNext)
		{
			mHead = mHead->mNext;
			mHead->mPrev = NULL;
		}
		else
		{
			mHead = NULL;
			mTail = NULL;
		}
	}

	void _removeTail()
	{
		if (mTail->mPrev)
		{
			mTail = mTail->mPrev;
			mTail->mNext = NULL;
		}
		else
		{
			mTail = NULL;
			mTail = NULL;
		}
	}
};


#endif // !_DOUBLE_LINKED_LIST_H_