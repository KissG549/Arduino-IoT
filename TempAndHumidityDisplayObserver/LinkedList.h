#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
 
/* 
* function names differs in this header, to being compatible with the std::list naming. 
*/

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

	~LinkedList()
	{
		clear();
	};

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

  T& at(uint8_t pIdx)
  {
    if (pIdx == mSize - 1)
    {
      return *(mTail->myVal);
    }

    mListNode<T>* current = mHead;
    for (uint8_t it = 1; it <= pIdx && it < mSize; ++it)
    {
      current = current->mNext;
    }
    return *(current->myVal);
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

	void insert(T& value)
	{
		push_front(value);
	};

	void push_back(T pValue)
	{
    mListNode<T>* newNode = new mListNode<T>(pValue);
    if( mSize == 0 )
    {
      mHead = newNode;
      mTail = newNode;
    }
    else
    {
      mTail->mNext = newNode;
      newNode->mPrev = mTail;
      mTail = newNode;
    }
    mSize++;
	};
	
	void pop_back()
	{
		_remove(mTail);
	};

	void push_front(T pValue)
	{
		mListNode<T>* newNode = new mListNode<T>(pValue);
    if( mSize == 0 )
    {
      mHead = newNode;
      mTail = newNode;
    }
    else
    {
      newNode->mNext = mHead;
      mHead->mPrev = newNode;
      mHead = newNode;
    }
    mSize++;
	};
		
	void pop_front()
	{
		_remove(mHead);
	};

private:
	mListNode<T>* mHead;
	mListNode<T>* mTail;
	mListNode<T>* mCurrent;
	uint8_t mSize;
	
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
