#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
using namespace std;

// Class used to store a unique word that appears in a tweet. It contains a vector of all
// the words that appear after this particular word. Words placed in the followers vector don't have to
// be unique and can appear multiple times.
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


vector<Word> Markov_links;  // Vector containing the "Markov chain links". Holds every word found and the words seen following that particular word.
string tweet = "";          // Contains the words of the current tweet being generated.

// Returns the index of where a particular base word in Markov link vector
int get_link_index(string word){
    for(int i = 0; i < Markov_links.size(); i++){
        if(Markov_links.at(i).word.compare(word) == 0){
            return i;
        }
    }
    return -1;
}

// When a word not previously recorded is found in a tweet, add this new word and the word following it
// to the Markov links vector
// If the entry for a word already exists, go to that word's entry and add the follower word to its list of followers.
void add_pair(string word, string follower){
    int index = get_link_index(word);
    
    if(index == -1){
        Markov_links.push_back(Word(word, follower));
    }
    else{
        Markov_links.at(index).followers.push_back(follower);
    }
}

// Analyzes a single tweet (1 line from a text file)
// This breaks up the tweet into base words and the word following it.
void parse_line(string line){
    size_t word_start_index = 0;
    size_t word_end_index;
    
    size_t follower_start_index;
    size_t follower_end_index;
    
    size_t word_len;
    size_t follower_len;
    
    string word = "";
    string follower = "";
    
    // Goes through all the words in the tweet
    while(word_start_index != string::npos){
        word_end_index = line.find(" ", word_start_index);
        word_len = word_end_index - word_start_index;
        
        // Find the next base word
        while(word_len == 0){
            word_start_index++;
            word_end_index = line.find(" ", word_start_index);
            word_len = word_end_index - word_start_index;
            
            // Case where the last word in the tweet has been reached
            if(word_start_index >= line.length()){
                word_start_index = line.length(); // Should always be the length anyway due to logic of the find function, but set in case of boundary conditions 
                break;
            }
        }
        
        word = line.substr(word_start_index, word_len); // Now holds a single word from the tweet.
        
        // Now find the following word (if there is one)
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
            follower = line.substr(follower_start_index, follower_len); // Now contains the word right after the base word
            
            word_start_index = follower_start_index;    // The next base word will be the current base word's follower
        }
        else{
            follower = "";  // If the base word is that last word in the tweet, then it does not have a following word
            word_start_index = string::npos;
        }
        
        add_pair(word, follower);   // Add the base word and its follower to the vector of Markov links
                                    // or update the base word's vector of followers if there is already a record
    }
}

// Read in the text file and record the words+followers in each tweet
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

// Used to select a word o
int get_rand_index(int choices){
    return rand() % choices;
}

// Using the values in the Markov links, generate a tweet of up 280 characters long.
void write_tweet(){
    
    int word_index = get_rand_index(Markov_links.size()); // Randomly select the first word of the tweet
    int follower_index = 0;
    int follower_num = 0;
    
    bool tweet_done = false;
    string last_word = ""; // most recent word added, used to search vector
    
    tweet = "";
    tweet = Markov_links.at(word_index).word; // First word of the tweet
    last_word = tweet;  // Since the tweet is only 1 word long right now, first word = last word
    
    while(!tweet_done){
        word_index = get_link_index(last_word);
        Word temp = Markov_links.at(word_index);
        
        follower_num = temp.followers.size();
        follower_index = get_rand_index(follower_num);  // Randomly select the next word. Because of word distribution in sentences,
                                                        // the follower word, on average, should be the most frequent word seen after a given base word
        last_word = temp.followers.at(follower_index);
        
        if(last_word.length() == 0){    // If the last "word" is an empty string, then the tweet is complete.
            tweet_done = true;
        }
        else{
            if(tweet.length() + last_word.length() > 279){ // If adding the next word makes the tweet longer than 279 characters, don't add it to the tweet.
                tweet_done = true;
            }
            else{
                tweet = tweet + " " + last_word;    // Add the next word to the tweet with a space between the words. (This space is why we use the 279 above, instead of 280)
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
        cout << "Fetching tweets from user " << arguement << "..." << endl;
        
        string arg = "python3 get_tweets.py " + arguement;
        int err = system(arg.c_str());  // Use the python file to pull tweets from by user and store them in a text file.
        
        if(err == -1){
            cout << "Error running get_tweets.py" << endl;
        }
        
        arguement = "textfiles/" + arguement.substr(1, arguement.length()) + ".txt";
    }
    
    cout << "Parsing text file " << arguement << "..." << endl;
    
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
        cout << "File "<< arguement << " was not found" << endl;
    }
    
    return 0;
}