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
import string
import requests
import socket
from bs4 import BeautifulSoup
import os
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from collections import Counter
from nltk.stem import PorterStemmer
from nltk.stem import LancasterStemmer
from nltk.stem import SnowballStemmer
nltk.download('stopwords')
nltk.download('punkt')
# Download the stopwords



def get_all_links(url): # DONE: 从网页获取多个包含文本的链接
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

def get_shakespeare_text(url): # DONE: 从网页获取莎士比亚文本
    response = requests.get(url)
    if response.status_code != 200:
        return "Error: Failed to retrieve data"

    # 使用 BeautifulSoup 解析 HTML
    soup = BeautifulSoup(response.content, 'html.parser')

    # 文本被 <blockquote> 标签包围
    text_blocks = soup.find_all('blockquote')
    if text_blocks:
        text = ''
        for block in text_blocks:
            text += block.get_text() + '\n'  # 可以根据需要添加分隔符
        return text.strip()  # 去除首尾的空白字符
    else:
        return "Error: Text blocks not found"


def word_count_and_stopwords_identification(text):
    words = word_tokenize(text)

    # 没用这个，自己根据出现频率判断停用词
    stop_words = set(stopwords.words('english'))
    punctuations = set(string.punctuation)
    # filtered_words = [word.lower() for word in words if word.lower() not in stop_words]

    filtered_words = [word.lower() for word in words]
    word_counts = Counter(filtered_words)
    noisy_words = set()
    noisy_dic = {}
    interesting_words = set()
    interesting_dic = {}

    for word, count in word_counts.items():
        if count >= 100:  # stop words 的阈值
            noisy_words.add(word)
            noisy_dic[word] = count
        else:
            interesting_words.add(word)
            interesting_dic[word] = count

    return noisy_dic, interesting_dic


def stage_texts():
    # 示例：获取《哈姆雷特》的文本（需要根据实际URL调整）
    base_url = 'http://shakespeare.mit.edu/'
    works_links = get_all_links(base_url)

    directory_name = 'shakespeare_works'
    if not os.path.exists(directory_name):
        os.makedirs(directory_name)
    for link in works_links:
        text = get_shakespeare_text(link)
        text_without_punc = re.sub(r'[^\w\s]', ' ', text)
        file_name = link.split('/')[-1].replace('.html', '') + '.txt'
        file_path = os.path.join(directory_name, file_name)
        with open(file_path, 'w') as file:
            file.write(text_without_punc)

    all_texts = ''
    directory = 'shakespeare_works'
    for filename in os.listdir(directory):
        # 检查文件是否以txt为扩展名
        if filename.endswith('.txt'):
            # 构建文件的完整路径
            filepath = os.path.join(directory, filename)
            # 读取文件内容并将其添加到all_texts中
            with open(filepath, 'r') as file:
                all_texts += file.read()

    file_path = os.path.join(directory, '0_all_texts')
    with open(file_path, 'w') as file:
        file.write(all_texts)

def main():
    # stage_texts()

    with open(os.path.join('shakespeare_works', '0_all_texts'), 'r') as file:
        all_texts = file.read()

    # 获取停用词和词频，用字典存储，用于后续的词频统计
    noisy_dic, interesting_dic = word_count_and_stopwords_identification(all_texts)
    print(noisy_dic)


if __name__ == '__main__':
    main()