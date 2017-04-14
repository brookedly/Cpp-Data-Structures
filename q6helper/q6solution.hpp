#ifndef Q6SOLUTION_HPP_
#define Q6SOLUTION_HPP_


#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>                 // std::swap
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "q6utility.hpp"


////////////////////////////////////////////////////////////////////////////////

//Problem 1

//Write this function
template<class T>
void selection_sort(LN<T>* l) {
    for (LN<T> *head = l; head != nullptr; head = head->next)
    {
        LN<T>* head_of_min = head;
        for (LN<T>* front = head->next; front != nullptr; front = front->next)
        {
            if (front->value < head_of_min->value)
                head_of_min = front;
        }
        std::swap(head->value, head_of_min->value);
    }
}


////////////////////////////////////////////////////////////////////////////////

//Problem 2

//Write this function
template<class T>
void merge(T a[], int left_low,  int left_high,
                    int right_low, int right_high) {
    int left = left_low;
    int right = right_low;
    int size = (right_high-left_low+1);
    int arr[size];
    for (int x = 0; x <= size; x++)
    {
        if (left > left_high )
            arr[x] = a[right++];
        else if (right > right_high )
            arr[x] = a[left++];
        else if (a[left] < a[right])
                arr[x] = a[left++];
        else {
            arr[x] = a[right++];
            }
        }
    for (int x = left_low; x < size; x++)
    {
        a[x] = arr[x];
    }
}


////////////////////////////////////////////////////////////////////////////////

//Problem 3

int select_digit (int number, int place)
{return number/place % 10;}


//Write this function
void radix_sort(int a[], int length) {
    ics::ArrayQueue<int> buckets[10];
    for (int place = 1; place <= 100000; place *= 10)
    {
        int a_num = 0;
        for (int value = 0; value < length; value++){
            int at = select_digit(a[value], place);
            buckets[at].enqueue(a[value]);}
        for (int b = 0; b <= 9; b++) {
            while (!buckets[b].empty()) {
                a[a_num] = buckets[b].dequeue();
                a_num++;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////

//Problem 4

//Write this function

//Test how well a partition function (choose_pivot_index: see last and median3) works.
//Test is on an array of the values 0..length-1, randomly shuffled num_tests times
//Returns the average size of the larger of the left and right partitions.
//At best the array is cut in half, yielding the biggest side as 0.5 (50%)
double test_partition(int length, int num_tests, int (*choose_pivot_index) (int a[], int low, int high)) {
//    double percent;
    int counter = 0;
    int p[length];
    for (int x = 0; x < length; x++ ){
        p[x] = x;
    }
    for (int x = 0; x < num_tests; x++) {
        std::random_shuffle(&p[0], &p[length]);

        int piv = choose_pivot_index(p, 0, length - 1);
        int part = partition(p, 0, length - 1, piv);
        if (part > (length - part) ){
            counter = counter + part;
        }
        else {
            counter = counter + (length - part);
        }
    }
    return double(counter/num_tests)/double(length);
}


#endif /* Q6SOLUTION_HPP_ */
