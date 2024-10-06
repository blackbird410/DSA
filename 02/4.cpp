#include <iostream>
#include <cstring>

#define SIZE 100

using namespace std;

class Queue
{
public:
  Queue()
  {
      top = 0;
      bot = -1;
    count = 0;
  }

  int enqueue(int data)
  {
    if (count == SIZE)
        return -1;
    this->data[top] = data;

    top = (top + 1) % SIZE;  
    count++;
    return 1;
  }

  int *dequeue()
  {
    if (count == 0)
        return NULL;
    bot = (bot + 1) % SIZE;  
    count--;
    return &data[bot];
  }

private:
  int data[SIZE];
  int top, bot, count;
};

int main()
{
  int data, *temp;
  char command[50];
  Queue *queue = new Queue();
  while(1)
  {
      cin>>command;
      if(strcmp(command, "exit") == 0)
      {
          break;
      }
      else if(strcmp(command, "enqueue") == 0)
      {
          cout<<"Please input a integer data:";
          cin>>data;
          if(queue->enqueue(data) == 1)
          {
              cout<<"Successfully enqueue data "<<data<<" into queue."<<endl;
          }
          else
          {
              cout<<"Failed to enqueue data into queue."<<endl;
          }
      }
      else if(strcmp(command, "dequeue") == 0) 
      {
          temp = queue->dequeue();
          if(temp == NULL)
          {
              cout<<"Failed to dequeue a data from queue.\n";
          }
          else
          {
              cout<<"Dequeue data "<<*temp<<" from queue."<<endl;
          }
      }
  }

  delete queue;
  return 0;
}

