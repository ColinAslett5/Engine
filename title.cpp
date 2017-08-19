//chess engine using a bitboard, Colin Aslett
//Debugging: g++ -g title.cpp then gdb title.cpp then backtrace and then frame #
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
using namespace std;
//global board
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
  string piece;//might not need the initialization, just for promoting pawns
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
int analyze(int depth,int beta,int alpha,int player);
void MakeMove(int oldx,int oldy,int newx,int newy,string piece);
void UndoMove(int oldx,int oldy,int newx,int newy,string piece,string temp);
bool KingSafe();
void flipboard();//flipping the board for the computer
int rating();
void draw();
int main(){
  //finding the kings posiiton
  while(Board[KingPositionC/8][KingPositionC%8] != "A"){
    KingPositionC++;
  }
  while(Board[KingPositionL/8][KingPositionL%8] != "a"){
    KingPositionL++;
  }
  //variables for alpha beta tree
  int depth = 4;//setting the depth of the analysis
  int beta = 1000000;
  int alpha = -1000000;
  int player = 0;
  //flipboard();
  analyze(depth,beta,alpha,player);
  return 0;
}
void draw(){
  for(int i = 0;i < 8;i++){
    for(int b = 0;b < 8;b++){
      cout << Board[i][b];
    }
    cout << endl;
  }
}
int analyze(int depth,int beta,int alpha,int player){
  vector<Node*> list = PossibleMove();
  int size = list.size();
  if(depth == 0 || list.size() == 0){
    //cout << endl;
    //cout << endl;
    //cout << endl;
    //cout << "Rating: " << rating() << "   size: " << list.size() << "  Depth: " << depth << endl;
    return (rating()*(player*2-1));
  }
  player = 1-player;
  for(int i = 0;i < size;i++){
    string temp = Board[list[i]->newx][list[i]->newy];
    //draw();
    MakeMove(list[i]->oldx,list[i]->oldy,list[i]->newx,list[i]->newy,list[i]->piece);
    int returnInt = analyze(depth-1,beta,alpha,player);
    //flipboard();
    UndoMove(list[i]->oldx,list[i]->oldy,list[i]->newx,list[i]->newy,list[i]->piece,temp);
    if(player == 0){
      if(returnInt <= beta){
	beta = returnInt;
      }
    }
    else{
      if(returnInt>alpha){
	alpha = returnInt;
      }
    }
    if(alpha >= beta){
      if(player == 0){
	return beta;
      }
      else{
	return alpha;
      }
    }
  }    
  if(player == 0){
    return beta;
  }
  else{
    return alpha;
  }
  return 0;
}
int rating(){
  /*
  int rate;
  //pieces
  for(int i = 0;i < 64;i++){
    int r = i/8;
    int c = i%8;
    if(Board[r][c] == "P"){
      rate++;
    }
    if(Board[r][c] == "R"){
      rate += 5;
    }
    if(Board[r][c] == "B"){
      rate += 3;
    }
    if(Board[r][c] == "N"){
      rate += 3;
    }
    if(Board[r][c] == "Q"){
      rate += 9;
    }
    if(Board[r][c] == "p"){
      rate--;
    }
    if(Board[r][c] == "r"){
      rate -= 5;
    }
    if(Board[r][c] == "b"){
      rate -= 3;
    }
    if(Board[r][c] == "n"){
      rate -= 3;
    }
    if(Board[r][c] == "q"){
      rate -= 9;
    }
  }
  */
  return 2;
  //return rate;
}  
//flipping the board
void flipboard(){
  //draw();
  string temp;
  string xx;
  string yy;
  for(int i = 0;i < 32;i++){
    int r = i/8;
    int c = i%8;
    xx = Board[r][c];
    if(isupper(xx[0]) == true){
      temp = tolower(xx[0]);
    }
    else{
      temp = toupper(xx[0]);
    }
    yy = Board[7-r][7-c];
    if(isupper(yy[0]) == true){
      Board[r][c] = tolower(yy[0]);
    }
    else{
      Board[r][c] = toupper(yy[0]);
    }
    Board[7-r][7-c] = temp;
  }
  int Kingtemp = KingPositionC;
  KingPositionC=63-KingPositionL;
  KingPositionL=63-Kingtemp;
  //draw();
}
//making moves and undoing moves
void MakeMove(int oldx,int oldy,int newx,int newy,string piece){
  if(piece == "Q" || piece == "R" || piece == "B" || piece == "N"){//should work
    Board[oldx][oldy] = " ";
    Board[newx][newy] = piece;
  }
  else{
    Board[newx][newy] = Board[oldx][oldy];
    Board[oldx][oldy] = " ";
    if(Board[newx][newy] == "A"){
      KingPositionC = 8*newx+newy;
    }
  }
}
void UndoMove(int oldx,int oldy,int newx,int newy,string piece,string temp){
  if(piece == "Q" || piece == "R" || piece == "B" || piece == "N"){
    Board[oldx][oldy] = "P";
    Board[newx][newy] = temp;
  }
  else{
    Board[oldx][oldy] = Board[newx][newy];
    Board[newx][newy] = temp;
    if(Board[oldx][oldy] == "A"){
      KingPositionC = 8*oldx+oldy;
    }
  }
}
/*
  everything below is pieces
  and kingsafe checks

*/
vector<Node*> PossibleMove(){
  vector<Node*> list;//will contain all possible moves for this turn
  for(int i = 0;i < 64;i++){
    if(Board[i/8][i%8] == "P"){
      Pawn(i,list);
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
  /*
  string oldpiece;
  int r = i/8;
  int c = i%8;
  for(int j = -1;j <= 1; j+=2){//captures
    if(r-1 > -1 && r-1 < 8 && c+j > -1 && c+j < 8){
      if((Board[r-1][c+j] == "a" || Board[r-1][c+j] == "r" || Board[r-1][c+j] == "b" || Board[r-1][c+j] == "q" || Board[r-1][c+j] == "n" || Board[r-1][c+j] == "p") && i >= 16){//cant promote
	oldpiece = Board[r-1][c+j];
	Board[r][c] = " ";
	Board[r-1][c+j] = "P";
	if(KingSafe()){
	  Node* name = new Node();
	  name->oldx = r;
	  name->oldy = c;
	  name->newx = r-1;
	  name->newy = c+j;
	  list.push_back(name);
	}
	Board[r][c] = "P";
	Board[r-1][c+j] = oldpiece;
      }
    }
    if(r-1 > -1 && r-1 < 8 && c+j > -1 && c+j < 8){
      if((Board[r-1][c+j] == "a" || Board[r-1][c+j] == "r" || Board[r-1][c+j] == "b" || Board[r-1][c+j] == "q" || Board[r-1][c+j] == "n" || Board[r-1][c+j] == "p") && i < 16){
	string temp[] = {"Q","R","B","N"};
	for(int k = 0;k < 4;k++){
	  oldpiece = Board[r-1][c+j];
	  Board[r][c] = " ";
	  Board[r-1][c+j] = temp[k];
	  if(KingSafe()){
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r-1;
	    name->newy = c+j;
	    name->piece = temp[k];
	    list.push_back(name);
	  }
	  Board[r][c] = "P";
	  Board[r-1][c+j] = oldpiece;
	}
      } 
    }
  }
  //moving pawn up one
  if(r-1 > -1 && r-1 < 8 && c > -1 && c < 8){
    if(Board[r-1][c] == " " && i >= 16){
      oldpiece = Board[r-1][c];
      Board[r][c] = " ";
      Board[r-1][c] = "P";
      if(KingSafe()){
	Node* name = new Node();
	name->oldx = r;
	name->oldy = c;
	name->newx = r-1;
	name->newy = c;
	list.push_back(name);
      }
      Board[r][c] = "P";
      Board[r-1][c] = oldpiece;
    }
    if(Board[r-1][c] == " " && i < 16){
      string temp[] = {"Q","R","B","N"};
      for(int k = 0;k < 4;k++){
	oldpiece = Board[r-1][c];
	Board[r][c] = " ";
	Board[r-1][c] = temp[k];
	if(KingSafe()){
	  Node* name = new Node();
	  name->oldx = r;
	  name->oldy = c;
	  name->newx = r-1;
	  name->newy = c;
	  name->piece = temp[k];
	  list.push_back(name);
	}
	Board[r][c] = "P";
	Board[r-1][c] = oldpiece;
      }
    }
  }
  //moving up two squares
  if(r-2 > -1 && r-2 < 8 && c > -1 && c < 8){
    if(Board[r-1][c] == " " && Board[r-2][c] == " " && i >= 48){
      oldpiece = Board[r-2][c];
      Board[r][c] = " ";
      Board[r-2][c] = "P";
      if(KingSafe()){
	Node* name = new Node();
	name->oldx = r;
	name->oldy = c;
	name->newx = r-2;
	name->newy = c;
	list.push_back(name);
      }
      Board[r][c] = "P";
      Board[r-2][c] = oldpiece;
    }
  }
  */
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
	Board[r][c+temp*j] = "R";
	if(KingSafe()){
	  if(c+temp*j < 8 && c+temp*j > -1){
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r;
	    name->newy = c+temp*j;
	    list.push_back(name);
	  }
	}
	Board[r][c] = "R";
	Board[r][c+temp*j] = oldpiece;
	//maybe should be inside of the if statement
	if(c+temp*j < 7 && c+temp*j > -1){
	  temp++;
	}
	else{
	  break;
	}
      }
      if(Board[r][c+temp*j] == "r" || Board[r][c+temp*j] == "p" || Board[r][c+temp*j] == "q" || Board[r][c+temp*j] == "b" || Board[r][c+temp*j] == "n" || Board[r][c+temp*j] == "a"){
	oldpiece = Board[r][c+temp*j];
        Board[r][c] = " ";
        Board[r][c+temp*j] == "R";
        if(KingSafe()){
	  if(c+temp*j > -1 && c+temp*j < 8){
	    Node* name = new Node();
	    name->oldx = r;
	    name->oldy = c;
	    name->newx = r;
	    name->newy = c+temp*j;
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
	  if(r+temp*j > -1 && r+temp*j < 8){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
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
	  if(r+temp*j > -1 && r+temp*j < 8 && c > -1 && c < 8){
          Node* name = new Node();
	  name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
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
  cout << endl;
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
	  if(c+temp*j < 8 && c+temp*j > -1){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r;
          name->newy = c+temp*j;
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
	  if(c+temp*j > -1 && c+temp*j < 8){
	  Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r;
          name->newy = c+temp*j;
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
	  if(r+temp*j > -1 && r+temp*j < 8){
          Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
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
	  if(r+temp*j > -1 && r+temp*j < 8){
	  Node* name = new Node();
          name->oldx = r;
          name->oldy = c;
          name->newx = r+temp*j;
          name->newy = c;
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
	  Board[r][c] = "N";
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
	  Board[r][c] = "N";
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
	if(Board[KingPositionC/8+i][KingPositionC%8+j*2] == "n"){
	  return false;
	}
      }
      if(KingPositionC/8+i*2 > -1 && KingPositionC/8+i*2 < 8 && KingPositionC%8+j > -1 && KingPositionC%8+j < 8){
	if(Board[KingPositionC/8+i*2][KingPositionC%8+j*2] == "n"){
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
