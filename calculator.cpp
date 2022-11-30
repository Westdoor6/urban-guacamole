#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <ctype.h>

using namespace std;

bool string_is_numeric(string input)
{ 
  for(auto c : input)
    {
      if(!isdigit(c))
	return false;
    }
  return true;
}

bool char_is_operator(char input)
{
  return (input == '%' ||input == '/' ||input == '+' ||input == '-' || input == '*');
}

int get_user_input(string test_in)
{
  if(string_is_numeric(test_in))
    {
      return atoi(test_in.c_str());
    }
  else
    {
      cout << "Try again, that's not a number!" << endl;
      return 0;
    }
}

int calc(int first, int second, char oper)
{
  switch(oper)
    {
    case('%'):
      return first % second;
      break;
    case('/'):
      return first / second;
      break;
    case('+'):
      return first + second;
      break;
    case('-'):
      return first - second;
      break;
    case('*'):
      return first * second;
      break;
  }
  return 0;
}

int main()
{
  
  int num_1 = 0, num_2 = 0;
  char oper = '0';
  
  while(num_1 == 0)
    {
      string test_in;
      cout << "Please enter the first number of the equation" << endl;
      cin >> test_in;
      
      num_1 = get_user_input(test_in);
    }
  
  while(num_2 == 0)
    {
      string test_in;
      cout << "Please enter the second number of the equation" << endl;
      cin >> test_in;
      
      num_2 = get_user_input(test_in);
    }
  
  while(oper == '0')
    {
      char test_in;
      cout << "Please enter the operator of the equation" << endl;
      cin >> test_in;
      
      if(char_is_operator(test_in))
	oper = test_in;
      else
	cout << "Not a valid operator!" << endl;
    }
  
  int ans = calc(num_1, num_2, oper);
  cout << "Result of operation " << num_1 << " " <<oper << " " << num_2 << " is " << ans << endl;
}
