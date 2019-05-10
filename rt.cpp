//#include<bits/stdc++.h> //inclui tudo mas compila mais lento
#include<iostream>
#include<fstream>
#include<vector>
#define pl '\n'
using namespace std;






int main(){
    const int W = 500;
    const int H = 500;
    ofstream out("out.ppm");//arquivo resultado
    out << "P3" << '\n' << W << '\n' << H << '\n' << "255" << '\n';


    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            //calcular cor de um pixel aqui

            //colocar r g b do pixel no out
            out << i << ' ' << j << ' ' << j << '\n';
        }
    }



}