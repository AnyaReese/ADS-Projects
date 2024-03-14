# Roll Your Own Mini Search Engine
#
# In this project, you are supposed to create your own mini search engine
# which can handle inquiries over “The Complete Works of William Shakespeare”
# (http://shakespeare.mit.edu/).
#
# You may download the functions for handling stop words and stemming from the Internet,
# as long as you add the source in your reference list.
#
# Your tasks are:
#
# (1) Run a word count over the Shakespeare set and try to identify the stop words
# (also called the noisy words) – How and where do you draw the line between “interesting” and “noisy” words?
#
# (2) Create your inverted index over the Shakespeare set with word stemming.
# The stop words identified in part (1) must not be included.
#
# (3) Write a query program on top of your inverted file index, which will
# accept a user-specified word (or phrase) and return the IDs of the documents that contain that word.
#
# (4) Run tests to show how the thresholds on query may affect the results.

import re
import socket
import os
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from nltk.stem import PorterStemmer
from nltk.stem import LancasterStemmer
from nltk.stem import SnowballStemmer
import requests
from bs4 import BeautifulSoup
nltk.download('stopwords')
nltk.download('punkt')
# Download the stopwords

# TODO: 从网页获取多个莎士比亚文本
def get_shakespeare_text(url):
    response = requests.get(url)
    if response.status_code != 200:
        return "Error: Failed to retrieve data"

    # 使用 BeautifulSoup 解析 HTML
    soup = BeautifulSoup(response.content, 'html.parser')

    # 假设文本被 <blockquote> 标签包围（需要根据实际网页结构调整）
    text_block = soup.find('blockquote')
    if text_block:
        return text_block.get_text()
    else:
        return "Error: Text block not found"

# Read the file
def read_file(file_path):
    with open(file_path, 'r') as file:
        data = file.read()
    return data


def remove_punctuation(text):
    return re.sub(r'[^\w\s]', '', text)


def remove_stopwords(text):
    stop_words = set(stopwords.words('english'))
    word_tokens = word_tokenize(text)
    filtered_sentence = [w for w in word_tokens if not w in stop_words]
    return ' '.join(filtered_sentence)


def stem_text(text, stemmer):
    word_tokens = word_tokenize(text)
    stemmed_sentence = [stemmer.stem(w) for w in word_tokens]
    return ' '.join(stemmed_sentence)


def word_count(text):
    words = text.split()
    return len(words)


def create_inverted_index(text):
    inverted_index = {}
    words = text.split()
    for i, word in enumerate(words):
        if word in inverted_index:
            inverted_index[word].append(i)
        else:
            inverted_index[word] = [i]
    return inverted_index


def search_word(word, inverted_index):
    return inverted_index.get(word, [])


def main():
    # 示例：获取《哈姆雷特》的文本（需要根据实际URL调整）
    hamlet_text = get_shakespeare_text('http://shakespeare.mit.edu/')

    # 打印获取的文本开头部分
    print(hamlet_text[:1000])


if __name__ == '__main__':
    main()
