// Made by Gaurav Mahendraker


#include <bits/stdc++.h>
using namespace std;
// #define compress_file _compress_file
// #define Node node
// #define _get_huffman_representation construct_repr
// #define _write_string insert_string

class node{
    private:
    
    public:
    node* left,* right;
    long long int freq;
    unsigned char c;
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
//comparator for priority_queue
struct comp{
    bool operator()(node* a, node* b)
    {
        return (a->freq) > (b->freq);
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

void insert_string(ofstream &out, string &s, bool pad = false)
{
    static unsigned char temp = '\0';
    static int counter = 0;
    long long int size = s.size();
    for (long long int i = 0; i < size; i++)
    {
        temp = temp << 1 | (s[i]-'0');
        counter++;
        if (counter == 8)
            out.put(temp), counter = 0, temp = '\0';
    }
    if (counter && pad)
    {
        // cout<<"yes\n";
        while(counter!=8) counter++, temp <<= 1;
        counter = 0, out.put(temp);
    }
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
        cnt[int(ch)%256]++;
        total_chars++;
    }
    in.clear();
    in.seekg(0,ios::beg); //to read from begining of the file again;
    priority_queue<node*,vector<node*>,comp>pq; //huffman heap
    for(int i=0;i<256;i++)
    {
        if(cnt[i]>0)
        {
        pq.push(new node(cnt[i],(unsigned char)i));
        }
    }
    int16_t total_distinct_chars=pq.size();
    while(pq.size()>1)
    {
        node* left,*right;
        left=pq.top();
        pq.pop();
        right = pq.top();
        pq.pop();
        long long int fre=(left->freq)+(right->freq);
        pq.push( new node(fre,'\0',left,right));
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
        if(repr[i]!="")//if present in char then encoding the ascii of char and binary represnetation relation
        {
            out.put(i); // first 8 bit represnt the key char
            int16_t total_bits_repr=repr[i].length(); //next 16 bit represnt the number of bits in value of key
            out.put((total_bits_repr>>8)%256);
            out.put(total_bits_repr%256);
            insert_string(out,repr[i],true);

        }
    }
    // 64 bit number representing the number of chars
    for(int i=56;i>=0;i-=8)
    {
        out.put((total_chars >> i)%256);
    }
  //  cout<<total_chars;
    int count=0;
    string temp1;
    while(in.get(ch))
    {

        insert_string(out,repr[(256+ch)%256],false);
        count++;
    }
   // cout<<count<<endl;
    string temp;
    insert_string(out,temp,true);
    cout<<"Compression successful..."<<endl;
    return ;

}
int min(int a, int b)
{
    if(a<b)
    {
        return a;
    }
    return b;
}
//converts charecter to string 
string ch_to_str(char c,int nums)
{
    int val=(int)c;
    vector<int>temp(8);
    for(int i=7;i>-1;i--)
    {
        if((1<<i)&val)
        {
            temp[7-i]=1;
        }
    }
    string ans;
    for(int i=0;i<nums;i++)
    {
        ans+=(temp[i]+'0');
    }
    return ans;
}
node* construct_trie(vector<string> & repr)
{
    node* root=new node(-1,'\0');
    for(int i=0;i<256;i++)
    {
        if(repr[i]!="")
        {
            node* cur=root;
            for(int j=0;j<repr[i].length();j++)
            {
                if(repr[i][j]=='1')
                {
                    if(!cur->right)
                    {
                        cur->right=new node(-1,'\0');
                    }
                    cur=cur->right;
                }
                else{
                    if(!cur->left)
                    {
                        cur->left= new node(-1,'\0');
                    }
                    cur=cur->left;
                }
            }
            cur->c=i+'\0';
        }
    }
    return root;
}
void decode_file(ifstream & in, ofstream & out)
{
    if(!in || !out)
    {
        cout<<"Incorrect path"<<endl;
        return ;
    }
    char ch;
    int16_t number_of_chars=0;
    in.get(ch);
    number_of_chars+=((int)ch)<<8;
    in.get(ch);
    number_of_chars+=((int)ch);
    vector<string>repr(256);//stores string representation of each char
    
    for(int i=0;i<number_of_chars;i++)
    {
        in.get(ch);
        int ind=(ch+256)%256;
        int16_t bit_len=0;
        in.get(ch);
        bit_len+=((int)ch)<<8;
        in.get(ch);
        bit_len+=((int)ch);
        // int chars_to_read=(bit_len)/8;
        // if(bit_len%8)
        // {
        //     chars_to_read++;
        // }
        string s;
        while(bit_len)
        {
            in.get(ch);
            s+=ch_to_str(ch,min(8,bit_len));
            bit_len-=min(8,bit_len);
        }
        repr[ind]=s;

    }
    node* trie=construct_trie(repr);
    int tot_chars;
    for(int i=0;i<8;i++)
    {
        in.get(ch);
        long long int val=(int)ch;
        tot_chars+=((val)<<((7-i)*8));

    }
    node* cur=trie;
    int count=0;
  //  cout<<"going..";
    while(in.get(ch))
    {
        count++;
        int val=(ch+256)%256;
      //  cout<<val<<endl;
        for(int i=7;i>-1;i--)
        {
            if(cur->c!='\0')
            {
                
                out.put(cur->c);
               // cout<<cur->c;
                cur=trie;
            }
            if(val&(1<<i))
            {
                if(cur->right)
                {
                cur=cur->right;
                }
            }
            else
            {
                if(cur->left)
                {
                cur=cur->left;
                }
            }
        }
    }
    cout<<count<<endl;
    cout<<"Decoding successful..."<<endl;
    return ;
    

}
void compress(string input_path, string output_path)
{
    ifstream in(input_path,ios::binary); //reads file
    if(!in)
    {
        cout<<"Incorrect input path"<<endl;
        return;
    }
    ofstream out(output_path+".ziph",ios::binary); //writes on file
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

 void decode(string & input_path, string & output_path)
 {
    ifstream in(input_path,ios::binary);//this is ziphuffman file
    ofstream out(output_path+".txt",ios::binary);//here this is textfile
    if(!in || !out)
    {
        cout<<"error reading or writing the files"<<endl;
    }
    cout<<"Decoding Started..."<<endl;
    decode_file(in,out);
    in.close();
    out.close();
    cout<<"Decoding finished.."<<endl;
    
 }
void help_banner()
{
    cout<<"\nThis program implements huffman coding algorithm to compress and decompress any type of files."<<endl;
    cout<<"Usage: ./huffman [command] [input_file] [output_file]"<<endl;
    cout<<"Alternatively, you can run the program without any arguments and follow the instructions."<<endl;
    cout<<"Commands:"<<endl;
    cout<<"compress (c) - Compresses the input file and saves it as output file"<<endl;
    cout<<"decompress (d) - Decompresses the input file and saves it as output file"<<endl;
    cout<<"help (h) - Displays this help message"<<endl;
    cout<<"exit - Exits the program\n"<<endl;
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
        else if(command=="decode" || command=="d")
        {
            string input_file_path=argv[2],output_file_path=argv[3];
            cout<<"Decoding"<<input_file_path<<" to "<<output_file_path<<endl;
            decode(input_file_path,output_file_path);
        }
         else if (command == "help" || command == "h" || command == "--h")
            help_banner();
        else
            cout << "Invalid command" << endl;


    }
    else
    {
        cout << "Welcome to Huffman Compressor Decompressor Program\nCreated by Gaurav Mahendraker. All rights reserved.\nPress enter help(h) for instructions." << endl;
        string command, input_file, output_file;
        while (true)
        {
            cout << "\nEnter command: ";
            cin >> command;
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "compress" || command == "c")
            {
                cout << "Enter input file: ";
                cin >> input_file;
                cout << "Enter output file (without file extension): ";
                cin >> output_file;
                cout << "Compressing " << input_file << " to " << output_file + ".ziph" << endl;
                compress(input_file, output_file);
            }
            else if (command == "decompress" || command == "d")
            {
                cout << "Enter input file (with .ziph extension): ";
                cin >> input_file;
                cout << "Enter output file (without original file extension): ";
                cin >> output_file;
                cout << "Decompressing " << input_file << " to " << output_file << endl;
                decode(input_file, output_file);
            }
            else if (command == "help"|| command == "h" || command == "--h")
                help_banner();
            else if (command == "exit")
            {
                cout << "\nExiting...\nThank you for using the program!" << endl;
                break;
            }
            else
                cout << "Invalid command" << endl;
        }
    }
}