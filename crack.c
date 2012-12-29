

/* 
 * Crack by Chris Fortier
 * 
 * This program will accept a DES hash (presumably form a *NIX /etc/passwd file) and will attempt to crack it.
 * 
 * The first attempt is through a list of 10,000 most commonly used passwords obtained from:
 *                  http://xato.net/passwords/more-top-worst-passwords/
 * The next attempt uses the /usr/share/dict/words file
 * 
 * Finally we recursively go through every possible combination of valid ASCII characters hoping to find the password. 
 *
 */

#define _XOPEN_SOURCE
#define CHAR_SET_SIZE 94

#include <stdio.h>
#include <cs50.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <time.h>

// GLOBALS
// these are used by multiple functions within the program and declared here to provide easy access.
string password_hash;
char salt[2];
int long long counter = 0;
char *PASSWORD = NULL;
bool found = false;
bool verbose = true;
 
//PROTOTYPES
int crack_it(char *word);
int common_passwords(void);
int linux_words(void);
int brute_force(void);
int iterator(char *guess, int *char_set, int this_size, int position);

//main (duh)
int main (int argc, string argv[])
{

    //user enters a hashed value to try and crack
    if (argc == 2)
    {
        //assign password_hash
        password_hash = argv[1];
        
        //get salt from hash
        salt[0] = (char)password_hash[0];
        salt[1] = (char)password_hash[1];
        //printf("[0]: %d\n", password_hash[0]);
        //printf("[1]: %d\n", password_hash[1]);
        
        printf("\n");
        //formatting:
        if (verbose)
        {
            printf("\n");
            for (int i = 0; i < 51; i++)
            {    
                printf("*");
            }
            printf("\n");
            //printf("  ");
        }
        //PASSWORD = "TEMP";
        //printf("PASSWORD: %s\n", PASSWORD);        
    }
    else 
    {
        printf("\nYou did not enter a hash. Program is exiting now.\n");
        return 1;
    }
    
    //start timer
    time_t start, end;
    double dif;
    time (&start);
    
    //call common_passwords
    if (!found)
    {
        if(verbose)
        {
            printf("Checking a list of common passwords...\n");
        }
        int common_passwords_return = common_passwords();    
        //printf("common_passwords_return: %d\n", common_passwords_return);
         //printf("password: %s\n", PASSWORD);
        if (common_passwords_return == 0) 
            found = true;
    }
    
    //call linux_words
    if (!found)
    {
        if(verbose)
        {
            printf("Checking a list of dictionary words...\n");
        }
        int linux_words_return = linux_words();    
        //printf("linux_words_return: %d\n", linux_words_return);
        if (linux_words_return == 0) 
            found = true;
    }
    
    //call brute_force
    if (!found)
    {
        if(verbose)
        {
            printf("Starting a brute force attack...\n");
        }
        int brute_force_return = brute_force();    
        //printf("brute_force_return: %d\n", brute_force_return);
        if (brute_force_return == 0) 
            found = true;
    }
    
        
    //all loops processed
    if (found)
    {   
        if (verbose)
        {  
            printf("Found it The password is: %s\n", PASSWORD);
        }
        else
        {
            printf("%s\n", PASSWORD);
        }
  
    }
    else
    {
        if (verbose)
        {
            printf("A match could not be found :(\n");
        }
    }
    
    //end timer
    time (&end);
    dif = difftime(end, start);
    
    //output stats
    if (verbose)
    {
        printf("There were %llu combinations checked.\n", counter);    
        printf("The program ran for: %.0lf seconds.\n", dif);
        printf("Processed: %.0f combinations per second.\n", (counter/dif));
        
        //formatting:
        printf("\n");
        for (int i = 0; i < 51; i++)
            printf("*");
        printf("\n");
    }
    
    return 0;
}

/* 
 * crack_it - this accepts a word provided by one of the functions within the program. 
 * It removes any newline or carriage returns (windows) and creates a hash. 
 * Then compares that hash to the password_hash provided
 * 
 */
int crack_it(char *word)
{

    //trim \n & \r
    if( word[strlen(word)-1] == '\n' )
        word[strlen(word)-1] = '\0';
    
    //printf("strlen(line): %d\n", strlen(line));
    
    if( word[strlen(word)-1] == '\r' )
        word[strlen(word)-1] = '\0';
    
    char *word_hash = crypt(word, salt);
    
    //debug stuff
    //printf("%s\n", word_hash);
    //printf("\nsalt: %d, %d, %s, %s, : %s, %d\n", salt[0], salt[1], password_hash, word, word_hash, strcmp(password_hash, word_hash))
    //printf("%d\n", strcmp(password_hash, word_hash) );
                        
    //check for match
    if (strcmp(password_hash, word_hash) == 0)
    { 
        PASSWORD = word;
        //printf("password_hash: %s\n", password_hash); 
        //printf("crack_it:PASSWORD: %s\n", PASSWORD); 
        //printf("crack_it:word: %s\n", word);
        return 0; //for success       
    }
    else
    {
        return 1; //for no match
    }
}

//checks through the 10,000 most common passwords
int common_passwords(void)
{
    //open file for dictionary attack and read into array
    FILE *common_passwords = fopen("most_common_passwords.txt", "r");
    
    //read each line of most_common_passwords and pass to crack it
    char word[80];
        
    while (fgets ( word, sizeof(word), common_passwords) != NULL)
    {
        //call crack_it
        int crack_it_return =  crack_it(word); 
        //printf("crack_it_return: %d  \n",crack_it_return);
        counter++;
        
        if (crack_it_return == 0)
        {   
            //password = word;
            //printf("%s\n", password); 
            //printf("%s\n", word); 
            //notFound = false;
            fclose(common_passwords);
            return 0;
             
        }
    }
    
    //close file
    fclose(common_passwords);
    
    return 1;
}

//checks through all the words in /usr/share/dict/words
int linux_words(void)
{
    //open file for dictionary attack and read into array
    FILE *linux_words = fopen("/usr/share/dict/words", "r");
    
    //read each line of most_common_passwords and pass to crack it
    char word[80];
        
    while (fgets ( word, sizeof(word), linux_words) != NULL)
    {
        //call crack_it
        int crack_it_return =  crack_it(word); 
        //printf("crack_it_return: %d  \n",crack_it_return);
        counter++;
        
        if (crack_it_return == 0)
        {   
            //password = word;
            //printf("%s\n", password); 
            //printf("%s\n", word); 
            //notFound = false;
            fclose(linux_words);
            return 0;
             
        }
    }
    
    //close file
    fclose(linux_words);
    
    return 1;
}

int brute_force (void)
 {
     //maximum size of word to guess 
    int max_size = 8; 
     
    // Create an array of all possible ASCII characters
    int char_set[CHAR_SET_SIZE];
    for (int i = 0; i < CHAR_SET_SIZE; i++)
    {
        char_set[i] = (i + 33);
    }
  
    // print array to make sure the characters are correct 
    /*  
    for (int i = 0; i < CHAR_SET_SIZE; i++)
    {    
        printf("i: %d = %c\n", i, (char)char_set[i]);
    } 
    printf("sizeof(char_set): %d\n", sizeof(char_set));
    */  
    
    // call iterator starting with a 4 letter word and working up to max_size
    for (int this_size = 4; this_size <= max_size; this_size++)
    {   
        if (!found)
        {   
            //define word
            char b_word[this_size + 1];
        
             // declare starting characters for word
            for (int i = 0; i < this_size; i++)
            {
                b_word[i] = (char)char_set[0];
                
                //terminate string
                if (i == (this_size - 1))
                    b_word[i + 1] = '\0';
            }
            //printf("starting word: %s\n", b_word);
         
            int iterator_out = iterator(b_word, char_set, this_size, 0);
            
            if (iterator_out == 0)
                found = true;
            //printf("%s\n", word);
        }
    }
    
    return 0; 
 }
 
 // Recursively iterate through each possible character in char_set
 int iterator(char *guess, int *char_set, int this_size, int position)
 {
  
    //base case
    if (position == (this_size - 1)) // this is the right most digit
    {   
         
        //check every character in char_set
        for(int i = 0; i < (CHAR_SET_SIZE - 1); i++)
        {
            //incremet position
            guess[position] = char_set[i];
            
            //printf("%s, %d\n", guess, strlen(guess));
            
            //call crack_it
            int crack_it_return =  crack_it(guess); 
            //printf("crack_it_return: %d  \n",crack_it_return);
            counter++;
            
            if (crack_it_return == 0)
            {   
                //password = word;
                //printf("%s\n", password); 
                //printf("%s\n", word); 
                //notFound = false;
                return 0; // for success    
            }            
        } 
        
        return 1; // not found
        
    }
    
    //recursive case
    else
    {
        int result;
        
        for(int i = 0; i < (CHAR_SET_SIZE - 1); i++)
        {   
            guess[position] = (char)char_set[i];
            result = iterator(guess, char_set, this_size, (position + 1));
            
            if (result == 0)
                return 0; //success
              
        }
        return 1; //not found
    }
 }

