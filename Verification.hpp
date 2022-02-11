#include "ListNode.hpp"

class Verification {
  public:
  Verification() {}
  
  bool verify(ListNode* userOutput, ListNode* referenceOutput, string& developer) {
    ListNode* entry = userOutput;
    ListNode* refEntry = referenceOutput;
    
    bool success = true;
    
    while (refEntry != nullptr) {
      if (entry == nullptr || refEntry->val != entry->val) {
        success = false;
        break;
      }
      refEntry = refEntry->next;
      entry = entry->next;
    }
    if (entry != nullptr) {
      success = false;
    }
    
    refEntry = referenceOutput;
    entry = userOutput;
    
    if (not success) {
      cout << "Expected output:" << endl << "[";
      while (refEntry != nullptr) {
        cout << refEntry->val;
        refEntry = refEntry->next;
        if (refEntry) {
          cout << ",";
        }
      }
      cout << "]" << endl;
      
      cout << "Program output:" << endl << "[";
      while (entry != nullptr) {
        cout << entry->val;
        entry = entry->next;
        if (entry) {
          cout << ",";
        }
      }
      cout << "]" << endl;
    } 
    return success;
  }
};