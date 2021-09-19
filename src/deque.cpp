/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){
    n1 = 0;  //leftmost index
    n2 = -1;  //rightmost index
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    n2 += 1;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T left = peekL();
    n1 += 1;

    // one item case
    if (isEmpty()) {
        n1 = 0;
        n2 -= 1;
        data.resize(0);
        return left;
    }
    // all item in data can fit in empty chunk of spaces before it
    if (n2 - n1 + 1 <= n1) {
        vector<T> temp;
        for (int i = n1; i <= n2; i++) temp.push_back(data[i]);
        n1 = 0;
        data.resize(temp.size());
        n2 = data.size() - 1;
        for (int i = n1; i <= n2; i++) data[i] = temp[i];    
    }
    
    return left;
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T right = peekR();
    data.pop_back();
    n2 -= 1;

    if (n2 < n1) return right;
   
    // all item in data can fit in empty chunk of spaces before it
    if (n2 - n1 + 1 <= n1) {
        vector<T> temp;
        for (int i = n1; i <= n2; i++) temp.push_back(data[i]);
        n1 = 0;
        data.resize(temp.size());
        n2 = data.size() - 1;
        for (int i = n1; i <= n2; i++) data[i] = temp[i];    
    }
    
    return right;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[n2];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n1 > n2;
}
