 
 #include <cs50.h>
 #include <stdio.h>
 #include <string.h>
 #include <time.h>
 
 //prototype
 int iterator(char *guess, int *char_set, int this_size, int position);


 
 //this is here to help in debugging the code to allow a smaller char_set     
 int char_set_size = 94; 
 bool verbose = true;
 int long long counter = 0;
 
 int main (void)
 {
 
    //start timer
    time_t start, end;
    double dif;
    time (&start);
    
     //maximum size of word to guess 
    int max_size = 8; 
     
    // Create an array of all possible ASCII characters
    int char_set[char_set_size];
    for (int i = 0; i < char_set_size; i++)
    {
        char_set[i] = (i + 33);
    }
  
    // print array to make sure the characters are correct 
    /*  
    for (int i = 0; i < char_set_size; i++)
    {    
        printf("i: %d = %c\n", i, (char)char_set[i]);
    } 
    printf("sizeof(char_set): %d\n", sizeof(char_set));
    */  
    
    // call iterator starting with a 4 letter word and working up to max_size
    for (int this_size = 8; this_size <= max_size; this_size++)
    {   
        //define word
        char word[this_size + 1];
    
         // declare starting characters for word
        for (int i = 0; i < this_size; i++)
        {
            word[i] = (char)char_set[0];
            
            //terminate string
            if (i == (this_size - 1))
                word[i + 1] = '\0';
        }
        printf("starting word: %s\n", word);
     
        iterator(word, char_set, this_size, 0);
        //printf("%s\n", word);
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
 
 // Recursively iterate through each possible character in char_set
 int iterator(char *guess, int *char_set, int this_size, int position)
 {
   
    //base case
    if (position == (this_size - 1)) // this is the right most digit
    {   
         
        //check every character in char_set
        for(int i = 0; i < (char_set_size - 1); i++)
        {
            //incremet position
            guess[position] = char_set[i];
            counter++;
            
            if (counter % 10000000 == 0)
                printf("counter: %lld, guess: %s\n", counter, guess);
            
            //crack_it
            //printf("%s\n", guess);
            //return 0; // for success
            
        } 
        return 1; // not found
        
    }
    
    //recursive case
    else
    {
        int temp;
        
        for(int i = 0; i < (char_set_size - 1); i++)
        {   
            guess[position] = (char)char_set[i];
            temp = iterator(guess, char_set, this_size, (position + 1));
              
        }
        return temp;
       
        
    }
    
   
 }
 
 
 
 
