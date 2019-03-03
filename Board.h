//
//  Board.h
//  othello
//
//  Created by Yakumorin on 2018/11/10.
//

#ifndef board_h
#define board_h
#include<cstdlib>
#include<iostream>

int dx[8]={0,0,1,1,1,-1,-1,-1};
int dy[8]={1,-1,-1,0,1,-1,0,1};

class Board{
    
public:
	char b[8][8];
	int turn;
    Board() {
        turn = 4;
        for(int i=0; i<8; ++i)
            for(int j=0; j<8; ++j)
                b[i][j] = '.';
        b[3][3] = b[4][4] = 'w';
        b[4][3] = b[3][4] = 'b';
    }
    void import(Board &newBoard) {
        for(int i = 0;i<8;++i){
        	for(int j = 0;j<8;++j){
        		newBoard.b[i][j] = b[i][j];
        	}
        }
        newBoard.turn = turn;
    }
    void show() {
        std::cout << " ";
        for(int i=0; i<8; ++i)
            std::cout << char('A'+i);
        std::cout << std::endl;
        for(int i=0; i<8; ++i) {
            std::cout<<i+1;
            for(int j=0; j<8; ++j)
                std::cout << b[i][j];
            std::cout << std::endl;
        }
        if(turn % 2 == 0) {
            std::cout << "Now is black turn" << std::endl;
        } else {
            std::cout << "Now is white turn" << std::endl;
        }
    }
    
    void flip(int x, int y, int dir, char c) {
        int nx=x+dx[dir], ny=y+dy[dir];
        while(b[nx][ny] != c) {
            b[nx][ny] = c;
            nx=nx+dx[dir];
            ny=ny+dy[dir];
        }
    }
    
    bool set(int x, int y, char c) {
        b[x][y] = c;
        turn++;
        for(int k=0; k<8; ++k) {
            int nx=x+dx[k], ny=y+dy[k];
            int ok = 0;
            if(nx<0 || nx>=8 || ny<0 || ny>=8) continue;
            if(b[nx][ny] != c && b[nx][ny] != '.') ok = 1;
            else continue;
            while(1) {
                nx=nx+dx[k];
                ny=ny+dy[k];
                if(nx<0 || nx>=8 || ny<0 || ny>=8) break;
                if(b[nx][ny] == '.') break;
                if(b[nx][ny] == c) {
                    ok = 2;
                    break;
                }
            }
            if(ok == 2) {
                flip(x, y, k, c);
            }
        }
        return true;
    }
    
    bool isok(int x, int y, char c) {
        if(b[x][y] != '.') return false;
        for(int k=0; k<8; ++k) {
            int nx=x+dx[k], ny=y+dy[k];
            int ok = 0;
            if(nx<0 || nx>=8 || ny<0 || ny>=8) continue;
            if(b[nx][ny] != c && b[nx][ny] != '.') ok = 1;
            else continue;
            while(1) {
                nx=nx+dx[k];
                ny=ny+dy[k];
                if(nx<0 || nx>=8 || ny<0 || ny>=8) break;
                if(b[nx][ny] == '.') break;
                if(b[nx][ny] == c) {
                    ok = 2;
                    break;
                }
            }
            if(ok == 2) return true;
        }
        return false;
    }
    bool check(bool black, bool hint=false) {
        for(int i=0; i<8; ++i)
            for(int j=0; j<8; ++j)
                if(isok(i, j, black?'b':'w')) {
                    if(hint)
                        std::cout << ('w') <<" is ok at (" << char(j+'A') << "," << i+1 << ")." << std::endl;
                    return true;
                }
        return false;
    }

    
    std::vector<std::string> getAllMoves(bool black){
    	std::vector<std::string>moves;
    	for(int i=0; i<8; ++i){
            for(int j=0; j<8; ++j){
                if(black && isok(i, j, 'b')) {
                    std::string tmp = "";
                    tmp = std::to_string(i) + std::to_string(j);
                    //std::cout<<"TMp is "<<tmp<<" " << tmp[0] <<" " << tmp[1]<<std::endl;
                    moves.push_back(tmp);
                }
                if(!black && isok(i, j, 'w')) {
                    std::string tmp = "";
                    tmp = std::to_string(i) + std::to_string(j);
                    //std::cout<<"TMp is "<<tmp<<" " << tmp[0] <<" " << tmp[1]<<std::endl;
                    moves.push_back(tmp);
                }
            }
    	}
    	return moves;
    }
    
    int cornerCheck(bool black){
    	int res = 0;
    	char target = 'w';
    	if(black){
    		target = 'b';
    	}
    	if(b[0][0] == target)res++;
    	if(b[0][7] == target)res++;
    	if(b[7][0] == target)res++;
    	if(b[7][7] == target)res++;
    	return res;
    }
    
    int getDisc(bool black) {
        int res = 0;
        char target = black?'b':'w';
        for(int i=0; i<8; ++i)
            for(int j=0; j<8; ++j) {
                if(b[i][j] == '.') continue;
                if(b[i][j] == target) res++;
                if(b[i][j] != target) res--;
            }
        return res;
    }

    int check_win() {
        int count = 0;
        for(int i=0; i<8; ++i)
            for(int j=0; j<8; ++j) {
                if(b[i][j] == 'b') count++;
                if(b[i][j] == 'w') count--;
            }
        return count;
    }
    
    int get_turn() {
        return turn;
    }
    
};


#endif /* board_h */
