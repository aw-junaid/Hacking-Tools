Accessing elements of a single-dimensional array in High-Level Assembly (HLA) is straightforward and allows you to manipulate data stored in contiguous memory locations. Here's a detailed explanation of how to declare, access, modify, and iterate through a single-dimensional array in HLA.

### 1. **Declaring a Single-Dimensional Array**

In HLA, you can declare a single-dimensional array using the `array` directive. Here's an example of declaring an array of integers:

```assembly
program AccessArrayElements;

#include("stdlib.hhf")

begin AccessArrayElements;

    // Declare a single-dimensional array of 5 integers
    intArray: int[5] := (1, 2, 3, 4, 5);

end AccessArrayElements;
```

### 2. **Accessing Elements of the Array**

To access elements of a single-dimensional array, you use the array name followed by the index in square brackets. HLA uses zero-based indexing, meaning the first element has an index of `0`.

#### **A. Reading Array Elements**

You can read an element from the array and store it in a register or variable:

```assembly
mov(intArray[0], eax); // Load the first element (1) into EAX
mov(intArray[1], ebx); // Load the second element (2) into EBX
```

#### **B. Modifying Array Elements**

You can also modify the value of an element directly:

```assembly
intArray[2] := 10; // Change the third element (3) to 10
```

### 3. **Example Program: Accessing Array Elements**

Here's a complete HLA program that demonstrates how to declare an array, access its elements, and modify them:

```assembly
program AccessArrayElements;

#include("stdlib.hhf")

begin AccessArrayElements;

    // Declare a single-dimensional array of 5 integers
    intArray: int[5] := (1, 2, 3, 4, 5);
    
    // Declare a variable to hold the sum
    sum: int := 0; 
    i: int; // Loop counter

    // Output original array elements
    stdout.put("Original Array: ", intArray, nl);

    // Access and modify elements
    mov(intArray[2], eax);    // Load the third element (3) into EAX
    stdout.put("Third Element: ", eax, nl); // Output the third element

    // Modify the third element
    intArray[2] := 10; // Change the third element to 10
    stdout.put("Modified Third Element: ", intArray[2], nl); // Output the modified element

    // Calculate the sum of all elements
    for(i := 0; i < 5; i++) do
        sum := sum + intArray[i]; // Accumulate the sum
    end for;

    // Output the sum of array elements
    stdout.put("Sum of Array Elements: ", sum, nl);

end AccessArrayElements;
```

### 4. **Looping Through a Single-Dimensional Array**

Looping through an array allows you to access or modify each element systematically. You can use a `for` loop to iterate through the array's indices:

```assembly
for(i := 0; i < 5; i++) do
    stdout.put("Element at index ", i, ": ", intArray[i], nl);
end for;
```

### 5. **Summary**

Accessing elements in a single-dimensional array in HLA involves:
- Declaring the array using a specific type and size.
- Accessing elements via indexing with square brackets.
- Reading or modifying the values directly.

By utilizing these techniques, you can effectively manage and manipulate collections of data within your HLA programs.
