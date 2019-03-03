//
//  launch.cpp
//  othello
//
//  Created by Yakumorin on 2018/11/10.
//

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "Board.h"
#define DEPTH 8
#define CORNER_WEIGHT 1000
#define MOVE_WEIGHT 10
using namespace std;

string evaluation(Board &board, bool max){
	vector<string>moves1, moves2;
	moves1 = board.getAllMoves(max);
    moves2 = board.getAllMoves(1-max);
	int legalMoves = (((int)moves1.size())-((int)moves2.size()))*MOVE_WEIGHT;
	int cornerValue = (board.cornerCheck(max)-board.cornerCheck(1-max))*CORNER_WEIGHT;//weight, may change later
    int discCount = board.getDisc(max);
	string res = std::to_string(legalMoves+cornerValue+discCount);
	return res;
}

string minMax(Board &board, int depth, int a, int b, bool max){
/* 
	 Algorithm:

	 if depth is 0 reach the target level:
	return the evalution value with format "move+evaluationValue" eg. res = 'D2234' res[0] and res[1] are the move, the 234 are the evalutation value.	
 
	 get a list of possible moves in this board, assume max means white move
 
	 if max:
	for each move in the move list:
		set the board to the new move
		res = max(res,miMax(board,depth-1,a,b,False))
		a = max(a,res)
		if(a >=b){pruning}
	return res
	  if min:
	for each move in the movelist:
		set the board after the new move
		res = min(res,minMax(board,depth-1,a,b,True))
		b = min(b,res)
		if(a>=b){pruning}
	return res
*/
	if(!board.check(max)) {
        if(board.check(1-max) && depth) {
            board.turn++;
            return minMax(board,depth-1,a,b,1-max);
        }
        int count = board.check_win();
        if(count == 0) return "0";
        if(count > 0) return "1000000000";
        else return "-1000000000";
	}
	if(depth == 0){
	    //cout<<"find leaf with evaluation value"<< evaluation(board,max)<<endl;
		return evaluation(board,true);
	}
	
	vector<string> moves;
	int x,y;
	string target="NO";
	string ff;
	//cout<<"Current depth is "<<depth<<endl;
	if(max){
	    //cout<<"max"<<endl;
	    int res = std::numeric_limits<int>::min();
	    moves = board.getAllMoves(true);
        random_shuffle(moves.begin(), moves.end());
	    //cout<<"find possible moves "<< moves.size()<<endl;
	    for(int i=0;i<moves.size();i++){
		    string move = moves[i];
		    //get current move
		    x = move[0] - '0';
		    y = move[1] - '0';
		    //cout<<"Possible move are "<<move <<" x: "<<x << " y " << y<<endl;
		    //set the new board
		    Board new_board;
		    board.import(new_board);
		    new_board.set(x, y, 'b');
		    //get result from next layer
		    target = minMax(new_board,depth-1,a,b,false);
		    //if(depth == DEPTH)
            //    cout<<"evaluation for this max round is " << target<<endl;
		    int eval = std::stoi(target);
		    
		    if(eval>res){
			    ff = std::to_string(x) + std::to_string(y) + target;
			    res = eval;
		    }
		    
		    a = std::max(a,res);
		    if(a>=b){
			    break;
		    }
	    }
	}
	else{
	    //cout<<"min"<<endl;
	    int res = std::numeric_limits<int>::max();
        moves = board.getAllMoves(false);
        random_shuffle(moves.begin(), moves.end());
        for(int i=0;i<moves.size();i++){
            string move = moves[i];
            //get current move
            x = move[0] - '0';
            y = move[1] - '0';
            //out<<"Possible move are x: "<<x << "y" << y<<endl;
            //set the new board
            Board new_board;
            board.import(new_board);
            new_board.set(x, y, 'w');
            //get result from next layer
            target = minMax(new_board,depth-1,a,b,true);
            //target format = "move + value"
            //if(depth == DEPTH-1)
            //    cout<<"evaluation for this min round is " << target<<endl;
            int eval = std::stoi(target);

            if(eval<res){
                ff = std::to_string(x) + std::to_string(y) +target;
                res = eval;
            }
            
            b = std::min(b,res);
            if(a>=b){
                break;
            }
        }
	}
	if(moves.size()==0){
	    return "-1";
	}
	
	//cout<<"DEpth: "<<depth <<" evaluation for this round is " << final<<endl;
	//board.show();
	
	if(depth == DEPTH){
	    //cout<<"OK the final is "<<final<<endl;
        //cout << ff << endl;
	    return ff.substr(0,2);
	}
	
	return ff.substr(2,ff.size()-1);
}



void game(Board &board) {
    int turn = board.get_turn();
    if(turn % 2 == 1) {
        if(board.check(0) == 0) {
            if(board.check(1)) {
                board.turn++;
                game(board);
                return;
            }
            int count = board.check_win();
            if(count > 0) {
                cout << "You win!" << endl;
            } else if(count == 0) {
                cout << "drew" << endl;
            } else {
                cout << "You lose" << endl;
            }
            return;
        }
        cout<<"--------------------------------------------"<<endl;
        cout << "Please choose a position for white(A1 to H8)" << endl;
        string op;
        cin >> op;
        int x, y;
        x=op[1]-'1'; y = op[0]-'A';
        bool ok = board.isok(x, y, 'w');
        while(!ok || op.size() != 2 || op[0] > 'H' || op[0] < 'A' || op[1] > '8' || op[1] < '1') {
              cout << "Invalid input!\nPlease choose a position for white(A1 to H8)" << endl;
              cin >> op;
              x=op[1]-'1'; y = op[0]-'A';
              ok = board.isok(x, y, 'w');
        }
        board.set(x, y, 'w');
        board.show();
        game(board);
    } else {
        if(!board.check(1)) {
            if(board.check(0)) {
                board.turn++;
                game(board);
                return;
            }
            int count = board.check_win();
            if(count > 0) {
                cout << "You win!" << endl;
            } else if(count == 0) {
                cout << "drew" << endl;
            } else {
                cout << "You lose" << endl;
            }
            return;
        }
        
        string op;
        op = minMax(board, DEPTH, std::numeric_limits<int>::min(),std::numeric_limits<int>::max(),true);
        int x,y;
        x=op[0]-'0';
        y=op[1]-'0';
        //cout<<"The result op is "<< op<<endl;
        cout<<"The recommend move is " << x+1 << " : " << y+1<<endl;
        board.set(x, y, 'b');

        board.show();
        game(board);
    }
}

int main(int argc, const char * argv[]) {
	  
    Board board;
    board.show();
    game(board);
    return 0;
}
