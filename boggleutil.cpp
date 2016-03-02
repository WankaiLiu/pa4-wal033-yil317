#include "boggleutil.h"
#include <iostream>

const std::string &BoardPos::getText() const{
  return this->text;
}
bool BoardPos::getVisited() const{
  return this->visited;
}
bool BoardPos::getReVisited() const{
  return this->reVisited;
}
void BoardPos::setVisited(bool visited){
  this->visited = visited;
}
void BoardPos::setReVisited(bool reVisited){
  this->reVisited = reVisited;
}
LexNode::LexNode(){
  //Initialize the Lexnode.
  this->endNode = 0;
  for(int i = 0; i<26; i++){
    children[i] = 0;
  }
}
LexNode* LexNode::addLexNode(char &code, bool isEndNode){
  //Check the corresponding children node, then return the next node.
  if (children[code-'a'] !=0 ){
    if(isEndNode == 1){
      this->endNode = 1;
    }  
  }
  else{
    children[code-'a'] = new LexNode;
        if(isEndNode == 1){
	  children[code-'a']->endNode = 1;;
    }
  }   
  return children[code-'a'];
}


