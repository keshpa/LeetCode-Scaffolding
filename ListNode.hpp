#pragma once

struct ListNode {
  int val;
  ListNode *next;
  bool visited;
  ListNode() : val(0), next(nullptr), visited(false) {}
  ListNode(int x) : val(x), next(nullptr), visited(false) {}
  ListNode(int x, ListNode *next) : val(x), next(next), visited(false) {}
};
