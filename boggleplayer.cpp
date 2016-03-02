#include "boggleplayer.h"
using namespace std;
#include <iostream>
#include <stack> 
BogglePlayer::BogglePlayer(){
  board_built = 0;
}
BogglePlayer::~BogglePlayer(){
  deleteNode(root);
}

void BogglePlayer::ResetBoard(){
  for (unsigned int i = 0; i < board.size() ; i++){
    (board.at(i)).setVisited(0);
    (board.at(i)).setReVisited(0);
  }
  
}
//Use recursion to delete all subNodes.
void BogglePlayer::deleteNode(LexNode *root){
  for (unsigned int i = 0; i < 26; i++){
    if(root->children[i] != 0){
      deleteNode(root->children[i]);
    }
  }
  delete root;
  return;
     
}
void BogglePlayer::setBoard(unsigned int rows, unsigned int cols,
			    std::string **diceArray){
  this->rows = rows;
  this->cols = cols;
  string tempStr;
  //Assign text to every cell.
  for (unsigned int i = 0 ; i < rows; i++){
    for (unsigned int j = 0 ; j < cols; j++){
      tempStr = diceArray[i][j];
      for (unsigned int k=0; k< tempStr.size(); k++){
	tempStr[k] = tolower(tempStr[k]);
      }
      this->board.push_back(BoardPos(tempStr));
    }
  }
  this->board_built = 1;
}
vector<int> BogglePlayer::isOnBoard(const string& word_to_check){
  string tempStr; //Store the temp text in cell.
  stack<int> wordStack; // Store the position of text to be visited.
  unsigned int pos = 0; // Record the current index of word to check.
  BoardPos *tempBoard; // Temporary class of BoardPos.
  int tempLength; // The length of current text.
  vector<int> tempVector; //Store the position of each text 
  int cur_i;
  int cur_j;
  for (int i = 0 ; i < rows; i++){
    for (int j = 0 ; j < cols; j++){
      tempBoard = &board.at(cols*i+j);
      pos = 0;
      this->ResetBoard();
      wordStack.push(cols*i+j);
      //Use backtracking to find the checking word until the stack is empty.
      while(!wordStack.empty()){
	tempBoard = &board.at(wordStack.top());	
	cur_i = wordStack.top() / cols;
	cur_j = wordStack.top() % cols;
	wordStack.pop();
	tempStr = tempBoard->getText();
	//If the current cell has been visited, then pop it from stack.
	if(tempBoard->getReVisited() == 1){
	  tempBoard->setReVisited(0);
	  tempBoard->setVisited(0);
	  string tempBackStr = (board.at(tempVector.back())).getText();
	  tempLength = tempBackStr.size();
	  tempVector.pop_back();
	  pos -= tempLength;
	}
	//If the current cell has not been visited and is smae as the next text to be visit, push it into stack.
	else if (!tempBoard->getVisited() && 
		 tempStr.compare(0,tempStr.size(),word_to_check,pos,
				 tempStr.size()) == 0){
	  tempBoard->setVisited(1);
	  pos += tempStr.size();
	  tempLength = tempStr.size();
	  tempVector.push_back(cols*cur_i+cur_j);
	  //If all text were found, return the position of every text.
	  if (pos == word_to_check.size()){
	    return tempVector; 
	  }
	  tempBoard->setReVisited(1);
	  wordStack.push(cols*cur_i+cur_j);
	  //Traverse all adjecent cells of current cell.
	  if (cur_j != 0  && 
	      (board.at(cols*cur_i+cur_j-1)).getVisited()==0){
	    wordStack.push(cols*cur_i+cur_j-1);
	  }
	  if (cur_j != cols-1  && 
	      (board.at(cols*cur_i+cur_j+1)).getVisited()==0){
	    wordStack.push(cols*cur_i+cur_j+1);
	  }
	  if (cur_i != 0 && (board.at(cols*(-1+cur_i)+cur_j)).getVisited()==0 ){
	    wordStack.push(cols*(-1+cur_i)+cur_j);
	  }
	  if (cur_i != rows-1 && 
	      (board.at(cols*(1+cur_i)+cur_j)).getVisited()==0){
	    wordStack.push(cols*(cur_i+1)+cur_j);
	  }
	  if (cur_i != 0 && cur_j != 0 && 
	      (board.at(cols*(-1+cur_i)+cur_j-1)).getVisited()==0){
	    wordStack.push(cols*(-1+cur_i)+cur_j-1);
	  }
	  if (cur_j != cols-1 && cur_i != 0 && 
	      (board.at(cols*(-1+cur_i)+cur_j+1)).getVisited()==0){
	    wordStack.push(cols*(-1+cur_i)+cur_j+1);
	  }
	  if (cur_j != 0 && cur_i != rows-1 && 
	      (board.at(cols*(1+cur_i)+cur_j-1)).getVisited()==0){
	    wordStack.push(cols*(1+cur_i)+cur_j-1);
	  }
	  if (cur_i != rows-1 && cur_j != cols-1 && 
	      (board.at(cols*(1+cur_i)+cur_j+1)).getVisited()==0){
	    wordStack.push(cols*(cur_i+1)+cur_j+1);
	  }
	}
      }
    }
  }
return tempVector; 
}
void BogglePlayer::buildLexicon(const set<string>& word_list){
  std::string::iterator it;
  std::string eachWord;
  this->root = new LexNode;
  //Use iterator to set up Lexicon.
  for (std::set<string>::iterator itWordList=word_list.begin(); 
       itWordList!=word_list.end(); ++itWordList){
    LexNode *cur = this->root;
    bool isEnd = 0;
    unsigned int i = 0;
    eachWord = *itWordList;
    //If the letter has been created, then pointer move to this node. Otherwise, Create a new node.
    for (it = eachWord.begin(); it != eachWord.end(); ++it){
      char code = *it;
      if( ++i == eachWord.size()){
	isEnd = 1;
      }
      cur = cur->addLexNode(code,isEnd);
    }
  }
}

bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words){
  vector<int> tempSequence;
  for(int i = 0; i <rows*cols ; i++){
    //Use rucursion to find all valid words with start index.
    subGetAllWords(minimum_word_length,tempSequence,root,i,words);
  } 
  if (words->empty() == 1)
    return 0;
  else
    return 1;
}
/*********************************
SubFunction to Find all words.
**********************************/
void BogglePlayer::subGetAllWords(unsigned int minimum_word_length,
				  vector<int> tempSequence,LexNode* curLex,
				  int startPos,set<string>* words){
  LexNode* tempCurLex; //The current pointer in multiple tries.
  string tempStr; //Temporary string to store parital word.
  string tempWord; //The word to be insert to set.
  string::iterator itStr; // String Iterator to read every char in string.
  int cur_i = startPos / this->cols;
  int cur_j = startPos % this->cols;
  BoardPos *tempBoard; // Temporary class of BoardPos
  BoardPos *tempBoard2;
  int tempLexIndex;
  bool flagComplete;//To check whether the word is complete.
  vector<int> tempSequence2;
  vector<int>::iterator itVec; // Iterator to get every element in vector.
  tempSequence2 = tempSequence;
  //To check whether this node has been visited.
  for (std::vector<int>::iterator itVec2 = tempSequence2.begin() ;
       itVec2 != tempSequence2.end(); ++itVec2){
    if(*itVec2 == startPos)
      return;
  }
  //Get the current board.
  tempBoard = &board.at(startPos);
  tempStr.assign(tempBoard->getText());
  flagComplete = 1;
 
  tempCurLex = curLex;
  //If the next text is same as the next part of word, store the index in vector. Otherwise, drop it.
  for(itStr = tempStr.begin() ; itStr != tempStr.end(); ++itStr){
    tempLexIndex = *itStr-'a';
    if (tempCurLex->children[tempLexIndex] != 0){
      tempCurLex = tempCurLex->children[tempLexIndex];
    }
    else{
      flagComplete = 0;
      break;
    }
  }
  //If the text is same as the next text, push it into vector.
  if(flagComplete == 1){
    tempSequence2.push_back(startPos);
  }
  //if visit the last letter of valid word, read all index from vector and then store the word in set.
  if (flagComplete == 1 && tempCurLex->endNode == 1){
    tempWord.clear();
    for(itVec = tempSequence2.begin(); 
	itVec != tempSequence2.end(); ++itVec){
      tempBoard2 = &board.at(*itVec);
      tempWord += tempBoard2->getText();
    }
    if(tempWord.size() >= minimum_word_length){
      words->insert(tempWord);
    }
  }
  //Traverse all its adjacent cells.
  if(flagComplete == 1)  {
    if (cur_j != 0){ 
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,startPos-1,words);
    } 
    if (cur_j != cols-1){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,startPos+1,words);
    }
    if (cur_i != 0 ){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,(cols*(-1+cur_i)+cur_j),words);
    }
    if (cur_i != rows-1 ){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,(cols*(cur_i+1)+cur_j),words);
    }
    if (cur_i != 0 && cur_j != 0){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,(cols*(-1+cur_i)+cur_j-1),words);
    }
    if (cur_j != cols-1 && cur_i != 0){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,(cols*(-1+cur_i)+cur_j+1),words);
    }
    if (cur_j != 0 && cur_i != rows-1 ){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,(cols*(1+cur_i)+cur_j-1),words);
    }
    if (cur_i != rows-1 && cur_j != cols-1 ){
      subGetAllWords(minimum_word_length,tempSequence2,
		     tempCurLex,(cols*(cur_i+1)+cur_j+1),words);
    }
  }
  return;
} 
bool BogglePlayer::isInLexicon(const string& word_to_check){
  LexNode *cur = this->root;
  string tempword = word_to_check;
  std::string::iterator itWord;
  //Search through multiple trie via input string.
  for (itWord = tempword.begin();
       itWord != tempword.end(); ++itWord){
    if(cur->children[*itWord-'a'] != 0){
      cur = cur->children[*itWord-'a'];
    }
    else{
      return 0;
    }
  }
  if(cur->endNode == 1){
    return 1;
  }
  else{
    return 0;
  }
  
}

void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols){
  //Return Customboard.
  *rows = this->rows;
  *cols = this->cols;
  BoardPos *tempBoard;
  for (int i = 0 ; i < this->rows; i++){
    for (int j = 0 ; j < this->cols; j++){
      tempBoard = &board.at(this->cols*i+j);
      new_board[i][j] = tempBoard->getText();
    }
  }
}
