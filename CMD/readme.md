# Hash Table in C

## What is a hash table used for?
A hash table uses a hash function to use any object as a key and uses it to access another object in ideally, O(1) time. This would mean no matter how large the table, one can access objects in it in constant time.

This implementation of a hash table can use arbitrarily large keys and map them to arbritrarily large values. The code also allows one to switch out one hash function for another using function pointers.

Eg. 
```C
char key[20] = "Hello my name jeff"; // an arbritarily long key
int value = 500; // the value to be accessed

HashTable *table = hash_table_new(); // make a hash table
hash_table_set_value(table, (char *)&value, key, 20); // store the value for the specified key

int *rvalue = (int *) hash_table_get_value(table, key, 20); // get the value back (ideally constant time)
print ("accessed value: %d", *rvalue); // will print: 500
```


## What is a hash function?
A hash function is a one-to-one function that maps an arbitrarily large set of objects to a finite positive set. Unfortunately, this is an ideal case and in reality, a hash function is a many-to-one function because our domain is almost infinite but our range of the possible values upon hashing is finite.

Y = h(X); where h is our hash function, X is our input value and Y is our hashed value. X in R, Y in [0, m)

