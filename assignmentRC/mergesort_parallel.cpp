#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <thread> // For parallel

#define DEBUG 0
#define PARALLEL_THRESHOLD 50000 // Use parallel sorting when the array segment is larger than this value

void generateMergeSortData (std::vector<int>& arr, size_t n) {
  for (size_t  i=0; i< n; ++i) {
    arr[i] = rand();
  }
}

void checkMergeSortResult (std::vector<int>& arr, size_t n) {
  bool ok = true;
  for (size_t  i=1; i<n; ++i)
    if (arr[i]< arr[i-1])
      ok = false;
  if(!ok)
    std::cerr<<"notok"<<std::endl;
}

void merge(int * arr, size_t  l, size_t  mid, size_t r, int* temp) {
#if DEBUG
  std::cout<<l<<" "<<mid<<" "<<r<<std::endl;
#endif

  if (l == r) return;
  if (r-l == 1) {
    if (arr[l] > arr[r]) {
      size_t temp = arr[l];
      arr[l] = arr[r];
      arr[r] = temp;
    }
    return;
  }

  size_t i, j, k;
  size_t n = mid - l;

  // init temp arrays
  for (i=0; i<n; ++i)
    temp[i] = arr[l+i];

    i = 0;    // temp left half
    j = mid;  // right half
    k = l;    // write to

    // merge
  while (i<n && j<=r) {
     if (temp[i] <= arr[j] ) {
       arr[k++] = temp[i++];
     } else {
       arr[k++] = arr[j++];
     }
  }

  // exhaust temp
  while (i<n) {
    arr[k++] = temp[i++];
  }
}

void mergesort(int * arr, size_t l, size_t r, int* temp) {
  if (l < r) {
    size_t mid = (l+r)/2;

    if ((r - l) > PARALLEL_THRESHOLD) {
      // Parallel sorting left and right
      std::thread left_thread(mergesort, arr, l, mid, temp);
      std::thread right_thread(mergesort, arr, mid+1, r, temp);
      left_thread.join();
      right_thread.join();
    } else {
      // sequential execution
      mergesort(arr, l, mid, temp);
      mergesort(arr, mid+1, r, temp);
    }

    merge(arr, l, mid+1, r, temp);
  }
}

int main (int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr<<"Usage: "<<argv[0]<<" <n>"<<std::endl;
    return -1;
  }

  // exhaust temp
  size_t n = atol(argv[1]);

    // get arr data
  std::vector<int> arr (n);
  generateMergeSortData (arr, n);

#if DEBUG
  for (size_t i=0; i<n; ++i) 
    std::cout<<arr[i]<<" ";
  std::cout<<std::endl;
#endif

  // begin timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  std::vector<int> temp (n);
  // sort
  mergesort(&(arr[0]), 0, n-1, &(temp[0]));

  // end timing
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  // display time to cerr
  std::cerr<<elpased_seconds.count()<<std::endl;
  checkMergeSortResult (arr, n);

#if DEBUG
  for (size_t i=0; i<n; ++i) 
    std::cout<<arr[i]<<" ";
  std::cout<<std::endl;
#endif

  return 0;
}
