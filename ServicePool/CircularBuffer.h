#pragma once

#include<vector>

template<class T>
class CircularBuffer
{
public: 
	CircularBuffer() {}; 
	~CircularBuffer();
	void InitializeBuffer(int queueSize, T(*ResourceProvider)(unsigned bufferIndex));
	void ResetBuffer(); 
	void Enqueue(T Data);
	T  Dequeue();
	bool Empty(); 
	bool Full();
	void Print();

private:
	unsigned _queueSize;
	unsigned _first;
	unsigned _last;
	unsigned _count;
	std::vector<T> _buffer;	
	CircularBuffer(const CircularBuffer &copy) {};
};

template<class T>
inline CircularBuffer<T>::~CircularBuffer() 
{
	_buffer.clear();
}

template<class T>
inline void CircularBuffer<T>::InitializeBuffer(int queueSize, T(*ResourceProvider)(unsigned bufferIndex))
{
	_first = 0;
	_last = queueSize - 1;
	_count = 0;
	_queueSize = queueSize;

	for (unsigned i = 0; i < _queueSize; i++)
	{
		_buffer.push_back((*ResourceProvider)(i));
	} 
	return;
}

template<class T>
inline void CircularBuffer<T>::Enqueue(T Data)
{ 
	if (!Full())
	{
		_last = (_last + 1) % _queueSize;
		_buffer.at(_last) = Data;
		_count = _count + 1;
	}

	return;
	
}

template<class T>
inline T CircularBuffer<T>::Dequeue()
{
	if(!Empty())
	{ 
		unsigned oldFirst;
		oldFirst = _first;
		_first = (_first + 1) % _queueSize;
		_count = _count - 1;
		return _buffer.at(oldFirst);
	}
	return NULL;
}

template<class T>
inline bool CircularBuffer<T>::Empty()
{
	return (_count <= 0);
}

template<class T>
inline bool CircularBuffer<T>::Full()
{
	return (_count >= _queueSize);
}

template<class T>
inline void CircularBuffer<T>::ResetBuffer()
{
	_buffer.clear();
}

template<class T>
inline void CircularBuffer<T>::Print()
{
	if (Empty())
	{
		cout << "Buffer is empty..."<<endl;
		return;
	}
		

	cout << "Printing..." << endl;
	for (int i = _first, cur = 0; cur < _count; i = ((i + 1) % _queueSize), cur++)
	{
		cout << _buffer.at(i) << endl;
	}
	cout << "finished..." << endl;
	return;
}