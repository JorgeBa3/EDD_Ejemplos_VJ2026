#include <iostream>

using namespace std;

int matriz[3][3] = {
    {1,2,3},
    {4,5,6},
    {7,8,9}
};

int main(){
    cout << "Orden en memoria Row-Major: " << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << "- ";
        
    }
    cout << endl;
    cout << "Orden en memoria Column-Major" << endl;
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << " -";
    }
    
    return 0;
}