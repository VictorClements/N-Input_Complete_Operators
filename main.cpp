//including necessary libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
//for ease of use
using namespace std;

//might want to get rid of these in favor of dynamic arrays, but that'll be later
int const rows = 8;
int const cols = 8;

bool charToBool(char c);
int binaryToDecimal(int binary);
string decimalToBinary(int decimal, int bitwidth);

int main(){
    //ofstream variable to write to output file the results of our computations
    ofstream outfile;
    //Array to hold the literals
    bool literalArray[rows][cols];
    //string to hold which operator we want to check
    string binaryOperator;

    //prompt user for binary operator input
    cout << "enter the operator you would like to check in binary\n**";
    cin >> binaryOperator;

    //this code will be to essentially hardcode the inputs, since thats how all inputs are anyways, and its easier :)
    //populates array without the need of an input file, just need the operator we are testing (and the number of inputs)
    int inputs = (cols>>1)-1;   //half of the columns and minus the one output column
    string inputStr;
    cout << "number of inputs: " << inputs << endl;
    for(int i = 0; i < (1<<inputs); i++){   //for loop to go through all possible input combination counts in binary
        inputStr = decimalToBinary(i, inputs);  //inputStr holds the string of the current row's inputs, given by decimalToBinary function
        for(int j = 0; j < inputs; j++){
            literalArray[i][j] = charToBool(inputStr[j]);   //assigns the current element of literalArray to the boolean of the inputStr's current character
        }
        literalArray[i][inputs] = charToBool(binaryOperator[i]);
        for(int j = 0; j < inputs+1; j++){
            literalArray[i][j+inputs+1] = !literalArray[i][j];  //set second half of array to be the negation of first half
        }
    }

/*  //checks if array is properly populated
    cout << endl << "end of populating the array:" << endl << endl;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cout << literalArray[i][j] << " ";
        }
        cout << endl;
    }
*/

    //maybe we can include some error checking later

    //get the output file ready to be written to...
    string filename = binaryOperator+"outputs.txt";
    outfile.open(filename.c_str());

    int currentRowInput;
    int currentColumnArray[rows];
    bool exitFor = 0;
    bool badMatch = 0;
    int onesCount = 0;

    //now we want to calculate every possible combination :(
    //selects our three input columns, going through all possible combinations of input columns
    for(int i = 0; i < cols; i++){
        for(int j = 0; j < cols; j++){
            for(int k = 0; k < cols; k++){
                //once all 3 input columns have been selected, we need to find the matching row which will assign its truth value
                //write to output file the 3 input columns that have been selected
                outfile << i+1 << "," << j+1 << "," << k+1 << ": ";
                for(int h = 0; h < rows; h++){
                    //assigns currentRowInput to hold the number of which row will encode the truth value we want for the current binary number
                    currentRowInput = binaryToDecimal(literalArray[h][i]*100 + literalArray[h][j]*10 + literalArray[h][k]);
                    //now we need to take that row which we are reading from, and write it to the new truth table column for the current row output
                    currentColumnArray[h] = literalArray[currentRowInput][3];   //read from current row and the 3rd (actually 4th column) since it stores the encodings of the current truth assignments
                    outfile << currentColumnArray[h];   //write the column to output file
                }
                
                //now the currentColumnArray has been filled with the entire column it is meant to hold, and its been written to the output file, so now we want to check if it matches other literals
                for(int h = 0; h < cols; h++){
                    for(int g = 0; g < rows; g++){
                        //check if the current column matches all the elements in a single column from the literal array
                        if(currentColumnArray[g] != literalArray[g][h]) break;  //if any individual elements dont match, then break from inner for loop and check next column
                        if(g+1 == rows){    //otherwise if you are on the last row then every element has matched and you can set the exitFor flag to true, and output that we have a match!
                            exitFor = 1;
                            outfile << " Matches Column " << h+1 << "!";
                        }
                    }
                    // if we already hit a column match, then no need to continue looping:
                    if(exitFor) break;
                }
                //now, for a single input column, we have formed the output column, determined if it matches any of the literal columns, then printed that info to the screen
                //so move to next output line, and set the exitFor flag back to 0
                if(!exitFor)    badMatch = 1;
                outfile << endl;
                exitFor = 0;
                

               //now we shall check if the currentColumn array has 4 1's and 4 0's;
              /*
                onesCount = 0;
                for(int i = 0; i < rows; i++){
                    onesCount += currentColumnArray[i];
                }
                if(onesCount == 4)  outfile << " 4 ones!" << endl;
                else{
                    outfile << " I was wrong..." << endl;
                    badMatch = 1;
                }*/
            }
        }
    }
    if(badMatch)    outfile << "\ncould be a complete operator?" << endl;
    else            outfile << "\nIncomplete Operator!";
    outfile.close();
    cout << "Finished writing file" << endl;
    return 0;
}

bool charToBool(char c){
    switch(c){
        case '0':   return 0;
        default:    return 1;
    }
}

int binaryToDecimal(int binary){
    int remainder;
    int num = 0;
    for(int i = 0; binary != 0; ++i){
        remainder = binary % 10;
        binary = binary/10;
        num = num + remainder*pow(2,i);
    }
    return num;
}

string decimalToBinary(int decimal, int bitwidth){
    int power = 0;
    int binary = 0;
    while(decimal > 0){
        //if the remainer is 1, then take binary, and assign it to itself plus 2^power
        if(decimal%2)   binary = binary + pow(10, power);
        decimal = decimal/2;
        power++;
    }
    string tempStr = to_string(binary);
    string binaryStr = "";
    binaryStr.append(bitwidth-(tempStr.length()), '0');
    binaryStr += tempStr;
    return binaryStr;
}
