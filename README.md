# Project 3: Markovian Tweets
CPEG 676-010, Fall 2017 (http://sec.prof.ninja/projects/)  

## Team Members: 
Casey Campbell, Miguel Hernandez, and Benjamin Steenkamer  

## Project Description
(Due December 13th) (no red-team)  
The task is to take in sample text and generate a tweet (280 characters) in the style of that text using Markov Chains.
For super-extra credit consume a user name from Twitter and grab some tweets by the person as your source material.  
Python 3 must be installed for the `get_tweets.py` script to run.  
This API needs to be installed: `sudo pip3 install tweepy`  
Twitter API keys from `get_tweets.py` have been purposely removed. You must provide your own!  
Tested to work on GNU/Linux.  

## How To Use  
To compile the program, type: `make`  
This will produce an executable called `markov_generator`. Next you'll need to open up `get_tweets.py`. Inside this,
you will need to provide API keys for the following variables: `consumer_key`, `consumer_secret`, `access_key`, and 
`access_secret`. These values come from <https://apps.twitter.com/>. You will need to 
create a Twitter account, create an new application entry, and then copy and paste the consumer and access keys 
provided by Twitter. Now the automatic tweet fetcher can be run. This program will work without these keys, but
you will only be able to read from local text files.  
  
To run the program, type `./markov_generator` followed by the path to a text file or a Twitter user name.
The Twitter user name must have the "@" symbol in front of it. Here are some examples:  
`./markov_generator textfiles/brady.txt`  
`./markov_generator @Vinesauce`  
  
If you give the program a text file, it will read each line as a new "tweet". Simply add a new line at the end of
every sentence if you want the program to consider each line as a separate tweet. Shorter sentences/lines will cause the generator
to create shorter tweets. Very long lines or sentences not ended with new a line will cause the generator to almost always 
create maximum length tweets (280 characters).  
  
If you instead give the generator a twitter user name, the main program will call the `get_tweets.py` file.
This file uses the Twitter API to get the last 200 tweets by a user. It then puts these tweets in a file called
"textfiles/user-name-here.txt". The python script will then return control to the main program. The main program will
open the newly created text file and use its contents to generate tweets.  
  
Regardless of how the source text file is created, the program will generate 10 tweets and then ask you if you want to 
generate 10 more. If you select "no," the program will exit. The generator may create very short tweets by chance, and in most cases, the tweets with be gibberish.  

## Resources Used: 
<http://www.bitsofpancake.com/programming/markov-chain-text-generator/>  
<https://gist.github.com/yanofsky/5436496>  
<http://adilmoujahid.com/posts/2014/07/twitter-analytics/>  
<http://docs.tweepy.org/en/v3.5.0/api.html>  
