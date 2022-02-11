
void adjustChild(int32_t begin, int32_t childIndex, ListNode** array, int32_t& numEntries) {
    if (childIndex < 1) {
        return;
    }
    int32_t parentIndex = (childIndex - 1) / 2;
    while ((begin + childIndex) != begin) {
        if (array[begin + childIndex]->val < array[begin + parentIndex]->val) {
            ListNode* temp = array[begin + childIndex];
            array[begin + childIndex] = array[begin + parentIndex];
            array[begin + parentIndex] = temp;
            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        } else {
            break;
        }
    }
}

void adjustRoot(int32_t begin, ListNode** array, int32_t& numEntries) {
    int32_t maxIndex = numEntries-1;
    if (maxIndex == 0) {
        return;
    }
    int32_t parentIndex = 0;

    while (parentIndex < maxIndex) {
        int32_t leftIndex = (parentIndex * 2) + 1;
        if (leftIndex > maxIndex) {
            return;
        }
        int32_t rightIndex = (parentIndex * 2) + 2; 
        rightIndex = rightIndex > maxIndex ? leftIndex : rightIndex;
        bool swapped = false;
        if (rightIndex != leftIndex) {
            if (array[begin+rightIndex]->val < array[begin+leftIndex]->val) {
                if (array[begin+rightIndex]->val < array[begin+parentIndex]->val) {
                    ListNode* temp = array[begin+rightIndex];
                    array[begin+rightIndex] = array[begin+parentIndex];
                    array[begin+parentIndex] = temp;
                    parentIndex = rightIndex;
                    swapped = true;
                }
            } else {
                if (array[begin+leftIndex]->val < array[begin+parentIndex]->val) {
                    ListNode* temp = array[begin+leftIndex];
                    array[begin+leftIndex] = array[begin+parentIndex];
                    array[begin+parentIndex] = temp;
                    parentIndex = leftIndex;
                    swapped = true;
                }
            }
        } else {
            if (array[begin+leftIndex]->val < array[begin+parentIndex]->val) {
                ListNode* temp = array[begin+leftIndex];
                array[begin+leftIndex] = array[begin+parentIndex];
                array[begin+parentIndex] = temp;
                parentIndex = leftIndex;
                swapped = true;
            }
        }
        if (not swapped) {
            break;
        }
    }
}


inline ListNode* popAndHeapify(ListNode** array, int32_t& numEntries) {
    ListNode* t = array[0];
    if (numEntries == 1) {
        numEntries = 0;
        return t;
    }

    array[0] = array[numEntries-1];
    --numEntries;
    adjustRoot(0, array, numEntries);
    return t;
}

inline ListNode* replaceFrontAndHeapify(ListNode* t, ListNode**array, int32_t& numEntries) {
    auto ret = array[0];
    array[0] = t;
    adjustRoot(0, array, numEntries);
    return ret;
}

class Solution {
public:
  static inline bool compareAscending(const ListNode* parent, const ListNode* child) {
      if (child->val < parent->val) {
          return true;
      }
      return false;
  }
  
  ListNode* mergeKLists(vector<ListNode*>& lists) {
      int32_t numLists = lists.size();
      int32_t emptyLists = 0;
      
      if (numLists == 0) {
          return nullptr;
      }
      
      ListNode* myVector[10000];
      int32_t numEntries = 0;

      for (auto& ln : lists) {
          if (ln) {
              myVector[numEntries++] = ln;
              ln = ln->next;
          } else {
              ++emptyLists;
          }
      }
      
      if (emptyLists == numLists) {
          return nullptr;
      }
      
      if (numEntries > 1) {
          int32_t i = 1;
          do {
              adjustChild(0, i++, myVector, numEntries);
          } while (i != numEntries);
      }

      ListNode* sortedList = nullptr;
      ListNode* sortedListTail = nullptr;
      
      while (numEntries != 0) {
          ListNode* temp;
          if (myVector[0]->next == nullptr) {
              temp = popAndHeapify(myVector, numEntries);
          } else {
              temp = replaceFrontAndHeapify(myVector[0]->next, myVector, numEntries);
          }
          
          if (sortedList == nullptr) {
              sortedList = sortedListTail = temp;
          } else {
              sortedListTail->next = temp;
              sortedListTail = temp;
          }

      }
      return sortedList;
  }
};