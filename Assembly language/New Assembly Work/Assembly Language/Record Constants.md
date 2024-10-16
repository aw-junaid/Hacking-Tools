In High-Level Assembly (HLA), a record is a composite data structure that allows you to group related data of different types under a single identifier. This is particularly useful for organizing data in a way that reflects its logical structure, making your programs more readable and maintainable.

### 1. **Defining a Record**

To define a record in HLA, you typically use the `record` keyword followed by the fields of the record. Each field can be of different data types. 

#### **Example of Defining a Record**

Here’s how to define a record that represents a `Person` with a name, age, and height:

```assembly
record Person
    name: string[50];     // String field for the name
    age: int;             // Integer field for age
    height: real;         // Real number field for height
end record;
```

### 2. **Declaring Record Constants**

You can declare a constant record using the `constant` keyword. Here’s how to create a constant record for a specific person:

```assembly
constant John: Person := ( "John Doe", 30, 5.9 );
```

### 3. **Using Records in HLA**

After defining a record and its constants, you can access and manipulate the fields of the record. Here's an example of how to declare a record constant, access its fields, and output the values.

#### **Complete Example Program**

```assembly
program RecordExample;

#include("stdlib.hhf")

// Define a record type for Person
record Person
    name: string[50];
    age: int;
    height: real;
end record;

// Declare a constant record
constant John: Person := ( "John Doe", 30, 5.9 );

begin RecordExample;

    // Output the fields of the record
    stdout.put("Name: ", John.name, nl);
    stdout.put("Age: ", John.age, nl);
    stdout.put("Height: ", John.height, " ft", nl);

end RecordExample;
```

### 4. **Explanation of the Example**

1. **Record Definition**:
   - The `Person` record is defined with three fields: `name`, `age`, and `height`.

2. **Constant Declaration**:
   - A constant `John` of type `Person` is declared and initialized with specific values.

3. **Accessing Record Fields**:
   - The fields of the `John` record are accessed using the dot notation (e.g., `John.name`, `John.age`, and `John.height`).
   - The values are printed to the standard output using `stdout.put`.

### 5. **Benefits of Using Records**

- **Logical Grouping**: Records allow you to group related data together, which helps in organizing complex data structures.
- **Type Safety**: Each field in a record can be of a different data type, providing type safety and clarity in your data structures.
- **Ease of Use**: Records make it easier to pass complex data to functions or procedures since you can pass the entire record rather than individual fields.

### 6. **Conclusion**

Records in HLA are powerful constructs for organizing and managing related data. By defining constants and using records, you can write more structured and maintainable code.
