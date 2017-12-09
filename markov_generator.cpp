#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h> 
using namespace std;

class Word{
    public:
        string word;
        vector<string> followers;
        
        Word(string wrd, string follower){
            word = wrd;
            followers.push_back(follower);
        }
        
        void print_all(){
            print_word();
            print_followers();
        }
        
        void print_word(){
            cout << "Word: " << word << endl;
        }
        
        void print_followers(){
            cout << "Followers: ";
            for(int i = 0; i < followers.size(); i++){
                cout << followers.at(i) << ", ";
            }
            cout << endl;
        }
        
};

vector<Word> Markov_links;
int character_count = 0;
string tweet = "";

int get_link_index(string word){
    for(int i = 0; i < Markov_links.size(); i++){
        if(Markov_links.at(i).word.compare(word) == 0){
            return i;
        }
    }
    return -1;
}

void add_pair(string word, string follower){
    int index = get_link_index(word);
    
    if(index == -1){
        Markov_links.push_back(Word(word, follower));
    }
    else{
        Markov_links.at(index).followers.push_back(follower);
    }
}

void parse_line(string line){
    size_t word_start_index = 0;
    size_t word_end_index;
    
    size_t follower_start_index;
    size_t follower_end_index;
    
    size_t word_len;
    size_t follower_len;
    
    string word = "";
    string follower = "";
    
    while(word_start_index != string::npos){
        word_end_index = line.find(" ", word_start_index);
        word_len = word_end_index - word_start_index;
        while(word_len == 0){
            word_start_index++;
            word_end_index = line.find(" ", word_start_index);
            word_len = word_end_index - word_start_index;
            if(word_start_index >= line.length()){
                word_start_index = line.length(); // should always be the length anyway due to logic of the find function, but set in case of boundary conditions 
                break;
            }
        }
        word = line.substr(word_start_index, word_len);
        
        if(word_end_index != string::npos){
            follower_start_index = word_end_index + 1;
            follower_end_index = line.find(" ", follower_start_index);
            follower_len = follower_end_index - follower_start_index;
            
            while(follower_len == 0){
                follower_start_index++;
                follower_end_index = line.find(" ", follower_start_index);
                follower_len = follower_end_index - follower_start_index;
                if(follower_start_index >= line.length()){
                    follower_start_index = line.length(); // should always be the length anyway due to logic of the find function, but set in case of boundary conditions 
                    break;
                }
            }
            
            word = line.substr(word_start_index, word_len);
            follower = line.substr(follower_start_index, follower_len);
            
            word_start_index = follower_start_index;
        }
        // cout << line.substr(start_index, word_len) << endl;
        
        else{
            follower = "";
            word_start_index = string::npos;
        }
        add_pair(word, follower);
        // cout << "length: " << word.length() << endl;
        // cout << "word: " << word << endl;
        // cout << "follower: " << follower << endl;
        // cout << endl;
    }
}

int parse_text_file(string filename){
    string line;
    
    ifstream myfile(filename.c_str());
    if (myfile.is_open()){
        while(getline (myfile,line)){
            parse_line(line);
        }
        myfile.close();
        return 1;
    }
    else{
        return 0;
    }
}

void print_link(int index){
    Markov_links.at(index).print_all();
}

int get_rand_index(int choices){
    return rand()%choices;
}

void write_tweet(){
    int word_index = get_rand_index(Markov_links.size()); // index of the word
    
    int follower_index = 0;
    int follower_num = 0;
    
    bool tweet_done = false;
    string last_word = ""; // most recent word added, used to search vector
    
    tweet = "";
    tweet = Markov_links.at(word_index).word;
    last_word = tweet;
    
    
    while(!tweet_done){
        word_index = get_link_index(last_word);
        Word temp = Markov_links.at(word_index);
        
        follower_num = temp.followers.size();
        follower_index = get_rand_index(follower_num);
        last_word = temp.followers.at(follower_index);
        if(last_word.length() == 0){
            tweet_done = true;
        }
        else{
            if(tweet.length() + last_word.length() > 279){
                tweet_done = true;
            }
            else{
                tweet = tweet + " " + last_word;
            }
        }
    }
    cout << tweet << endl;
    cout << "[" << tweet.length() << " characters]" << endl;
}

void print_ten_tweets(){
    for(int i = 0; i < 10; i++){
        write_tweet();
        cout << endl;
    }
}

int main(int argc, char* argv[]){
    
    if(argc <= 1){
        cout << "Please provide path/to/file.txt or @Twitter_Username" << endl;
        return 0;
    }
    
    string arguement(argv[1]);
    srand(time(NULL));
    
    if(arguement.at(0) == '@'){
        cout << "Fecthing tweets from " << arguement << endl;
        
        // Need to sanitize the user name
        // arguement = arguement.substr(1, );  //Strip off the @ symbol
        // cout << "Created file of tweets: textfiles/" << arguement << ".txt" << endl;
    }
    
    cout << "Parsing text file..." << endl;
    if(parse_text_file(arguement)){
        cout << "Parsing complete. Data vector size: " << sizeof(vector<Word>) + (sizeof(Word) * Markov_links.size()) << " bytes" << endl;
        
        cout << "Generating 10 tweets using " << arguement << endl << endl;
        print_ten_tweets();
        
        string option = "n";
        while(true){
            cout << "Print 10 more tweets? (y/n) ";
            cin >> option;
            if(option.length() == 1 && option.compare("y") == 0){
                cout << "Generating 10 more tweets using " << arguement << endl << endl;
                print_ten_tweets();
            }
            else{
                break;
            }
        }
        
    }
    else{
        cout << "File "<< arguement << " is not found" << endl << endl;
    }
    return 0;
}