#ifndef SET_TYPE
#define SET_TYPE

#include<string>
#include<iostream>
#include<forward_list>
#include<unordered_map>
using namespace std;

class IteratorOutOfBounds {
public:
    string Msg() {
        return "Iterator is out of bounds.  Nothing left to iterate over.";
    }
};

const int DEFAULT_BUCKETS = 10;
const double DEFAULT_LOAD_FACTOR = 1.0;

template<class T>
class SetType
{
public:
    // Constructors
    //Function: Default constructor
    //Pre: -
    //Post: Initialize private variables
    SetType();

    //Function: Parameterized constructor
    //Pre: -
    //Post: Initialize private variables and set buckets with number as argument
    explicit SetType(int numBucks);

    //Function: Copied the set
    //Pre: -
    //Post: Copies from current set to result set
    SetType(SetType& other);

    //Function: Destructor
    //Pre: -
    //Post: Deallocates the buckets memory
    ~SetType();

    //Function: Adds an element to set
    //Pre: Element should not already exist in the set
    //Post: Result set will contain the added element
    void Add(T elem);

    //Function: Removes an element from the set
    //Pre: Set must be declared
    //Post: Result set will not contain the element
    void Remove(T elem);

    //Function: Returns true if set contains the element
    //Pre: Set must be declared
    //Post: Returns boolean depending on if the element is present in the set
    bool Contains(T elem);

    //Function: Deletes every bucket in the structure
    //Pre: buckets are declared
    //Post: Post deletion, resets numElems to 0
    void MakeEmpty();

    //Returns value of numElems
    int Size() const {
        return numElems;
    }

    //Function: Updates LoadFactor based on the number of buckets
    double LoadFactor() const;

    //Function: Updates maxLoad if less than 0.1
    void SetMaxLoad(double max);

    //Function: Rehashes based on new number of buckets
    void Rehash(int newNumBuckets);

    //Function: Overloads + operator by adding elem to set
    SetType operator+(T elem);				      // Add

    //Function: Overloads - operator by removing an elem from set
    SetType operator-(T elem);				      // Remove

    //Function: Overloads + operator by union of 2 sets
    SetType operator+(SetType& otherSet);         // Union

    //Function: Overloads - operator by difference between two sets
    SetType operator-(SetType& otherSet);         // Difference

    //Function: Overloads * operator by intersection of 2 sets
    SetType operator*(SetType& otherSet);         // Intersection

    //Function: Overloads = operator by performing a deep copy from one set to another
    SetType& operator=(SetType const& otherSet);  // Assignment (does deep copy)

    //Function: Resets bucket variable values
    void ResetIterator();	// Reset iterator

    //Function: Retrieves the next item in the list
    T GetNextItem();

private:
    forward_list<T>* buckets;	// An array of forward_list's
                                // (each index is a forward_list)

    int numBuckets;     // total number of buckets

    int GetHashIndex(const T& key);    // Gets the hash index given the elem

    int numElems;       // total number of elements

    double maxLoad;     // load factor of the Set

    // This function is used by the
    // copy constructor and the assignment operator.
    void copySet(const SetType& otherSet);

    // Iterator variables
    int currBucket;											// What bucket is the iterator on?
    int iterCount;                                          // What element are we on?
    mutable typename forward_list<T>::iterator bucketIter;	// The iterator of the current bucket

    // Any other private functions and variables you want/need
};

#include "SetType.cpp"

#endif