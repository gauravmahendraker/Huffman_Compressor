// Made by Gaurav Mahendraker


#include <bits/stdc++.h>
using namespace std;

class node{
    private:
    node* left,* right;
    long long int freq;
    char c;
    public:
    node(){

    }
    node(long long int fre,char p)
    {
        c=p;
        freq=fre;
        left=NULL;
        right=NULL;

    }
    node(long long int fre, char p, node* l, node* r)
    {
        left=l;
        right=r;
        c=p;
        freq=fre;
    }
    bool is_leaf(){
        return (left==NULL)&&(right==NULL);
    }
};
void compress_file(ifstream & in,ofstream & out)
{
    
}
void compress(string input_path, string output_path)
{
    ifstream in(input_path); //reads file
    if(!in)
    {
        cout<<"Incorrect input path"<<endl;
        return;
    }
    ofstream out(output_path+".ziph"); //writes on file
    if(!out)
    {
        cout<<"Incorrect output path"<<endl;
        return;
    }
    cout<<"Compression Started..."<<endl;
    compress_file(in,out);
    out.close();
    in.close();

    cout<<"Compression Finished..."<<endl;

}
int main(int argc,char *argv[]) //comand line arrguments in c++ argc is argument count and argv is argument values
{
    if(argc>1)
    {
        string command= argv[1];
        if(command=="compress" || command=="c")
        {
            string input_file_path=argv[2],output_file_path=argv[3];
            cout<<"Compressing "<<input_file_path<<" to "<<output_file_path<<endl;
            compress(input_file_path,output_file_path);
            
        }


    }
}