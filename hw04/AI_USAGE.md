# AI Usage

## Tools Used

I used ChatGPT as an AI assistant while working on this assignment.

## How I Used AI

I used ChatGPT to help me understand the Tally ADT requirements, reason through the implementation, write and improve tests, understand the opaque type concept, and document the project.

I also used it to troubleshoot compilation and test results. I ran the commands myself and checked the output to verify the behavior of my code.

## One Thing the Model Got Wrong

At one point, ChatGPT suggested test code without correctly accounting for the number of distinct names already stored in the tally. This could have made the test check the wrong capacity condition. We reviewed the test logic and corrected it to verify that a tally accepts 16 distinct names, rejects a new name when full, and still increments an existing name.
