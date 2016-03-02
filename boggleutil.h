// Kyle Barron-Kraus <kbarronk>

#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <cctype>
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>


/**
 * Class that represents a position on the Boggle Board.
 *
 * Each position contains its sanitized string that represents the
 * characters on that board position, as well as a boolean that
 * marks the board position as visited by a given search algorithm.
 */
class BoardPos {
  public:
    /**
     * Constructs a BoardPos with the given text. Text must have been
     * sanitized by the caller.
     */
 BoardPos(const std::string &text2):text(text2){};

    /**
     * Returns the characters on this BoardPos.
     */
    const std::string &getText() const;
    
    /**
     * Returns whether this BoardPos has been visited or not.
     */
    bool getVisited() const;
    bool getReVisited() const;
    /**
     * Sets whether this BoardPos has been visited or not.
     */
    void setVisited(bool visited);
    void setReVisited(bool reVisited);

  private:
    /**
     * The characters that this BoardPos contains.
     */
    const std::string text;

    /**
     * Whether this BoardPos has been visited or not.
     */
    bool visited;
    bool reVisited;
};



/**
 * Implement class for Trie
 */
class LexNode {
 
 public:
  LexNode();
  LexNode* addLexNode(char &code, bool isEndNode);
 //private:
  bool endNode;
 // Map of letters to the child LexNodes nodes corresponding to those letters
 LexNode* children[26];	
 // A field to indicate end of word
};



#endif // BOGGLEUTIL_H
