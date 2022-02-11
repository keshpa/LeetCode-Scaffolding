#include <assert.h>
#include <iterator>
#include <stdint.h>
#include <string.h>

class refSolution {
public:
    #define MAXPOSITIVEVAL  10000
    #define MAXVALRANGE     ((2*MAXPOSITIVEVAL)+1) //from -10000 to +10000
    #define SUBRANGEVAL     256    // Track whether any buckets grouped by SUBRANGEVAL have non-null LinkNode*

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int16_t numLists = lists.size();
        
        ListNode* hashHead[MAXVALRANGE];
        ListNode* hashTail[MAXVALRANGE];
        int32_t     hashNodeCount[MAXVALRANGE];
        
        memset(hashHead, 0, sizeof(ListNode *)*MAXVALRANGE);
        memset(hashTail, 0, sizeof(ListNode *)*MAXVALRANGE);
        
        if (numLists == 0) {
            return nullptr;
        }
        
        int32_t numNodes = 0;
        int32_t rangeCounters[(MAXVALRANGE/SUBRANGEVAL)+1];
        memset(rangeCounters, 0, sizeof(int32_t)*((MAXVALRANGE/SUBRANGEVAL)+1));
        memset(hashNodeCount, 0, sizeof(int32_t)*(MAXVALRANGE));
        
        for (auto ln : lists) {
          while (ln) {
              ++numNodes;
              ListNode* next = ln->next;
              int16_t val = ln->val;
              ++rangeCounters[(val+MAXPOSITIVEVAL)/SUBRANGEVAL];
              if (hashHead[val+MAXPOSITIVEVAL] == nullptr) {
                  hashHead[val+MAXPOSITIVEVAL] = hashTail[val+MAXPOSITIVEVAL] = ln;
              } else {
                  hashTail[val+MAXPOSITIVEVAL]->next = ln;
                  hashTail[val+MAXPOSITIVEVAL] = ln;
              }
              ++hashNodeCount[val+MAXPOSITIVEVAL];
              ln->next = nullptr;
              assert(ln->visited == false);
              ln->visited = true;
              ln = next;
          }
        }
        
        ListNode* sortedList = nullptr;
        ListNode* sortedListTail = nullptr;

        for (int16_t i = 0; i < MAXVALRANGE;) {
            if (rangeCounters[i/SUBRANGEVAL] == 0) {
                i += SUBRANGEVAL;
                continue;
            }
            if (hashHead[i]) {
                if (sortedList == nullptr) {
                    sortedList = hashHead[i];
                    sortedListTail = hashTail[i];
                } else {
                    sortedListTail->next = hashHead[i];
                    sortedListTail = hashTail[i];
                }
                numNodes -= hashNodeCount[i];
            }
            ++i;
            if (numNodes == 0) {
                break;
            }
        }

        return sortedList;
    }
};