import requests
from flask import Flask, request, jsonify
import os
import json
import re
import time
from GetFontHex import GetData

app = Flask(__name__)

user_info_path = './user_info'
if not os.path.exists(user_info_path):
    os.makedirs(user_info_path)


# POST
@app.route('/<user_id>/post', methods=['POST'])
def PostInfo(user_id):
    data = request.get_json()
    name = data.get('name')
    password = data.get('password')
    money = data.get('money')

    user_file_path = os.path.join(user_info_path, f'{user_id}.json')
    user_info = {'name': name, 'password': password, 'money': money}

    with open(user_file_path, 'w') as f:
        json.dump(user_info, f)

    return '', 200


# GET
@app.route('/<user_id>/info')
def get_user_info(user_id):
    user_file_path = os.path.join(user_info_path, f'{user_id}.json')

    with open(user_file_path, 'r') as f:
        user_info = json.load(f)

    return str(user_info.get('name')) + str(user_info.get('password')) + str(user_info.get('money')), 200


@app.route('/list')
def extract_numbers_from_filenames():
    folder_path = user_info_path
    numbers = []

    for filename in os.listdir(folder_path):
        file_path = os.path.join(folder_path, filename)
        match = re.search(r'\d+', filename)

        if match:
            numbers.append(match.group(0))
    # 返回以逗号隔开的用户索引列表
    return str(','.join(numbers)), 200


# DELETE
@app.route('/<user_id>/delete', methods=['DELETE'])
def delete_user(user_id):
    user_file_path = os.path.join(user_info_path, f'{user_id}.json')

    os.remove(user_file_path)
    return '', 200


# 获取时间
@app.route('/time', methods=['GET'])
def get_time():
    t = int(time.time())
    return str(t), 200


if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0', port=5000)
