# Roll Your Own Mini Search Engine

In this project, you are supposed to create your own mini search engine which can handle inquiries over “The Complete Works of William Shakespeare” (http://shakespeare.mit.edu/).<br>在这个项目中，你应该创建自己的迷你搜索引擎，可以处理对“威廉·莎士比亚全集”（http://shakespeare.mit.edu/）的查询。

You may download the functions for handling stop words and stemming from the Internet, as long as you add the source in your reference list.<br>您可以下载用于处理停用词和来自 Internet 的词源的函数，只要您在参考列表中添加源即可。

## Your tasks are:

**(1)** Run a word count over the Shakespeare set and try to identify the stop words (also called the noisy words) – How and where do you draw the line between “interesting” and “noisy” words?<br>对莎士比亚系列进行字数统计，并尝试识别停用词（也称为嘈杂词）——您**如何**以及在**哪里**划定“有趣”和“嘈杂”词之间的界限？

**(2)** Create your inverted index over the Shakespeare set with word stemming. The stop words identified in part (1) must not be included.<br>在莎士比亚集上创建带有词干的倒索引。不得包括第 （1） 部分中标识的停用词。

**(3)** Write a query program on top of your inverted file index, which will accept a user-specified word (or phrase) and return the IDs of the documents that contain that word.<br>在倒排文件索引之上编写一个查询程序，该程序将接受用户指定的单词（或短语）并返回包含该单词的文档的 ID。

**(4)** Run tests to show how the thresholds on query may affect the results.<br>运行测试以显示查询上的阈值如何影响结果。

## Grading Policy:
Programming: Write the programs for word counting (1 pt.), index generation (5 pts.) and query processing (3 pts.) with sufficient comments.<br>编写字数统计（1 分）、索引生成（5 分）和查询处理（3 分）的程序，并附上足够的注释。

Testing: Design tests for the correctness of the inverted index (2 pts.) and thresholding for query (2 pts.). Write analysis and comments (3 pts.). Bonus: What if you have 500 000 files and 400 000 000 distinct words? Will your program still work? (+2 pts.)<br>设计测试倒排索引的正确性（2 分）和查询阈值（2 分）。撰写分析和评论（3 分）。奖励：如果您有 500 000 个文件和 400 000 000 个不同的单词怎么办？您的程序还能正常工作吗？（+2 分）

Documentation: Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).

Note: Anyone who does excellent job on answering the Bonus question will gain extra points.