#ifndef SCOPED_RESOURCE_H_INCLUDED
#define SCOPED_RESOURCE_H_INCLUDED

/**
* scoped_resource.h: defines a class template, scoped_resource, designed to
* implement the Resource Acquisition Is Initialization (RAII) approach
* to resource management. scoped_resource is designed to be used when
* a resource is initialized at the beginning or middle of a scope,
* and released at the end of the scope. The template argument
* ReleasePolicy is a functor which takes an argument of the
* type of the resource, and releases it.
*
* scoped_resource is a fairly simple class, and should only be used
* in the scenario mentioned above. Some functors and derived classes
* (in the absence of templated typedefs), are also supplied.
*
* Usage examples:
*
* for files:
*
* struct close_file { void operator(int fd) const {close(fd);} };
* ...
* {
*    scoped_resource<int,close_file> file(open("file.txt",O_RDONLY));
*    read(file, buf, 1000);
* } // file is automatically closed here
*
* for pointers to arrays:
*
* {
*    scoped_array<char> ptr(new char[50]);
*    ...use ptr as you would a normal char*...
* } // ptr is automatically deleted here
*
* note that the syntax scoped_array<char> ptr = new char[50]; is illegal
* and should not be used, scoped_array<char> ptr(new char[50]); is the
* correct way to initialize
*/

namespace util
{

template<typename T,typename ReleasePolicy>
class scoped_resource
{
	T resource;
	ReleasePolicy release;
	
	//prohibited operations
	scoped_resource(const scoped_resource&);
	scoped_resource& operator=(const scoped_resource&);
public:
	typedef T resource_type;
	typedef ReleasePolicy release_type;
	
	explicit scoped_resource(resource_type res,release_type rel=release_type())
			: resource(res), release(rel) {}
	~scoped_resource()
	{
		release(resource);
	}

	operator resource_type() const { return resource; }

  resource_type get() const { return resource; }

	//only use this if resource_type is a pointer
	resource_type operator->() const { return resource; }

};

//helper policies
struct delete_item {
	template<typename T>
	void operator()(T*& p) const { delete p; p = 0; }
};

struct delete_array {
	template<typename T>
	void operator()(T*& p) const { delete [] p; p = 0; }
};

template<typename T>
struct scoped_ptr : public scoped_resource<T*,delete_item>
{
	scoped_ptr(T* p) : scoped_resource<T*,delete_item>(p) {}
};

template<typename T>
struct scoped_array : public scoped_resource<T*,delete_array>
{
	scoped_array(T* p) : scoped_resource<T*,delete_array>(p) {}
};

}

#endif
