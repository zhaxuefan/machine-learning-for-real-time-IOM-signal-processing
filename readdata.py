# -*- coding: utf-8 -*-
"""
Author:Xuefan Zha
Data source: CDI
Data structure explain: 
for every file
#1 channel number: two channel in total, used for seek
#2 total number of record
#3 observation interval
#4 sampling rate:1200HZ
#5 A-D upper voltage
#6 A-D lower voltage
#7 amplifier gain
#8 amplifier low band
#9 amplifier high band

for every epoch:
#1 record label
#2 epoch start time
#3 epoch length(number of data point)
#4- data for each epoch
"""
import numpy as np
import scipy.io

data = np.fromfile("C:/Users/xzha/Desktop/CDI unlabel data/ai_0010.bin")
#this is public parameter
channel_number = int(data[0])
total_record = int(data[1])
record_label = []
epoch_time = []
epoch_length = []
record_data = []
for i in range(total_record):
    record_label.append(int(data[1203*i + 9]))
    epoch_time.append(data[1203*i + 10])
    epoch_length.append(int(data[1203*i + 11]))
    start_point = 1203*i + 12
    end_point = int(start_point + 1200)
    record_data.append(data[start_point:end_point])

record_label = np.asmatrix(record_label).T
epoch_time = np.asmatrix(epoch_time).T
epoch_length = np.asmatrix(epoch_length).T
record_data = np.asmatrix(record_data)

mat_data = {}
mat_data['record_label'] = record_label
mat_data['epoch_time'] = epoch_time
mat_data['epoch_length'] = epoch_length
mat_data['record_data'] = record_data

scipy.io.savemat('mat/ai_0010.mat',mat_data)
    
