/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdint.h>
#include <vector>

using namespace std;

#include "ListNode.hpp"
class Solution;
class Verification;

#include "Solution.hpp"
#include "refSolution.hpp"
#include "Verification.hpp"


#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#define bad_news(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int32_t numLists = 10000;
int32_t minEntryValue = -10000;
int32_t maxEntryValue = 10000;

int32_t minEntriesPerList = 3;
int32_t maxEntriesPerList = 10;

bool printInput = false;

vector<ListNode*> vecList;
ListNode *ret = nullptr;

ListNode *retRef = nullptr;
vector<ListNode*> refVecList;
int64_t userTime = 0L;

void runTestProgram();
void runReferenceProgram();
int64_t refTime = 0L;

#define STACKINT64  (256*1024)
static ucontext_t uctx_main, uctx_func1, uctx_func2;
int64_t func1_stack[STACKINT64]; // 2M stack
int64_t func2_stack[STACKINT64]; // 2M stack
int64_t userStackUsageKbytes = 0L;
int64_t refStackUsageKbytes = 0L;
  


int main(int argc, char** argv)
{
  string developer;
  
  if (argc == 2) {
    developer = argv[1];
  } else {
    developer = "Unnamed ";
  }
  
  int32_t entrySpan = maxEntryValue - minEntryValue;
  
  cout << "Testing code written by " << developer << endl;
  
  srand(0);
  
  //numLists = 100; //TODO remove me!
  
  for (int32_t i = 0; i < numLists; ++i) {
    int32_t numEntries = rand();
    numEntries %= maxEntriesPerList;
    if (numEntries < minEntriesPerList) {
      numEntries += minEntriesPerList;
    }
    
    vector<int> list;
    for (int32_t j = 0; j < numEntries; ++j) {
      list.push_back((rand() % entrySpan) + minEntryValue);
    }
    
    sort(list.begin(), list.end());
    ListNode* head = nullptr;
    ListNode* tail = nullptr;

    ListNode* refHead = nullptr;
    ListNode* refTail = nullptr;
    
    for (const auto& entry : list) {
      ListNode* ln = new ListNode(entry);
      ListNode* rln = new ListNode(entry);
      
      if (head == nullptr) {
        head = tail = ln;
        refHead = refTail = rln;
      } else {
        tail->next = ln;
        tail = ln;
        refTail->next = rln;
        refTail = rln;
      }
    }
    vecList.push_back(head);
    refVecList.push_back(refHead);
  }

  if (printInput) {
    for (const auto& list : vecList) {
      for (ListNode* ln = list; ln != nullptr; ln = ln->next) {
        cout << " " << ln->val;
      }
      cout << endl;
    }
  }
  
  int32_t actualMeasured = 0;
  for(auto ln = vecList[0]; ln != nullptr; ln = ln->next) {
    ++actualMeasured;
  }




  for (int64_t j = 0; j < STACKINT64; ++j) {
    func1_stack[j] = 0xdeadbeefdeadbeef;
    func2_stack[j] = 0xdeadbeefdeadbeef;
  }
  
  if (getcontext(&uctx_func1) == -1) {
    bad_news("Failed retrieving current context (getcontext())");
  }
  uctx_func1.uc_stack.ss_sp = func1_stack;
  uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
  uctx_func1.uc_link = &uctx_main;
  makecontext(&uctx_func1, runTestProgram, 0);

   if (getcontext(&uctx_func2) == -1) {
        bad_news("Failed retrieving current context (getcontext())");
   }
  uctx_func2.uc_stack.ss_sp = func2_stack;
  uctx_func2.uc_stack.ss_size = sizeof(func2_stack);
  /* Successor context is func1() */
  uctx_func2.uc_link = &uctx_func1;
  makecontext(&uctx_func2, runReferenceProgram, 0);

  cout << "Running reference program." << endl;
  if (swapcontext(&uctx_main, &uctx_func2) == -1) {
    bad_news("Failed calling reference program context.");
  }


  cout << endl << "======================================================" << endl << endl;

  bool success = Verification().verify(ret, retRef, developer);
  
  if (success) {
    cout << "Time taken to execute program developed by " << developer << ": " << userTime << " usec. Stack space used : " << userStackUsageKbytes << " KBytes" << endl;
    cout << "Time taken to execute reference solution " << refTime << " usec. Stack space used : " << refStackUsageKbytes << " KBytes" << endl;
  }
  cout << endl << "======================================================" << endl << endl;
  return 0;
}

void runTestProgram() {
  
  cout << "In user test code" << endl;
  auto start = chrono::steady_clock::now();
  ret = Solution().mergeKLists(vecList);
  auto end = chrono::steady_clock::now();
  
  userTime = chrono::duration_cast<chrono::microseconds>(end-start).count();
  int64_t k = 0L;
  for (; k < STACKINT64; ++k) {
    if (func1_stack[k] != 0xdeadbeefdeadbeef) {
      break;
    }
  }
  userStackUsageKbytes = ((STACKINT64-k)*sizeof(int64_t))/1024;
  if (swapcontext(&uctx_func1, &uctx_func2) == -1) {
    bad_news("Failed switching over to main program context.");
  }
}
  
void runReferenceProgram() {
  
  cout << "In reference code" << endl;
  auto start = chrono::steady_clock::now();
  retRef = refSolution().mergeKLists(refVecList);
  auto end = chrono::steady_clock::now();
  
  refTime = chrono::duration_cast<chrono::microseconds>(end-start).count();
  int64_t k = 0L;
  for (; k < STACKINT64; ++k) {
    if (func2_stack[k] != 0xdeadbeefdeadbeef) {
      break;
    }
  }
  refStackUsageKbytes = ((STACKINT64-k)*sizeof(int64_t))/1024;
  if (swapcontext(&uctx_func2, &uctx_func1) == -1) {
    bad_news("Failed switching over to user program context.");
  }
}
