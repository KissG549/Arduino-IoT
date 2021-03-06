
#ifndef _BUFFER_H_
#define _BUFFER_H_

#ifndef DEBUG_BUFFER
 // #define DEBUG_BUFFER 1
#endif

/*
	A custom buffer to store values temporary

	Usage:

	int main()
	{
		Buffer<int> buff;

		for (int i = 0; i < 5; ++i)
		{
			std::cout << "add " << i << std::endl;
			buff.add(i);
		}

		for (int i = 0; i < buff.size(); ++i)
		{
			std::cout << buff[i] << std::endl;
		}

		return 0;
	}

*/

static const uint8_t MAX_BUFFER_SIZE = 3;

template<class T>
class Buffer
{
public:

	Buffer(uint8_t pSize = MAX_BUFFER_SIZE)
		: mMaxSize(pSize), mSize(0)
	{	
    mRawBuffer = new T[pSize];
#ifdef DEBUG_BUFFER
     Serial.print("Buffer created, size: ");
     Serial.println(pSize); 
#endif
	};

	~Buffer()
	{
		delete[] mRawBuffer;
#ifdef DEBUG_BUFFER
     Serial.print("Buffer deleted, size: ");
     Serial.println(mSize); 
#endif
	};

	void add(T pValue)
	{
		if (mSize >= mMaxSize)
		{
			moveForward();
		}
		else 
		{
			++mSize;
		}

		mRawBuffer[mSize-1] = pValue;

	};

	uint8_t size() const
	{
		return mSize;
	};

  T& at(const uint8_t pPos) const
  {
    return mRawBuffer[pPos];
  };

	T& operator[](const uint8_t pPos) const
	{
		return mRawBuffer[pPos];
	};

	void clear()
	{
		delete[] mRawBuffer;
		mRawBuffer = new T[mMaxSize];
	};

private:
	uint8_t mMaxSize;
	uint8_t mSize;
	T* mRawBuffer;

	void moveForward()
	{
		//std::cout << "Move: ";

		for (uint8_t pos = 1;
			pos < mSize;
			++pos)
		{
			mRawBuffer[pos - 1] = mRawBuffer[pos];
		}

	};
};


#endif // !_BUFFER_H_
