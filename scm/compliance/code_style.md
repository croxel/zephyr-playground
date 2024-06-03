Due to the fact this project is being built under Zephyr OS, the following Standards and Guidelines are needed to be followed:

**Coding Guidelines**
=====================

The project must follow the [Zephyr Coding Guidelines](https://docs.zephyrproject.org/latest/contribute/coding_guidelines/#coding-guidelines)

Custom Coding Guidelines
------------------------

In addition to the Zephyr Project coding guidelines we define here a set of rules that extends or modifies some rules.

### One Return Per Function

The practice of having one return statement per function in C/C++ is considered a good programming practice for several reasons:

**Code Readability:** Having a single exit point makes the code more readable and easier to understand. Developers can quickly identify where a function returns and what value it returns without having to scan through the entire function body.

**Maintenance and Debugging:** Debugging becomes simpler when there is only one return point. It is easier to set breakpoints, trace the flow of execution, and understand the state of variables at the point of return. This can significantly reduce the time required for maintenance and bug fixing.

**Predictable Control Flow:** A single return point ensures a more predictable control flow. It helps in avoiding unexpected jumps out of the function from different locations, making it easier to reason about the code, it also helps the compiler optimizer and the branch predictor.

**Resource Cleanup:** If a function allocates resources (memory, file handles, etc.), having a single return point allows for centralized resource cleanup. This is crucial for preventing memory leaks and ensuring that all resources are properly released before exiting the function.

**Consistent Error Handling:** One return statement promotes consistent error handling. Error conditions can be checked at various points in the function, and if an error is encountered, the function can handle it uniformly before reaching the single return statement.

**Enhanced Code Review:** During code reviews, having one return statement per function simplifies the process. Reviewers can focus on the logic leading to the single return point and verify that the function behaves correctly under different scenarios.

**Compatibility with Some Coding Standards:** Some coding standards or guidelines, such as MISRA-C, recommend or enforce the use of a single return statement for better code maintainability and reliability.

While the practice of having a single return statement is generally recommended, it's important to note that there are cases where multiple returns might be appropriate. Ultimately, the key is to strike a balance between code readability and the specific requirements of the code being written.

### Positive Logic

This is the practice of avoiding negative or error handling logic inside the body of an "if" statement and instead placing the main or happy path logic there. This approach is often referred to as "positive logic" in the sense that the main or successful path is expressed without the use of negations or error handling inside the "if" block. There are several reasons why this can be considered a good practice:

1.  **Readability and Clarity:** Placing the main logic in the body of the "if" statement makes the code more readable. Developers can easily identify the primary path of execution without having to mentally navigate through nested or negated conditions.
    
2.  **Reduced Nesting Levels:** Positive logic often leads to reduced nesting levels, which improves code readability. Deeply nested code can be harder to follow, and by placing the main logic in the "if" block, you avoid unnecessary indentation levels.
    
3.  **Ease of Code Review:** Code reviews become simpler when the main logic is expressed in the positive form. Reviewers can focus on the primary flow of the program without being distracted by error handling or negations.
    
4.  **Maintainability:** Positive logic can contribute to better code maintainability. When modifications or enhancements are needed, developers can quickly understand the primary flow and make changes without disrupting the error handling logic.
    
5.  **Avoidance of Double Negatives:** Using positive logic helps avoid double negatives, which can be confusing. Code with fewer negations is generally easier to understand and less error-prone.
    
6.  **Easier Testing:** Positive logic often results in clearer test cases. Test scenarios can be designed to cover the main path separately from error or exceptional cases, making it easier to verify that the primary functionality works as intended.
    

Here's an example to illustrate the concept:

```java
// Positive Logic
int divide(int a, int b) {
    if (b != 0) {
        return a / b;  // Happy path logic
    } else {
        // Handle division by zero error
        return 0;
    }
}

// Negative Logic
int divide(int a, int b) {
    if (b == 0) {
        // Handle division by zero error
        return 0;
    } else {
        return a / b;  // Happy path logic
    }
}
```

In the positive logic example, the main logic (happy path) is in the "if" block, while in the negative logic example, the error handling is in the "if" block. The positive logic version is often preferred for its clarity and readability.

**Exceptions**

While this is generally a good practice, there are cases where employing Positive Logic may result in larger function code (in terms of the assembly code generated by the compiler), which is not advisable for embedded systems applications. For example when the condition for the happy path is too big:

```java
int function(At a, Bt b, Ct c, Dt d, Et e) {
    int status = 0;

    if (a && b && c && d && e)
		{
        // Happy path here!
    }
		else if (!a)
		{
		    status = ERROR_A;
    }
		else if (!b)
		{
		    status = ERROR_B;
    }
		else if (!c)
		{
		    status = ERROR_C;
    }
		else if (!d)
		{
		    status = ERROR_D;
    }
		else if (!e)
		{
		    status = ERROR_E;
    }

		return status;
}
```

In this case it is better to first evaluate the errors conditions and let the happy path in the `else` statement.

```java
int function(At a, Bt b, Ct c, Dt d, Et e) {
    int status = 0;

		if (!a)
		{
		    status = ERROR_A;
    }
		else if (!b)
		{
		    status = ERROR_B;
    }
		else if (!c)
		{
		    status = ERROR_C;
    }
		else if (!d)
		{
		    status = ERROR_D;
    }
		else if (!e)
		{
		    status = ERROR_E;
    }
    else
    {
				// Happy path here.
		}

		return status;
}
```

**Coding Style**
================

Use these coding guidelines to ensure that your development complies with the project’s style and naming conventions.

In general, follow the [Linux kernel coding style](https://kernel.org/doc/html/latest/process/coding-style.html), with the following exceptions:

*   The line length is 100 columns or fewer. In the documentation, longer lines for URL references are an allowed exception.
    
*   Add braces to every `if`, `else`, `do`, `while`, `for` and `switch` body, even for single-line code blocks. Use the `-ignore BRACES` flag to make _checkpatch_ stop complaining.
    
*   Exception against Linux kernel coding style: Use spaces instead of tabs to align comments after declarations, as needed (8 characters long)
    
*   Use C89-style single line comments, `/* */`. The C99-style single line comment, `//`, is not allowed.
    
*   Use `/** */` for doxygen comments that need to appear in the documentation.
    

When there are differences between the guidelines above and the formatting generated by code formatting tools, the guidelines above take precedencines above and the formatting generated by code formatting tools, the guidelines above take precedene.