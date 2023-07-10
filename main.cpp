// Made by Gaurav Mahendraker


#include <bits/stdc++.h>
using namespace std;

class node{
    private:
    
    public:
    node* left,* right;
    long long int freq;
    char c;
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

struct comp{
    bool operator()(node* a, node* b)
    {
        return (a->freq) < (b->freq);
    }
};
void construct_repr(node* root,vector<string>& repr, string s) //fills the representation vector corresponding to each char
{
    if(!root)
    {
        return ;
    }
    if(root->is_leaf())
    {
        if(s=="")
        {
            repr[root->c]="0";
        }
        else
        {
            repr[root->c]=s;
        }

    }
    construct_repr(root->left,repr,s+'0');
    construct_repr(root->right,repr,s+'1');
}

//My compression will first store the huffman table then store the total number of bytes in input file and then compress input
void compress_file(ifstream & in,ofstream & out)
{
    vector<long long int> cnt(256,0); //stores count of each ascii in input file
    if(!in || !out)
    {
        cout<<"Incorrect path"<<endl;
        return ;
    }
    char ch;
    long long int total_chars=0;
    while(in.get(ch))
    {
        cnt[int(ch)]++;
        total_chars++;
    }
    in.clear();
    in.seekg(0,ios::beg); //to read from begining of the file again;
    priority_queue<node*,vector<node*>,comp>pq; //huffman heap
    for(int i=0;i<256;i++)
    {
        if(cnt[i]>0)
        {
        pq.push(new node(cnt[i],'\0'+i));
        }
    }
    int total_distinct_chars=pq.size();
    while(pq.size()>1)
    {
        node* left,*right;
        left=pq.top();
        pq.pop();
        right = pq.top();
        pq.pop();
        long long int fre=(left->freq)+(right->freq);
        pq.push(new node(fre,'\0',left,right));
    }
    vector<string> repr(256);//stores the binary representation of each char in string format
    node* root=pq.top();
    pq.pop();
    construct_repr(root,repr,"");
    //First a 16 bit (2 byte) number representing the number of total_distinct_chars;
    out.put(total_distinct_chars>>8);
    out.put(total_distinct_chars%256);
    for(int i=0;i<256;i++)
    {
        if(cnt[i])//if present in char then encoding the ascii of char and binary represnetation relation
        {
            out.put(i); // first 8 bit represnt the key char
            int16_t total_bits_repr=repr[i].length(); //next 16 bit represnt the number of bits in value of key
            out.put(total_bits_repr>>8);
            out.put(total_bits_repr%256);
            
        }
    }




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
