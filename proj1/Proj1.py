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
from colorama import init, Fore, Back, Style

STOP_POINT = 2000       # stop words 的阈值
init(autoreset=True)    # colorama init

# When first use, please uncomment next two line!!
# nltk.download('stopwords')
# nltk.download('punkt')
# Download the stopwords

'''
    @author AnyaReese
    @brief every URL that contains texts
    @param url: the base URL
    @return: a list that includes every URL that contains texts
'''
def get_all_links(url):  # DONE: get all links that contains texts
    response = requests.get(url)
    if response.status_code != 200:
        return "Error: Failed to retrieve data"

    soup = BeautifulSoup(response.content, 'html.parser')
    links = soup.find_all('a')  # every sub link in the page

    valid_doc_links = []
    valid_scene_links = []

    for link in links:
        href = link.get('href')
        if href and not href.startswith('http'):  # except for external links
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


def get_shakespeare_text(url):  # DONE: get the text from the URL
    response = requests.get(url)
    if response.status_code != 200:
        return "Error: Failed to retrieve data"

    # use BeautifulSoup to parse the HTML
    soup = BeautifulSoup(response.content, 'html.parser')

    # text <blockquote> blocks
    text_blocks = soup.find_all('blockquote')
    if text_blocks:
        text = ''
        for block in text_blocks:
            text += block.get_text() + '\n'  # add a new line after each block
        return text.strip()  # get rid of leading/trailing whitespaces
    else:
        return "Error: Text blocks not found"


def stage_texts():
    print("[Info] getting urls...")
    base_url = 'http://shakespeare.mit.edu/'
    works_links = get_all_links(base_url)

    directory_name = 'shakespeare_works'

    if not os.path.exists(directory_name):
        os.makedirs(directory_name)
    count = 0
    print("[Info] saving texts to local place... Please wait a while...")
    for link in works_links:
        if count % 50 == 0:
            print("[Info] Saving text process " + str(int((count / 913) * 100)) + "%")
        count += 1
        text = get_shakespeare_text(link)
        text_without_punc = re.sub(r'[^\w\s]', ' ', text)
        file_name = link.split('/')[-1].replace('.html', '') + '.txt'
        file_path = os.path.join(directory_name, file_name)
        with open(file_path, 'w') as file:
            file.write(text_without_punc)

    all_texts = ''
    directory = 'shakespeare_works'
    for filename in os.listdir(directory):
        # check if the file is a text file
        if filename.endswith('.txt'):
            # build the full path to the file
            filepath = os.path.join(directory, filename)
            # read the file
            with open(filepath, 'r') as file:
                all_texts += file.read()

    file_path = os.path.join(directory, '0_all_texts')
    with open(file_path, 'w') as file:
        file.write(all_texts)

    redunt_file = os.path.join(directory, 'full.txt')
    os.remove(redunt_file)


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
        if count >= STOP_POINT:  # stop words' sherhold
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
    # generate all combinations of the data
    all_combinations = itertools.product(*(data[key] for key in sorted(data.keys())))

    min_variance = float('inf')
    min_combination = None

    # return the combination with the minimum variance
    for combination in all_combinations:
        variance = np.var(combination)
        if variance < min_variance:
            min_variance = variance
            # min_combination = combination

    return min_variance


def find_word_intersection_and_positions(inverted_index, words):
    # use PorterStemmer to stem the words
    stemmer = PorterStemmer()
    file_word_positions = defaultdict(lambda: defaultdict(list))
    min_comb = [None, None, None, None, None]
    min_var = [114514, 114514, 114514, 114514, 114514]
    appear_list = []

    for word in words:
        stemmed_word = stemmer.stem(word.lower())
        # find word in the inverted index
        if stemmed_word in inverted_index:
            if stemmed_word not in appear_list:
                appear_list.append(stemmed_word)
                # traverse the file positions of the word
                for filename, position in inverted_index[stemmed_word]:
                    # record the file and the position of the word
                    file_word_positions[filename][stemmed_word].append(position)
    # print(file_word_positions.items())
    # find the intersection of the files
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

    print(Fore.GREEN + "[info]" + Fore.RESET + " accessing noisy dic...")
    # get the noisy words and interesting words
    noisy_dic, interesting_dic = word_count_and_stopwords_identification(all_texts)
    with open('./noisy_dic.txt', 'w', encoding='utf-8') as file:
        for word, count in noisy_dic.items():
            file.write(f"{word}: {count}\n")

    print(Fore.GREEN + "[info]" + Fore.RESET + " building inverted file index...")
    folder_path = './shakespeare_works'  # directory of the texts
    output_path = './output.txt'  # output file
    inverted_index = build_inverted_index(folder_path, noisy_dic)
    save_inverted_index(inverted_index, output_path)
    while 1:
        # Provide input prompts and obtain input sentences
        sentence = input(Fore.GREEN + "[Input Info]" + Fore.RESET + " Please enter the word or sentence you want to query, enter -1 to quit: ")
        # Split the words in the sentence and convert them to lowercase uniformly
        input_words = sentence.lower().split()
        # If the user wishes to exit the program, i.e. enters -1, then we exit the current loop
        if input_words == ["-1"]:
            break
        # Call the search function to obtain a list of book titles
        name = find_word_intersection_and_positions(inverted_index, input_words)
        # If the list of book titles returned is empty, then we consider the query to have failed and return a failure prompt
        if name[0] is None:
            print(Fore.RED + "[ERROR]" + Fore.RESET + " nothing found,because your words is noisy_words!")
        # If not, we will start outputting our five possible query results
        else:
            for i in range(5):
                # There may be a situation where there are fewer than five search results, and if encountering a situation where the loop ends directly
                if name[i] is None:
                    break
                # 为了是搜索结果更加美观，我们将文件名尝试拆分，可以得到三类：
                # 1. <BookName>.<Act>.<Scene>.txt
                # 2. <BookName>.<Chapter>.txt
                # 3. <BookName>.txt
                book_name = name[i].split(".")
                if book_name[1] == "txt":
                    print("\"" + sentence + "\" probably comes from ", book_name[0])
                elif book_name[2] == "txt":
                    print("\"" + sentence + "\" probably comes from Chapter", book_name[1], "of", book_name[0])
                # In this case, there may be some special situations where we have applied some patches to optimize the user experience
                else:
                    henryiv_flag = 0
                    part = book_name[1][0]
                    if 'henryiv' in book_name[0]:
                        book_name[0] = 'Henry IV'
                        henryiv_flag = 1
                    if henryiv_flag:
                        print("\"" + sentence + "\" probably comes from Part " + part + ", Act " + book_name[
                            1] + ", Scene", book_name[2], "of", book_name[0])
                    else:
                        print("\"" + sentence + "\" probably comes from Act " + book_name[1] + ", Scene", book_name[2],
                              "of", book_name[0])
                if (i == 0) & (name[1] is not None):
                    print("More possible results are as follows (we list 4 more at most):")



if __name__ == '__main__':
    main()

# it is the east and Juliet is the sun
