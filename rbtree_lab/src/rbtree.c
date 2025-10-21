
#include "rbtree.h"

#include <stdlib.h>


node_t* get_cousin(node_t* node);
node_t* get_grandparent(node_t* node);
void insert_node_case3(rbtree* t,node_t* node);
void insert_node_case2(rbtree* t,node_t* node);
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: 필요한 경우 구조체를 초기화합니다.

  return p;
}

void delete_node(node_t* node){
  if(node == NULL){
    return;
  }
  delete_node(node->left);//왼쪽 자식 확인
  delete_node(node->right); //오른쪽 자식 확인
  free(node); // 죽은 노드 메모리 해제
  return;
}

void delete_rbtree(rbtree *t) {
  // TODO:  트리 노드의 메모리를 회수합니다
  node_t* root = t->root;
  delete_node(root);
  free(t);
}


void insert_node_case1(rbtree* t, node_t* node){
  if(node->parent == NULL){
    node->color = RBTREE_BLACK;
    return;
  }else{
    insert_node_case2(t,node);
  }
}
// Case 2 — 부모가 빨강이고, 삼촌도 빨강
// -> 부모/삼촌을 검정으로, 조부모를 빨강으로 -> 다시 위로 올라가서 검사.
void insert_node_case2(rbtree* t, node_t* node){
  node_t* cousin = get_cousin(node);
  if(cousin != NULL && cousin->color == RBTREE_RED){
    cousin->color = RBTREE_BLACK;
    node->parent->color = RBTREE_BLACK;
    get_grandparent(node)->color = RBTREE_RED;
    insert_node_case1(t,get_grandparent(node));
  }else{
    insert_node_case3(t,node);
  }
}
//왼쪽 회전 함수
void left_rotation(rbtree* t, node_t* node) {
    node_t* right = node->right;
 
    //  node의 오른쪽 서브트리를 이동
    node->right = right->left;
    if (right->left != NULL)
        right->left->parent = node;

    //right의 부모를 node의 부모로 변경
    right->parent = node->parent;

    if (node->parent == NULL)
        t->root = right;
    else if (node == node->parent->left)
        node->parent->left = right;
    else
        node->parent->right = right;

    //node를 right의 왼쪽 자식으로 연결
    right->left = node;
    node->parent = right;
}


void right_rotation(rbtree* t, node_t* node){
  node_t* left = node->left;
  node->left = left->right;
  if(left->right != NULL){
    left->right->parent = node;
  }
  left->parent = node->parent; 
  
  if(node->parent == NULL){
    t->root = left;
  }else if (node->parent->right ==  node){
    node->parent->right = left;
  }else{
    node->parent->left = left;
  }
  
  node->parent = left;
  left->right = node;
}
// Case 3 — 부모가 빨강, 삼촌이 검정
// 삽입 위치가 부모의 반대쪽 자식이면 먼저 회전 (좌/우)
// 그 다음 부모와 조부모 색을 바꾸고 다시 회전

void insert_node_case3(rbtree* t ,node_t* node){
  node_t* parent = node->parent;
  node_t* grandparent = get_grandparent(node);
  if(node == parent->right && parent == grandparent->left){
    left_rotation(t,parent);
  }else if(node == parent->left && parent == grandparent->right){
    right_rotation(t,parent);
  }

  parent = node->parent;
  grandparent = get_grandparent(node);

  //일직선 구현
  if(node == parent->left){
    right_rotation(t,grandparent);
  }else{
    left_rotation(t,grandparent);
  }

  parent->color = RBTREE_BLACK;
  grandparent->color = RBTREE_RED;
}

node_t* get_grandparent(node_t* node){
  if(node != NULL && node->parent != NULL){
    return node->parent->parent;
  }else{
    return NULL;
  }
}

node_t* get_cousin(node_t* node){
  node_t* gp = get_grandparent(node);
  if(gp == NULL){
    return NULL;
  }else{
    if(node->parent == gp->left){
      return gp->right;
    } 
    else{
      return gp->left;
    } 
  }
}
//주석ㅌ

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: 삽입 구현
  node_t* cur = t->root;
  node_t* new_node = (node_t*)malloc(sizeof(node_t));//새로운 노드 
  // new_node 초기화
  new_node->key = key;
  new_node->color = RBTREE_RED; // new_node : 색
  new_node->left = NULL; // new_node : 왼쪽 자식값
  new_node->right = NULL; // new_node : 오른쪽 자식값
  new_node->parent = NULL; // New_node : 부모 노드 값

  // 무작위로 new_node 트리에 추가
  if(t->root == NULL){ // 트리가 없을 때
    t->root = new_node;
    return new_node;
  }else{
    while (1)
    {
      if(cur->key > key){
        if(cur->left != NULL){
          cur = cur->left;
        }else{
          cur->left = new_node;
          new_node->parent = cur;
          break;
        }
      }else{
        if(cur->right != NULL){
          cur = cur->right;
        }else{
          cur->right = new_node;
          new_node->parent = cur;
          break; 
        }
      }
    } 
  }
  insert_node_case1(t,new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: 찾기 구현
  node_t * cur = t->root;
  while (cur != NULL)
  {
    if(cur->key == key){ //현재 노드가 찾는 값이면 break;
      break;
    } 
    if(cur->key > key){ // 현재 노드 보다 찾는 값이 작으면 
      cur = cur->left;
    }else{
      cur = cur->right; // 현재 노드 보다 찾는 값이 크면
    }
  }
  
  return  cur;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: 찾기 구현
  node_t* cur = t->root;
  while (cur->left->left != NULL)
  {
    cur = cur->left;
  }
  return cur; 
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: 찾기 구현 
   node_t* cur = t->root;
  while (cur->right->right != NULL)
  {
    cur = cur->right;
  }
  return cur;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: 지우기 구현
  node_t* cur = t->root; // 현재 노드
  node_t* c_node = t->root; // 삭제 노드의 대체 노드
  int key = p->key;
  while (c_node != NULL){
    if(cur->key > key){
      cur= cur->left;
    }else if(cur->key < key){
      cur = cur->right;
    }else{ // 삭제 노드를 찾았을 때
      cur = cur->right; // 오른쪽 노드로 이동
      while (c_node != NULL){ // 삭제 노드와 가장 근접한 노드 찾기 
        c_node = cur->left;
      }
    }
  }
  {
    /* code */
  }
  
  
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}