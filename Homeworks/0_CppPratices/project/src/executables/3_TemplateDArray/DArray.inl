// implementation of class DArray
#include<iostream>
#include <cassert>  //import assert.h
using namespace std;

template<class T>

// default constructor
 DArray<T>::DArray() {
	Init();
}

template<class T>

// set an array with default values
DArray<T>::DArray(int nSize, T dValue) {
	//TODO
	m_pData = new T[nSize];
	m_nSize = nSize;
	m_nMax = nSize;

	for (int i = 0; i < nSize; i++)  m_pData[i] = dValue;
}

template<class T>
DArray<T>::DArray(const DArray& arr) {
	//TODO
	m_pData = new T[arr.m_nSize];
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;

	for (int i = 0; i < m_nSize; i++) m_pData[i] = arr.m_pData[i];
}

template<class T>
// deconstructor
DArray<T>::~DArray() {
	Free();
}


// display the elements of the array
template<class T>

void DArray<T>::Print() const {
	//TODO
	cout << "size = " << m_nSize << ":";
	for (int i = 0; i < m_nSize; i++)
		cout << "  " << GetAt(i);

	cout << endl;
}

// initilize the array
template<class T>

void DArray<T>::Init() {
	//TODO
	m_pData = nullptr;
	m_nSize = 0;
	m_nMax = 0;
}

// free the array
template<class T>

void DArray<T>::Free() {
	//TODO
	delete[] m_pData;
	m_pData = nullptr;

	m_nSize = 0;
	m_nMax = 0;
}

// get the size of the array
template<class T>
int DArray<T>::GetSize() const {
	//TODO
	return m_nSize;
}

template<class T>
void DArray<T>::Reserve(int nSize) {  
	if (m_nMax >= nSize)
		return;
	while (m_nMax < nSize)
		m_nMax = m_nMax == 0 ? 1 : 2 * m_nMax;

	T* pData = new T[m_nMax];
	memcpy(pData, m_pData, m_nSize * sizeof(T));

	delete[] m_pData;
	m_pData = pData;
}

// set the size of the array
template<class T>

void DArray<T>::SetSize(int nSize) {
	//TODO
	if (m_nSize == nSize)
		return;

	Reserve(nSize);
	for (int i = m_nSize; i < nSize; i++)
		m_pData[i] = static_cast<T>(0);

	m_nSize = nSize;
}

// get an element at an index
template<class T>

const T& DArray<T>::GetAt(int nIndex) const {
	//TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// set the value of an element 
template<class T>

void DArray<T>::SetAt(int nIndex, T dValue) {
	//TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

// overload operator '[]'
template<class T>

const T & DArray<T>::operator[](int nIndex) const{
	// TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}


// add a new element at the end of the array
template<class T>

void DArray<T>::PushBack(T dValue) {
	//TODO
	Reserve(m_nSize + 1);
	m_pData[m_nSize] = dValue;
	m_nSize++;
}

// delete an element at some index
template<class T>

void DArray<T>::DeleteAt(int nIndex) {
	//TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	for (int i = nIndex + 1; i < m_nSize; i++)
		m_pData[i - 1] = m_pData[i];

	m_nSize--;
/*
	T* pTemp = new T[static_cast<size_t>(m_nSize) - 1];

	for (int i = 0; i < nIndex; i++)
		pTemp[i] = m_pData[i];

	for (int i = nIndex; i < m_nSize - 1; i++)
		pTemp[i] = m_pData[i + 1];

	delete[] m_pData;
	m_pData = pTemp;  //这样的话类型可能不统一
	m_nSize--;
	*/
}

// insert a new element at some index
template<class T>

void DArray<T>::InsertAt(int nIndex, T dValue) {
	//TODO
	assert(nIndex >= 0 && nIndex <= m_nSize); // nIndex == m_nSize is legal

	Reserve(m_nSize + 1);

	for (int i = m_nSize; i > nIndex; i--)
		m_pData[i] = m_pData[i - 1];

	m_pData[nIndex] = dValue;

	m_nSize++;
}

// overload operator '='
template<class T>
 DArray<T>&DArray<T>::operator = (const DArray& arr) {
	//TODO
	Reserve(arr.m_nSize);

	m_nSize = arr.m_nSize;
	memcpy(m_pData, arr.m_pData, arr.m_nSize * sizeof(double));

	return *this;
}
