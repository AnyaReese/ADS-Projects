# Texture Packing

## Problem Statement:

Texture Packing is to pack multiple rectangle shaped textures into one large texture. The resulting texture must have a given width and a minimum height.

This project requires you to design an approximation algorithm that runs in polynomial time. You must generate test cases of different sizes (from 10 to 10,000) with different distributions of widths and heights. A thorough analysis on all the factors that might affect the approximation ratio of your proposed algorithm is expected.

## Grading Policy:

**Programming**: Implement the approximation algorithm (6 pts.). Write a test of performance program (1 pts.). All the codes must be sufficiently commented.
**Testing**: Generate the test cases and give the run time table (2 pts.). Plot the run times vs. input sizes for illustration (2 pts.). Write analysis and comments (5 pts.). The analysis must be based on your testing results.
**Documentation**: Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).
**Bonus**: Compare your algorithm to any of the known strip algorithms.
**Note**: Anyone who does excellent job on answering the Bonus question will gain an extra point.

## Prerequisites

- GNU Make
- GCC compiler

## Compilation and Run

To compile the program, follow these steps:

1. Open a terminal and navigate to the directory containing the Makefile.

2. Run the following command to compile and run the program:

   ```
   make run
   ```

## Input Format

When prompted, provide the input in the following format:

1. **Number of Rectangles and Bin Width:**
   
   - First, input the number of rectangles `n` and the width of the large bin `given_width`.
   - Example:
     ```
     Input the number of rectangles and the width of the large bin:
     3 10
     ```
   
2. **Width and Height of Each Rectangle:**
   - Next, input the width and height for each of the `n` rectangles.
   - Enter each rectangle's width and height on a new line.
   - Example:
     ```
     Input the width and height of each rectangle:
     2 5
     3 4
     1 6
     ```

Here is an example of a complete input session:
```
Input the number of rectangles and the width of the large bin:
3 10
Input the width and height of each rectangle:
2 5
3 4
1 6
```

The program will read this input and process the rectangles accordingly.

## Cleaning Up

To remove the compiled executable and any object files generated during the build process, run:

```
make clean
```

This command will delete the `program` executable and all `.o` files.
