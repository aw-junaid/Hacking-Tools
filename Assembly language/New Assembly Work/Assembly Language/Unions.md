In High-Level Assembly (HLA), a **union** is a data structure that allows you to store different data types in the same memory location. Unlike a record, which allocates memory for each of its fields, a union uses a single memory area to hold one of its fields at any given time. This is particularly useful when you want to manage memory efficiently or when the exact type of data that will be used varies.

### 1. **Defining a Union**

To define a union in HLA, you use the `union` keyword, followed by the different fields it can hold. Each field can be of a different data type, but the total size of the union will be the size of the largest field.

#### **Example of Defining a Union**

Here’s how to define a union that can represent either an `int`, a `real`, or a `string`:

```assembly
union Data
    intValue: int;          // Field for integer value
    realValue: real;        // Field for real number value
    stringValue: string[50]; // Field for string value
end union;
```

### 2. **Declaring Union Variables**

You can declare a union variable just like you would with a record. Here’s how to declare a union variable and use it:

```assembly
variable myData: Data; // Declare a union variable
```

### 3. **Using Unions in HLA**

You can access and modify the fields of a union. However, you must ensure that you only access the field that was last written to, as the union can only hold one value at a time.

#### **Complete Example Program**

Here’s an example that demonstrates how to declare a union, assign values, and access its fields:

```assembly
program UnionExample;

#include("stdlib.hhf")

// Define a union type for Data
union Data
    intValue: int;
    realValue: real;
    stringValue: string[50];
end union;

begin UnionExample;

    // Declare a union variable
    variable myData: Data;

    // Assign an integer value
    myData.intValue := 42;
    stdout.put("Integer Value: ", myData.intValue, nl);

    // Assign a real number value
    myData.realValue := 3.14;
    stdout.put("Real Value: ", myData.realValue, nl);

    // Assign a string value
    myData.stringValue := "Hello, World!";
    stdout.put("String Value: ", myData.stringValue, nl);

    // Since unions share the same memory location,
    // accessing previously assigned fields may lead to unexpected results.
    stdout.put("Accessing Integer Value after assigning String: ", myData.intValue, nl); // This may produce unexpected results

end UnionExample;
```

### 4. **Explanation of the Example**

1. **Union Definition**:
   - The `Data` union is defined with three fields: `intValue`, `realValue`, and `stringValue`.

2. **Union Variable Declaration**:
   - A union variable `myData` is declared.

3. **Assigning Values**:
   - The program assigns values to each of the fields in the union, printing them out to show the current value of the active field.

4. **Accessing Previously Assigned Values**:
   - The program attempts to access the integer value after assigning a string. Since unions use the same memory location, this can lead to unexpected results. Accessing fields that were not the last written field can produce garbage data or incorrect values.

### 5. **Benefits of Using Unions**

- **Memory Efficiency**: Unions allow you to use memory more efficiently, as they allocate enough space for only the largest field.
- **Flexibility**: They provide flexibility to work with different data types using a single variable.

### 6. **When to Use Unions**

- Use unions when you need to manage different data types but don't need to use them simultaneously.
- They are beneficial in applications like protocol headers, where different fields are used based on the context.

### 7. **Conclusion**

Unions in HLA are powerful tools for managing memory efficiently while allowing the storage of different types of data. However, care must be taken to ensure that you access only the last-written field to avoid unexpected behavior.
