#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__

#include <queue>


template<class T, class Container, class Compare>
class PriorityQueue: public std::priority_queue<T, Container, Compare> {
  public:
    inline PriorityQueue(const Compare&);

    inline void remove(const T&);
};


template<class T, class Container, class Compare>
inline PriorityQueue<T, Container, Compare>::PriorityQueue(const Compare& comp): std::priority_queue<T, Container, Compare>(comp) {}


template<class T, class Container, class Compare>
inline void PriorityQueue<T, Container, Compare>::remove(const T& item) {
  auto it = std::find(this->c.begin(), this->c.end(), item);

  if (it != this->c.end()) {
    this->c.erase(it);
    std::make_heap(this->c.begin(), this->c.end(), this->comp);
  }
}


#endif
