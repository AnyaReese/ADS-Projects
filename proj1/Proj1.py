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
import itertools
import numpy as np
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
from collections import defaultdict

STOP_POINT = 2000  # stop words 的阈值

# When first use, please uncomment next two line!!
# nltk.download('stopwords')
# nltk.download('punkt')
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


def stage_texts():
    # 获取文本（需要根据实际URL调整）
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
        if file_name == "full":
            continue
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
# --------------------------------------------------------------------------------------- ↑ 爬取 shakespeare 的内容


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
        if count >= STOP_POINT:  # stop words 的阈值
            noisy_words.add(word)
            noisy_dic[word] = count
        else:
            interesting_words.add(word)
            interesting_dic[word] = count

    return noisy_dic, interesting_dic

# stop_words = set(stopwords.words('english'))


def build_inverted_index(folder_path, noisy_dic):
    stemmer = PorterStemmer()
    inverted_index = {}
    for filename in os.listdir(folder_path):
        if filename.endswith('.txt'):
            file_path = os.path.join(folder_path, filename)
            with open(file_path, 'r', encoding='utf-8') as file:
                content = file.read().lower()
                words = nltk.word_tokenize(content)
                for position, word in enumerate(words):
                    stemmed_word = stemmer.stem(word)
                    if stemmed_word not in noisy_dic:
                        if stemmed_word not in inverted_index:
                            inverted_index[stemmed_word] = [(filename, position)]
                        else:
                            inverted_index[stemmed_word].append((filename, position))
    sorted_index = dict(sorted(inverted_index.items(), key=lambda x: len(x[1])))
    return sorted_index


def save_inverted_index(inverted_index, output_path):
    with open(output_path, 'w', encoding='utf-8') as file:
        for word, file_positions in inverted_index.items():
            file.write(f"{word}: ")
            file_entries = [f"{filename}@{position}" for filename, position in file_positions]
            file.write(f"{', '.join(sorted(file_entries))}\n")



# --------------------------------------------------------------------------------------- ↑ 构建倒排索引


def calculate_the_min_var(data):
    # 生成所有可能的数字组合
    all_combinations = itertools.product(*(data[key] for key in sorted(data.keys())))

    min_variance = float('inf')
    min_combination = None

    # 遍历所有组合以找到方差最小的组合
    for combination in all_combinations:
        variance = np.var(combination)
        if variance < min_variance:
            min_variance = variance
            # min_combination = combination

    return min_variance


def find_word_intersection_and_positions(inverted_index, words):
    # 使用默认字典来存储每个文件及其对应的单词位置
    stemmer = PorterStemmer()
    file_word_positions = defaultdict(lambda: defaultdict(list))
    min_comb = [None,None,None,None,None]
    min_var = [114514, 114514, 114514, 114514, 114514]
    appear_list = []
    # 对于查询中的每个单词
    for word in words:
        stemmed_word = stemmer.stem(word.lower())
        # 在倒排索引中查找单词
        if stemmed_word in inverted_index:
            if stemmed_word not in appear_list:
                appear_list.append(stemmed_word)
                # 遍历该单词出现的所有文件及位置
                for filename, position in inverted_index[stemmed_word]:
                    # 记录文件中单词的位置
                    file_word_positions[filename][stemmed_word].append(position)
    # print(file_word_positions.items())
    # 筛选出同时包含所有单词的文件
    intersection_files = {
        filename: positions for filename, positions in file_word_positions.items()
        if len(positions) == len(appear_list)
    }
    for filename in intersection_files:
        var = calculate_the_min_var(intersection_files[filename])
        for i in range(5):
            if min_var[i] > var:
                min_var[i] = var
                min_comb[i] = filename
                break

    return min_comb

# --------------------------------------------------------------------------------------- ↑ 搜索


def main():
    # stage_texts()

    with open(os.path.join('shakespeare_works', '0_all_texts'), 'r') as file:
        all_texts = file.read()

    print("accessing noisy dic...")
    # 获取停用词和词频，用字典存储，用于后续的词频统计
    noisy_dic, interesting_dic = word_count_and_stopwords_identification(all_texts)
    with open('./noisy_dic.txt', 'w', encoding='utf-8') as file:
        for word, count in noisy_dic.items():
            file.write(f"{word}: {count}\n")

    print("building inverted file index...")
    folder_path = './shakespeare_works'  # 资料文件夹路径
    output_path = './output.txt'  # 输出文件路径
    inverted_index = build_inverted_index(folder_path, noisy_dic)
    save_inverted_index(inverted_index, output_path)
    while 1:
        sentence = input("请输入需要查询的内容，如果要退出查询请输入-1:")
        input_words = sentence.lower().split()
        if input_words == ["-1"]:
            break
        name = find_word_intersection_and_positions(inverted_index, input_words)
        if name[0] is None:
            print("nothing found,because your words is noisy_words!")
        else:
            for i in range(5):
                if name[i] is None:
                    break
                book_name1 = name[i].split(".")
                if book_name1[1] == "txt":
                    print("\""+sentence+"\" probably comes from ", book_name1[0])
                elif book_name1[2] == "txt":
                    print("\""+sentence+"\" probably comes from Chapter", book_name1[1], "of", book_name1[0])
                else:
                    flag = 0
                    part = book_name1[1][0]
                    if 'henryiv' in book_name1[0]:
                        book_name1[0] = 'Henry IV'
                        flag = 1
                    if flag:
                        print("\""+sentence+"\" probably comes from Part "+part+", Act "+book_name1[1]+", Scene", book_name1[2], "of", book_name1[0])
                    print("\""+sentence+"\" probably comes from Act "+book_name1[1]+", Scene", book_name1[2], "of", book_name1[0])
                if i == 0:
                    print("If there's more possible results, they are as follows(we list 4 more at most):")


if __name__ == '__main__':
    main()

# it is the east and Juliet is the sun