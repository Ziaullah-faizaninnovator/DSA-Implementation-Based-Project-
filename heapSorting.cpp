#include<iostream>
#include<cstring>
#include<iomanip>
#include<vector>
#include<algorithm>
using namespace std;

class Heap{
    int *heap;
    int cap;
    public:
    Heap(int s=0){
        cap=s;
        heap=new int[cap];
        for(int i=0; i<cap; i++){
           heap[i]=0;
        }
    }
};
void swap(int &a,int &b){
    int temp=a;
    a=b;
    b=temp;
}
void heapify(int *arr,int n,int i){
    int largest=i;
    int leftChild=2*i+1;
    int rightChild=2*i+2;
    if(leftChild<n && arr[leftChild]<arr[largest]){
        largest=leftChild;
    }
     if(rightChild<n && arr[rightChild]<arr[largest]){
        largest=rightChild;
    }
    if(largest!=i){
        swap(arr[i],arr[largest]);
        heapify(arr,n,largest);
    }
}
void heapSort(int *arr,int n){
    for(int i=n/2-1; i>=0; i--){
        heapify(arr,n,i);
    }
    for(int i=n-1; i>0; i--){
        swap(arr[0],arr[i]);
        heapify(arr,i,0);
    }
}
void display(int *arr,int n){
    for(int i=0; i<n; i++){
        cout<<arr[i]<<" ";
    }
}

int main() {
  Heap h(10);  
  int arr[3]={3,2,4};
  display(arr,3);
  cout<<endl;
  heapSort(arr,3);
  display(arr,3);


    return 0;
}