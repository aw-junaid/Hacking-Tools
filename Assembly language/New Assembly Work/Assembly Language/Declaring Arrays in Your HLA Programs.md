In High-Level Assembly (HLA), declaring and using arrays is straightforward and similar to other high-level programming languages. HLA provides a syntax that simplifies array manipulation, making it easier for programmers to manage collections of data. Here’s a detailed guide on declaring and using arrays in HLA programs.

### 1. **Declaring Arrays in HLA**

Arrays in HLA are defined in the `data` section using specific directives. You can declare fixed-size arrays, which are allocated in memory at compile time.

#### **A. Basic Array Declaration**

To declare a simple array in HLA, you can use the `array` keyword followed by the type and size of the array. Here’s an example of declaring an array of integers:

```assembly
program ArrayExample;

#include("stdlib.hhf")

begin ArrayExample;

    // Declare an array of 5 integers
    intArray: int[5] := (1, 2, 3, 4, 5);

end ArrayExample;
```

In this example:
- `intArray` is an array of 5 integers, initialized with values from 1 to 5.

### 2. **Accessing and Modifying Array Elements**

You can access and modify array elements using the array index. HLA supports zero-based indexing, which means the first element of the array has an index of `0`.

#### **A. Accessing Elements**

To access an element in the array, use the array name followed by the index in square brackets:

```assembly
mov(intArray[0], eax); // Load the first element (1) into EAX
```

#### **B. Modifying Elements**

You can modify elements in the array in a similar way:

```assembly
intArray[2] := 10; // Change the third element (index 2) from 3 to 10
```

### 3. **Example: Working with Arrays**

Here’s a complete HLA program demonstrating how to declare an array, access its elements, and modify them:

```assembly
program ArrayExample;

#include("stdlib.hhf")

begin ArrayExample;

    // Declare an array of 5 integers
    intArray: int[5] := (1, 2, 3, 4, 5);
    
    // Output the original array
    stdout.put("Original Array: ", intArray, nl);

    // Modify the third element
    intArray[2] := 10; // Change the third element from 3 to 10

    // Output the modified array
    stdout.put("Modified Array: ", intArray, nl);

end ArrayExample;
```

### 4. **Looping Through Arrays**

You can use loops to iterate through the elements of an array, which is useful for processing or modifying multiple elements.

#### **Example of Looping Through an Array**

Here’s an example of using a loop to sum the elements of an array:

```assembly
program ArraySum;

#include("stdlib.hhf")

begin ArraySum;

    // Declare an array of 5 integers
    intArray: int[5] := (1, 2, 3, 4, 5);
    sum: int := 0; // Variable to store the sum
    i: int; // Loop counter

    // Loop through the array and calculate the sum
    for(i := 0; i < 5; i++) do
        sum := sum + intArray[i];
    end for;

    // Output the sum
    stdout.put("Sum of Array Elements: ", sum, nl);

end ArraySum;
```

### 5. **Multi-Dimensional Arrays**

HLA also supports multi-dimensional arrays. You can declare a two-dimensional array by specifying multiple dimensions:

```assembly
program MultiDimArray;

#include("stdlib.hhf")

begin MultiDimArray;

    // Declare a 2x3 array of integers
    intArray: int[2][3] := ((1, 2, 3), (4, 5, 6));

    // Access an element in the array
    mov(intArray[1][2], eax); // Load the element at row 1, column 2 (6)

    // Modify an element
    intArray[0][1] := 10; // Change the element at row 0, column 1 to 10

end MultiDimArray;
```

### 6. **Conclusion**

Declaring and using arrays in HLA is simple and intuitive, making it easy to manage collections of data. By leveraging HLA's array capabilities, you can efficiently perform operations on sets of values, facilitating cleaner and more organized code.
