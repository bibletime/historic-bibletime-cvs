#ifndef SCOPED_RESOURCE_H_INCLUDED
#define SCOPED_RESOURCE_H_INCLUDED

/**
* The util namespace should take all classes which are of a generic type,
* used to perform common tasks which are not BibleTime-specific. See
* @ref scoped_resource for an example.
*/
namespace util
{
/**
* A class template, scoped_resource, designed to
* implement the Resource Acquisition Is Initialization (RAII) approach
* to resource management. scoped_resource is designed to be used when
* a resource is initialized at the beginning or middle of a scope,
* and released at the end of the scope. The template argument
* ReleasePolicy is a functor which takes an argument of the
* type of the resource, and releases it.
*
* Usage example, for working with files:
*
* @code
* struct close_file { void operator(int fd) const {close(fd);} };
* ...
* {
*    scoped_resource<int,close_file> file(open("file.txt",O_RDONLY));
*    read(file, buf, 1000);
* } // file is automatically closed here
* @endcode
*/
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
	
  /**
  * Please put some docs on the important member functions like here.
	*
	* @ param res This is the bla #1.
	* @ param rel This is the bla #2.
	* @ret This funtion returns bla.
  */
	explicit scoped_resource(resource_type res,release_type rel=release_type())
			: resource(res), release(rel) {}

  /**
  * The destructor is the main point in this class. It takes care of proper
	* deletion of the resource, using the provided release policy.
	*/
	~scoped_resource()
	{
		release(resource);
	}

  /**
  * This operator makes sure you can access and use the scoped_resource
	* just like you were using the resource itself.
	*/
	operator resource_type() const { return resource; }

  /**
  * Please put some docs on the important member functions like here.
	*
	* @ret This funtion returns bla.
  */
  resource_type get() const { return resource; }

	/**
  * only use this if resource_type is a pointer
  */
	resource_type operator->() const { return resource; }

};

/**
* A helper policy for scoped_ptr.
* It makes sure the Object is deleted with delete on block exit.
*/
struct delete_item {
	template<typename T>
	void operator()(T*& p) const { delete p; p = 0; }
};
/**
* A helper policy for scoped_array.
* It makes sure the array is deleted with delete[] on block exit.
*/
struct delete_array {
	template<typename T>
	void operator()(T*& p) const { delete [] p; p = 0; }
};

/**
* A fairly simple class which implements RAII. See @ref scoped_resource
* for a thorough description of this matter. Some functors and derived classes
* (in the absence of templated typedefs), are also supplied.
*
* Usage example:
* @code
* {
*    scoped_ptr<char> ptr(new char);
*    ...use ptr as you would a normal char*...
* } // ptr is automatically deleted here
* @endcode
*/
template<typename T>
struct scoped_ptr : public scoped_resource<T*,delete_item>
{
	scoped_ptr(T* p) : scoped_resource<T*,delete_item>(p) {}
};
/**
* A fairly simple class which implements RAII, very similar to
* @ref scoped_ptr, only that it works for arrays. Some functors and derived classes
* (in the absence of templated typedefs), are also supplied.
*
* Usage example:
* @code
* {
*    scoped_array<char> ptr(new char[50]);
*    ...use ptr as you would a normal char*...
* } // ptr is automatically deleted here
* @endcode
*
* note that the syntax scoped_array<char> ptr = new char[50]; is illegal
* and should not be used, scoped_array<char> ptr(new char[50]); is the
* correct way to initialize
*/
template<typename T>
struct scoped_array : public scoped_resource<T*,delete_array>
{
	scoped_array(T* p) : scoped_resource<T*,delete_array>(p) {}
};

}

#endif
