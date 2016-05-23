#ifndef _NODE_H_
#define _NODE_H_
#include <iostream>
#include <vector>
#include <algorithm>

class Node {
  private:
  	int pos;
  	int custo;
  	int caminho;
  	Node *pai;

  public:
  	Node(int pos, int custo, int caminho, Node* pai, int num);
  	Node();

  	int getPos();
  	int getCusto();
  	int getCaminho();
  	Node* getPai();
  	void setPos(int pos);
  	void setCusto(int custo);
  	void setCaminho(int caminho);
  	void setPai(Node *pai);

  };

#endif