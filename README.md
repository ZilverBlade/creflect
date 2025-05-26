# Sorting unalgorithms

Normally, in computer science we try to find the fastest algorithms, whether it be sorting, computing, and the such.

But what if we didn't try to be efficient, but the exact opposite? What is the worst possible way to sort an array?

**Edit:** You can now try out the prebuilt x64 windows binaries [here](https://github.com/ZilverBlade/Sorting-Unalgorithms/releases) 

### Stooge sort
A very slow sorting algorithm that works based on sorting 2/3 of the left elements, and then the right elements, and left again, recursively.
Visualised it's very slow, but unfortunately it only has a time complexity of O(n^(log 3 / log 1.5)) (approximately O(n^2.709))

> Can we go slower?

### Slow sort
This is takes inspiration from the blazingly fast merge sort, but instead of merging the sorted arrays, it will only swap the largest elements within the array, 
making sure that the largest element is on the end of the array, followed by recursively slow sorting the rest of the array (except for the last element). 
Now this sounds very slow, which it is, but again, unfortunately has the same time complexity as Stooge Sort! :(

> Let's be creative

### Lookup Sort
What if we created a dictionary, that contains every possible array in the universe as the key, and placed the corresponding sorted array as the value? Surely it would be an O(1) time complexity?
```
| [1, 2, 0] -> [0, 1, 2] |
| [0, 2, 1] -> [0, 1, 2] |
| [2, 0, 1] -> [0, 1, 2] |
| [2, 1, 0} -> [0, 1, 2] |
```
It would be extremely impractical however, as it has a space complexity of O(∞). 
The actual space complexity, assuming we strictly use 32 bit integer for our array, would be (where n is the maximum array length):
* `O( (2^32)^(n) ))`

If we were to even implement this sorting algorithm for an array of length 1, it would require at least 51GB of memory! (Assuming a key size of 8 bytes and the array size being exactly 4 bytes)

**[Try it out!](https://github.com/ZilverBlade/Sorting-Unalgorithms/tree/master/src/lookup_sort)**

> Can we maybe try going for the worst time complexity?

### Permute Sort
An array of objects can be sorted in different fashions
For instance, an array `[0, 1, 2]` can also be listed as `[1, 0, 2]`, `[2, 1, 0]`, `[2, 0, 1]`, `[1, 2, 0]`, `[0, 2, 1]`. In total, there are `N!` possible ways to organise an array, where N is the array length.
What's interesting is that at least one combination of elements is guaranteed to be sorted! So, what's our algorithm?
Just try every single combination until it's sorted!
The time complexity is thus `O(n!)`

**[Try it out!](https://github.com/ZilverBlade/Sorting-Unalgorithms/tree/master/src/permute_sort)**

> A twist on bogo sort

### Parallel Bogo Sort
Bogo sort is extremely inefficient, it has an average time complexity of O(n*n!)! But we can make it faster, by making use of multiple threads! The great thing is, it's GPU compatible!
It works as follows: 
* On one thread, we first bogosort the entire array.
* If it's not sorted, then we create 2 new threads, and copy the bogo sorted array onto them.
* We continue this recursively until one thread has a sorted array, and we notify all the other threads that the array is sorted!

But how do we compute the time complexity?
Let a function `T(n)` that takes in an array of length `n`
Our function's complexity looks as follows:
* `T(n) = T(n) / 2`.

We divide by 2 to represent that our work load is divided in half, and we add `n` to account for us checking if the array is sorted.
We know that bogo sort's average time complexity is `O(n*n!)`, so we just need to account for that complexity in each recursive call, so let's add it in
Since there is no base case time complexity, this may continue infinitely. But what is the closed form? Let's try to unravel it:
* `T(n) = n * n! + T(n) / 2`

Conveniently, as we dont have any factors in our recursive term, we can just subctract `T(n) / 2` on both sides
* `T(n)  - T(n) / 2 = n * n! + T(n) / 2 - T(n) / 2  <=> T(n) / 2 = n * n!  <=>  T(n) = 2 * n * n! `

Huh? Our time complexity is larger? The time complexity accounts for the amount of comparisons, it doesn't really account for parallelism, and since bogo sort actually accounts for the permutations,
we need to think about probability.

The chance of an array to be sorted is `1/n!`. Theoretically, this chance duplicates if we evaluate it on a new thread.
So let `P(n)` be the probability of the array being sorted
* `P(n) = 1 / n!`

This would be the probability of the array being sorted after 1 run. Lets add another input to our equation `r`, the number of runs
* `P(n, r) = r / n!`

We double our runs every single permutation, so the probability of it being sorted duplicates!
* `P(n, r) = r / n! + P(n, 2r)`

The closed form of the equation would be as follows
* `P(n, r) = r / n! + P(n, 2r)`
* `P(n, r) = r / n! + 2r / n! + P(n, 4r)`
* `P(n, r) = 3r / n! + 4r / n! + P(n, 8r)`
* `P(n, r) = 7r / n! + 8r / n! + P(n, 16r)`
* `P(n, r) = 15r / n! + 16r / n! + P(n, 32r)`

We see a pattern, let `lim x -> inf` be the total amount of runs until the list is sorted, and lets start our function with 1 round only.
* `P(n, 1) = (2^x - 1) / n!`

We have an exponential function is our equation, so our time complexity in theory would be `O(log(n * n!))`! Success!
Of course, while in theory this is much more efficient, in practicality we are limited by the number of threads on the CPU. As soon as we create more threads than are available, the operating system is responsible for scheduling multiple virtual threads, that share the same physical thread (which leads to thrashing thanks to many context switches). In fact, this may even reach a physical limit after even a handful of rounds as each new thread requires a new stack frame, which makes this by far one of the most resource hungry algorithm (alongside lookup sort)

**[Try it out!](https://github.com/ZilverBlade/Sorting-Unalgorithms/tree/master/src/parallel_bogo_sort)**

> Maybe the better approach is to do less work, rather than more

### Muscle Sort
Like you know, us humans are able to naturally build muscle, and gain strength, the same logic applies to numbers.
If the next element in the array is smaller the the previous one, we send it to the gym until it's greater than the previous one.
Here is how muscle sort would sort this array:
`[2, 1, 10, 3, 249, 5]`
* 2 is the first element in the array, it does not need to work out,
* 1 on the other hand, is lazy, as it's weaker than the previous element, it must be sent to the gym
* Our array looks as follows: `[2, 3, 10, 3, 249, 5]`
* Finally, after applying the same process to each element, we obtain the array `[2, 3, 10, 11, 249, 250]` which is indeed sorted

The greatest achievement of this sorting algorithm is that it has a time complexity of O(n), which makes it faster than quick sort and merge sort!

**[Try it out!](https://github.com/ZilverBlade/Sorting-Unalgorithms/tree/master/src/muscle_sort)**

> Can we do EVEN less work?

### Vacuous Sort
This is the wikipedia definition for a *Sorted Array*
> A sorted array is an array data structure in which each element is sorted in numerical, alphabetical, or some other order [...\]

In other words, if every element in the array is larger than the previous one (or the converse for descending order), it is sorted.

Logically, for an array of size 0, all the elements in the array are sorted by default (It is vacuously true as our domain is empty).

Clearly, we can implement a sorting algorithm which returns an empty array. You asked for a sorted array, should've been more specific... ;)

Here is how Vacuous Sort would sort this array: `[2, 8, 9, 0, 3, 5]`
* `vacuous_sort([2, 8, 9, 0, 3, 5]) -> []`

We can also try this on an already sorted array.
* `vacuous_sort([1, 2, 3, 4]) -> []`

Even on an array of length 1!
* `vacuous_sort([1]) -> []`

We just created the fasting sorting algorithm, as it's one constant time operation (returning an empty array). Our algorithm is in O(1)!

**[Try it out!](https://github.com/ZilverBlade/Sorting-Unalgorithms/tree/master/src/vacuous_sort)**
