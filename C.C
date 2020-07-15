#include <stdio.h>

 



 

 

//We take two arrays X and Y of 16 element each.

 

int X[16] = {3, 14, 1, 10, 4, 9, 5, 6, 8, 11, 15, 2, 13, 12, 0, 7};

int Y[16] = {2, 14, 0, 4,13, 12, 3, 9, 5, 1, 10, 6, 7, 11, 8,15};

int XYmatrix[16][16];

 

//XOR between X and the key

 

int operation1(int i, int key)

{

    return X[key ^ i];   

}

 

//Encryption function by XORing the keys.

 

 

int encryptionfunc(int i, int key0, int key1)

{

    int X0 = operation1(i, key0);

    return X0 ^ key1;       

}

 

//The smallest value in the table is 0 and maximum value is 16 and occurs for many difference pairs.

//Now we generate matrix table of input and output difference.

 

 

void Differentialdistribution()

{

    printf("\n  Differential matrix between input and output\n");

   

    int in, out;

   

    for(in = 0; in < 16; in++)

        for(out= 0; out < 16; out++)

            XYmatrix[in ^ out][X[in] ^ X[out]]++;       

 

    for(in = 0; in < 16; in++)

    {

        for(out = 0; out < 16; out++)

            printf("  %d ", XYmatrix[in][out]);

        printf("\n");

    }

   

    printf("\nDisplaying most probable differentials based on probability:\n");

   

    for(in = 0; in < 16; in++)

        for(out = 0; out < 16; out++)

            if (XYmatrix[in][out] == 6)           

                printf(" %i --> %i has the probability 6/16\n", in, out); 

}

 

// Now creating 4 empty arrays for message and cipher texts.

 

int m0[1000];

int m1[1000];

int c0[1000];

int c1[1000];

 

int goodplain0, goodplain1, goodcipher0, goodcipher1;

 

int totalpairs;

 

int initialarray[16];

int initial  = 0;

 

 

void Generatingpairs(int differential_input, int differential_output)

{

            printf("\nGenerating pairs based on probability(%i --> %i):\n", differential_input, differential_output);

   

            initial  = 0;

            int a;

            for(a = 0; a < 16; a++)

            {

                int p = a ^ differential_input;

               

                if ((X[a] ^ X[p]) == differential_output)        

                {

                    printf("  Possible value pairs:   %i + %i --> %i + %i\n", a, p, X[a], X[p]);

                    initialarray[initial] = a;

                    initial++;

                }

            }   

}

 

//Creating random keys using rand function for mod 16 function to limit value under 16.

 

 

void Randomkeys(int differential_input)

{

    printf("\nRandom values based on differential difference %i\n", differential_input);

   

    int randomkey0 = rand() % 16;                                                       //Create random subkey0

    int randomkey1 = rand() % 16;                                                       //Create random subkey0

   

    printf("  Random Key0 = %i\n", randomkey0);

    printf("  Random Key1 = %i\n", randomkey1);

   

   

    for(int b = 0; b < totalpairs; b++)                                               //Create plaintext pairs with XOR difference of indiff

    {

        m0[b] = rand() % 16;

        m1[b] = m0[b] ^ differential_input;

        c0[b] = encryptionfunc(m0[b], randomkey0, randomkey1);

        c1[b] = encryptionfunc(m1[b], randomkey0, randomkey1);

    } 

}

 

// Finding good pairs by checking with the differential output.

 

void goodpairs(int differential_output)

{

    printf("\nSearching for best pairs........\n");

    for(int c = 0; c < totalpairs; c++)

        if ((c0[c] ^ c1[c]) == differential_output)                               //Does the ciphertext pair fit the characteristic?

        {  

            goodcipher0= c0[c];

            goodcipher0 = c1[c];

            goodplain0 = m0[c];

            goodplain1 = m1[c];  

            printf(" The matching pairs are: (P0 = %i, P1 = %i) --> (C0 = %i, C1 = %i)\n", goodplain0, goodplain1, goodcipher0, goodcipher1);       

            return;   

        }    

    printf("No matching pairs found\n");

}

 

 

//Now testing our encryption function.

 

 

int testing(int testkey0, int testkey1)

{

    int c;

    int dummy = 0;

    for(c = 0; c < totalpairs; c++)

    {

        if ((encryptionfunc(m0[c], testkey0, testkey1) != c0[c]) || (encryptionfunc(m1[c], testkey0, testkey1) != c1[c]))

        {

            dummy = 1;

            break;

        }       

    }

   

    if (dummy == 0)

        return 1;    

    else   

        return 0;  

}

 

 

//Now decrypting using our test and array.

void DECRYPT()

{

    printf("\nAttacking reduced keyspace:\n");

   

    for(int z = 0; z < initial; z++)                                           

    {

        int testkey0 = initialarray[z] ^ goodplain0;

        int testkey1 = X[initialarray[z]] ^ goodcipher0;

 

        if (testing(testkey0, testkey1) != 1)

            printf(" Finally key found! (%i, %i)\n", testkey0, testkey1);

           

        else

            printf("       (%i, %i)\n", testkey0, testkey1);

    } 

}

 

// Using all the functions

int main()

{

    srand(time(NULL));

    Differentialdistribution();                                                                

    totalpairs = 8;                                                              

    Generatingpairs(1, 13);

    Generatingpairs(4, 7);

    Generatingpairs(5, 8);

    Generatingpairs(15, 14);

    Randomkeys(4);                                                               

    goodpairs(7);                                                           

    DECRYPT();                                                                                                                                  

 

    while(1){}

    return 0;   

}
