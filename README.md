# Detection Idiom

A useful technique to detect features of classes passed in as template arguments.
After reading the article by Sy Brand here: https://blog.tartanllama.xyz/detection-idiom/ I created this example.
When using C++20 or above I expect concepts to be a better choice than this somewhat unintuitive technique, this serves as a solution for C++17/14 code that needs to check the presence of a given method in a template class.

