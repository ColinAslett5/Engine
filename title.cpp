//chess engine using a bitboard, Colin Aslett
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
//global board
bool white = true;//computer is white
static int KingPositionC;
static int KingPositionL;
string Board[8][8]={
            {"r","n","b","q","a","b","n","r"},
            {"p","p","p","p","p","p","p","p"},
            {" "," "," "," "," "," "," "," "},
            {" "," "," "," "," "," "," "," "},
            {" "," "," "," "," "," "," "," "},
            {" "," "," "," "," "," "," "," "},
            {"P","P","P","P","P","P","P","P"},
            {"R","N","B","Q","A","B","N","R"}};
struct Node{
  int oldx;
  int oldy;
  int newx;
  int newy;
  string temp = " ";//might not need the initialization, just for promoting pawns
};
//functions
void Initiate();
vector<Node*> PossibleMove();
void Pawn(int i,vector<Node*> &list);
void Rook(int i,vector<Node*> &list);
void Bishop(int i,vector<Node*> &list);
void Queen(int i,vector<Node*> &list);
void King(int i,vector<Node*> &list);
void Knight(int i,vector<Node*> &list);
void analyze(vector<Node*> list);
bool KingSafe();
int main(){
  //finding the kings posiiton
  while(Board[KingPositionC/8][KingPositionC%8] != "A"){
    KingPositionC++;
  }
  while(Board[KingPositionL/8][KingPositionL%8] != "a"){
    KingPositionL++;
  }
  analyze(PossibleMove());
  return 0;
}
void analyze(vector<Node*> list){
  int size = list.size();
  for(int i = 0;i < size;i++){
    cout << list[i]->oldx << list[i]->oldy << list[i]->newx << list[i]->newy << endl;
  }
}
vector<Node*> PossibleMove(){
  vector<Node*> list;//will contain all possible moves for this turn
  for(int i = 0;i < 64;i++){
    if(Board[i/8][i%8] == "P"){
      //Pawn(i,list);
    }
    else if(Board[i/8][i%8] == "R"){
      Rook(i,list);
    }
    else if(Board[i/8][i%8] == "B"){
      Bishop(i,list);
    }
    else if(Board[i/8][i%8] == "Q"){
      Queen(i,list);
    }
    else if(Board[i/8][i%8] == "A"){
      King(i,list);
    }
    else if(Board[i/8][i%8] == "N"){
      Knight(i,list);
    }
  }
  return list;
}
void Pawn(int i,vector<Node*> &list){
  string oldpiece;
  int r = i/8;
  int c = i%8;
  for(int j = -1;j <= 1; j+=2){//captures
    if(r-1 > -1 && r-1 < 8 && c+j > -1 && c+j < 8){
      if((Board[r-1][c+j] == "a" || Board[r-1][c+j] == "r" || Board[r-1][c+j] == "b" || Board[r-1][c+j] == "q" || Board[r-1][c+j] == "n") && i >= 16){//cant promote
	oldpiece = Board[r-1][c+j];
	Board[r][c] = " ";
	Board[r-1][c+j] = "P";
	if(KingSafe()){
	  Node* name = New Node();
	  name->oldx = r;
	  name->oldy = c;
	  name->newx = r-1;
	  name->newy = c+j;
	}
	Board[r][c] = "P";
	Board[r-1][c+j] = oldpiece;
      }
    }
  }
}
void Rook(int i,vector<Node*> &list){
  string oldpiece;
  int r = i/8;
  int c = i%8;
  int temp = 1;
  for(int j = -1;j<=1;j+=2){
    if(c+temp*j > -1 && c+temp*j < 8 && r > -1 && r < 8){
      while(Board[r][c+temp*j] == " "){
	oldpiece = Board[r][c+temp*j];
	Board[r][c] = " ";
	Board[r][c+temp*j] == "R";
	if(KingSafe()){
	  Node* name = new Node();
	  name->oldx = r;
	  name->oldy = c;
	  name->newx = r;
	  name->newy = c+temp*j;
	  if(c+temp*j < 8 && c+temp*j > -1){
	    list.push_back(name);
	  }
	}
	Board[r][c] = "R";
	Board[r][c+temp*j] = oldpiece;
	temp++;
      }
      if(Board[r][c+temp*j] == "r" || Board[r][c+temp*j] == "p" || Board[r][c+temp*j] == "q" || Board[r][c+temp*j] == "b" || Board[r][c+temp*j] == "n" || Board[r][c+temp*j] == "a"){
	oldpiece = Board[r][c+temp*j];
        Board[r][c] = " ";
        Board[r][c+temp*j] == "R";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r;
	  name->newy = c+temp*j;
	  if(c+temp*j > -1 && c+temp*j < 8){
	    list.push_back(name);
	  }
        }
        Board[r][c] = "R";
        Board[r][c+temp*j] = oldpiece;
      }
    }
    temp = 1;
    if(c > -1 && c < 8 && r+temp*j > -1 && r+temp*j < 8){
      while(Board[r+temp*j][c] == " "){
        oldpiece = Board[r+temp*j][c];
        Board[r][c] = " ";
        Board[r+temp*j][c] == "R";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
	  if(r+temp*j > -1 && r+temp*j < 8){
	    list.push_back(name);
	  }
        }
        Board[r][c] = "R";
        Board[r+temp*j][c] = oldpiece;
        temp++;
      }
      if(Board[r+temp*j][c] == "r" || Board[r+temp*j][c] == "p" || Board[r+temp*j][c] == "q" || Board[r+temp*j][c] == "b" || Board[r+temp*j][c] == "n" || Board[r+temp*j][c] == "a"){
	oldpiece = Board[r+temp*j][c];
        Board[r][c] = " ";
        Board[r+temp*j][c] == "R";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
	  if(r+temp*j > -1 && r+temp*j < 8){
	    list.push_back(name);
	  }
        }
        Board[r][c] = "R";
        Board[r+temp*j][c] = oldpiece;
      }
    }
    temp = 1;
  }
}
void Bishop(int i,vector<Node*> &list){//done
  string oldpiece;
  int r = i/8;
  int c = i%8;
  int temp = 1;
  for(int j = -1;j <= 1;j+=2){
    for(int k = -1;k <= 1;k += 2){
      if(r+temp*j > -1 && r+temp*j < 8 && c+temp*k > -1 && c+temp*k < 8){//make sure it is in bounds for the array
	while(Board[r+temp*j][c+temp*k] == " "){
	  oldpiece = Board[r+temp*j][c+temp*k];
	  Board[r][c] = " ";
	  Board[r+temp*j][c+temp*k] = "B";
	  if(KingSafe()){
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r+temp*j;
	    name->newy = c+temp*k;
	    list.push_back(name);
	  }
	  Board[r][c] = "B";
	  Board[r+temp*j][c+temp*k] = oldpiece;
	  temp++;
	}
	if(Board[r+temp*j][c+temp*k] == "p" || Board[r+temp*j][c+temp*k] == "r" || Board[r+temp*j][c+temp*k] == "b" || Board[r+temp*j][c+temp*k] == "q" || Board[r+temp*j][c+temp*k] == "n" || Board[r+temp*j][c+temp*k] == "a"){//enemy pieces
	  oldpiece = Board[r+temp*j][c+temp*k];
          Board[r][c] = " ";
          Board[r+temp*j][c+temp*k] = "B";
          if(KingSafe()){
            Node* name = new Node();
            name->oldx = r;
            name->oldy = c;
            name->newx = r+temp*j;
            name->newy = c+temp*k;
            list.push_back(name);
          }
          Board[r][c] = "B";
          Board[r+temp*j][c+temp*k] = oldpiece;
	}
      }
      temp = 1;
    }
  }
}
void Queen(int i,vector<Node*> &list){
  string oldpiece;
  int r = i/8;
  int c = i%8;
  int temp = 1;
  for(int j = -1;j <= 1;j+=2){
    for(int k = -1;k <= 1;k += 2){
      if(r+temp*j > -1 && r+temp*j < 8 && c+temp*k > -1 && c+temp*k < 8){//make sure it is in bounds for the array
        while(Board[r+temp*j][c+temp*k] == " "){
          oldpiece = Board[r+temp*j][c+temp*k];
          Board[r][c] = " ";
          Board[r+temp*j][c+temp*k] = "Q";
          if(KingSafe()){
            Node* name = new Node();
            name->oldx = r;
            name->oldy = c;
            name->newx = r+temp*j;
            name->newy = c+temp*k;
            list.push_back(name);
          }
          Board[r][c] = "Q";
          Board[r+temp*j][c+temp*k] = oldpiece;
          temp++;
        }
        if(Board[r+temp*j][c+temp*k] == "p" || Board[r+temp*j][c+temp*k] == "r" || Board[r+temp*j][c+temp*k] == "b" || Board[r+temp*j][c+\
temp*k] == "q" || Board[r+temp*j][c+temp*k] == "n" || Board[r+temp*j][c+temp*k] == "a"){//enemy pieces
          oldpiece = Board[r+temp*j][c+temp*k];
          Board[r][c] = " ";
          Board[r+temp*j][c+temp*k] = "Q";
          if(KingSafe()){
            Node* name = new Node();
            name->oldx = r;
            name->oldy = c;
            name->newx = r+temp*j;
            name->newy = c+temp*k;
            list.push_back(name);
          }
          Board[r][c] = "Q";
          Board[r+temp*j][c+temp*k] = oldpiece;
        }
      }
      temp = 1;
    }
  }
/*
    
ROOK PART
  
*/
  r = i/8;
  c = i%8;
  temp = 1;
  for(int j = -1;j<=1;j+=2){
    if(c+temp*j > -1 && c+temp*j < 8 && r > -1 && r < 8){
      while(Board[r][c+temp*j] == " "){
        oldpiece = Board[r][c+temp*j];
        Board[r][c] = " ";
        Board[r][c+temp*j] == "Q";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r;
          name->newy = c+temp*j;
          if(c+temp*j < 8 && c+temp*j > -1){
            list.push_back(name);
          }
        }
        Board[r][c] = "Q";
        Board[r][c+temp*j] = oldpiece;
        temp++;
      }
      if(Board[r][c+temp*j] == "r" || Board[r][c+temp*j] == "p" || Board[r][c+temp*j] == "q" || Board[r][c+temp*j] == "b" || Board[r][c+t\
emp*j] == "n" || Board[r][c+temp*j] == "a"){
        oldpiece = Board[r][c+temp*j];
        Board[r][c] = " ";
        Board[r][c+temp*j] == "Q";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r;
          name->newy = c+temp*j;
          if(c+temp*j > -1 && c+temp*j < 8){
            list.push_back(name);
          }
        }
        Board[r][c] = "Q";
        Board[r][c+temp*j] = oldpiece;
      }
    }
    temp = 1;
    if(c > -1 && c < 8 && r+temp*j > -1 && r+temp*j < 8){
      while(Board[r+temp*j][c] == " "){
        oldpiece = Board[r+temp*j][c];
        Board[r][c] = " ";
        Board[r+temp*j][c] == "Q";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
          if(r+temp*j > -1 && r+temp*j < 8){
            list.push_back(name);
          }
        }
        Board[r][c] = "Q";
        Board[r+temp*j][c] = oldpiece;
        temp++;
      }
      if(Board[r+temp*j][c] == "r" || Board[r+temp*j][c] == "p" || Board[r+temp*j][c] == "q" || Board[r+temp*j][c] == "b" || Board[r+temp\
*j][c] == "n" || Board[r+temp*j][c] == "a"){
        oldpiece = Board[r+temp*j][c];
        Board[r][c] = " ";
        Board[r+temp*j][c] == "Q";
        if(KingSafe()){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
          if(r+temp*j > -1 && r+temp*j < 8){
            list.push_back(name);
          }
        }
        Board[r][c] = "Q";
        Board[r+temp*j][c] = oldpiece;
      }
    }
    temp = 1;
  }
}
void King(int i,vector<Node*> &list){//almost, castling
  string oldpiece;
  int r = i/8;
  int c = i%8;
  for(int j = 0;j < 9;j++){
    if(j!=4){
      if(r-1+j/3 > -1 && r-1+j/3 < 8 && c-1+j%3 > -1 && c-1+j%3 < 8){
	if(Board[r-1+j/3][c-1+j%3] == "p" || Board[r-1+j/3][c-1+j%3] == "r" || Board[r-1+j/3][c-1+j%3] == "b" || Board[r-1+j/3][c-1+j%3] == "q" || Board[r-1+j/3][c-1+j%3] == "a" || Board[r-1+j/3][c-1+j%3] == "n" || Board[r-1+j/3][c-1+j%3] == " "){
	  oldpiece = Board[r-1+j/3][c-1+j%3];
	  Board[r][c] = " ";
	  int kingtemp = KingPositionC;
	  KingPositionC = i+(j/3)*8+j%3-9;
	  if(KingSafe()){
	    //pushback to vector
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r-1+j/3;
	    name->newy = c-1+j%3;
	    list.push_back(name);
	  }
	  Board[r][c] = "A";
	  Board[r-1+j/3][c-1+j%3] = oldpiece;
	  KingPositionC = kingtemp;
	}
      }
    }
  }
}
void Knight(int i,vector<Node*> &list){//pretty sure this works, might have accidently entered a number incorrectly
  string oldpiece;
  int r = i/8;
  int c = i%8;
  for(int j = -1;j <= 1;j+=2){
    for(int k = -1;k <= 1;k += 2){
      if(r+j > -1 && r+j < 8 && c+k*2 < 8 && c+k*2 > -1){
	if(Board[r+j][c+k*2] == " " || Board[r+j][c+k*2] == "p" || Board[r+j][c+k*2] == "r" || Board[r+j][c+k*2] == "b" || Board[r+j][c+k*2] == "n" || Board[r+j][c+k*2] == "a" || Board[r+j][c+k*2] == "q"){
	  oldpiece = Board[r+j][c+k*2];
	  Board[r][c] = " ";
	  if(KingSafe()){
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r+j;
	    name->newy = c+k*2;
	    list.push_back(name);
	  }
	  Board[r][c] = "K";
	  Board[r+j][c+k*2] = oldpiece;
	}
      }
      if(r+j*2 > -1 && r+j*2 < 8 && c+k < 8 && c+k > -1){
	if(Board[r+j*2][c+k] == " " || Board[r+j*2][c+k] == "p" || Board[r+j*2][c+k] == "r" || Board[r+j*2][c+k] == "b" || Board[r+j*2][c+k] == "n" || Board[r+j*2][c+k] == "a" || Board[r+j*2][c+k] == "q"){
	  oldpiece = Board[r+j*2][c+k];
	  Board[r][c] = " ";
	  if(KingSafe()){
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r+j*2;
	    name->newy = c+k;
	    list.push_back(name);
	  }
	  Board[r][c] = "K";
	  Board[r+j*2][c+k] = oldpiece;
	}
      }
    }
  }
}
bool KingSafe(){
  //BIshop and queen
  int temp = 1;
  for(int i = -1;i <= 1;i += 2){
    for(int j = -1;j <= 1;j+=2){
      if(KingPositionC/8+temp*i > -1 && KingPositionC/8+temp*i < 8 && KingPositionC%8+temp*j > -1 && KingPositionC%8+temp*j < 8){
	while(Board[KingPositionC/8+temp*i][KingPositionC%8+temp*j] == " "){
	  temp++;
	}
	if(Board[KingPositionC/8+temp*i][KingPositionC%8+temp*j] == "b" || Board[KingPositionC/8+temp*i][KingPositionC%8+temp*j] == "q"){
	  return false;
	}
      }
      temp = 1;
    }
  }
  //Rook and Queen
  for(int i = -1;i <= 1;i+=2){
    if(KingPositionC/8 > -1 && KingPositionC/8 < 8 && KingPositionC%8+temp*i > -1 && KingPositionC%8+temp*i < 8){
      while(Board[KingPositionC/8][KingPositionC%8+temp*i] == " "){
	temp++;
      }
      if(Board[KingPositionC/8][KingPositionC%8+temp*i] == "r" || Board[KingPositionC/8][KingPositionC%8+temp*i] == "q"){
	return false;
      }
    }
    temp = 1;
    if(KingPositionC/8+temp*i > -1 && KingPositionC/8+temp*i < 8 && KingPositionC%8 > -1 && KingPositionC%8 < 8){
      while(Board[KingPositionC/8+temp*i][KingPositionC%8] == " "){
	temp++;
      }
      if(Board[KingPositionC/8+temp*i][KingPositionC%8] == "r" || Board[KingPositionC/8+temp*i][KingPositionC%8] == "q"){
	return false;
      }
    }
    temp = 1;
  }
  //Knight
  for(int i = -1;i <= 1;i+=2){
    for(int j = -1;j <= 1;j+=2){
      if(KingPositionC/8+i > -1 && KingPositionC/8+i < 8 && KingPositionC%8+j*2 > -1 && KingPositionC%8+j*2 < 8){
	if(Board[KingPositionC/8+i][KingPositionC%8+j*2] == "k"){
	  return false;
	}
      }
      if(KingPositionC/8+i*2 > -1 && KingPositionC/8+i*2 < 8 && KingPositionC%8+j > -1 && KingPositionC%8+j < 8){
	if(Board[KingPositionC/8+i*2][KingPositionC%8+j*2] == "k"){
	  return false;
	}
      }
    }
  }
  //pawn
  if(KingPositionC>=16){
    if(KingPositionC/8-1 > -1 && KingPositionC/8-1 < 8 && KingPositionC%8-1 > -1 && KingPositionC%8-1 < 8){
      if(Board[KingPositionC/8-1][KingPositionC%8-1] == "p"){
	return false;
      }
    }
    if(KingPositionC/8-1 > -1 && KingPositionC/8-1 < 8 && KingPositionC%8+1 > -1 && KingPositionC%8+1 < 8){
      if(Board[KingPositionC/8-1][KingPositionC%8+1] == "p"){
        return false;
      }
    }
  }
  //King
    for(int i = -1;i <= 1;i++){
      for(int j = -1;j <= 1;j++){
        if(i != 0 || j != 0){
          if(KingPositionC/8+i > -1 && KingPositionC/8+i < 8 && KingPositionC%8+j > -1 && KingPositionC%8+j < 8){
            if(Board[KingPositionC/8+i][KingPositionC%8+j] == "a"){
              return false;
            }
          }
        }
      }
    }
  return true;
}
