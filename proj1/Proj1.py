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


def get_all_links(url): # DONE: 从网页获取多个莎士比亚文本
    response = requests.get(url)
    if response.status_code != 200:
        return "Error: Failed to retrieve data"

    soup = BeautifulSoup(response.content, 'html.parser')
    links = soup.find_all('a')  # 假设所有的子目录都是通过<a>标签链接的

    valid_doc_links = []
    valid_scene_links = []

    for link in links:
        href = link.get('href')
        if href and not href.startswith('http'):  # 排除外部链接
            if href and href.endswith('VenusAndAdonis.html'):
                valid_scene_links.append(url + href)
            elif href and href.endswith('LoversComplaint.html'):
                valid_scene_links.append(url + href)
            elif href and href.endswith('RapeOfLucrece.html'):
                valid_scene_links.append(url + href)
            elif href and href.endswith('elegy.html'):
                valid_scene_links.append(url + href)
            else:
                valid_doc_links.append(url + href)

    for link in valid_doc_links:
        sub_response = requests.get(link)
        if response.status_code != 200:
            return "Error: Failed to retrieve data"

        sub_soup = BeautifulSoup(sub_response.content, 'html.parser')
        sub_links = sub_soup.find_all('a')
        for sub_link in sub_links:
            href = sub_link.get('href')
            if href and href.endswith('.html') and link.endswith('index.html'):
                valid_scene_links.append(link.replace('index.html', href))
            elif href and href.endswith('.html') and link.endswith('sonnets.html'):
                valid_scene_links.append(link.replace('sonnets.html', href))

    print(valid_scene_links)
    return valid_scene_links

def get_shakespeare_text(url):
    response = requests.get(url)
    if response.status_code != 200:
        return "Error: Failed to retrieve data"

    # 使用 BeautifulSoup 解析 HTML
    soup = BeautifulSoup(response.content, 'html.parser')

    # 假设文本被 <blockquote> 标签包围（需要根据实际网页结构调整）
    text_blocks = soup.find_all('blockquote')
    if text_blocks:
        text = ''
        for block in text_blocks:
            text += block.get_text() + '\n'  # 可以根据需要添加分隔符
        return text.strip()  # 去除首尾的空白字符
    else:
        return "Error: Text blocks not found"


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
    base_url = 'http://shakespeare.mit.edu/'
    works_links = get_all_links(base_url)

    directory_name = 'shakespeare_works'
    if not os.path.exists(directory_name):
        os.makedirs(directory_name)

    for link in works_links:
        text = get_shakespeare_text(link)
        # print(text)  # 或者进行其他处理，比如保存到文件中
        # 保存到文件中
        file_name = link.split('/')[-1].replace(".html", '') + '.txt'
        file_path = os.path.join(directory_name, file_name)
        with open(file_path, 'w') as file:
            file.write(text)


if __name__ == '__main__':
    main()
