// 11327132李晨希 11327141羅元廷
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
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
string getFileNumber() {
  string input;
    
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
int GetNumWithRange(int min, int max) {
  int num = 0;
    
  while (!(cin >> num) || num < min || num > max) {
    if (cin.fail()) {
      HandleInvalidInput("");
    } else {
      cout << "\n### The number must be in [" << min << "," << max << "] ###" << endl;
    }
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
  QueueNode* rear;
  int size;
  int maxSize;
  
public:
  Queue(int max = 3) : front(nullptr), rear(nullptr), size(0), maxSize(max) {}
  ~Queue() {
    while (!isEmpty()) {
      dequeue();
    }
  }
  
  bool isFull() {
    return size >= maxSize;
  }
  
  bool isEmpty() const {
    return size == 0;
  }
  
  int getSize() const {
    return size;
  }
  
  bool enqueue(Order order) {
    if (isFull()) return false;
    
    QueueNode* newNode = new QueueNode(order);
    
    if (rear == nullptr) {
      front = rear = newNode;
    } else {
      rear->next = newNode;
      rear = newNode;
    }
    
    size++;
    return true;
  }
  
  Order dequeue() {
    if (isEmpty()) {
      throw runtime_error("Queue is empty!");
    }
    
    QueueNode* temp = front;
    Order order = temp->order;
    
    front = front->next;
    if (front == nullptr) {
      rear = nullptr;
    }
    
    delete temp;
    size--;
    return order;
  }
  
  Order peek() const {
    if (isEmpty()) {
      throw runtime_error("Queue is empty!");
    }
    return front->order;
  }
  
};

struct Record {
    int OID;
    int time;      // Abort 或 Departure
    int Delay;
    int CID;       // 廚師編號
};

class OrderManager {
private:
  Order* orders;
  int count;
  
public:
  OrderManager() : orders(nullptr), count(0) {}
  
  ~OrderManager() {
    if (orders != nullptr) {
      delete[] orders;
    }
  }
  
  int compare(Order a, Order b) {
    if (a.Arrival != b.Arrival)
      return a.Arrival - b.Arrival;
    return a.OID - b.OID;
  }
  void shellSort() {
    // gap 序列：n/2, n/4, n/8, ..., 1
    for (int gap = count/2; gap > 0; gap /= 2) {
      // insertion sort with gap
      for (int i = gap; i < count; i++) {
        Order temp = orders[i];
        int j;
        for (j = i; j >= gap && compare(orders[j-gap], temp) > 0; j -= gap) {
          orders[j] = orders[j-gap];
        }
        orders[j] = temp;
      }
    }
  }
    
  bool readFile(const string& fileName, long long& readTime) {
      auto start = chrono::high_resolution_clock::now();
      
      ifstream file(fileName);
      if (!file.is_open()) {
        cout << "\n### " << fileName << " does not exist! ###" << endl;
        return false;
      }
      
      // 跳過標題列
      string header;
      getline(file, header);
      
      // 先計算有多少筆訂單
      count = 0;
      string line;
      while (getline(file, line)) {
        if (!line.empty()) count++;
      }
      
      // 動態配置陣列
      orders = new Order[count];
      
      // 重新讀取資料
      file.clear();
      file.seekg(0);
      getline(file, header); // 跳過標題
      
      for (int i = 0; i < count; i++) {
        file >> orders[i].OID >> orders[i].Arrival
        >> orders[i].Duration >> orders[i].Timeout;
      }
      
      file.close();
      
      auto end = chrono::high_resolution_clock::now();
      readTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
      
      return orders;
    }
    
  void writeFile(const string& fileName, long long& writeTime) {
      auto start = chrono::high_resolution_clock::now();
      
      ofstream file(fileName);
      
      // 寫入標題
      file << "OID\tArrival\tDuration\tTimeOut" << endl;
      
      // 寫入資料
      for (int i = 0; i < count; i++) {
        file << orders[i].OID << "\t"
        << orders[i].Arrival << "\t"
        << orders[i].Duration << "\t"
        << orders[i].Timeout << endl;
      }
      
      file.close();
      
      auto end = chrono::high_resolution_clock::now();
      writeTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    }
    
  void display() {
    cout << "\tOID\tArrival\tDuration\tTimeOut" << endl;
    for (int i = 0; i < count; i++) {
      cout << "(" << (i + 1) << ")\t"
      << orders[i].OID << "\t"
      << orders[i].Arrival << "\t"
      << orders[i].Duration << "\t"
      << orders[i].Timeout << endl;
    }
  }
    
};

class Chef {
 private:
  int idleTime;
  Queue* queue;
    
public:
    Chef(int chefId) : idleTime(0), queue(new Queue(3)) {}
    
    ~Chef() {
        delete queue;
    }
    
    int getIdleTime() const {
        return idleTime;
    }
    
    void setIdleTime(int time) {
        idleTime = time;
    }
    
    bool isIdle(int currentTime) const {
        return idleTime <= currentTime && queue->isEmpty();
    }
    
    bool canAcceptOrder() const {
        return !queue->isFull();
    }
    
    int getQueueSize() const {
        return queue->getSize();
    }
    
    bool enqueueOrder(Order order) {
        return queue->enqueue(order);
    }
    
    Order dequeueOrder() {
        return queue->dequeue();
    }
    
    bool hasWaitingOrders() const {
        return !queue->isEmpty();
    }
};

// 任務一
void task1() {
  cout << "Input a file number (e.g., 401, 402, 403, ...): " << endl;
  string fileNum = getFileNumber();
  
  string inputFile = "input" + fileNum + ".txt";
  string outputFile = "sorted" + fileNum + ".txt";
    
  OrderManager manager;
    
  // 讀取檔案
  long long readTime = 0;
  if (!manager.readFile(inputFile, readTime)) {
    cout << "\n### " << inputFile << " does not exist! ###" << endl;
    return;
  }
    
  // 顯示原始資料
  manager.display();
    
  // 排序
  auto sortStart = chrono::high_resolution_clock::now();
  manager.shellSort();
  auto sortEnd = chrono::high_resolution_clock::now();
  long long sortTime = chrono::duration_cast<chrono::microseconds>(sortEnd - sortStart).count();
    
  // 寫入檔案
  long long writeTime = 0;
  manager.writeFile(outputFile, writeTime);
  
  // 顯示執行時間
  cout << "Reading data: " << readTime << " us." << endl;
  cout << "Sorting data: " << sortTime << " us." << endl;
  cout << "Writing data: " << writeTime << " us." << endl;
}

int main() {
    int cmd;
    
    do {
        ShowMenu();
        cin >> cmd;
        
        switch (cmd) {
            case 1:
                task1();
                break;
            case 0:
                cout << "\nGoodbye!" << endl;
                break;
            default:
                cout << "\n### Invalid command! ###" << endl;
                break;
        }
    } while (cmd != 0);
    
    return 0;
}
