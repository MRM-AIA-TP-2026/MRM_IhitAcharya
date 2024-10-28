#include <iostream>
using namespace std;

int main(int argc, char** argv) {
  
  int x = stoi(argv[1]);
  
  //add code below this line
if ((0<=x)&&(x<=25) || (75<=x)&&(x<=100))
cout << x << " is between 0 and 25 or 75 and 100";


  //add code above this line
  
  return 0;
  
}
