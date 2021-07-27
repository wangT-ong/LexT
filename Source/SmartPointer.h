#pragma once
#include<stdexcept>
template<typename T>
class SmartPointer
{
public:
	template<typename T0>
	friend class SmartPointer;
	~SmartPointer() { rest(); };
	SmartPointer():data(nullptr),count(nullptr) {};
	SmartPointer(SmartPointer&& pointer);
	SmartPointer(const SmartPointer& pointer);
	SmartPointer(T* point);
	SmartPointer& operator=(SmartPointer p2);
	template<typename T1>
	SmartPointer& operator=(SmartPointer<T1> p2);

	T* operator->() { return data; }
	T& operator*() { return *data; }
	T* get() { return data; }
	bool isValidate()const { return data != nullptr; }
	
	template<typename T2>
		SmartPointer(SmartPointer<T2>&&pointer);
	template<typename T3>
		SmartPointer(const SmartPointer<T3>& pointer);

private:
	void rest();
	int* count;
	T* data;
};

template<typename T1,typename T2>
bool operator==(SmartPointer<T1> p1,  SmartPointer<T2> p2)
{
	if (typeid(T1) != typeid(T2))
		return false;
	return p1.get() == p2.get();
}


template<typename T>
SmartPointer<T>::SmartPointer(T* point) :data(nullptr), count(nullptr)
{
	if (point != nullptr) {
		data = point;
		count = new int(1);

	}
}


template<typename T>
SmartPointer<T>::SmartPointer(SmartPointer&& pointer) :data(nullptr), count(nullptr)
{

	if (pointer.isValidate()) {
		data = pointer.data;
		count = pointer.count;
		++(*count);
	}
}

template<typename T>
SmartPointer<T>::SmartPointer(const SmartPointer& pointer) :data(nullptr), count(nullptr)
{

	if (pointer.isValidate()) {
		data = pointer.data;
		count = pointer.count;
		++(*count);
	}
}




template<typename T>
template<typename T2>
SmartPointer<T>::SmartPointer(SmartPointer<T2>&& pointer) :data(nullptr), count(nullptr)
{
	T* p = dynamic_cast<T*>(pointer.data);
	data = p;
	if (data != nullptr) {
		count = pointer.count;
		++(*count);
	}
}

template<typename T>
template<typename T2>
SmartPointer<T>::SmartPointer(const SmartPointer<T2>& pointer) :data(nullptr), count(nullptr)
{
	T* p = dynamic_cast<T*>(pointer.data);
	data = p;
	if (data != nullptr) {
		count = pointer.count;
		++(*count);
	}
}

template<typename T>
template<typename T1>
SmartPointer<T>& SmartPointer<T>::operator=(SmartPointer<T1> p1)
{
	T* p = dynamic_cast<T*>(p1.data);
	
	if (p1.isValidate()&&p==nullptr)
	{
		throw std::runtime_error("ÀàÐÍ×ª»»Ê§°Ü");
	}
	rest();
	data = p;
	count = p1.count;
	if(count!=nullptr)
		++(*count);
	return *this;
}


template<typename T>
SmartPointer<T>& SmartPointer<T>::operator=(SmartPointer p1)
{
	rest();
	if (p1.isValidate()) {
		data = p1.data;
		count = p1.count;
		++(*count);
	}
	return *this;
}


template<typename T>
void SmartPointer<T>::rest()
{
	
	if (data != nullptr)
	{
		_ASSERT(count != nullptr);
		--(*count);
		if (*count == 0)
		{
			delete data;
			delete count;
		}
	}
	data = nullptr;
	count = nullptr;
}



