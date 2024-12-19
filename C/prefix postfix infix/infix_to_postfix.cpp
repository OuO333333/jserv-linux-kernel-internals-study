#include <stdio.h>
#include <vector>
#include <iostream>
#include <string.h>

using namespace std;
vector<char> mystack;

char input[100];

int main(){
    while(scanf("%s", input)){
        mystack.clear();
        for(int i=0; i<strlen(input); i++){
            if(input[i]>= '0' && input[i] <= '9')
                printf("%c", input[i]);
            else if(input[i] == ')'){
                printf("%c", mystack.back());
                mystack.pop_back();
            }
            else if(input[i] != '(')
                mystack.push_back(input[i]);
        }
        while(mystack.empty()!=true){
            printf("%c", mystack.back());
            mystack.pop_back();
        }
        printf("\n");
    }
    return 0;
