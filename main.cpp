#include <iostream>
#include "Deque.hpp"
using namespace fefu_laboratory_two;

int main(){

    std::vector<int> first = {1,2,3};

    Deque<int,Allocator<Node<int>>> a(first.begin(), first.end());

    Deque_iterator<int> it = a.end();

    Deque_reverse_iterator<Deque_iterator<int>> second(it);


    std::cout << *second;






}