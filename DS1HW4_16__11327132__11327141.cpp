// 11327132李晨希 11327141羅元廷
#include <iostream>
using namespace std;

// **************** Utility Functions ************************
void ShowMenu() {
  cout << "\n*** (^_^) Data Structure (^o^) ***\n";
  cout << "*** Find the Goal(s) in a Maze ***\n";
  cout << "* 0. Quit                        *\n";
  cout << "* 1. Find one goal               *\n";
  cout << "* 2. Find goal(s) as requested   *\n";
  cout << "* 3. How many goals?             *\n";
  cout << "* 4. Shortest path to one goal   *\n";
  cout << "**********************************\n";
  cout << "Input a command(0, 1, 2, 3, 4): ";
}
void HandleInvalidInput(const string &message) {
  cout << message << endl;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
int GetCommand() {
  if (cin.peek() == '\n') {
    cin.ignore();
  }
  
  int cmd;
  while (true) {
    if (cin >> cmd && cmd >= 0 && cmd <= 4) {
      char next = cin.peek();
      while (next == ' ' || next == '\t') {
        cin.get();
        next = cin.peek();
      }
      
      if (cin.peek() == '\n' || cin.peek() == EOF) {
        cin.ignore();
        return cmd;
      }
    }
      
    HandleInvalidInput("Command does not exist!");
    ShowMenu();
  }
}
string getFileNumber() {
  string input;
  cout << "\nInput a file number: ";
    
  while (true) {
    getline(cin, input);
    size_t start = input.find_first_not_of(" \t\r\n");
    size_t end = input.find_last_not_of(" \t\r\n");
        
    if (start != string::npos) {
      input = input.substr(start, end - start + 1);
        return input;  // 有內容就回傳
    }
  }
}
int GetNumWithRange(int min, int max) {
  cout << "\nNumber of G (goals): ";
  int num = 0;
    
  while (!(cin >> num) || num < min || num > max) {
    if (cin.fail()) {
      HandleInvalidInput("");
    } else {
      cout << "\n### The number must be in [" << min << "," << max << "] ###" << endl;
    }
    
    cout << "\nNumber of G (goals): ";
  }
    
  return num;
}
// ***********************************************************

struct Order {
    int OID;        // 訂單編號
    int Arrival;    // 下單時刻
    int Duration;   // 製作耗時
    int Timeout;    // 逾時時刻
};
struct QueueNode {
    Order order;
    QueueNode* next;
};

class Queue {
private:
    QueueNode* front;
    QueueNode* back;
    int size;
    int maxSize;  // = 3
    
public:
    Queue(int max = 3);
    ~Queue();
    bool enqueue(Order order);
    Order dequeue();
    bool isEmpty();
    bool isFull();
    int getSize();
};

int compare(Order a, Order b) {
    if (a.Arrival != b.Arrival)
        return a.Arrival - b.Arrival;
    return a.OID - b.OID;
}
void shellSort(Order* orders, int n) {
    // gap 序列：n/2, n/4, n/8, ..., 1
    for (int gap = n/2; gap > 0; gap /= 2) {
        // insertion sort with gap
        for (int i = gap; i < n; i++) {
            Order temp = orders[i];
            int j;
            for (j = i; j >= gap && compare(orders[j-gap], temp) > 0; j -= gap) {
                orders[j] = orders[j-gap];
            }
            orders[j] = temp;
        }
    }
}

