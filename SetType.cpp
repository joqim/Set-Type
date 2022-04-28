#include <string>
#include <iostream>
#include "SetType.h"

using namespace std;

template<class T>
SetType<T>::SetType() {
    // Create an array of forward_lists and initially set to an empty forward_list
    buckets = new forward_list<T>[DEFAULT_BUCKETS];

    numBuckets = DEFAULT_BUCKETS;
    currBucket = 0;
    numElems = 0;
    iterCount = 0;
    maxLoad = DEFAULT_LOAD_FACTOR;
}

template<class T>
SetType<T>::SetType(SetType &otherSet) {
    // This should make use of copySet
    copySet(otherSet);
}

template<class T>
SetType<T>::~SetType() {
    delete [] buckets;
}

template<class T>
SetType<T> SetType<T>::operator+(T elem) {
    SetType<T> result;

    result = *this;
    result.Add(elem);

    return result;
}

template<class T>
SetType<T> SetType<T>::operator-(T elem) {
    SetType<T> result;

    result = *this;
    result.Remove(elem);
    return result;
}

template<class T>
SetType<T> SetType<T>::operator+(SetType& otherSet) {
    SetType<T> result;

    ResetIterator();
    //first add elements from original set Reset Iterator
    for (int i = 0; i< Size(); i++) {
        T item = GetNextItem();
        result.Add(item);
    }

    otherSet.ResetIterator();
    for(int i=0; i<Size(); i++) {
        T item = otherSet.GetNextItem();
        result.Add(item);
    }

    return result;
}

template<class T>
SetType<T> SetType<T>::operator*(SetType& otherSet) {
    SetType<T> result;
    // reset the iterator of this Set to start
    ResetIterator();

    // loop over the number of elements of this set
    for(int i=0;i<Size();i++)
    {
        T item = GetNextItem(); // get the next item from the iterator
        if(otherSet.Contains(item)) // otherSet contains item, insert item into result using Add
            result.Add(item);
    }

    return result;
}

template<class T>
SetType<T> SetType<T>::operator-(SetType& otherSet) {
    SetType<T> result;

    ResetIterator();
    for (int i=0; i<Size(); i++) {
        T nextItem = GetNextItem();
        if(!otherSet.Contains(nextItem)) {
            result.Add(nextItem);
        }
    }
    return result;
}

template<class T>
T SetType<T>::GetNextItem() {
    // Returns the current item and then move to the next item
    T item;

    // Your code here
    if(iterCount == numElems) throw IteratorOutOfBounds();

    while(bucketIter == buckets[currBucket].end()) {
        currBucket++;
        bucketIter = buckets[currBucket].begin();
    }
    item = *bucketIter;
    iterCount++;
    ++bucketIter;

    return item;
}

template<class T>
int SetType<T>::GetHashIndex(const T& key) {
    // This is done... No touching!
    unordered_map<T,T> mapper;
    typename unordered_map<T,T>::hasher hashFunction = mapper.hash_function();
    return static_cast<int>(hashFunction(key) % numBuckets);
}

template<class T>
void SetType<T>::SetMaxLoad(double max) {
    // This function is done
    if (max < 0.1)
        maxLoad = 0.1;
    else
        maxLoad = max;
}

template<class T>
SetType<T>& SetType<T>::operator=(SetType const &other) {
    cout<<"\n Inside equal operator overload";
    copySet(other);

    return *this;
}


template<class T>
void SetType<T>::Rehash(int newNumBuckets) {
    cout<<"\n Inside Rehash";
    SetType<T> rehashedSet(newNumBuckets);

    // Your code here
    // create a new set of newNumBuckets
    ResetIterator(); // reset the iterator to start

    // loop over the elements of this set, adding it to rehashedSet using the Add function
    for(int i=0;i<Size();i++)
    {
        rehashedSet.Add(GetNextItem());
    }

    *this = rehashedSet;
}

template<class T>
SetType<T>::SetType(int numBucks) {
    numBuckets = numBucks; // set number of buckets to numBucks
    // create an array of forward_list of size numBuckets
    buckets = new forward_list<T>[numBuckets];
    numElems = 0; // set number of elements to 0
    maxLoad = DEFAULT_LOAD_FACTOR; // set maxLoad to DEFAULT_LOAD_FACTOR
    currBucket = 0; // set current bucket for iterator to start bucket i.e 0
    iterCount = 0; // set iterCount to 0
    bucketIter = buckets[currBucket].begin(); // make bucketIter to point to start element of currBucket
}

template<class T>
void SetType<T>::Add(T elem) {
    cout<<"\n Inside Add "<<elem;
    if(!Contains(elem)) // validates that elem is not in the set
    {
        int bucket = GetHashIndex(elem); // get the index of the bucket where the elem should be inserted

        buckets[bucket].push_front(elem); // insert elem to front of bucket
        numElems++; // increment number of elements of set by 1

        if(LoadFactor() > maxLoad) // if load factor is greater than maxLoad, call Rehash with twice the number of existing buckets
            Rehash(2*numBuckets);
    }
}

template<class T>
void SetType<T>::Remove(T elem) {
    if(Contains(elem)) // validate elem is present in Set
    {
        int bucket = GetHashIndex(elem); // get the index of bucket where the elem is
        buckets[bucket].remove(elem); // remove elem from bucket
        numElems--; // decrement number of elements of set
    }
}

template<class T>
bool SetType<T>::Contains(T elem) {
    cout<<"\n Inside contains";
    // get the bucket where the elem should be
    int bucket = GetHashIndex(elem);
    // create an iterator to start of the bucket
    typename forward_list<T>::iterator itr = buckets[bucket].begin();

    // loop over the bucket till the end
    for(;itr != buckets[bucket].end(); ++itr)
    {
        if(*itr == elem) // elem found
            return true;
    }

    return false; // elem not found
}

template<class T>
void SetType<T>::MakeEmpty() {
    // loop over the buckets
    for(int i=0;i<numBuckets;i++)
    {
        buckets[i].clear(); // clear the ith bucket
    }

    numElems = 0; // set number of elements to 0
}

template<class T>
double SetType<T>::LoadFactor() const {
    // validate number of buckets is greater than 0
    if(numBuckets > 0)
        // load factor = current number of elements divided by number of buckets
        // cast numElems to double so that the result preserves decimal digits
        return ((double)numElems)/numBuckets;
    return 0; // if number of buckets is 0, return 0
}

template<class T>
void SetType<T>::ResetIterator() {
    currBucket = 0;
    iterCount = 0;
    bucketIter = buckets[currBucket].begin();
}

template<class T>
void SetType<T>::copySet(const SetType &otherSet) {
    if(this != &otherSet) // avoids self-assignment
    {
        MakeEmpty(); // empty the set
        //delete buckets; // delete the buckets

        // copy number of buckets, number of elements and max load from otherSet
        numBuckets = otherSet.numBuckets;
        numElems = otherSet.numElems;
        maxLoad = otherSet.maxLoad;

        // create array of numBuckets
        buckets = new forward_list<T>[numBuckets];

        // loop over the buckets of otherSet
        for (int i = 0; i < numBuckets; i++) {
            // insert the elements of ith bucket of otherSet to ith bucket of this set using insert_after
            buckets[i].insert_after(
                    buckets[i].before_begin(),
                    otherSet.buckets[i].begin(),
                    otherSet.buckets[i].end()
            );
        }

        // update current bucket and iterCount to 0
        currBucket = 0;
        iterCount = 0;
        bucketIter = buckets[currBucket].begin(); // point the iterator to beginning of first bucket
    }
}


